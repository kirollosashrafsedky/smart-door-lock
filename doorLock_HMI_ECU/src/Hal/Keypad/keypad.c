/******************************************************************************
 *
 * Module: PUSH BUTTON
 *
 * File Name: button.c
 *
 * Description: Source file for the PUSH BUTTON driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "keypad.h"

/* For using DIO functions */
#include "../../Mcal/Dio/dio.h"

#if KEYPAD_CHECK_DEBOUNCE_ENABLED == 1

/* For using delay function */
#include "../../Mcal/Timer/timer.h"

#endif /* KEYPAD_CHECK_DEBOUNCE_ENABLED == 1 */

/*******************************************************************************
 *                      Static Functions Prototypes	                           *
 *******************************************************************************/

/*
 * [Function Name]: KEYPAD_numberToChar
 * [Function Description]: converts the number of the keypad button clicked to the
 * 						   corresponding char from the config file
 * [Args]:
 * [in]: uint8_t a_number
 * 		 number of the clicked keypad button
 * [Return]: uint8_t
 * 			 corresponding char
 */
static uint8_t KEYPAD_numberToChar(uint8_t a_number);

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

/*
 * [Function Name]: KEYPAD_init
 * [Function Description]: Initializes the keypad pins
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void KEYPAD_init(void)
{

	uint8_t loopCounter, rowMask, colsMask;
	rowMask = 0;
	colsMask = 0;

	/* loop to generate ones in the mask equal to the number of rows
	 * and control the internal pull of the rows */
	for(loopCounter = 0; loopCounter < KEYPAD_NUM_ROWS; loopCounter++)
	{
		rowMask <<= 1;
		rowMask |= 1;
	}

	/* loop to generate ones in the mask equal to the number of cols */
	for(loopCounter = 0; loopCounter < KEYPAD_NUM_COLS; loopCounter++)
	{
		colsMask <<= 1;
		colsMask |= 1;
	}

	/* init row pins as inputs */
	DIO_portInitPartial(KEYPAD_PORT, PORT_INPUT, rowMask, KEYPAD_FIRST_ROW_PIN);

	/* control internal pull of the rows */
	for(loopCounter = 0; loopCounter < KEYPAD_NUM_ROWS; loopCounter++)
	{
		DIO_controlPinInternalPull(KEYPAD_FIRST_ROW_PIN + loopCounter, KEYPAD_ROWS_INTERNAL_PULL);
	}

	/* init cols pins as outputs */
	DIO_portInitPartial(KEYPAD_PORT, PORT_OUTPUT, colsMask, KEYPAD_FIRST_COL_PIN);
}

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
uint8_t KEYPAD_getPressedKey(void)
{
	uint8_t row, col, colsMask, rowReadValue;

	/* hold the index of the selected row to
	 * use in checking the key is released from the
	 * previous time pefore proceeding
	 */
	static uint8_t selectedRow = 0;

	colsMask = 0;

	while(DIO_readPin(KEYPAD_FIRST_ROW_PIN + selectedRow) == KEYPAD_BUTTON_PRESSED);

	/* loop to generate ones in the mask equal to the number of cols */
	for(col = 0; col < KEYPAD_NUM_COLS; col++)
	{
		colsMask <<= 1;
		colsMask |= 1;
	}

#if KEYPAD_BUTTON_PRESSED == LOW

	/* write ones to all cols pins */
	DIO_writePortPartial(KEYPAD_PORT, ALL_HIGH, colsMask, KEYPAD_FIRST_COL_PIN);

#else

	/* write zeros to all cols pins */
	DIO_writePortPartial(KEYPAD_PORT, ALL_LOW, colsMask, KEYPAD_FIRST_COL_PIN);

#endif

	while(1)
	{
		for (col = 0; col < KEYPAD_NUM_COLS; col ++)
		{
			/* write 0 or 1 to col by col depending on the value of KEYPAD_BUTTON_PRESSED */
			DIO_writePin(KEYPAD_FIRST_COL_PIN + col, KEYPAD_BUTTON_PRESSED);

			for (row = 0; row < KEYPAD_NUM_ROWS; row ++)
			{
				/* read the value of each row */
				rowReadValue = DIO_readPin(KEYPAD_FIRST_ROW_PIN + row);

				if(rowReadValue == KEYPAD_BUTTON_PRESSED)
				{
#if KEYPAD_CHECK_DEBOUNCE_ENABLED == 1

					/* delay for some time to check again - debounce effect */
					TIMER_DELAY_MS(KEYPAD_CHECK_DEBOUNCE_DELAY_MS);

					/* read the row pin again after debounce */
					rowReadValue = DIO_readPin(KEYPAD_FIRST_ROW_PIN + row);

					if(rowReadValue == KEYPAD_BUTTON_PRESSED)
					{
						/* save the value of the selected row */
						selectedRow = row;

						/* convert the number of the pressed key to ascii and return it */
						return KEYPAD_numberToChar((row * KEYPAD_NUM_COLS) + col + 1);
					}

#else

					/* save the value of the selected row */
					selectedRow = row;

					/* convert the number of the pressed key to ascii and return it */
					return KEYPAD_numberToChar((row * KEYPAD_NUM_COLS) + col + 1);

#endif /* KEYPAD_CHECK_DEBOUNCE_ENABLED == 1 */

				}
			}

			/* return the current col pin to its initial state
			 * HIGH if KEYPAD_BUTTON_PRESSED = LOW or
			 * LOW  if KEYPAD_BUTTON_PRESSED = HIGH
			 */
			DIO_writePin(KEYPAD_FIRST_COL_PIN + col, !KEYPAD_BUTTON_PRESSED);
		}
	}
}

/*
 * [Function Name]: KEYPAD_numberToChar
 * [Function Description]: converts the number of the keypad button clicked to the
 * 						   corresponding char from the config file
 * [Args]:
 * [in]: uint8_t a_number
 * 		 number of the clicked keypad button
 * [Return]: uint8_t
 * 			 corresponding char
 */
static uint8_t KEYPAD_numberToChar(uint8_t a_number)
{
	/* key will hold the ascii of each number */
	uint8_t key;

	/* select each key depending on its number */
	switch(a_number)
	{
	case 1:
		key = KEYPAD_KEY_1;
		break;
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 2
	case 2:
		key = KEYPAD_KEY_2;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 3
	case 3:
		key = KEYPAD_KEY_3;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 4
	case 4:
		key = KEYPAD_KEY_4;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 5
	case 5:
		key = KEYPAD_KEY_5;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 6
	case 6:
		key = KEYPAD_KEY_6;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 7
	case 7:
		key = KEYPAD_KEY_7;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 8
	case 8:
		key = KEYPAD_KEY_8;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 9
	case 9:
		key = KEYPAD_KEY_9;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 10
	case 10:
		key = KEYPAD_KEY_10;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 11
	case 11:
		key = KEYPAD_KEY_11;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 12
	case 12:
		key = KEYPAD_KEY_12;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 13
	case 13:
		key = KEYPAD_KEY_13;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 14
	case 14:
		key = KEYPAD_KEY_14;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 15
	case 15:
		key = KEYPAD_KEY_15;
		break;
#endif
#if KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS >= 16
	case 16:
		key = KEYPAD_KEY_16;
		break;
#endif
	}

	/* return the ascii of the key */
	return key;
}
