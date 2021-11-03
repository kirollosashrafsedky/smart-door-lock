 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm-config.h
 *
 * Description: Config file for the AVR PWM driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __PWM_CONFIG_H__
#define __PWM_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Frequency of the pwm output signal can be calculated as follows:
 * ----------------------------------------------------------------
 * pwm 0, pwm 2 => Frequency = F_CPU / (prescaler * 256)
 * pwm 1A, pwm 1B => Frequency = F_CPU / (prescaler * (1 + top))
 *
 * You can control the values of the prescaler or the top values from below
 */

/* prescaler to use with PWM 0 */
/* Available values:
 * 					 PWM_0_PRESCALLER_1   - PWM_0_PRESCALLER_8
 * 					 PWM_0_PRESCALLER_64  - PWM_0_PRESCALLER_256
 * 					 PWM_0_PRESCALLER_1024
 */
#define PWM_0_PRESCALER			PWM_0_PRESCALLER_64

/* prescaler to use with PWM 1A or PWM 1B */
/* Available values:
 * 					 PWM_1_PRESCALLER_1   - PWM_1_PRESCALLER_8
 * 					 PWM_1_PRESCALLER_64  - PWM_1_PRESCALLER_256
 * 					 PWM_1_PRESCALLER_1024
 */
#define PWM_1_PRESCALER			PWM_1_PRESCALLER_64

/* top value to be used with PWM 1A or PWM 1B, can be 16-bit unsigned data */
#define PWM_1_TOP				(255U)

/* prescaler to use with PWM 2 */
/* Available values:
 * 					 PWM_2_PRESCALLER_1   - PWM_2_PRESCALLER_8
 * 					 PWM_2_PRESCALLER_32  - PWM_2_PRESCALLER_64
 * 					 PWM_2_PRESCALLER_128 - PWM_2_PRESCALLER_256
 * 					 PWM_2_PRESCALLER_1024
 */
#define PWM_2_PRESCALER			PWM_2_PRESCALLER_64

#endif /* __PWM_CONFIG_H__ */
