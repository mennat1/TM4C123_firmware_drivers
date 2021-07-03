/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  mcu.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "std_types.h"
#include "mcu.h"
#include "mcu_hw.h"

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static const Mcu_ConfigType* Mcu_ConfigPtr = NULL_PTR;//pointer to array of structs 
static Mcu_StatusType Mcu_Status = MCU_NOT_INITIALIZED;

volatile static uint32 GlobalSystemClock;//IS THIS STATIC????


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void Mcu_Init ( const Mcu_ConfigType* ConfigPtr )        
* \Description     : This service initializes the MCU driver.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ConfigPtr       Pointer to MCU driver configuration set.                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void Mcu_Init ( const Mcu_ConfigType* ConfigPtr )
{
	Mcu_Status = MCU_INITIALIZED; //1
	Mcu_ConfigPtr = ConfigPtr;
}
/******************************************************************************
* \Syntax          : void Mcu_PerformReset( void )        
* \Description     : The service performs a microcontroller reset.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                           
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
#if (MCU_PERFORM_RESET_API == STD_ON)
void Mcu_PerformReset(void)
{
	/* clear reset cause regester*/
	RESC = 0x00; //RESC reg in System Control Module (not SCB)
	/* perform reset */
	//SYSREQ is a 1 bit field in APINT reg:
	/*	0 -> No effect.
		1 -> Resets the core and all on-chip peripherals except the Debug interface.
	*/
	APINT = (APINT_VECTKEY << APINT_VECTKEY_FIELD_OFFSET) | (1 << SYSREQ_FIELD_OFFSET); //APINT reg in SCB
	
}
#endif/*MCU_PERFORM_RESET_API==STD_ON*/
/******************************************************************************
* \Syntax          : Mcu_RawResetType Mcu_GetResetRawValue( void )        
* \Description     : The service reads the reset type from the hardware, if supported.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None                            
* \Parameters (out): None                                                      
* \Return value:   : Mcu_RawResetType   Reset raw value
*******************************************************************************/
Mcu_RawResetType Mcu_GetResetRawValue(void)
{
	Mcu_RawResetType resetCause = RESC;
	RESC = 0x0;
	return resetCause;
}

/******************************************************************************
* \Syntax          : Std_ReturnType Mcu_DistributePllClock( void )        
* \Description     : This service activates the PLL clock to the MCU clock distribution.                                                                                                              
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                            
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType 
*                    E_OK: Command has been accepted
*                    E_NOT_OK: Command has not been accepted
*******************************************************************************/
Std_ReturnType Mcu_DistributePllClock(void)
{
	Std_ReturnType ret = E_NOT_OK;
	/* PWRDN Value Description:
		0 -> The PLL is operating normally.
		1 -> The PLL is powered down. Care must be taken to ensure that
		another clock source is functioning and that the BYPASS bit is
		set before setting this bit.
	*/
	/* check if PLL is enabled */
	if(RCC.B.PWRDN == 0) //The PLL is operating normally.
	{
		/* check if Pll is locked (stable)  */
		if(Mcu_GetPllStatus() == MCU_PLL_STATUS_LOCKED)
		{
			/* distribute PLL */
			/* PLL BYPASS Value Description:
			 	0 -> then the system clock is the PLL output clock divided by the divisor specified by SYSDIV.
				1 -> then the system clock is derived from the OSC source and divided by the divisor specified by SYSDIV.
			*/
			RCC.B.BYPASS = 0;
			ret = E_OK;
		}
	}
	return ret;
	
}
/******************************************************************************
* \Syntax          : void Mcu_Init ( const Mcu_ConfigType* ConfigPtr )        
* \Description     : This service initializes the PLL and other MCU specific clock options.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : clockSettingID       clockSettingID.                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType 

*******************************************************************************/
Std_ReturnType Mcu_InitClock(Mcu_ClockType clockSettingID)//choose clockSetting ID
{
	Std_ReturnType ret = E_NOT_OK;
	uint8 locGateRegOffset, locGateBitOffset;
	uint8 locSourceFreq_MHz;
	volatile uint32 sysdiv_value;

	if(Mcu_Status == MCU_INITIALIZED)//i.e. called mcu_init()
	{
	    /*
	    Enable Configured Pripherals Gates using RCGCx (Run-Mode Clock Gate Control) Registers:
	    starting from RCGCWD at offset 0x600
	    */
	    //System Control Base address = 0x400F.E000
	    for(uint8 i = 0; i < MCU_ACTIVATED_CLOCK_GATES_SIZE; i++)
	    {
	    	locGateRegOffset = (Mcu_ActivatedClockGates[i] / 10) * WORD_LENGTH_BYTES;
	    	locGateBitOffset = Mcu_ActivatedClockGates[i] % 10;
	    	GET_HWREG(SYSCTR_RCG_BASE_ADDR,locGateRegOffset) |= (1 << locGateBitOffset);	
	    }

	    // RCC: Run-Mode Clock Configuration Register in System Control Module
	    // OSCSRC: Oscillator Source
	    /* Value Input Source:
			0x0 -> MOSC
			0x1 -> PIOSC (default)
			0x2 -> PIOSC/4
			0x3 -> LFIOSC
		*/
	    RCC.B.OSCSRC = Mcu_ConfigPtr[clockSettingID].ClockSource;
	    
	    /* Calculate SYS_DIV Value */
	    if(Mcu_ConfigPtr[clockSettingID].PllUse == ENABLE)
	    {
	    	/*Enable PLL*/
	    	/*
			PWRDWN: PLL Power Down
			Value Description:
			0 -> The PLL is operating normally.
			1 -> The PLL is powered down. Care must be taken to ensure that
				another clock source is functioning and that the BYPASS bit is
				set before setting this bit.
	    	*/
	    	RCC.B.PWRDN = 0x0;

	    	//MCU_PLL_OUTPUT_DIV2 = 200
	    	/* System Divisor SYSDIV specifies which divisor is used to generate the system clock from either
			the PLL output or the oscillator source (depending on how the BYPASS
			bit in this register is configured).*/

			/*
				SYSDIV Divisor Frequency(BYPASS=0) 		Frequency(BYPASS=1)		TivaWare™ Parametera
								(PLL ENABLED)				(PLL DISABLED)
				0x0 	/1 		reserved 				Clock source frequency/1 	SYSCTL_SYSDIV_1
				0x1 	/2 		reserved				Clock source frequency/2 	SYSCTL_SYSDIV_2
				0x2 	/3 		66.67 MHz 				Clock source frequency/3 	SYSCTL_SYSDIV_3
				0x3 	/4 		50 MHz 					Clock source frequency/4 	SYSCTL_SYSDIV_4
				0x4 	/5 		40 MHz 					Clock source frequency/5 	SYSCTL_SYSDIV_5
				0x5 	/6 		33.33 MHz 				Clock source frequency/6 	SYSCTL_SYSDIV_6
				0x6 	/7 		28.57 MHz 				Clock source frequency/7 	SYSCTL_SYSDIV_7
				0x7 	/8 		25 MHz 					Clock source frequency/8 	SYSCTL_SYSDIV_8
				0x8 	/9 		22.22 MHz 				Clock source frequency/9 	SYSCTL_SYSDIV_9
				0x9 	/10 	20 MHz 					Clock source frequency/10 	SYSCTL_SYSDIV_10
				0xA 	/11 	18.18 MHz 				Clock source frequency/11 	SYSCTL_SYSDIV_11
				0xB 	/12 	16.67 MHz 				Clock source frequency/12 	SYSCTL_SYSDIV_12
				0xC 	/13 	15.38 MHz 				Clock source frequency/13 	SYSCTL_SYSDIV_13
				0xD 	/14 	14.29 MHz 				Clock source frequency/14 	SYSCTL_SYSDIV_14
				0xE 	/15 	13.33 MHz 				Clock source frequency/15 	SYSCTL_SYSDIV_15
				0xF 	/16 	12.5 MHz (default) 		Clock source frequency/16 	SYSCTL_SYSDIV_16
			*/

	    	//MCU_PLL_OUTPUT_DIV2 = 200
	    	//ex: Freq_required = 25 MHz = 200/(8) -> SYSDIV = 7 = (200/25) - 1
	    	//Available F values = [(200/16):(200/3)] = [12.5:66.67] MHz, so -> sysdiv_value should be = [0x2:0xF]
	    	sysdiv_value = (MCU_PLL_OUTPUT_DIV2 / (Mcu_ConfigPtr[clockSettingID].Freq_KHz/1000)) - 1;	
	    	RCC.B.SYSDIV = sysdiv_value;	
	    }
	    else
	    {	//PllUse = DISABLE

	    	//LFIOSC: Low Freq Internal Oscillator gives [10:90] KHz
	    	//BYPASS = 1, OSCSRC = 0x3
	    	if (Mcu_ConfigPtr[clockSettingID].ClockSource == MCU_CLOCK_SOURCE_LFIOSC)
	    	{
	    		RCC.B.SYSDIV = 0x0;
	    	}
	    	else
	    	{	
	    		//MOSC: Main Oscillator
	    		if(Mcu_ConfigPtr[clockSettingID].ClockSource == MCU_CLOCK_SOURCE_MOSC)
	            {
	            	/*	MOSCDIS: Main Oscillator Disable (1 by default)*/
	            	/*	XTAL: Crystal Value (5 bits field)
						This field specifies the crystal value attached to the main oscillator. The
						encoding for this field is provided below.
						Frequencies that may be used with the USB interface are indicated in
						the table. To function within the clocking requirements of the USB
						specification, a crystal of 5, 6, 8, 10, 12, or 16 MHz must be used.
	            	*/
	            	/*
	            			Value 			Crystal Frequency (MHz) 	 		Crystal Frequency (MHz)
												NOT Using the PLL					Using the PLL
							0x00-0x5 									reserved
							0x06 					4 MHz 								reserved
							0x07 					4.096 MHz 							reserved
							0x08 					4.9152 MHz 							reserved
							0x09 										5 MHz (USB)
							0x0A = 10									5.12 MHz
							0x0B = 11									6 MHz (USB)
							0x0C = 12									6.144 MHz
							0x0D = 13									7.3728 MHz
							0x0E = 14									8 MHz (USB)
							0x0F = 15									8.192 MHz
							0x10 = 16									10.0 MHz (USB)
							0x11 = 17									12.0 MHz (USB)
							0x12 = 18									12.288 MHz
							0x13 = 19									13.56 MHz
							0x14 = 20									14.31818 MHz
							0x15 = 21									16.0 MHz (USB)
							0x16 = 22									16.384 MHz
							0x17 = 23									18.0 MHz (USB)
							0x18 = 24									20.0 MHz (USB)
							0x19 = 25									24.0 MHz (USB)
							0x1A = 26									25.0 MHz (USB)
	            	*/
	            	RCC.B.MOSCDIS = 0x0;
	            	RCC.B.XTAL = MCU_XTAL_CLOCK_FREQ_MHZ ; /*TODO : The value need to be mapped */
	    	    	locSourceFreq_MHz = MCU_XTAL_CLOCK_FREQ_MHZ; //16
	            }
	            //PIOSC: Precision Internal Oscillator
	    	    else if (Mcu_ConfigPtr[clockSettingID].ClockSource == MCU_CLOCK_SOURCE_PIOSC)
	    	    {
	    	    	locSourceFreq_MHz = 16;
	    	    	
	    	    }
	    	    else if (Mcu_ConfigPtr[clockSettingID].ClockSource == MCU_CLOCK_SOURCE_PIOSC_DIV4)
	    	    {
	    	    	locSourceFreq_MHz = 4;
	    	    	
	    	    }
	    	    else
	    	    {//HFIOSC????
					locSourceFreq_MHz = 4;
				}
				//PIOSC gives 16 MHz -> Available Freqs = (16/1) -> (16/16) MHz
				//ex: Freq_req = 16 MHz using PIOSC which gives 16 MHZ -> SYSDIV = 0x0 = (16/16) - 1
				//ex: Freq_req = 4 MHz = 16/(4) using PIOSC -> SYSDIV = 0x3 = (16/4) - 1
				sysdiv_value = (locSourceFreq_MHz / (Mcu_ConfigPtr[clockSettingID].Freq_KHz/1000)) - 1;	
				RCC.B.SYSDIV = sysdiv_value;
	    	}	
	    }
			GlobalSystemClock = Mcu_ConfigPtr[clockSettingID].Freq_KHz;//WHAT IS THIS???
        	ret = E_OK;
    }
	return ret;
}
/******************************************************************************
* \Syntax          : Mcu_PllStatusType Mcu_GetPllStatus        
* \Description     : This service provides the lock status of the PLL.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : Mcu_PllStatusType  PLL Status
*******************************************************************************/
Mcu_PllStatusType Mcu_GetPllStatus(void)
{
	//PLLSTAT register's 1st bit field (LOCK) shows the direct status of the PLL lock. (READ ONLY)
	/*	0 -> The PLL is unpowered or is not yet locked (UNSTABLE) -> MCU_PLL_STATUS_UNLOCKED
		1 -> The PLL is powered and locked. (STABLE) -> MCU_PLL_STATUS_LOCKED
	*/
	return (PLLSTAT & 0x1); 
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
