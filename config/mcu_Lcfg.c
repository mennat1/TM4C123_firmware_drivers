/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Mcu_Lcfg.c
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
#include "mcu_cfg.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

//McuConfig is an array of Mcu_ConfigType structs: {Mcu_ClockSourceType ClockSource; boolean PllUse; Mcu_FreqType Freq_KHz;}
//ClockSource is an Mcu_ClockSourceType enum: 
//{MCU_CLOCK_SOURCE_MOSC, MCU_CLOCK_SOURCE_PIOSC, MCU_CLOCK_SOURCE_PIOSC_DIV4,
// MCU_CLOCK_SOURCE_LFIOSC, MCU_CLOCK_SOURCE_HIOSC}

const Mcu_ConfigType McuConfig[MCU_CLOCK_SETTING_SIZE] =
{
/*clocksettingID*/ /*clockSource				PLL		frequency */
/*  0  			*/	{MCU_CLOCK_SOURCE_MOSC	, ENABLE	, 25000},
/*  1 			*/	{MCU_CLOCK_SOURCE_PIOSC	, DISABLE	, 16000}
};

uint8 Mcu_ActivatedClockGates[MCU_ACTIVATED_CLOCK_GATES_SIZE] =
{
	MCU_CLOCK_GATE_GPIOA,
	MCU_CLOCK_GATE_GPIOB,
	MCU_CLOCK_GATE_GPIOC,
	MCU_CLOCK_GATE_GPIOD,
	MCU_CLOCK_GATE_GPIOE,
	MCU_CLOCK_GATE_GPIOF,
	MCU_CLOCK_GATE_16_32_BIT_GPT0,
	MCU_CLOCK_GATE_32_64_BIT_GPT1,
	MCU_CLOCK_GATE_32_64_BIT_GPT4,
	MCU_CLOCK_GATE_WD0
};

/**********************************************************************************************************************
 *  END OF FILE: Mcu_Lcfg.c
 *********************************************************************************************************************/
