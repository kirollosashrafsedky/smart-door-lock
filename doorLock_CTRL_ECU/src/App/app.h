/******************************************************************************
 *
 * Project Name			: 	Door Lock System - CTRL ECU
 *
 * File Name			: 	app.h
 *
 * Project Description	: 	Door Lock that supports
 * 							-----------------------
 * 							1.  entering 5-char password to unlock the door, and can be changed to any number of chars.
 * 							2.  delete last entered character from the password '-'
 * 							3.  clear the screen during entering the password 'c'
 * 							4.  submiting the password '='
 * 							5.  password is required to unlock door or change password
 * 							6.  for a first-time user, a new password must be entered and confirmed, then it's
 * 							    save in the eeprom so the next time, it shows the menu directly
 * 							7.  password is saved in an external eeprom 'M24C16'
 * 							8.  password can be entered wrong for only 3-times successively, then a buzzer will
 * 							    beep for 1 minute.
 * 							9.  A new password can mismatch its confirmation for 5 times and after that the system
 * 							    returns to home screen
 * 							10. A list of helping messages that appear on the lcd
 * 							11. Can be easily modified to implement one of the sleep modes to reduce power consumption
 *
 * Author				:	Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __APP_H__
#define __APP_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* For using std types */
#include "../Lib/types.h"

/* For using defines */
#include "../Lib/common.h"

/*******************************************************************************
 *                             	  Definitions                                  *
 *******************************************************************************/

/* length of the password
 * the mcu will send SHOW_PASS_RULES_ERROR_TEXT_CMD if the
 * entered password not the same as PASSWORD_LENGTH
 */
#define PASSWORD_LENGTH						5

/* main timer used for delaying some time before
 * sending other commands to the othe MCU */
#define MAIN_TIMER							TIMER_0

/* main timer mode */
#define MAIN_TIMER_MODE						TIMER_0_CTC

/* main timer prescaler */
#define MAIN_TIMER_PRESCALER				TIMER_0_PRESCALER_1024

/* main timer prescaler in numbers - used for calulating total ticks from time */
#define MAIN_TIMER_PRESCALER_NUMBERS		1024

/* default time for displaying any message on the screen */
#define DEFAULT_MSG_TIME_MS					1000

/* time for displaying warning msg on the screen */
#define WARNING_MSG_TIME_MS					60000

/* time for turning the motor on during unlocking */
#define MOTOR_UNLOCK_TIME_MS				15000

/* time for turning the motor on during locking */
#define MOTOR_LOCK_TIME_MS					15000

/* time for turning the motor off between locking and unlocking */
#define MOTOR_HOLD_TIME_MS					3000

/* character responsible for choosing "open door" command */
#define OPEN_DOOR_MENU_CHAR					'+'

/* character responsible for choosing "change pass" command */
#define CHANGE_PASS_MENU_CHAR				'-'

/* range of available chars as password */
#define PASS_ALLOWED_START_CHAR				'0'
#define PASS_ALLOWED_END_CHAR				'9'

/* character responsible for deleting a char from the pass */
#define PASS_BACKSPACE_CHAR					'-'

/* character responsible for cleering the screen during passwrod entry */
#define PASS_CLEAR_SCREEN_CHAR				'c'

/* character responsible for submiting the password */
#define PASS_ENTER_CHAR						'='

/* number of available trials to enter a new password,
 * number of wrong mismatches before returning to main menu
 */
#define NEW_PASSWORD_TRIALS					5

/* number of available traisl for entering a password */
#define PASSWORD_TRIALS						3

/* location of first time checking in the eeprom */
#define FIRST_TIME_CHECK_ADDRESS			0x00

/* location of the first address in the eeprom to store the address */
#define PASSWORD_EEPROM_START_ADDRESS		0x01

/* code for representing that the user is not a "first-time" user */
#define NOT_FIRST_TIME_EEPROM_VAL			0x55

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_AppStates
 * [Enum Description]: contains app states
 */
typedef enum
{
	/* initial state during setting up the connection
	 * with the other MCU, sending initial text to screen
	 * and determining whether it's the first time to use
	 * the app (no passwords in the eeprom) or not, to determine
	 * the next state accordingly
	 */
	PREPAIRING_CONNECTION_STATE,

	/* the main state where the MCU waits for the user's choice */
	MAIN_MENU_STATE,

	/* state for changing password,
	 * whether at the beginning if it's not set or
	 * after the user manually chooses it */
	CHANGE_PASS_STATE,

	/* state of unlocking the door, holding, then locking it */
	OPEN_DOOR_STATE,

	/* state of asking for password to check matching */
	AUTHORIZING_STATE

}EN_AppStates;

/*
 * [Enum Name]: EN_AppCommands
 * [Enum Description]: contains app commands sent and received
 * 					   between the 2 MCUs
 */
typedef enum
{
	/* sent at the beginning and after executing
	 * the previous cmd
	 */
	ACK_CMD = 'A',

	/* show "DOOR LOCK SYSTEM" on lcd */
	SHOW_DOOR_LOCK_TEXT_CMD,

	/* read a new pass, at the beginning or at changing current pass */
	READ_NEW_PASS_CMD,

	/* new password confirmation */
	CONFIRM_PASS_CMD,

	/* receive next pass character */
	READ_NEXT_PASS_CHAR_CMD,

	/* skip pass character, when entering non valid pass character like '+', 'x', '/' */
	SKIP_PASS_CHAR_CMD,

	/* delete last entered pass character, when receiving '-' */
	BACKSPACE_PASS_CHAR_CMD,

	/* clear all entered pass characters, when receiving 'c' */
	CLEAR_ALL_PASS_CHARS_CMD,

	/* show "Password Mismatch" on lcd,
	 * when confirmation pass is different
	 */
	SHOW_PASS_MISMATCH_TEXT_CMD,

	/* show "Password Changed" on lcd */
	SHOW_PASS_CHANGED_CMD,

	/* Show menu and get selected option */
	GET_MENU_OPTION_CMD,

	/* get password from user to verify */
	ENTER_PASS_CMD,

	/* stop receiving other characters, because user reached max pass chars */
	STOP_RECEIVING_PASS_CMD,

	/* show "Wrong Password" on lcd */
	SHOW_WRONG_PASS_TEXT_CMD,

	/* show "Access Denied" on lcd
	 * during buzzer warning
	 */
	SHOW_ACCESS_DENIED_TEXT_CMD,

	/* show "Unlocking Door" on lcd */
	SHOW_DOOR_UNLOCKING_TEXT_CMD,

	/* show "Locking Door" on lcd */
	SHOW_DOOR_LOCKING_TEXT_CMD,

	/* show "Door is Unlocked" on lcd */
	SHOW_DOOR_IS_UNLOCKED_TEXT_CMD,

}EN_AppCommands;

/*
 * [Enum Name]: EN_AwaitOptions
 * [Enum Description]: contains await states, whether to await receive interrupt,
 * 					   timer interrupt, both, or await nothing
 */
typedef enum
{
	/* no await */
	AWAIT_NOTHING,

	/* await uart receive interrupt */
	AWAIT_RESPONSE,

	/* await timer to finish counting */
	AWAIT_TIMER,

	/* await both timer and receive interrupt */
	AWAIT_RESPONSE_AND_TIMER

}EN_AwaitOptions;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * [Function Name]: app_init
 * [Function Description]: contains the app initializations
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void app_init(void);

/*
 * [Function Name]: app_update
 * [Function Description]: contains the app main program
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void app_update(void);

#endif /* __APP_H__ */
