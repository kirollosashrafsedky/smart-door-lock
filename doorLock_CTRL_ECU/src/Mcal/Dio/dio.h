 /******************************************************************************
 *
 * Module: DIO
 *
 * File Name: dio.h
 *
 * Description: Header file for the AVR DIO driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __DIO_H__
#define __DIO_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Number of ports in the mcu */
#define DIO_PORTS_NUM							4

/* Number of pins in each port in the mcu */
#define DIO_PINS_PER_PORT_NUM					8

/* Internal pullup support */
#define DIO_IS_INTERNAL_PULLUP_SUPPORTED		1

/* Internal pulldown support */
#define DIO_IS_INTERNAL_PULLDOWN_SUPPORTED		0

/* Internal pull not supported error
 * returned from the functions controlling internal pull
 * if the passed option is not supported
 */
#define DIO_INTERNAL_PULL_NOT_SUPPORTED			0

/* Internal pull supported success
 * returned from the functions controlling internal pull
 * if the passed option is supported
 */
#define DIO_INTERNAL_PULL_SUPPORTED				1

/*******************************************************************************
 *                                Macros                                       *
 *******************************************************************************/

/* Get DDRx register of a specific port */
#define GET_DDR_FROM_PORT_NO(PORT_NO) (*(volatile uint8_t*)((DDR_START_LOC) - (uint8_t)((PORT_NO) * PORTS_OFFSET)))

/* Get PORTx register of a specific port */
#define GET_PORT_FROM_PORT_NO(PORT_NO) (*(volatile uint8_t*)((PORT_START_LOC) - (uint8_t)((PORT_NO) * PORTS_OFFSET)))

/* Get PINx register of a specific port */
#define GET_PIN_FROM_PORT_NO(PORT_NO) (*(volatile uint8_t*)((PIN_START_LOC) - (uint8_t)((PORT_NO) * PORTS_OFFSET)))

/* check if port number is valid */
#define DIO_PORT_IS_VALID(PORT)	((PORT) < DIO_PORTS_NUM)

/* check if pin number is valid */
#define DIO_PIN_IS_VALID(PIN)	((PIN) < DIO_PINS_PER_PORT_NUM)

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: DIO_PinDirectionType
 * [Enum Description]: contains pin directions, whether input or output
 */

typedef enum
{
	PIN_INPUT,
	PIN_OUTPUT
}DIO_PinDirectionType;

/*
 * [Enum Name]: DIO_PORTDirectionType
 * [Enum Description]: contains port directions, whether input or output
 */
typedef enum
{
	PORT_INPUT,
	PORT_OUTPUT = 0xFF
}DIO_PORTDirectionType;

/*
 * [Enum Name]: DIO_InternalPullOptions
 * [Enum Description]: contains supported internal pull options
 */
typedef enum
{
	DIO_NO_PULL,
	DIO_PULL_UP
}DIO_InternalPullOptions;

/*******************************************************************************
 *                           Function Prototypes                               *
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
void DIO_portInit(uint8_t a_port, DIO_PORTDirectionType a_direction);

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
void DIO_portInitPartial(uint8_t a_port, DIO_PORTDirectionType a_direction, uint8_t a_mask, uint8_t a_startPin);

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
void DIO_pinInit(uint8_t a_pin, DIO_PinDirectionType a_direction);

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
void DIO_writePort(uint8_t a_port, uint8_t a_data);

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
void DIO_writePortPartial(uint8_t a_port, uint8_t a_data, uint8_t a_dataMask, uint8_t a_startPin);

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
void DIO_writePin(uint8_t a_pin, uint8_t a_data);

/*
 * [Function Name]: DIO_readPort
 * [Function Description]: read the value of the port
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the port number to read from
 * [Return]: uint8_t
 * 			 the data read from the port
 */
uint8_t DIO_readPort(uint8_t a_port);

/*
 * [Function Name]: DIO_readPin
 * [Function Description]: read the value of the pin
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to read from
 * [Return]: uint8_t
 * 			 the data read from the pin HIGH or LOW
 */
uint8_t DIO_readPin(uint8_t a_pin);

/*
 * [Function Name]: DIO_togglePort
 * [Function Description]: toggle all pins in a port
 * [Args]:
 * [in]: uint8_t a_port
 * 		 the port number to toggle
 * [Return]: void
 */
void DIO_togglePort(uint8_t a_port);

/*
 * [Function Name]: DIO_togglePin
 * [Function Description]: toggle a pin in the port
 * [Args]:
 * [in]: uint8_t a_pin
 * 		 the pin number to toggle
 * [Return]: void
 */
void DIO_togglePin(uint8_t a_pin);

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
uint8_t DIO_controlPinInternalPull(uint8_t a_pin, DIO_InternalPullOptions a_pull);

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
uint8_t DIO_controlPortInternalPull(uint8_t a_port, DIO_InternalPullOptions a_pull);

#endif /* __DIO_H__ */
