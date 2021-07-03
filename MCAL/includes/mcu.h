/******************************
****************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  <Write File Name>
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef MCU_H_
#define MCU_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "platform_types.h"
#include "std_types.h"
#include "mcu_cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define	MCU_EXTERNAL_RESET				1u
#define	MCU_POWER_ON_RESET				2u
#define	MCU_BROWN_OUT_RESET				4u
#define	MCU_WDT0_RESET					8u
#define	MCU_SOFTWARE_RESET				16u
#define	MCU_WDT1_RESET					32u
#define	MCU_MOSC_FAILURE_RESET		0x10000uL
#define MCU_NOT_INITIALIZED 	         0
#define MCU_INITIALIZED 		         1

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef uint8 Mcu_StatusType;
typedef uint32 Mcu_FreqType;
typedef uint8 Mcu_ClockType;
typedef uint32 Mcu_RawResetType;

typedef enum
{
	MCU_PLL_STATUS_UNLOCKED,
	MCU_PLL_STATUS_LOCKED
}Mcu_PllStatusType;

typedef enum
{
	MCU_CLOCK_SOURCE_MOSC,
	MCU_CLOCK_SOURCE_PIOSC,
	MCU_CLOCK_SOURCE_PIOSC_DIV4,
	MCU_CLOCK_SOURCE_LFIOSC,
	//MCU_CLOCK_SOURCE_HIOSC
}Mcu_ClockSourceType;

typedef struct
{
  	Mcu_ClockSourceType ClockSource;
	boolean PllUse;
	Mcu_FreqType Freq_KHz;
	
}Mcu_ConfigType;



/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern uint8 Mcu_ActivatedClockGates[];
extern const Mcu_ConfigType McuConfig[];
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
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
void Mcu_Init ( const Mcu_ConfigType* ConfigPtr );

/******************************************************************************
* \Syntax          : Mcu_RawResetType Mcu_GetResetRawValue( void )        
* \Description     : The service reads the reset type from the hardware, if supported.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None                            
* \Parameters (out): None                                                      
* \Return value:   : Mcu_RawResetType   Reset raw value
*******************************************************************************/
Mcu_RawResetType Mcu_GetResetRawValue( void );

/******************************************************************************
* \Syntax          : void Mcu_PerformReset( void )        
* \Description     : The service performs a microcontroller reset.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                           
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
#if (MCU_PERFORM_RESET_API==STD_ON)
void Mcu_PerformReset( void );
#endif
/******************************************************************************
* \Syntax          : void Mcu_Init ( const Mcu_ConfigType* ConfigPtr )        
* \Description     : This service initializes the PLL and other MCU specific clock options.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ClockSetting       ClockSetting.                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType 

*******************************************************************************/
Std_ReturnType Mcu_InitClock( Mcu_ClockType ClockSetting );

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
Std_ReturnType Mcu_DistributePllClock( void );

/******************************************************************************
* \Syntax          : Mcu_PllStatusType Mcu_GetPllStatus        
* \Description     : This service provides the lock status of the PLL.                                                                                                               
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : Mcu_PllStatusType  PLL Status
*******************************************************************************/
Mcu_PllStatusType Mcu_GetPllStatus( void );


#endif  /* MCU_H */
/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/

