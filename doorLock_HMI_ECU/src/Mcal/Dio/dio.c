/******************************************************************************
 *
 * Module: DIO
 *
 * File Name: dio.c
 *
 * Description: Source file for the AVR DIO driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "dio.h"

/* For using mcu registers */
#include "../Mcu/mcu.h"

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

/*
 * [Function Name]: DIO_portInit
 * [Function Description]: Initialize the whole port as input or output
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number to be initialized
 * [in]: DIO_PORTDirectionType a_direction
 * 		 the direction of the port PORT_INPUT or PORT_OUTPUT
 * [Return]: void
 */
void DIO_portInit(uint8_t a_port, DIO_PORTDirectionType a_direction)
{
	if(DIO_PORT_IS_VALID(a_port))
	{
		GET_DDR_FROM_PORT_NO(a_port) = a_direction;
	}
}

/*
 * [Function Name]: DIO_portInitPartial
 * [Function Description]: Initialize only selected pins of the port as input or output
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number
 * [in]: DIO_PORTDirectionType a_direction
 * 		 the direction of the port pins PORT_INPUT or PORT_OUTPUT
 * [in]: uint8_t a_mask
 * 		 mask to select pins to affect
 * [in]: uint8_t a_mask
 * 		 the pin to start writing from, it represents the shift amount
 * [Return]: void
 */
void DIO_portInitPartial(uint8_t a_port, DIO_PORTDirectionType a_direction, uint8_t a_mask, uint8_t a_startPin)
{
	uint8_t startPinNumber = GET_PIN_NO(a_startPin);
	if(DIO_PORT_IS_VALID(a_port))
	{
		COPY_BITS(GET_DDR_FROM_PORT_NO(a_port), a_mask, a_direction, startPinNumber);
	}
}

/*
 * [Function Name]: DIO_pinInit
 * [Function Description]: Initialize a pin as input or output
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to be initialized
 * [in]: DIO_PORTDirectionType a_direction
 * 		 the direction of the pin PIN_INPUT or PIN_OUTPUT
 * [Return]: void
 */
void  DIO_pinInit(uint8_t a_pin, DIO_PinDirectionType a_direction)
{
	uint8_t portNo, pinNo;
	portNo = GET_PORT_NO(a_pin);
	pinNo = GET_PIN_NO(a_pin);
	if(DIO_PORT_IS_VALID(portNo) && DIO_PIN_IS_VALID(pinNo))
	{
		if(a_direction == PIN_INPUT)
		{
			CLEAR_BIT(GET_DDR_FROM_PORT_NO(portNo), pinNo);
		}
		else
		{
			SET_BIT(GET_DDR_FROM_PORT_NO(portNo), pinNo);
		}
	}
}

/*
 * [Function Name]: DIO_writePort
 * [Function Description]: write an 8-bit data to the whole port if
						   the port is output, or control the internal
						   pullup resistor if the port is input
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number to write to
 * [in]: uint8_t a_data
 * 		 the data to be written
 * 		 if a pin is input the corrsponding bit in the a_data controls
 * 		 the internal pullup resistor, 1 => enable, 0 => disable
 * [Return]: void
 */
void DIO_writePort(uint8_t a_port, uint8_t a_data)
{
	if(DIO_PORT_IS_VALID(a_port))
	{
		GET_PORT_FROM_PORT_NO(a_port) = a_data;
	}
}

/*
 * [Function Name]: DIO_writePortPartial
 * [Function Description]: write to specific pins in the port without affecting others
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number
 * [in]: uint8_t a_data
 * 		 data to be written to the port after applying the mask to it
 * [in]: uint8_t a_mask
 * 		 mask applied to the data to select only specific number of bits
 * 		 bits with ones in the corresponding locations only will be written
 * [in]: uint8_t a_startPin
 * 		 the pin to start writing from, it represents the shift amount
 * [Return]: void
 */
void DIO_writePortPartial(uint8_t a_port, uint8_t a_data, uint8_t a_dataMask, uint8_t a_startPin)
{
	uint8_t startPinNumber = GET_PIN_NO(a_startPin);
	if(DIO_PORT_IS_VALID(a_port))
	{
		COPY_BITS(GET_PORT_FROM_PORT_NO(a_port), a_dataMask, a_data, startPinNumber);
	}
}

/*
 * [Function Name]: DIO_writePin
 * [Function Description]: write 1 or 0 to a pin if the pin is output
 * 						   and control the internal pullup resistor if the
 * 						   pin is input
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to write to
 * [in]: uint8_t a_data
 * 		 the data to be written if the pin is output, HIGH or LOW
 * 		 or if the pin is input, it controls the pin internal pullup
 * 		 HIGH => enable, LOW => disable
 * [Return]: void
 */
void DIO_writePin(uint8_t a_pin, uint8_t a_data)
{
	uint8_t portNo, pinNo;
	portNo = GET_PORT_NO(a_pin);
	pinNo = GET_PIN_NO(a_pin);
	if(DIO_PORT_IS_VALID(portNo) && DIO_PIN_IS_VALID(pinNo))
	{
		if(a_data == LOW)
		{
			CLEAR_BIT(GET_PORT_FROM_PORT_NO(portNo), pinNo);
		}
		else
		{
			SET_BIT(GET_PORT_FROM_PORT_NO(portNo), pinNo);
		}
	}
}

/*
 * [Function Name]: DIO_readPort
 * [Function Description]: read the value of the port
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the port number to read from
 * [Return]: uint8_t
 * 			 the data read from the port
 */
uint8_t DIO_readPort(uint8_t a_port)
{
	if(DIO_PORT_IS_VALID(a_port))
	{
		return GET_PIN_FROM_PORT_NO(a_port);
	}
	return 0;
}

/*
 * [Function Name]: DIO_readPin
 * [Function Description]: read the value of the pin
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to read from
 * [Return]: uint8_t
 * 			 the data read from the pin HIGH or LOW
 */
uint8_t DIO_readPin(uint8_t a_pin)
{
	uint8_t portNo, pinNo;
	portNo = GET_PORT_NO(a_pin);
	pinNo = GET_PIN_NO(a_pin);
	if(DIO_PORT_IS_VALID(portNo) && DIO_PIN_IS_VALID(pinNo))
	{
		return GET_BIT(GET_PIN_FROM_PORT_NO(portNo), pinNo);
	}
	return 0;
}

/*
 * [Function Name]: DIO_togglePort
 * [Function Description]: toggle all pins in a port
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number to toggle
 * [Return]: void
 */
void DIO_togglePort(uint8_t a_port)
{
	if(DIO_PORT_IS_VALID(a_port))
	{
		GET_PORT_FROM_PORT_NO(a_port) ^= ALL_HIGH;
	}
}

/*
 * [Function Name]: DIO_togglePin
 * [Function Description]: toggle a pin in the port
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to toggle
 * [Return]: void
 */
void DIO_togglePin(uint8_t a_pin)
{
	uint8_t portNo, pinNo;
	portNo = GET_PORT_NO(a_pin);
	pinNo = GET_PIN_NO(a_pin);
	if(DIO_PORT_IS_VALID(portNo) && DIO_PIN_IS_VALID(pinNo))
	{
		TOGGLE_BIT(GET_PORT_FROM_PORT_NO(portNo), pinNo);
	}
}

/*
 * [Function Name]: DIO_controlPinInternalPull
 * [Function Description]: control pin internall pull,
 * 						   whether not internally pulled or pulled up
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to control the pull
 * [in]: DIO_InternalPullOptions a_pull
 * 		 the internal pull option
 * [Return]: uint8_t
 * 			 returns DIO_INTERNAL_PULL_SUPPORTED or DIO_INTERNAL_PULL_NOT_SUPPORTED
 */
uint8_t DIO_controlPinInternalPull(uint8_t a_pin, DIO_InternalPullOptions a_pull)
{
	switch(a_pull)
	{
	case DIO_PULL_UP:
		DIO_writePin(a_pin, HIGH);
		break;
	case DIO_NO_PULL:
		DIO_writePin(a_pin, LOW);
		break;
	default:
		return DIO_INTERNAL_PULL_NOT_SUPPORTED;
	}
	return DIO_INTERNAL_PULL_SUPPORTED;
}

/*
 * [Function Name]: DIO_controlPinInternalPull
 * [Function Description]: control port internall pull,
 * 						   whether not internally pulled or pulled up
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number to control the pull
 * [in]: DIO_InternalPullOptions a_pull
 * 		 the internal pull option
 * [Return]: uint8_t
 * 			 returns DIO_INTERNAL_PULL_SUPPORTED or DIO_INTERNAL_PULL_NOT_SUPPORTED
 */
uint8_t DIO_controlPortInternalPull(uint8_t a_port, DIO_InternalPullOptions a_pull)
{
	switch(a_pull)
	{
	case DIO_PULL_UP:
		DIO_writePort(a_port, HIGH);
		break;
	case DIO_NO_PULL:
		DIO_writePort(a_port, LOW);
		break;
	default:
		return DIO_INTERNAL_PULL_NOT_SUPPORTED;
	}
	return DIO_INTERNAL_PULL_SUPPORTED;
}
