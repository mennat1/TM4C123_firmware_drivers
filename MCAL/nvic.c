/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Nvic.c
 *        \brief  Nested Vector Interrupt Controller Driver
 *
 *      \details  The Driver Configure All MCU interrupts Priority into gorups and subgroups
 *                Enable and Disable Navic Interrupt Gate for Peripherals
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "std_types.h"
#include "nvic.h"
#include "mcu_hw.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : void Nvic_Init(void)                                      
* \Description     : initialize Nvic Module by parsing the Configuration 
*                    into Nvic registers                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None
*******************************************************************************/
void nvic_Init(void){
	Nvic_IntType intNum;
	uint8 locGroup, locSubGroup, value_of_INTx;
	uint32 enRegOffset, enBitOffset, priRegOffset, priBitOffset;
	/*TODO #1: Configure Grouping\SubGrouping System in APINT register in SCB*/

	// *To write to this register, 0x05FA must be written to the VECTKEY field, otherwise the write is ignored.
	// *The PRIGROUP field indicates the position of the binary point that splits the INTx fields in the 
	// Interrupt Priority (PRIx) registers into separate group priority and subpriority fields.

	APINT = (APINT_VECTKEY << APINT_VECTKEY_FIELD_OFFSET) | (NVIC_GROUPING_SYSTEM << PRIGROUP_FIELD_OFFSET);
	
	//Enable each interrupt(ENx) and set its group/subgroup priority value(PRIx) 
	for(uint8 i = 0; i < NVIC_ACTIVATED_INT_SIZE; i++){
		intNum = Nvic_Cfg[i].interruptNumber;
		locGroup = Nvic_Cfg[i].groupPriority;
		locSubGroup = Nvic_Cfg[i].subgroupPriority;
		
		/*TODO #2 : Enable\Disable based on user configurations in ENx Nvic Registers, EN0 to EN4 */
	
        /*NVIC_ENx Register*/
		/*
		Register 4: Interrupt 0-31 Set Enable (EN0), offset 0x100
		Register 5: Interrupt 32-63 Set Enable (EN1), offset 0x104
		Register 6: Interrupt 64-95 Set Enable (EN2), offset 0x108
		Register 7: Interrupt 96-127 Set Enable (EN3), offset 0x10C
		Register 8: Interrupt 128-138 Set Enable (EN4), offset 0x110
		*/
   		enRegOffset = (intNum / WORD_LENGTH_BITS) * WORD_LENGTH_BYTES; //(0, 4, 8, 12, 16)for(EN0 -> EN4)
		enBitOffset = intNum % WORD_LENGTH_BITS; //(0, 1, 2, .. , 31)
		GET_HWREG(NVIC_ENABLE_BASE_ADDRESS,enRegOffset) |= (1 << enBitOffset); // Enable Inetrrupt
		
		/* Create Grouping Field */
		/* X for Group, Y for Subgroup */
		#if (NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XXX)
			    value_of_INTx = locGroup;
		#elif (NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XXY) 
		       value_of_INTx = ((locGroup << 1) & 0x6) | (locSubGroup & 0x1);
		#elif (NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_XYY) 
		       value_of_INTx = ((locGroup << 2) & 0x4) | (locSubGroup & 0x3);
		#elif (NVIC_GROUPING_SYSTEM == NVIC_GROUPING_SYSTEM_YYY) 
		       value_of_INTx = locSubGroup;
		#else
		     #error INVALID GROUPING SELECTION
		#endif 	/*NVIC_GROUPING_SYSTEM*/
				
		/*TODO #3 : Assign Group/Subgroup priority in PRIx Nvic Registers, PRI0 to PRI34*/
		//Each PRIx Reg has 4 INTx Bit Fields (INTA, INTB, INTC, INTD) (Each is 3 bits)
		//INTA for INT[4n], INTB for INT[4n+1], INTC for INT[4n+2], INTD for INT[4n+3]
		//INTA bits[5:7], INTB bits[13:15], INTC bits[21:23], INTD bits[29:31]

		/*
		Register 29: Interrupt 0-3 Priority (PRI0), offset 0x400
		Register 30: Interrupt 4-7 Priority (PRI1), offset 0x404
		Register 31: Interrupt 8-11 Priority (PRI2), offset 0x408
		Register 32: Interrupt 12-15 Priority (PRI3), offset 0x40C
		Register 33: Interrupt 16-19 Priority (PRI4), offset 0x410
		Register 34: Interrupt 20-23 Priority (PRI5), offset 0x414
		Register 35: Interrupt 24-27 Priority (PRI6), offset 0x418
		Register 36: Interrupt 28-31 Priority (PRI7), offset 0x41C
		Register 37: Interrupt 32-35 Priority (PRI8), offset 0x420
		Register 38: Interrupt 36-39 Priority (PRI9), offset 0x424
		Register 39: Interrupt 40-43 Priority (PRI10), offset 0x428
		Register 40: Interrupt 44-47 Priority (PRI11), offset 0x42C
		Register 41: Interrupt 48-51 Priority (PRI12), offset 0x430
		Register 42: Interrupt 52-55 Priority (PRI13), offset 0x434
		Register 43: Interrupt 56-59 Priority (PRI14), offset 0x438
		Register 44: Interrupt 60-63 Priority (PRI15), offset 0x43C
		Register 45: Interrupt 64-67 Priority (PRI16), offset 0x440
		Register 46: Interrupt 68-71 Priority (PRI17), offset 0x444
		Register 47: Interrupt 72-75 Priority (PRI18), offset 0x448
		Register 48: Interrupt 76-79 Priority (PRI19), offset 0x44C
		Register 49: Interrupt 80-83 Priority (PRI20), offset 0x450
		Register 50: Interrupt 84-87 Priority (PRI21), offset 0x454
		Register 51: Interrupt 88-91 Priority (PRI22), offset 0x458
		Register 52: Interrupt 92-95 Priority (PRI23), offset 0x45C
		Register 53: Interrupt 96-99 Priority (PRI24), offset 0x460
		Register 54: Interrupt 100-103 Priority (PRI25), offset 0x464
		Register 55: Interrupt 104-107 Priority (PRI26), offset 0x468
		Register 56: Interrupt 108-111 Priority (PRI27), offset 0x46C
		Register 57: Interrupt 112-115 Priority (PRI28), offset 0x470
		Register 58: Interrupt 116-119 Priority (PRI29), offset 0x474
		Register 59: Interrupt 120-123 Priority (PRI30), offset 0x478
		Register 60: Interrupt 124-127 Priority (PRI31), offset 0x47C
		Register 61: Interrupt 128-131 Priority (PRI32), offset 0x480
		Register 62: Interrupt 132-135 Priority (PRI33), offset 0x484
		Register 63: Interrupt 136-138 Priority (PRI34), offset 0x488
		*/
		priRegOffset = (intNum / NUM_OF_PRI_FIELDS_IN_PRIx) * WORD_LENGTH_BYTES; //(0, 4, 8, .. , 136)for(PRI0 -> PRI34)
		priBitOffset = 5 + ((intNum % NUM_OF_PRI_FIELDS_IN_PRIx) * 8); //(5, 13, 21, 29)for(INTA, INTB, INTC, INTD)
		GET_HWREG(NVIC_PRI_BASE_ADDRESS,priRegOffset) |= ((uint32)value_of_INTx << priBitOffset);
	}	
}

/**********************************************************************************************************************
 *  END OF FILE: Nvic.c
 *********************************************************************************************************************/
