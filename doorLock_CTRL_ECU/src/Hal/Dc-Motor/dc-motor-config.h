 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc-motor-config.h
 *
 * Description: Config file for the DC Motor Bridge driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __DC_MOTOR_CONFIG_H__
#define __DC_MOTOR_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* number of motors used in the project.
 * used as a size for an array holding all initialized motors.
 * If DCMOTORS_USED_COUNT = 1, the functions will not require the motor index
 * so motor config must be set below
 */
#define DCMOTORS_USED_COUNT							1

/* If  DCMOTOR_ENABLE_PIN_IS_CONNECTED = 1, the module will expect an enable pin
 * to be passed in the motor struct.
 * Put this definition = 1, if you want to
 * control the motor speed with pwm
 * Put it to 0 if you don't want to control speed or if pwm is not supported
 */
#define DCMOTOR_ENABLE_PIN_IS_CONNECTED				1

#if DCMOTORS_USED_COUNT == 1

/* the pins the motor is connected to
 * available only if DCDCMOTORS_USED_COUNT = 1
 */
#define DCMOTOR_PIN1								PD6
#define DCMOTOR_PIN2								PD7

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1

#define DCMOTOR_ENABLE_PIN							PD5

#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED = 1 */

#endif /* DCMOTORS_USED_COUNT == 1 */

#endif /* __DC_MOTOR_CONFIG_H__ */
