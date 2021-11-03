 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.h
 *
 * Description: Header file for the KEYPAD driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __KEYPAD_H__
#define __KEYPAD_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "keypad-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_KeypadInternalPull
 * [Enum Description]: contains keypad's internal pull states,
 * 					   using KEYPAD_PULL_UP or KEYPAD_PULL_DOWN, does nothing
 * 					   if they are not supported by the mcu
 */
typedef enum
{
	KEYPAD_NO_PULL,
	KEYPAD_PULL_UP,
	KEYPAD_PULL_DOWN
}EN_KeypadInternalPull;


/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: KEYPAD_init
 * [Function Description]: Initializes the keypad pins
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void KEYPAD_init(void);

/*
 * [Function Name]: KEYPAD_getPressedKey
 * [Function Description]: get the pressed key of the keypad
 * 						   It's a busy wait function that won't
 * 						   return till a button is pressed
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 the ascii of the pressed key
 */
uint8_t KEYPAD_getPressedKey(void);

#endif /* KEYPAD */
