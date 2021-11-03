/******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc-motor.c
 *
 * Description: Source file for the DC Motor Bridge driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "dc-motor.h"

/* For using dio init functions for pins */
#include "../../Mcal/Dio/dio.h"

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1

/* For using pwm on enable pin - only if enable pin is connected */
#include "../../Mcal/Pwm/pwm.h"

#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */

/*******************************************************************************
 *                      	   Global Variables		                           *
 *******************************************************************************/

#if DCMOTORS_USED_COUNT != 1

/* array to hold all initialized motors */
static ST_DCMOTOR g_dcMotors[DCMOTORS_USED_COUNT];

#endif /* DCMOTORS_USED_COUNT != 1 */

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

#if DCMOTORS_USED_COUNT == 1

/*
 * [Function Name]: DCMOTOR_init
 * [Function Description]: Initializes the motor pins
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void DCMOTOR_init(void)
{
	DIO_pinInit(DCMOTOR_PIN1, PIN_OUTPUT);
	DIO_pinInit(DCMOTOR_PIN2, PIN_OUTPUT);

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1

	DIO_pinInit(DCMOTOR_ENABLE_PIN, PIN_OUTPUT);

#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */
}

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
void DCMOTOR_start(EN_DcMotorDirection a_direction, uint8_t a_speedPercent)
{
	switch(a_direction)
	{
	case DCMOTOR_FORWARD:
		/* move motor in the forward direction */
		DIO_writePin(DCMOTOR_PIN1, HIGH);
		DIO_writePin(DCMOTOR_PIN2, LOW);
		break;
	case DCMOTOR_REVERSE:
		/* move motor in the reverse direction */
		DIO_writePin(DCMOTOR_PIN1, LOW);
		DIO_writePin(DCMOTOR_PIN2, HIGH);
		break;
	default:
		return;
	}
#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1
	/* check if the input speed is greater than 100 */
	if(a_speedPercent > 100)
	{
		return;
	}
	/* check if enable pin is connected to a pin that supports pwm */
	if(PWM_enable(DCMOTOR_ENABLE_PIN, a_speedPercent) == PWM_ERROR)
	{
		/* write high if pwm is not supported on enable pin */
		DIO_writePin(DCMOTOR_ENABLE_PIN, HIGH);
	}
#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */
}

/*
 * [Function Name]: DCMOTOR_stop
 * [Function Description]: stops the motor
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void DCMOTOR_stop(void)
{
	/* write low to both pins to stop the motor */
	DIO_writePin(DCMOTOR_PIN1, LOW);
	DIO_writePin(DCMOTOR_PIN2, LOW);

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1
	/* check if enable pin is connected to a pin that supports pwm */
	if(PWM_disable(DCMOTOR_ENABLE_PIN) == PWM_ERROR)
	{
		/* write low if pwm is not supported on enable pin */
		DIO_writePin(DCMOTOR_ENABLE_PIN, LOW);
	}
#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */
}

#else

/*
 * [Function Name]: DCMOTOR_init
 * [Function Description]: Initializes the motor pins
 * [Args]:
 * [in]: ST_DcMotor *a_motor
 * 		 array of motors structures to init
 * [Return]: void
 */
void DCMOTOR_init(const ST_DCMOTOR * a_dcMotors)
{
	uint8_t loopCounter;

	for(loopCounter = 0; loopCounter < DCMOTORS_USED_COUNT; loopCounter ++)
	{
		/* copy values from a_dcmotors to g_dcMotors */
		g_dcMotors[loopCounter] = a_dcMotors[loopCounter];

		/* init pin 1 and 2 of the motor */
		DIO_pinInit(g_dcMotors[loopCounter].pin1, PIN_OUTPUT);
		DIO_pinInit(g_dcMotors[loopCounter].pin2, PIN_OUTPUT);

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1

		g_dcMotors[loopCounter].enablePin = a_dcMotors[loopCounter].enablePin;

		/* init enable pin if connected */
		DIO_pinInit(g_dcMotors[loopCounter].enablePin, PIN_OUTPUT);

#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */

	}

}

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
void DCMOTOR_start(uint8_t a_dcMotorIndex, EN_DcMotorDirection a_direction, uint8_t a_speedPercent)
{
	if(a_dcMotorIndex < DCMOTORS_USED_COUNT)
	{
		switch(a_direction)
		{
		case DCMOTOR_FORWARD:
			/* move motor in the forward direction */
			DIO_writePin(g_dcMotors[a_dcMotorIndex].pin1, HIGH);
			DIO_writePin(g_dcMotors[a_dcMotorIndex].pin2, LOW);
			break;
		case DCMOTOR_REVERSE:
			/* move motor in the reverse direction */
			DIO_writePin(g_dcMotors[a_dcMotorIndex].pin1, LOW);
			DIO_writePin(g_dcMotors[a_dcMotorIndex].pin2, HIGH);
			break;
		default:
			return;
		}
#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1
		/* check if the input speed is greater than 100 */
		if(a_speedPercent > 100)
		{
			return;
		}
		/* check if enable pin is connected to a pin that supports pwm */
		if(PWM_enable(g_dcMotors[a_dcMotorIndex].enablePin, a_speedPercent) == PWM_ERROR)
		{
			/* write high if pwm is not supported on enable pin */
			DIO_writePin(g_dcMotors[a_dcMotorIndex].enablePin, HIGH);
		}
#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */
	}
}

/*
 * [Function Name]: DCMOTOR_stop
 * [Function Description]: stops the motor
 * [Args]:
 * [in]: uint8_t a_motorIndex
 * 		 motor index to stop, same index used in initializing the motors array
 * [Return]: void
 */
void DCMOTOR_stop(uint8_t a_dcMotorIndex)
{
	if(a_dcMotorIndex < DCMOTORS_USED_COUNT)
	{
		/* write low to both pins to stop the motor */
		DIO_writePin(g_dcMotors[a_dcMotorIndex].pin1, LOW);
		DIO_writePin(g_dcMotors[a_dcMotorIndex].pin2, LOW);

#if DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1
		/* check if enable pin is connected to a pin that supports pwm */
		if(PWM_disable(g_dcMotors[a_dcMotorIndex].enablePin) == PWM_ERROR)
		{
			/* write low if pwm is not supported on enable pin */
			DIO_writePin(g_dcMotors[a_dcMotorIndex].enablePin, LOW);
		}
#endif /* DCMOTOR_ENABLE_PIN_IS_CONNECTED == 1 */
	}
}

#endif /* DCMOTORS_USED_COUNT == 1 */
