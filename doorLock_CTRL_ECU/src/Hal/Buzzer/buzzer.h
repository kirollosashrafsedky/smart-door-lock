 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __BUZZER_H__
#define __BUZZER_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "buzzer-config.h"

/* for using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* buzzer states */
#define BUZZER_ON			 			 1
#define BUZZER_OFF			 			 0

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/************** BUZZER_init ****************/

#if BUZZERS_USED_COUNT == 1

/*
 * [Function Name]: BUZZER_init
 * [Function Description]: Initializes the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_init(void);

#else

/*
 * [Function Name]: BUZZER_init
 * [Function Description]: Initializes the buzzers
 * [Args]:
 * [in]: const uint8_t * a_buzzerPins
 * 		 array containing all pins connected to buzzers
 * [Return]: void
 */
void BUZZER_init(const uint8_t * a_buzzerPins);

#endif /* BUZZERS_USED_COUNT == 1 */

/************** BUZZER_on ****************/

#if BUZZERS_USED_COUNT == 1

/*
 * [Function Name]: BUZZER_on
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_on(void);

#else

/*
 * [Function Name]: BUZZER_on
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: uint8_t a_buzzerPin
 * 		 pin required to turn on the buzzer connected to it
 * [Return]: void
 */
void BUZZER_on(uint8_t a_buzzerPin);

#endif /* BUZZERS_USED_COUNT == 1 */

/************** BUZZER_off ****************/

#if BUZZERS_USED_COUNT == 1

/*
 * [Function Name]: BUZZER_off
 * [Function Description]: turns on the buzzer
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void BUZZER_off(void);

#else

/*
 * [Function Name]: BUZZER_off
 * [Function Description]: turns off the buzzer
 * [Args]:
 * [in]: uint8_t a_buzzerPin
 * 		 pin required to turn off the buzzer connected to it
 * [Return]: void
 */
void BUZZER_off(uint8_t a_buzzerPin);

#endif /* BUZZERS_USED_COUNT == 1 */

#endif /* __BUZZER_H__ */
