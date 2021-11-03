/******************************************************************************
 *
 * Project Name			: 	Door Lock System - HMI ECU
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

/* lcd text */
#define DOOR_LOCK_TEXT						"DOOR LOCK SYSTEM"
#define READ_NEW_PASS_TEXT					"Enter a new Pass"
#define CONFIRM_NEW_PASS_TEXT				"Confirm Pass"
#define PASS_MISMATCH_TEXT					"Pass Mismatch"
#define PASS_CHANGED_TEXT					"Pass Changed"
#define MENU_OPTIONS_UPPER_TEXT				"+: Open Door"
#define MENU_OPTIONS_LOWER_TEXT				"-: Change Pass"
#define ENTER_PASS_TEXT						"Enter Pass :"
#define WRONG_PASS_TEXT						"Wrong Pass"
#define ACCESS_DENIED_TEXT					"ACCESS DENIED"
#define DOOR_UNLOCKING_TEXT					"Unlocking Door"
#define DOOR_LOCKING_TEXT					"Locking Door"
#define DOOR_IS_UNLOCKED_TEXT				"Door is Unlocked"

/* passwrod display character */
#define PASS_DISPLAY_CHAR					'*'

/* lcd start positions */
#define FIRST_LINE_START_POS				0, 0
#define SECOND_LINE_START_POS				1, 0

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_AwaitOptions
 * [Enum Description]: contains await states, whether to await receive interrupt
 * 					   or await nothing
 */
typedef enum
{
	/* no await */
	AWAIT_NOTHING,

	/* await uart receive interrupt */
	AWAIT_RESPONSE,

}EN_AwaitOptions;

/*
 * [Enum Name]: EN_AppStates
 * [Enum Description]: contains app states
 */
typedef enum
{

	/* the state of the program after establishing
	 * the connection with the other MCU
	 */
	RECEIVE_COMMAND_STATE,

	/* state during reading any password */
	READING_PASS_STATE,

	READING_MENU_OPTIONS_STATE

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
