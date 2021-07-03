/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  mcu_cfg.h
 *       Module:  System control
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef MCU_CFG_H_
#define MCU_CFG_H_

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define MCU_PERFORM_RESET_API			   STD_ON
#define MCU_ACTIVATED_CLOCK_GATES_SIZE      10
#define MCU_CLOCK_SETTING_SIZE				2
#define MCU_XTAL_CLOCK_FREQ_MHZ             16

/*****************************************************/
/*At 0x600 offset (0:RCGCWD)*/
#define MCU_CLOCK_GATE_WD0       00u
#define MCU_CLOCK_GATE_WD1       01u
/*At 0x604 offset (1:RCGCTIMER)*/
#define MCU_CLOCK_GATE_16_32_BIT_GPT0       10u
#define MCU_CLOCK_GATE_16_32_BIT_GPT1       11u
#define MCU_CLOCK_GATE_16_32_BIT_GPT2       12u
#define MCU_CLOCK_GATE_16_32_BIT_GPT3       13u
#define MCU_CLOCK_GATE_16_32_BIT_GPT4       14u
#define MCU_CLOCK_GATE_16_32_BIT_GPT5       15u
/*At 0x608 offset (2:RCGCGPIO)*/
#define MCU_CLOCK_GATE_GPIOA     20u
#define MCU_CLOCK_GATE_GPIOB     21u
#define MCU_CLOCK_GATE_GPIOC     22u
#define MCU_CLOCK_GATE_GPIOD     23u
#define MCU_CLOCK_GATE_GPIOE     24u
#define MCU_CLOCK_GATE_GPIOF     25u

#define MCU_CLOCK_GATE_32_64_BIT_GPT0       150u
#define MCU_CLOCK_GATE_32_64_BIT_GPT1       151u
#define MCU_CLOCK_GATE_32_64_BIT_GPT2       152u
#define MCU_CLOCK_GATE_32_64_BIT_GPT3       153u
#define MCU_CLOCK_GATE_32_64_BIT_GPT4       154u
#define MCU_CLOCK_GATE_32_64_BIT_GPT5       155u


#define MCU_CLOCK_GATE_UART0     50u
#define MCU_CLOCK_GATE_SSI2      62u



#define	MCU_CLOCK_0			0
#define	MCU_CLOCK_1			1
#define	MCU_CLOCK_2			2

#endif  /* FILE_NAME_H */

/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/

