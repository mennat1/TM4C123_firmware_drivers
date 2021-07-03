/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  FileName.c
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
#include "port_cfg.h"
#include "port.h"
#include "mcu_hw.h"
#include "platform_types.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define CHANNEL_SIZE_IN_PORT               8u
#define MAX_NUMBER_OF_GPIO_PORTS           6u

#define GPIO_COMMIT_UNLOCKING_VALUE		0x4C4F434B
#define PIN_FIELD_SIZE_IN_GPIOPCTL			4
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static const uint32 Port_BaseAddr[MAX_NUMBER_OF_GPIO_PORTS] = 
{
	GPIO_APB_BASE_ADDRESS_A,
	GPIO_APB_BASE_ADDRESS_B,
	GPIO_APB_BASE_ADDRESS_C,
	GPIO_APB_BASE_ADDRESS_D,
	GPIO_APB_BASE_ADDRESS_E,
  	GPIO_APB_BASE_ADDRESS_F
};

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : void Port_Init ( const Port_ConfigType* ConfigPtr )        
* \Description     : initializing the port diver module                                   
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : ConfigPtr   Pointer to configuration set.                   
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Port_Init ( const Port_ConfigType* ConfigPtr )
{
	Port_PinType locPinId;
	Port_PinDirectionType locPinDir;
	Port_PinModeType locPinMode; 
	Port_PinOutputCurrentType locCurrentStrength;
	Port_PinInternalAttachType locInternalAttach;
	Port_PinExternalIntType locExternalInt;
	uint8 PortIndex,PinIndex;
	uint32 PortBaseAddr;
	
	for(uint8 i = 0; i < PORT_ACTIVATED_CHANNELS_SIZE; i++)
	{
		locPinId = ConfigPtr[i].pinId;
		locPinDir = ConfigPtr[i].pinDir;
		locPinMode = ConfigPtr[i].pinMode;
		locCurrentStrength = ConfigPtr[i].currentStrength;
		locInternalAttach = ConfigPtr[i].internalAttach;
		locExternalInt = ConfigPtr[i].externalInt;
		
		PortIndex = locPinId / CHANNEL_SIZE_IN_PORT;
		PinIndex  = locPinId % CHANNEL_SIZE_IN_PORT;
		PortBaseAddr = Port_BaseAddr[PortIndex] ;
		/*
			GPIO Data Direction Value Description:
				0 Corresponding pin is an input.
				1 Corresponding pins is an output.
		*/
		GPIODIR(PortBaseAddr) |= (locPinDir << PinIndex);

		/*Writing 0x4C4F.434B to the GPIOLOCK register unlocks the GPIOCR(GPIO COMMIT) register.*/
		/*
			GPIOCR bits[0:7] Value Description:
				0 -> The corresponding GPIOAFSEL(Alternate Function Select), GPIOPUR(Pull-Up Select),
				 	 GPIOPDR(Pull-Down Select), or GPIODEN(Digital Enable) bits cannot be written.

				1 -> The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
		*/
		/*
			The GPIOAFSEL register is the mode control select register. 
			If a bit is clear, the pin is used as a GPIO and is controlled by the GPIO registers.
			Setting a bit in this register configures the corresponding GPIO line to be controlled by 
			an associated peripheral. Several possible peripheral functions are multiplexed on each GPIO.
			he GPIO Port Control (GPIOPCTL) register is used to select one of the possible functions.
		*/
		/*
			The GPIOPCTL register selects one out of a set of peripheral functions
			for each GPIO, providing additional flexibility in signal definition.
			GPIOPCTL is divided to 8 PMCxs (Porst MUX Controllers PMC[0:7]), each PMC is 4 bits
			and controls one pin in the port. (There are diff GIOPCTL regs one for each port)
		*/
		/* setting pin mode */
		GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;//0x4C4F434B
		GPIOCR(PortBaseAddr) = 0x1;
		if(locPinMode == Port_PinMode_X_DIO)
		{
			// GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;//0x4C4F434B
			// GPIOCR(PortBaseAddr) = 0x1;
			GPIOAFSEL(PortBaseAddr) = 0;
		}
		else
		{
			// GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;
			// GPIOCR(PortBaseAddr) = 0x1;
			GPIOAFSEL(PortBaseAddr) = 1;
			GPIOPCTL(PortBaseAddr) |= (locPinMode << (PIN_FIELD_SIZE_IN_GPIOPCTL * PinIndex));
		}
		
		/* setting current strength */
		if(locCurrentStrength == PORT_PIN_CURRENT_2m)
		{
			GPIODR2R(PortBaseAddr) |= (1 << PinIndex);//GPIO 2-mA Drive Select
		}
		else if(locCurrentStrength == PORT_PIN_CURRENT_4m)
		{
			GPIODR4R(PortBaseAddr) |= (1 << PinIndex);//GPIO 4-mA Drive Select
		}
		else if(locCurrentStrength == PORT_PIN_CURRENT_8m)
		{
			GPIODR8R(PortBaseAddr) |= (1 << PinIndex);//GPIO 8-mA Drive Select
		}
		
		/* setting internal attach type */
		if (locInternalAttach == PORT_ATTACH_PULLUP)
		{
			// GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;
			// GPIOCR(PortBaseAddr) = 0x1;
			GPIOPUR(PortBaseAddr) |= (1 << PinIndex);//Pull Up
			
		} 
		else if (locInternalAttach == PORT_ATTACH_PULLDOWN)
		{
			// GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;
			// GPIOCR(PortBaseAddr) = 0x1;
			GPIOPDR(PortBaseAddr) |= (1 << PinIndex); //Pull Down
		}
		else if (locInternalAttach == PORT_ATTACH_OPENDRAIN)
		{
			// GPIOLOCK(PortBaseAddr) = GPIO_COMMIT_UNLOCKING_VALUE;
			// GPIOCR(PortBaseAddr) = 0x1;		
			GPIOODR(PortBaseAddr) |= (1 << PinIndex);
		}
		
		
		/* setting external interupt mode */
		if (locExternalInt == PORT_EXT_INT_DISABLE)
		{
			/*
				GPIOIM Value Description:
					0 -> The interrupt from the corresponding pin is masked.
					1 -> The interrupt from the corresponding pin is sent to the interrupt
					controller.
			*/
			GPIOIM(PortBaseAddr) &= (~(1 << PinIndex));//GPIO Interrupt Mask
		}
		/* configure the interupt with the edges */
		else if (locExternalInt == PORT_EXT_INT_RISING)
		{

			/*
			The GPIOIS register is the interrupt sense register. Setting a bit in the GPIOIS register 
			configures the corresponding pin to detect levels, while clearing a bit configures 
			the corresponding pin to detect edges. All bits are cleared by a reset.
			Note: To prevent false interrupts, the following steps should be taken when re-configuring GPIO
			edge and interrupt sense registers:
			1. Mask the corresponding port by clearing the IME field in the GPIOIM register.
			2. Configure the IS field in the GPIOIS register and the IBE field in the GPIOIBE register.
			3. Clear the GPIORIS register.
			4. Unmask the port by setting the IME field in the GPIOIM register.
			*/
			/*
				GPIO Interrupt Sense (GPIOIS) Value Description:
				0 -> The edge on the corresponding pin is detected (edge-sensitive).
				1 -> The level on the corresponding pin is detected (level-sensitive).
			*/
			/*
				The GPIOIBE register allows both edges to cause interrupts. When the corresponding
				bit in the (GPIOIS) register is set to detect edges,
				setting a bit in the GPIOIBE register configures the corresponding pin to detect
				both rising and falling edges, regardless of the corresponding bit in the (GPIOIEV) register.
				Clearing a bit configures the pin to be controlled by the GPIOIEV register.

			*/
			/*
				GPIO Interrupt Both Edges (GPIOBE) Value Description:
				0 -> Interrupt generation is controlled by the GPIO Interrupt Event
					(GPIOIEV) register.
				1 -> Both edges on the corresponding pin trigger an interrupt.
			*/
			/*
				The GPIOIEV register is the interrupt event register. Setting a bit in the GPIOIEV
				register configures the corresponding pin to detect rising edges or high levels,
				depending on the corresponding bit value in the GPIO Interrupt Sense (GPIOIS)
				register. Clearing a bit configures the pin to detect falling edges or low levels, 
				depending on the corresponding bit value in the GPIOIS register.
			*/
			/*
				GPIO Interrupt Event (GPIOIEV) Value Description:
					0 -> A falling edge or a Low level on the corresponding pin triggers an interrupt.
					1 -> A rising edge or a High level on the corresponding pin triggers an interrupt.
			*/
			/*
			The GPIORIS register is the raw interrupt status register. A bit in this register is set when
			an interrupt condition occurs on the corresponding GPIO pin. If the corresponding bit in the 
			GPIO Interrupt Mask (GPIOIM) register is set, the interrupt is sent to the interrupt controller. 
			Bits read as zero indicate that corresponding input pins have not initiated an interrupt. 
			For a GPIO level-detect interrupt, the interrupt signal generating the interrupt must be held until
			serviced. Once the input signal deasserts from the interrupt generating logical sense, the corresponding
			RIS bit in the GPIORIS register clears. For a GPIO edge-detect interrupt, the RIS bit in the GPIORIS 
			register is cleared by writing a ‘1’ to the corresponding bit in the GPIO Interrupt Clear (GPIOICR)
			register. The corresponding GPIOMIS bit reflects the masked value of the RIS bit.
			*/
			/*
				GPIO Interrupt Raw Status (GPIOIRS) Value Description:
					0 -> An interrupt condition has not occurred on the corresponding pin.
					1 -> An interrupt condition has occurred on the corresponding pin.
				- For edge-detect interrupts, this bit is cleared by writing a 1 to the
				corresponding bit in the GPIOICR register.
				- For a GPIO level-detect interrupt, the bit is cleared when the level is deasserted.
			*/

			GPIOIM(PortBaseAddr) 	&= (~(1 << PinIndex)); //mask interrupt on that pin
			GPIOIS(PortBaseAddr) 	|= (1 << PinIndex);//GPIO Interrupt Sense (level sensitive)
			GPIOIBE(PortBaseAddr)	&= (~(1 << PinIndex));//Interrupt generation is controlled by the (GPIOIEV) register.
			GPIOIEV(PortBaseAddr)	|= (1 << PinIndex);// A rising edge or a High level on the corresponding pin triggers an interrupt.
			GPIORIS(PortBaseAddr)	&= (~(1 << PinIndex));// clear the GPIO Raw Interrupt Status
			GPIOIM(PortBaseAddr) 	|= (1 << PinIndex);
			
		}
		else if (locExternalInt == PORT_EXT_INT_FALLING)
		{
			GPIOIM(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIS(PortBaseAddr)	|= (1 << PinIndex);
			GPIOIBE(PortBaseAddr)	&= (~(1 << PinIndex));
			GPIOIEV(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIORIS(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIM(PortBaseAddr)	|= (1 << PinIndex);
		}
		else if (locExternalInt == PORT_EXT_INT_BOTH)
		{
			GPIOIM(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIS(PortBaseAddr) 	|= (1 << PinIndex);
			GPIOIBE(PortBaseAddr)	|= (1 << PinIndex);
			GPIORIS(PortBaseAddr) 	&= (~(1 << PinIndex));
			GPIOIM(PortBaseAddr) 	|= (1 << PinIndex);
		}
	}
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/