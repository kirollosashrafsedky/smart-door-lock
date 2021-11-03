/******************************************************************************
 *
 * Project Name			: 	Door Lock System - HMI ECU
 *
 * File Name			: 	app.c
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

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* header file */
#include "app.h"

/* For using TIMER Module - delay function */
#include "../Mcal/Timer/timer.h"

/* For using UART Module - communication with the other MCU */
#include "../Mcal/Uart/uart.h"

/* For using KEYPAD Module */
#include "../Hal/Keypad/keypad.h"

/* For using LCD Module */
#include "../Hal/Lcd/lcd.h"

/*******************************************************************************
 *                      Static Functions Prototypes	                           *
 *******************************************************************************/

/*
 * [Function Name]: UartRxCallback
 * [Function Description]: contains the callback routine when
 * 						   an RX interrupt occurs
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void UartRxCallback(void);

/*
 * [Function Name]: controlLcdText
 * [Function Description]: shows different text on lcd depending on
 * 						   received cmd
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void controlLcdText(EN_AppCommands receivedCmd);

/*
 * [Function Name]: readPassword
 * [Function Description]: reads the password from the user and send it
 * 						   to the other MCU char by char till the other MCU
 * 						   sends STOP_RECEIVING_PASS_CMD,
 * 						   each call to the function reads only one char, the other
 * 						   MCU needs to send another cmd for example an ACK
 * 						   to ask for the next char
 *
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void readPassword(EN_AppCommands receivedCmd);

/*******************************************************************************
 *                        Global Variables	                                   *
 *******************************************************************************/

/* states whether new data has been received or not */
static volatile boolean g_isDataReceived = FALSE;

/* current app state */
static EN_AppStates g_state = RECEIVE_COMMAND_STATE;

/* await option, used to wait at the end of every main iteration
 * tell some event happens then procced
 */
static EN_AwaitOptions g_awaitOption = AWAIT_NOTHING;

/* received command from the other MCU */
static volatile EN_AppCommands g_receivedCmd = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * [Function Name]: app_init
 * [Function Description]: contains the app initializations
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void app_init(void)
{
	/* initialize the lcd */
	LCD_init();

	/* initialize the keypad */
	KEYPAD_init();

	/* initialize the uart */
	ST_UartConfig uartConfig = { UART_8_BITS, UART_PARITY_DISABLED,
			UART_STOP_BIT_1, 9600, UART_TX_INTERRUPT_DISABLED,
			UART_RX_INTERRUPT_ENABLED };
	UART_init(&uartConfig);

	/* set uart rx callback */
	UART_setRxInterruptCallback(UartRxCallback);

	/* enable global interrupt */
	ENABLE_GLOBAL_INTERRUPT();

	/* check if the other MCU is ready */
	if (g_receivedCmd == ACK_CMD)
	{
		/* send ready to establish the connection */
		UART_sendByteBlocking(ACK_CMD);
	}
}

/*
 * [Function Name]: app_update
 * [Function Description]: contains the app main program
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void app_update(void)
{
	/* copy the received cmd and use it in case another data was received
	 * during the executing of the app_update, so as to process previous data first
	 */
	uint8_t receivedCmdCpy = g_receivedCmd;

	/* reset await and data receive flag*/
	g_awaitOption = AWAIT_NOTHING;
	g_isDataReceived = FALSE;

	switch (g_state)
	{

	case RECEIVE_COMMAND_STATE:

		/* show text on lcd depending on command */
		controlLcdText(receivedCmdCpy);

		/* manage behavior related to menu or passwords */
		switch (receivedCmdCpy)
		{

		case GET_MENU_OPTION_CMD:
			g_state = READING_MENU_OPTIONS_STATE;
			break;

		case READ_NEW_PASS_CMD:
		case CONFIRM_PASS_CMD:
		case ENTER_PASS_CMD:
			g_state = READING_PASS_STATE;

			/* go to first position in the second line on the lcd to start entering password */
			LCD_setCursor(SECOND_LINE_START_POS);
			break;

		default:

			/* send ack to notify the other mcu that executing the cmd is finished */
			UART_sendByteBlocking(ACK_CMD);

			/* wait for another command */
			g_awaitOption = AWAIT_RESPONSE;
			break;

		}
		break;

		case READING_PASS_STATE:

			/* read a password char */
			readPassword(receivedCmdCpy);
			break;

		case READING_MENU_OPTIONS_STATE:

			if(receivedCmdCpy == ACK_CMD || receivedCmdCpy == GET_MENU_OPTION_CMD)
			{
				/* send user choice from the menu options */
				UART_sendByteBlocking(KEYPAD_getPressedKey());
				g_awaitOption = AWAIT_RESPONSE;

			}
			else
			{
				g_state = RECEIVE_COMMAND_STATE;
			}
			break;

		default:
			break;
	}

	/* await till reponse is received */
	while(g_awaitOption == AWAIT_RESPONSE && !g_isDataReceived);
}

/*
 * [Function Name]: UartRxCallback
 * [Function Description]: contains the callback routine when
 * 						   an RX interrupt occurs
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void UartRxCallback(void)
{
	g_isDataReceived = TRUE;

	/* save received cmd */
	g_receivedCmd = UART_receiveByteNonBlocking();
}

/*
 * [Function Name]: controlLcdText
 * [Function Description]: shows different text on lcd depending on
 * 						   received cmd
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void controlLcdText(EN_AppCommands receivedCmd)
{

	/* clear screen if cmd is known only */
	if (receivedCmd >= ACK_CMD && receivedCmd <= SHOW_DOOR_IS_UNLOCKED_TEXT_CMD)
	{
		/* clear the screen each time */
		LCD_sendCommand(LCD_CLEAR_SCREEN);
	}

	/* select text shown based on the cmd */
	switch (receivedCmd)
	{
	case SHOW_DOOR_LOCK_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) DOOR_LOCK_TEXT);
		break;

	case READ_NEW_PASS_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) READ_NEW_PASS_TEXT);
		break;

	case CONFIRM_PASS_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) CONFIRM_NEW_PASS_TEXT);
		break;

	case SHOW_PASS_MISMATCH_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) PASS_MISMATCH_TEXT);
		break;

	case SHOW_PASS_CHANGED_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) PASS_CHANGED_TEXT);
		break;

	case GET_MENU_OPTION_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) MENU_OPTIONS_UPPER_TEXT);
		LCD_sendStrAt(SECOND_LINE_START_POS,
				(uint8_t*) MENU_OPTIONS_LOWER_TEXT);
		break;

	case ENTER_PASS_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) ENTER_PASS_TEXT);
		break;

	case SHOW_WRONG_PASS_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) WRONG_PASS_TEXT);
		break;

	case SHOW_ACCESS_DENIED_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) ACCESS_DENIED_TEXT);
		break;

	case SHOW_DOOR_UNLOCKING_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) DOOR_UNLOCKING_TEXT);
		break;

	case SHOW_DOOR_LOCKING_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) DOOR_LOCKING_TEXT);
		break;

	case SHOW_DOOR_IS_UNLOCKED_TEXT_CMD:
		LCD_sendStrAt(FIRST_LINE_START_POS, (uint8_t*) DOOR_IS_UNLOCKED_TEXT);
		break;

	default:
		break;

	}
}

/*
 * [Function Name]: readPassword
 * [Function Description]: reads the password from the user and send it
 * 						   to the other MCU char by char till the other MCU
 * 						   sends STOP_RECEIVING_PASS_CMD,
 * 						   each call to the function reads only one char, the other
 * 						   MCU needs to send another cmd for example an ACK
 * 						   to ask for the next char
 *
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void readPassword(EN_AppCommands receivedCmd)
{
	switch(receivedCmd)
	{
	/* cmd to stop receiving chars, so change state */
	case STOP_RECEIVING_PASS_CMD:
		g_state = RECEIVE_COMMAND_STATE;
		UART_sendByteBlocking(ACK_CMD);
		break;

	/* smd to delete a char */
	case BACKSPACE_PASS_CHAR_CMD:
		/* move the cursor left, write a space then move it left again to write in the same position */
		LCD_sendCommand(LCD_MOVE_CURSOR_LEFT);
		LCD_sendChar(' ');
		LCD_sendCommand(LCD_MOVE_CURSOR_LEFT);
		break;

	case CLEAR_ALL_PASS_CHARS_CMD:
		/* cmd to clear the screen by writing spaces */
		LCD_sendStrAt(SECOND_LINE_START_POS, (uint8_t *)"                ");
		LCD_setCursor(SECOND_LINE_START_POS);
		break;

	case READ_NEXT_PASS_CHAR_CMD:
		/* show pass hash '*' char on lcd */
		LCD_sendChar(PASS_DISPLAY_CHAR);
		break;

	default:
		break;
	}

	/* get key tell stop cmd is received */
	if(receivedCmd != STOP_RECEIVING_PASS_CMD)
	{
		UART_sendByteBlocking(KEYPAD_getPressedKey());
	}
	g_awaitOption = AWAIT_RESPONSE;
}
