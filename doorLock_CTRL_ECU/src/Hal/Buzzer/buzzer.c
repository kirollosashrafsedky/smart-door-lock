 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "buzzer.h"

/* for using the DIO module */
#include "../../Mcal/Dio/dio.h"

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

#if BUZZERS_USED_COUNT == 1

/*
 * [Function Name]: BUZZER_init
 * [Function Description]: Initializes the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_init(void)
{
	/* init the buzzer pin as output */
	DIO_pinInit(BUZZER_PIN, PIN_OUTPUT);
}

/*
 * [Function Name]: BUZZER_on
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_on(void)
{
	/* write high to the buzzer pin */
	DIO_writePin(BUZZER_PIN, HIGH);
}

/*
 * [Function Name]: BUZZER_off
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_off(void)
{
	/* write low to the buzzer pin */
	DIO_writePin(BUZZER_PIN, LOW);
}

#else

/*
 * [Function Name]: BUZZER_init
 * [Function Description]: Initializes the buzzers
 * [Args]:
 * [in]: const uint8_t * a_buzzerPins
 * 		 array containing all pins connected to buzzers
 * [Return]: void
 */
void BUZZER_init(const uint8_t * a_buzzerPins)
{
	uint8_t loopCounter;

	/* init all pins as outputs */
	for(loopCounter = 0; loopCounter < BUZZERS_USED_COUNT; loopCounter++)
	{
		DIO_pinInit(a_buzzerPins[loopCounter], PIN_OUTPUT);
	}
}

/*
 * [Function Name]: BUZZER_on
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: uint8_t a_buzzerPin
 * 		 pin required to turn on the buzzer connected to it
 * [Return]: void
 */
void BUZZER_on(uint8_t a_buzzerPin)
{
	/* write high to the buzzer pin */
	DIO_writePin(a_buzzerPin, HIGH);
}

/*
 * [Function Name]: BUZZER_off
 * [Function Description]: turns off the buzzer
 * [Args]:
 * [in]: uint8_t a_buzzerPin
 * 		 pin required to turn off the buzzer connected to it
 * [Return]: void
 */
void BUZZER_off(uint8_t a_buzzerPin)
{
	/* write low to the buzzer pin */
	DIO_writePin(a_buzzerPin, LOW);
}

#endif /* BUZZERS_USED_COUNT == 1 */
