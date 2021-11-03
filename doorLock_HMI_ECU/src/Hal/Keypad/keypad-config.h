 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad-config.h
 *
 * Description: Config file for the KEYPAD driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __KEYPAD_CONFIG_H__
#define __KEYPAD_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS                  	4
#define KEYPAD_NUM_ROWS                  	4

/* Keypad Port */
#define KEYPAD_PORT		                	PORTC

#define KEYPAD_FIRST_ROW_PIN				PC0
#define KEYPAD_FIRST_COL_PIN				PC4

/* Keypad button logic configurations
 * make KEYPAD_BUTTON_PRESSED = LOW if the rows are connected with internal or external pull ups
 * make it = HIGH if they are connected with internal or external pull downs
 */
#define KEYPAD_BUTTON_PRESSED            	LOW

/* keypad rows pull
 * can be 	KEYPAD_NO_PULL, KEYPAD_PULL_UP, KEYPAD_PULL_DOWN
 */
#define KEYPAD_ROWS_INTERNAL_PULL			KEYPAD_PULL_UP

/* if KEYPAD_CHECK_DEBOUNCE_ENABLED = 1 => the module will add a delay
 * of BUTTON_CHECK_DEBOUNCE_DELAY_MS to prevent debounce effect,
 * if KEYPAD_CHECK_DEBOUNCE_ENABLED = 0, the module won't check for debounces
 */
#define KEYPAD_CHECK_DEBOUNCE_ENABLED		1

/* only used if KEYPAD_CHECK_DEBOUNCE_ENABLED = 1, used to specify
 * the time in ms used in check debounce delay
 */
#define KEYPAD_CHECK_DEBOUNCE_DELAY_MS		30

/* Keypad button asciis */
#define KEYPAD_KEY_1			           	'7'
#define KEYPAD_KEY_2			           	'8'
#define KEYPAD_KEY_3			           	'9'
#define KEYPAD_KEY_4			           	'/'
#define KEYPAD_KEY_5			           	'4'
#define KEYPAD_KEY_6			           	'5'
#define KEYPAD_KEY_7			           	'6'
#define KEYPAD_KEY_8			           	'x'
#define KEYPAD_KEY_9			           	'1'
#define KEYPAD_KEY_10			           	'2'
#define KEYPAD_KEY_11			           	'3'
#define KEYPAD_KEY_12			           	'-'
#define KEYPAD_KEY_13			           	'c'
#define KEYPAD_KEY_14			           	'0'
#define KEYPAD_KEY_15			           	'='
#define KEYPAD_KEY_16			           	'+'

#endif /* __KEYPAD_CONFIG_H__ */
