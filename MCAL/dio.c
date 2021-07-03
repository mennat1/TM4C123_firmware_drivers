/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  Dio.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Dio.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define CHANNEL_SIZE_IN_PORT               	8u 
#define MAX_NUMBER_OF_GPIO_PORTS			6u
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
* \Syntax          : Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId )        
* \Description     : Reads channel value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId   ID of DIO channel                     
* \Parameters (out): Dio_LevelType                                                      
* \Return value:   : Dio_LevelType  STD_LOW,
																		 STD_HIGH                                  
*******************************************************************************/
Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId )
{
	Dio_LevelType level;
	uint32 gpioDataOffset;
	uint32 portIndex, pinIndex, PortBaseAddr;

	/*ChannelID = (0:7)(8:15)(16:23)(24:31)(32:39)(40:47) for PORT(A)(B)(C)(D)(E)(F)*/
	portIndex = ChannelId / CHANNEL_SIZE_IN_PORT;//(0, 1, 2, 3, 4, 5)
	pinIndex = ChannelId % CHANNEL_SIZE_IN_PORT;//(0 -> 7)
	PortBaseAddr = Port_BaseAddr[portIndex];
	/*
		PORT MASK is ADDR[2:9] so offset 4=0b100 corresponds to bit0 mask
		4*(2^[0:7]) = (4=0b100, 8=0b1000, 16=0b10000, 32=0b100000, 64=0b1000000, 128,=0b10000000 256=0b100000000, 512=0b1000000000)
		for PORT Mask (0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000)
	*/
	gpioDataOffset = 4 * (1 << pinIndex);
	level = (*((uint32*)(PortBaseAddr + gpioDataOffset))) >> pinIndex;

	return level;
}
/******************************************************************************
* \Syntax          : void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level )        
* \Description     : Writes channel value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId   ID of DIO channel                     
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level )
{
	uint32 gpioDataOffset;
	uint32 portIndex,pinIndex,PortBaseAddr;
	/*ChannelID = (0:7)(8:15)(16:23)(24:31)(32:39)(40:47) for PORT(A)(B)(C)(D)(E)(F)*/
	portIndex = ChannelId / CHANNEL_SIZE_IN_PORT;//(0, 1, 2, 3, 4, 5)
	pinIndex = ChannelId % CHANNEL_SIZE_IN_PORT;//(0 -> 7)
	PortBaseAddr = Port_BaseAddr[portIndex];
	/*
		PORT MASK is ADDR[2:9] so offset 4=0b100 corresponds to bit0 mask
		4*(2^[0:7]) = (4=0b100, 8=0b1000, 16=0b10000, 32=0b100000, 64=0b1000000, 128,=0b10000000 256=0b100000000, 512=0b1000000000)
		for PORT Mask (0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b10000000)
	*/
	gpioDataOffset = 4 * (1 << pinIndex);
	*((uint32 volatile*)(PortBaseAddr + gpioDataOffset)) = (Level << pinIndex);
	
}
/******************************************************************************
* \Syntax          : Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId )       
* \Description     : Reads Port value
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : PortId   ID of DIO port                    
* \Parameters (out): Dio_PortLevelType                                                      
* \Return value:   : Dio_PortLevelType   uint8                                  
*******************************************************************************/
Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId )
{
	Dio_PortLevelType portLevel;
	uint32 gpioDataOffset = 0X3FC; // 0x3FC = 0b1111111100
	uint32 PortBaseAddr;
	PortBaseAddr = Port_BaseAddr[PortId];
	portLevel = *((uint32*)(PortBaseAddr + gpioDataOffset));
	
	return portLevel;
}
/******************************************************************************
* \Syntax          : void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level );       
* \Description     : writes Port value                                         
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : PortId: ID of DIO port, Level: Value to be written                    
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level )
{
	uint32 gpioDataOffset = 0x3FC;// 0x3FC = 0b1111111100
	uint32 PortBaseAddr;
	PortBaseAddr = Port_BaseAddr[PortId];
	*((uint32*)(PortBaseAddr+gpioDataOffset)) = Level;
}
/******************************************************************************
* \Syntax          : Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)     
* \Description     : writes Port value                                                                
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : ChannelId: ID of DIO channel                   
* \Parameters (out): Dio_LevelType                                                      
* \Return value:   : Dio_LevelType:	STD_LOW, STD_HIGH                                 
*******************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
	Dio_LevelType levelType;
	if (Dio_ReadChannel(ChannelId) == STD_LOW)
	{
		Dio_WriteChannel(ChannelId,STD_HIGH);
		levelType = STD_HIGH;
	}
	else if (Dio_ReadChannel(ChannelId) == STD_HIGH)
	{
		Dio_WriteChannel(ChannelId,STD_LOW);
		levelType = STD_LOW;
	}
	
	return levelType;
}
/**********************************************************************************************************************
 *  END OF FILE: Dio.c
 *********************************************************************************************************************/
