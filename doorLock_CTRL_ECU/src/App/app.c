/******************************************************************************
 *
 * Project Name			: 	Door Lock System - CTRL ECU
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

/* For using TIMER Module */
#include "../Mcal/Timer/timer.h"

/* For initializing the TWI Module */
#include "../Mcal/Twi/twi.h"

/* For using UART Module - communication with the other MCU */
#include "../Mcal/Uart/uart.h"

/* For using BUZZER Module */
#include "../Hal/Buzzer/buzzer.h"

/* For using EXTERNAL EEPROM Module */
#include "../Hal/External-Eeprom/external-eeprom.h"

/* For using MOTOR Module */
#include "../Hal/Dc-Motor/dc-motor.h"

/*******************************************************************************
 *                      Static Functions Prototypes	                           *
 *******************************************************************************/

/*
 * [Function Name]: uartRxCallback
 * [Function Description]: contains the callback routine when
 * 						   an RX interrupt occurs
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void uartRxCallback(void);

/*
 * [Function Name]: mainTimerCallback
 * [Function Description]: contains the callback routine when
 * 						   the main timer finishes counting,
 * 						   it stops the timer before returning
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void mainTimerCallback(void);

/*
 * [Function Name]: setAppState
 * [Function Description]: set the current and the previous app state
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void setAppState(EN_AppStates a_state);

/*
 * [Function Name]: establishConnection
 * [Function Description]: establishes the connection between the two MCUs
 * 						   and check if the user is a first-time user
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void establishConnection(void);

/*
 * [Function Name]: showMainMenu
 * [Function Description]: displays the main menu and process the choice
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void showMainMenu(void);

/*
 * [Function Name]: changePass
 * [Function Description]: responsible for changing the password, whether at the
 * 						   beginning or during running the program
 * [Args]:
 * [in]: boolean a_isAuthRequired
 * 		 if TRUE, a password is required,
 * 		 otherwise a password is not required, like in the case of a first-time user
 * 		 or if the password is already entered
 * [Return]: void
 */
static void changePass(boolean a_isAuthRequired);

/*
 * [Function Name]: openDoor
 * [Function Description]: responsible for locking and unlocking the door
 * [Args]:
 * [in]: boolean a_isAuthRequired
 * 		 if TRUE, a password is required,
 * 		 otherwise a password is not required, if the password is already entered
 * [Return]: void
 */
static void openDoor(boolean a_isAuthRequired);

/*
 * [Function Name]: auth
 * [Function Description]: acts as a middleware between states, verifies that the entered
 * 						   pass is correct, moves to the next state if so,
 * 						   or runs the warning routine if the max number of wrong passwords is reached
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void auth(void);

/*
 * [Function Name]: readPassword
 * [Function Description]: read a password from the user and handles user input,
 * 						   whether deleting char, clearing screen, submiting pass and so on
 * [Args]:
 * [in]: uint8_t * password
 * 		 empty array to store the password
 * [Return]: boolean
 * 			 TRUE if the user has already finished entering the password
 * 			 FALSE otherwise
 */
static boolean readPassword(uint8_t * password);

/*
 * [Function Name]: comparePasswords
 * [Function Description]: compare passwords and return true or false depending
 * 						   on matching or mismatching
 * [Args]:
 * [in]: uint8_t * pass1
 * 		 first pass of the comparison
 * [in]: uint8_t * pass2
 * 		 second pass of the comparison
 * [Return]: boolean
 * 			 TRUE if passwords are identical, FALSE otherwise
 */
static boolean comparePasswords(uint8_t * pass1, uint8_t * pass2);

/*
 * [Function Name]: savePassToEeprom
 * [Function Description]: save password to eeprom
 * [Args]:
 * [in]: uint8_t * password
 * 		 password to save
 * [Return]: void
 */
static void savePassToEeprom(uint8_t * password);

/*
 * [Function Name]: readPassFromEeprom
 * [Function Description]: read password from eeprom
 * [Args]:
 * [in]: uint8_t * password
 * 		 empty array to store the read password
 * [Return]: void
 */
static void readPassFromEeprom(uint8_t * password);

/*******************************************************************************
 *                        Global Variables	                                   *
 *******************************************************************************/

/* g_isDataReceived => states whether new data has been received or not
 * g_hasMainTimerFinished => states whether the timer has finished counting or not
 * g_firstTime => states whether the app is running for the first time where no pass is set or not
 */
static volatile boolean g_isDataReceived = FALSE,
		g_hasMainTimerFinished = FALSE,
		g_firstTime = FALSE;

/* g_currentState => current app state
 * g_previousState => previous app state, used when previous states are required to determine the next state
 */
static EN_AppStates g_currentState = PREPAIRING_CONNECTION_STATE,
		g_previousState = PREPAIRING_CONNECTION_STATE;

/* g_innerState => state used inside functions to mark function progress
 * g_passTrials => number of trials to enter password wrong
 */
static uint8_t g_innerState = 0, g_passTrials = 0;

/* await option, used to wait at the end of every main iteration
 * tell some event happens then procced
 */
static EN_AwaitOptions g_awaitOption = AWAIT_NOTHING;

/* received data from the other MCU */
static volatile uint8_t g_receivedData = 0;

/* main timer config structure - with default zero ticks */
static TIMER_config g_mainTimerConfig = {MAIN_TIMER, MAIN_TIMER_MODE, MAIN_TIMER_PRESCALER, 0, mainTimerCallback};

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
	/* init the buzzer */
	BUZZER_init();

	/* init the dc motor */
	DCMOTOR_init();

	/* init TWI with slave address 0x01, prescaler = 1,
	 * bit rate = 2 so the generated frequency = 400 kbps
	 */
	ST_TwiConfig twiConfig = {
			0x01,
			TWI_PRESCALER_1,
			0x02
	};
	TWI_init(&twiConfig);

	/* initialize the uart */
	ST_UartConfig uartConfig = {
			UART_8_BITS,
			UART_PARITY_DISABLED,
			UART_STOP_BIT_1,
			9600,
			UART_TX_INTERRUPT_DISABLED,
			UART_RX_INTERRUPT_ENABLED
	};
	UART_init(&uartConfig);

	/* set uart rx callback */
	UART_setRxInterruptCallback(uartRxCallback);

	/* enable global interrupt */
	ENABLE_GLOBAL_INTERRUPT();

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
	/* reset await, data receive flag, timer finish flag */
	g_awaitOption = AWAIT_NOTHING;
	g_isDataReceived = FALSE;
	g_hasMainTimerFinished = FALSE;

	/* choose the app behavior depending on the current state */
	switch (g_currentState)
	{
	case PREPAIRING_CONNECTION_STATE:
		/* establish the connection */
		establishConnection();
		break;

	case MAIN_MENU_STATE:
		/* show the main menu */
		showMainMenu();
		break;

	case CHANGE_PASS_STATE:
		/* change password with auth required if not already authorized, or if first-time user  */
		if(g_firstTime || g_previousState == AUTHORIZING_STATE)
		{
			changePass(FALSE);
		}
		else
		{
			changePass(TRUE);
		}
		break;

	case OPEN_DOOR_STATE:
		/* open door with auth except if the user is already authorized */
		if(g_previousState == AUTHORIZING_STATE)
		{
			openDoor(FALSE);
		}
		else
		{
			openDoor(TRUE);
		}
		break;

	case AUTHORIZING_STATE:
		/* authorize the user */
		auth();
		break;

	default:
		break;
	}

	/* await till reponse is received, timer has finished, await both or no await at all */
	while(
			(g_awaitOption == AWAIT_RESPONSE && !g_isDataReceived) ||
			(g_awaitOption == AWAIT_TIMER && !g_hasMainTimerFinished) ||
			(g_awaitOption == AWAIT_RESPONSE_AND_TIMER && (!g_isDataReceived || !g_hasMainTimerFinished))
	);

}

/*
 * [Function Name]: uartRxCallback
 * [Function Description]: contains the callback routine when
 * 						   an RX interrupt occurs
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void uartRxCallback(void)
{
	g_isDataReceived = TRUE;

	/* save the received data */
	g_receivedData = UART_receiveByteNonBlocking();
}

/*
 * [Function Name]: mainTimerCallback
 * [Function Description]: contains the callback routine when
 * 						   the main timer finishes counting,
 * 						   it stops the timer before returning
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void mainTimerCallback(void)
{
	g_hasMainTimerFinished = TRUE;

	/* stop the timer each time the callback is executed */
	TIMER_stop(TIMER_0);
}

/*
 * [Function Name]: setAppState
 * [Function Description]: set the current and the previous app state
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void setAppState(EN_AppStates a_state)
{
	g_previousState = g_currentState;
	g_currentState = a_state;
}

/*
 * [Function Name]: establishConnection
 * [Function Description]: establishes the connection between the two MCUs
 * 						   and check if the user is a first-time user
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void establishConnection(void)
{
	/* store if the user is a first-time user or not */
	uint8_t eepromValue;

	switch(g_innerState)
	{
	case 0:
		do
		{
			/* wait till an ack is received */
			UART_sendByteBlocking(ACK_CMD);
			TIMER_DELAY_MS(50);

		} while (g_receivedData != ACK_CMD);

		/* show "Door lock system" for some time */
		g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, DEFAULT_MSG_TIME_MS);
		TIMER_init(&g_mainTimerConfig);
		TIMER_start(TIMER_0);
		UART_sendByteBlocking(SHOW_DOOR_LOCK_TEXT_CMD);
		g_awaitOption = AWAIT_RESPONSE_AND_TIMER;
		g_innerState ++;

		break;

	case 1:

		/* read eeprom to check if first time or not
		 * if first time => set state to CHANGE_PASS_STATE
		 * else set state to MAIN_MENU_STATE
		 */
		while(EEPROM_readByte(FIRST_TIME_CHECK_ADDRESS, &eepromValue) == EEPROM_ERROR);

		if(eepromValue == NOT_FIRST_TIME_EEPROM_VAL)
		{
			g_firstTime = FALSE;
			setAppState(MAIN_MENU_STATE);
		}
		else
		{
			g_firstTime = TRUE;
			setAppState(CHANGE_PASS_STATE);
		}
		g_innerState = 0;
		break;
	}
}

/*
 * [Function Name]: showMainMenu
 * [Function Description]: displays the main menu and process the choice
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void showMainMenu(void)
{
	switch(g_innerState)
	{
	case 0:
		/* show menu and wait for a response */
		UART_sendByteBlocking(GET_MENU_OPTION_CMD);
		g_awaitOption = AWAIT_RESPONSE;
		g_innerState ++;
		break;

	case 1:

		/* check the option and send an ack if the command is not defined
		 * to wait for another response
		 */
		if(g_receivedData == OPEN_DOOR_MENU_CHAR)
		{
			setAppState(OPEN_DOOR_STATE);
			g_innerState = 0;
		}
		else if(g_receivedData == CHANGE_PASS_MENU_CHAR)
		{
			setAppState(CHANGE_PASS_STATE);
			g_innerState = 0;
		}
		else
		{
			UART_sendByteBlocking(ACK_CMD);
			g_awaitOption = AWAIT_RESPONSE;
		}
		break;
	}
}

/*
 * [Function Name]: changePass
 * [Function Description]: responsible for changing the password, whether at the
 * 						   beginning or during running the program
 * [Args]:
 * [in]: boolean a_isAuthRequired
 * 		 if TRUE, a password is required,
 * 		 otherwise a password is not required, like in the case of a first-time user
 * 		 or if the password is already entered
 * [Return]: void
 */
static void changePass(boolean a_isAuthRequired)
{
	/* store new entered pass and confirmation pass */
	static uint8_t newPass[PASSWORD_LENGTH], confirmationPass[PASSWORD_LENGTH];

	switch(g_innerState)
	{
	case 0:
		/* set trials to 0 at the beginning */
		g_passTrials = 0;
		g_innerState ++;
		break;

	case 1:
		if(a_isAuthRequired)
		{
			/* go authorize the user first if auth is required */
			setAppState(AUTHORIZING_STATE);
		}
		else
		{
			/* show "enter new pass" and wait for response */
			UART_sendByteBlocking(READ_NEW_PASS_CMD);
			g_awaitOption = AWAIT_RESPONSE;
			g_innerState ++;
		}
		break;

	case 2:
		if(readPassword(newPass))
		{
			/* user has finished entering the pass */
			UART_sendByteBlocking(CONFIRM_PASS_CMD);
			g_innerState ++;
		}
		g_awaitOption = AWAIT_RESPONSE;
		break;

	case 3:
		if(readPassword(confirmationPass))
		{
			/* user has finished entering the pass confirmation */
			if(comparePasswords(newPass, confirmationPass))
			{
				/* save pass to eeprom if two passwords match */
				savePassToEeprom(confirmationPass);

				if(g_firstTime)
				{
					/* write to the eeprom that the user is not a first-time user */
					while(EEPROM_writeByte(FIRST_TIME_CHECK_ADDRESS, NOT_FIRST_TIME_EEPROM_VAL) == EEPROM_ERROR);
					g_firstTime = FALSE;
				}

				/* show "pass changed" */
				UART_sendByteBlocking(SHOW_PASS_CHANGED_CMD);
				g_innerState ++;
			}
			else
			{
				/* show "pass mismatch" */
				UART_sendByteBlocking(SHOW_PASS_MISMATCH_TEXT_CMD);

				/* check the trials and return to menu if NEW_PASSWORD_TRIALS is reached */
				if(!g_firstTime)
				{
					g_passTrials ++;
				}
				if(g_passTrials < NEW_PASSWORD_TRIALS)
				{
					g_innerState = 1;
				}
				else
				{
					g_innerState ++;
				}

			}

			/* wait for the specified msg to be showed on the screen for some time */
			g_awaitOption = AWAIT_RESPONSE_AND_TIMER;
			g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, DEFAULT_MSG_TIME_MS);
			TIMER_init(&g_mainTimerConfig);
			TIMER_start(TIMER_0);

		}
		break;

	case 4:
		/* go to main menu */
		setAppState(MAIN_MENU_STATE);
		g_innerState = 0;
		break;

	}
}

/*
 * [Function Name]: openDoor
 * [Function Description]: responsible for locking and unlocking the door
 * [Args]:
 * [in]: boolean a_isAuthRequired
 * 		 if TRUE, a password is required,
 * 		 otherwise a password is not required, if the password is already entered
 * [Return]: void
 */
static void openDoor(boolean a_isAuthRequired)
{
	switch(g_innerState)
	{
	case 0:
		/* go to authorization if auth is required */
		if(a_isAuthRequired)
		{
			setAppState(AUTHORIZING_STATE);
		}
		else
		{
			/* four inner states controlling unlocking, holding and locking the door rescpectively */
			g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, MOTOR_UNLOCK_TIME_MS);
			TIMER_init(&g_mainTimerConfig);
			TIMER_start(TIMER_0);
			DCMOTOR_start(DCMOTOR_FORWARD, 50);
			UART_sendByteBlocking(SHOW_DOOR_UNLOCKING_TEXT_CMD);
			g_awaitOption = AWAIT_TIMER;
			g_innerState ++;
		}
		break;


	case 1:
		g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, MOTOR_HOLD_TIME_MS);
		TIMER_init(&g_mainTimerConfig);
		TIMER_start(TIMER_0);
		DCMOTOR_stop();
		UART_sendByteBlocking(SHOW_DOOR_IS_UNLOCKED_TEXT_CMD);
		g_awaitOption = AWAIT_TIMER;
		g_innerState ++;
		break;

	case 2:
		g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, MOTOR_LOCK_TIME_MS);
		TIMER_init(&g_mainTimerConfig);
		TIMER_start(TIMER_0);
		DCMOTOR_start(DCMOTOR_REVERSE, 50);
		UART_sendByteBlocking(SHOW_DOOR_LOCKING_TEXT_CMD);
		g_awaitOption = AWAIT_TIMER;
		g_innerState ++;
		break;

	case 3:
		DCMOTOR_stop();
		setAppState(MAIN_MENU_STATE);
		g_innerState = 0;
		break;
	}
}

/*
 * [Function Name]: auth
 * [Function Description]: acts as a middleware between states, verifies that the entered
 * 						   pass is correct, moves to the next state if so,
 * 						   or runs the warning routine if the max number of wrong passwords is reached
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void auth(void)
{
	/* will be removed */
	uint8_t rightPass[PASSWORD_LENGTH];

	static uint8_t pass[PASSWORD_LENGTH];

	switch(g_innerState)
	{
	case 0:
		g_passTrials = 0;
		g_innerState ++;
		break;
	case 1:
		/* show "Enter Pass" and wait for response */
		UART_sendByteBlocking(ENTER_PASS_CMD);
		g_awaitOption = AWAIT_RESPONSE;
		g_innerState ++;

		break;

	case 2:
		/* wait till password is entered */
		if(readPassword(pass))
		{
			g_innerState ++;
		}
		break;

	case 3:
		/* read pass from eeprom */
		readPassFromEeprom(rightPass);

		/* compare both passwords */
		if(comparePasswords(pass, rightPass))
		{
			/* go to the previous state if password is true */
			setAppState(g_previousState);
			g_innerState = 0;
		}
		else
		{
			/* show "password is wrong" and increase the trials counter */
			g_passTrials ++;
			if(g_passTrials < PASSWORD_TRIALS)
			{
				UART_sendByteBlocking(SHOW_WRONG_PASS_TEXT_CMD);
				g_innerState = 1;
				g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, DEFAULT_MSG_TIME_MS);
				TIMER_init(&g_mainTimerConfig);
				TIMER_start(TIMER_0);
				g_awaitOption = AWAIT_RESPONSE_AND_TIMER;
			}
			else
			{
				g_innerState ++;
			}
		}

		break;

	case 4:
		/* password max trials has reached, turn on buzzer and show "Access Denied" */
		g_mainTimerConfig.ticks = TIME_MS_TO_TICKS(MAIN_TIMER_PRESCALER_NUMBERS, WARNING_MSG_TIME_MS);
		TIMER_init(&g_mainTimerConfig);
		TIMER_start(TIMER_0);
		BUZZER_on();
		UART_sendByteBlocking(SHOW_ACCESS_DENIED_TEXT_CMD);
		g_awaitOption = AWAIT_TIMER;
		g_innerState ++;
		break;

	case 5:
		/* turn off the buzzer */
		BUZZER_off();
		setAppState(MAIN_MENU_STATE);
		g_innerState = 0;
		break;
	}
}

/*
 * [Function Name]: readPassword
 * [Function Description]: read a password from the user and handles user input,
 * 						   whether deleting char, clearing screen, submiting pass and so on
 * [Args]:
 * [in]: uint8_t * password
 * 		 empty array to store the password
 * [Return]: boolean
 * 			 TRUE if the user has already finished entering the password
 * 			 FALSE otherwise
 */
static boolean readPassword(uint8_t * password)
{
	static uint8_t passIndex = 0, passInnerState = 0;

	switch(passInnerState)
	{
	case 0:
		/* delete a character from the password */
		if(g_receivedData == PASS_BACKSPACE_CHAR && passIndex != 0)
		{
			passIndex --;
			UART_sendByteBlocking(BACKSPACE_PASS_CHAR_CMD);
		}
		/* clear password */
		else if(g_receivedData == PASS_CLEAR_SCREEN_CHAR)
		{
			passIndex = 0;
			UART_sendByteBlocking(CLEAR_ALL_PASS_CHARS_CMD);
		}
		/* save entered char to password if it's a number */
		else if(passIndex != PASSWORD_LENGTH && g_receivedData >= PASS_ALLOWED_START_CHAR && g_receivedData <= PASS_ALLOWED_END_CHAR)
		{
			password[passIndex] = g_receivedData;
			passIndex ++;
			UART_sendByteBlocking(READ_NEXT_PASS_CHAR_CMD);
		}
		/* submit entered pass */
		else if(passIndex == PASSWORD_LENGTH && g_receivedData == PASS_ENTER_CHAR)
		{
			passIndex = 0;
			UART_sendByteBlocking(STOP_RECEIVING_PASS_CMD);
			passInnerState ++;
		}
		/* undefined char, skip it */
		else
		{
			UART_sendByteBlocking(SKIP_PASS_CHAR_CMD);
		}

		/* await next char */
		g_awaitOption = AWAIT_RESPONSE;

		return FALSE;
		break;

	case 1:
		passInnerState = 0;
		return TRUE;
		break;
	}
	return FALSE;
}

/*
 * [Function Name]: comparePasswords
 * [Function Description]: compare passwords and return true or false depending
 * 						   on matching or mismatching
 * [Args]:
 * [in]: uint8_t * pass1
 * 		 first pass of the comparison
 * [in]: uint8_t * pass2
 * 		 second pass of the comparison
 * [Return]: boolean
 * 			 TRUE if passwords are identical, FALSE otherwise
 */
static boolean comparePasswords(uint8_t * pass1, uint8_t * pass2)
{
	uint8_t passIndex;

	/* looping through the passwords and return false when meeting a mismatch, otherwise return true */
	for(passIndex = 0; passIndex < PASSWORD_LENGTH; passIndex ++)
	{
		if(pass1[passIndex] != pass2[passIndex])
			return FALSE;
	}
	return TRUE;
}

/*
 * [Function Name]: savePassToEeprom
 * [Function Description]: save password to eeprom
 * [Args]:
 * [in]: uint8_t * password
 * 		 password to save
 * [Return]: void
 */
static void savePassToEeprom(uint8_t * password)
{
	uint8_t passIndex;

	for(passIndex = 0; passIndex < PASSWORD_LENGTH; passIndex ++)
	{
		/* write char by char to eeprom */
		while(EEPROM_writeByte(PASSWORD_EEPROM_START_ADDRESS + passIndex, password[passIndex]) == EEPROM_ERROR);
		TIMER_DELAY_MS(20);
	}
}

/*
 * [Function Name]: readPassFromEeprom
 * [Function Description]: read password from eeprom
 * [Args]:
 * [in]: uint8_t * password
 * 		 empty array to store the read password
 * [Return]: void
 */
static void readPassFromEeprom(uint8_t * password)
{
	uint8_t passIndex;

	for(passIndex = 0; passIndex < PASSWORD_LENGTH; passIndex ++)
	{
		/* read char by char from the eeprom */
		while(EEPROM_readByte(PASSWORD_EEPROM_START_ADDRESS + passIndex, &password[passIndex]) == EEPROM_ERROR);
		TIMER_DELAY_MS(20);
	}
}
