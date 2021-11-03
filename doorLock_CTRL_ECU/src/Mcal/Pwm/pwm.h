/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Header file for the AVR PWM driver. Till now the driver supports
 * 			    only fast pwm. Phase correct pwm is not supported yet
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __PWM_H__
#define __PWM_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* config file */
#include "pwm-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* indicates success of pwm enable or disable */
#define PWM_SUCCESS					1

/* indicates an error during pwm enable or disable, whether pwm pin not defined
   or dutyCycle is not within the range */
#define PWM_ERROR 					0

/* available values for PWM 0 prescalers */
#define PWM_0_PRESCALLER_1			1
#define PWM_0_PRESCALLER_8			2
#define PWM_0_PRESCALLER_64			3
#define PWM_0_PRESCALLER_256		4
#define PWM_0_PRESCALLER_1024		5

/* available values for PWM 1A and PWM 1B prescalers */
#define PWM_1_PRESCALLER_1			1
#define PWM_1_PRESCALLER_8			2
#define PWM_1_PRESCALLER_64			3
#define PWM_1_PRESCALLER_256		4
#define PWM_1_PRESCALLER_1024		5

/* available values for PWM 2 prescalers */
#define PWM_2_PRESCALLER_1			1
#define PWM_2_PRESCALLER_8			2
#define PWM_2_PRESCALLER_32			3
#define PWM_2_PRESCALLER_64			4
#define PWM_2_PRESCALLER_128		5
#define PWM_2_PRESCALLER_256		6
#define PWM_2_PRESCALLER_1024		7

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: PWM_enable
 * [Function Description]: Initialize pwm on the passed pin
 * 						   (if supported) with the percent specified
 * [Args]:
 * [in]: uint8_t pin
 * 		 pin to enable pwm on if supported (only PWM0, PWM1A, PWM1B, PWM2)
 * [in]: uint8_t a_dutyCycle
 * 		 duty cycle (on time percent) to set
 * 		 min = 0, max = 100
 * [Return]: uint8_t
 * 			 PWM_SUCCESS if pin supports pwm and dutyCycle is within the range
 * 			 or PWM_ERROR otherwise
 */
uint8_t PWM_enable(uint8_t a_pin, uint8_t a_dutyCycle);

/*
 * [Function Name]: PWM_disable
 * [Function Description]: disable pwm signal on the specified pin
 * 						   (if supported). An additional step is made in
 * 						   case of PWM 0 or PWM 2 pins, which is stoping the
 * 						   timer to reduce power consumption.
 * 						   This's not made in PWM 1 because TIMER_1 has two
 * 						   channels, so, disabling timer will disable both pwm channels
 * [Args]:
 * [in]: uint8_t pin
 * 		 pin to disable pwm on if supported (only PWM0, PWM1A, PWM1B, PWM2)
 * [Return]: uint8_t
 * 			 PWM_SUCCESS if pin supports pwm and dudtyCycle is within the range
 * 			 or PWM_ERROR otherwise
 */
uint8_t PWM_disable(uint8_t a_pin);

#endif /* __PWM_H__ */
