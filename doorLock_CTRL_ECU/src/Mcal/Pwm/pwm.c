/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the AVR PWM driver. Till now the driver supports
 * 			    only fast pwm. Phase correct pwm is not supported yet
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "pwm.h"

/* For using mcu registers */
#include "../Mcu/mcu.h"

/* For using DIO module */
#include "../Dio/dio.h"

/* For using TIMER module */
#include "../Timer/timer.h"

/*******************************************************************************
 *                          Functions Definition	                           *
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
uint8_t PWM_enable(uint8_t a_pin, uint8_t a_dutyCycle)
{
	uint16_t ticks;
	/* return error if the duty cycle value is not within range (> 100) */
	if(a_dutyCycle > 100)
	{
		return PWM_ERROR;
	}
	switch(a_pin)
	{
	case PWM0:
		/* calculate number of ticks from duty cycle */
		ticks = (uint16_t)((uint32_t)a_dutyCycle * TIMER_0_MAX_COUNT / 100);

		/* set OCR0 = ticks */
		OCR0_R = ticks;

		/* select fast pwm mode
		 * FOC0 = 0
		 * WGM00 = 1, WGM01 = 1
		 * COM00 = 0, COM01 = 1 (non-inverting)
		 */
		TCCR0_R = SELECT_BIT(WGM00) | SELECT_BIT(WGM01) | SELECT_BIT(COM01);

		/* init OC0 as output */
		DIO_pinInit(OC0, PIN_OUTPUT);

		/* start timer 0 */
		COPY_BITS(TCCR0_R, 0b00000111, PWM_0_PRESCALER, 0);

		break;
	case PWM1A:
	case PWM1B:
		/* calculate number of ticks from duty cycle */
		ticks = (uint16_t)((uint32_t)a_dutyCycle * PWM_1_TOP / 100);

		/* set pwm 1 top */
		ICR1_R = PWM_1_TOP;

		/* select fast pwm mode (common for both channels)
		 * FOC1A = 0, FOC1B = 0
		 * WGM10 = 0, WGM11 = 1, WGM12 = 1, WGM13 = 1
		 * COM1A0 = 0, COM1B0 = 1 (non-inverting)
		 */
		TCCR1A_R &= SELECT_INV_BIT(FOC1A) & SELECT_INV_BIT(FOC1B) \
				& SELECT_INV_BIT(COM1A0) & SELECT_INV_BIT(COM1B0) \
				& SELECT_INV_BIT(WGM10);
		TCCR1A_R |= SELECT_BIT(WGM11);
		TCCR1B_R |= SELECT_BIT(WGM12) | SELECT_BIT(WGM13);
		TCCR1B_R &= SELECT_INV_BIT(ICNC1);

		if(a_pin == PWM1A)
		{
			/* set OCR1A to ticks */
			OCR1A_R = ticks;

			/* enable pwm channel A */
			TCCR1A_R |= SELECT_BIT(COM1A1);

			/* init OC1A as output */
			DIO_pinInit(OC1A, PIN_OUTPUT);
		}
		else
		{
			/* set OCR1B to ticks */
			OCR1B_R = ticks;

			/* enable pwm channel B */
			TCCR1A_R |= SELECT_BIT(COM1B1);

			/* init OC1B as output */
			DIO_pinInit(OC1B, PIN_OUTPUT);

		}
		/* start timer 1 */
		COPY_BITS(TCCR1B_R, 0b00000111, PWM_1_PRESCALER, 0);

		break;
	case PWM2:
		/* calculate number of ticks from duty cycle */
		ticks = (uint16_t)((uint32_t)a_dutyCycle * TIMER_2_MAX_COUNT / 100);

		/* set OCR2 = ticks */
		OCR2_R = ticks;

		/* select fast pwm mode
		 * FOC2 = 0
		 * WGM20 = 1, WGM21 = 1
		 * COM20 = 0, COM21 = 1 (non-inverting)
		 */
		TCCR2_R = SELECT_BIT(WGM20) | SELECT_BIT(WGM21) | SELECT_BIT(COM21);

		/* init OC2 as output */
		DIO_pinInit(OC2, PIN_OUTPUT);

		/* start timer 2 */
		COPY_BITS(TCCR2_R, 0b00000111, PWM_2_PRESCALER, 0);
		break;
	default:
		return PWM_ERROR;
	}
	return PWM_SUCCESS;
}

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
uint8_t PWM_disable(uint8_t a_pin)
{
	switch(a_pin)
	{
	case PWM0:
		/* reset timer 0 and output low on OCR0 */
		TCCR0_R = 0;
		break;
	case PWM1A:
		/* disable pwm channel 1A and output low on OCR1A */
		TCCR1A_R &= SELECT_INV_BIT(COM1A1);
		break;
	case PWM1B:
		/* disable pwm channel 1B and output low on OCR1B */
		TCCR1A_R &= SELECT_INV_BIT(COM1B1);
		break;
	case PWM2:
		/* reset timer 2 and output low on OCR2 */
		TCCR2_R = 0;
		break;
	default:
		return PWM_ERROR;
	}
	return PWM_SUCCESS;
}
