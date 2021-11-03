 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc-motor.h
 *
 * Description: Header file for the DC Motor Bridge driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "dc-motor-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_DcMotorDirection
 * [Enum Description]: contains motor direction, whether forward or reverse
 */
typedef enum
{
	DCMOTOR_FORWARD,
	DCMOTOR_REVERSE
}EN_DcMotorDirection;

#if DCMOTORS_USED_COUNT != 1

/*
 * [Struct Name]: ST_DCMOTOR
 * [Struct Description]: contains motor pins connection
 * 						 pin1, pin2 and enable pin (if ENABLE_PIN_IS_CONNECTED == 1 only)
 */
typedef struct
{
	uint8_t pin1;
	uint8_t pin2;
#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1
	uint8_t enablePin;
#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED */
}ST_DCMOTOR;

#endif /* DCMOTORS_USED_COUNT != 1 */
/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

#if DCMOTORS_USED_COUNT == 1

/*
 * [Function Name]: DCMOTOR_init
 * [Function Description]: Initializes the motor pins
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void DCMOTOR_init(void);

/*
 * [Function Name]: DCMOTOR_start
 * [Function Description]: starts the motor with the given direction and speed
 * [Args]:
 * [in]: EN_DcMotorDirection a_direction
 * 		 direction to start the motor with
 * [in]: uint8_t a_speedPercent
 * 		 speed of the motor - ignored if enable pin is not connected
 * [Return]: void
 */
void DCMOTOR_start(EN_DcMotorDirection a_direction, uint8_t a_speedPercent);

/*
 * [Function Name]: DCMOTOR_stop
 * [Function Description]: stops the motor
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void DCMOTOR_stop(void);

#else

/*
 * [Function Name]: DCMOTOR_init
 * [Function Description]: Initializes the motor pins
 * [Args]:
 * [in]: ST_DcMotor *a_motor
 * 		 array of motors structures to init
 * [Return]: void
 */
void DCMOTOR_init(const ST_DCMOTOR * a_dcMotors);

/*
 * [Function Name]: DCMOTOR_start
 * [Function Description]: starts the motor with the given direction and speed
 * [Args]:
 * [in]: uint8_t a_motorIndex
 * 		 motor index to start, same index used in initializing the motors array
 * [in]: EN_DcMotorDirection a_direction
 * 		 direction to start the motor with
 * [in]: uint8_t a_speedPercent
 * 		 speed of the motor - ignored if enable pin is not connected
 * [Return]: void
 */
void DCMOTOR_start(uint8_t a_dcMotorIndex, EN_DcMotorDirection a_direction, uint8_t a_speedPercent);

/*
 * [Function Name]: DCMOTOR_stop
 * [Function Description]: stops the motor
 * [Args]:
 * [in]: uint8_t a_motorIndex
 * 		 motor index to stop, same index used in initializing the motors array
 * [Return]: void
 */
void DCMOTOR_stop(uint8_t a_dcMotorIndex);

#endif /* DCMOTORS_USED_COUNT == 1 */

#endif /* __DC_MOTOR_H__ */
