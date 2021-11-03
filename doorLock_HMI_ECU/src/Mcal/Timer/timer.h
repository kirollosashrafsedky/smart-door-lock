/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR TIMER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "timer-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Number of Timers supported by the MCU */
#define TIMERS_COUNT_SUPPORTED			3

#define TIMER_0							0

/* Timer 0 max count */
#define TIMER_0_MAX_COUNT 				255

#define TIMER_1							1

/* Timer 1 max count */
#define TIMER_1_MAX_COUNT 				65535

#define TIMER_2							2

/* Timer 2 max count */
#define TIMER_2_MAX_COUNT 				255

/* timer success in case of requesting an existing timer  */
#define TIMER_SUCCESS				1

/* timer success in case of requesting a
 * not defined timer,
 * not defined mood
 * ticks = 0 in case of CTC or OVF mood
 */
#define TIMER_ERROR 				0

/*******************************************************************************
 *                                Macros                                       *
 *******************************************************************************/

/* TIME_MS_TO_TICKS Macro to convert time in ms to ticks */
#define TIME_MS_TO_TICKS(prescaler,time) ((uint32_t)((time) * ((F_CPU) / (1000.0 * prescaler))) + 0.5)

/*
 * TIMER_DELAY_MS macro, used to delay (busy wait) for a specific time in ms
 * Please make sure to
 * -------------------
 * 1- enable compiler optimization to overcome calculations overhead
 * 2- use defined delay times during compilation time, using unknown
 * 	  delay values (variables) will result in more calculations overhead
 */
#if DELAY_TIMER == TIMER_1

#define TIMER_DELAY_MS(time) {								\
	/* check if time is greater than 0 */					\
	if ((float64_t)time > 0)								\
	{														\
		uint32_t counts = 1, totalTicks;					\
		/* calculate total ticks required */				\
		totalTicks = TIME_MS_TO_TICKS(1, (float64_t)		\
					((time) - DELAY_OVERHEAD));				\
		/* check if total ticks fit in 						\
		 compare match register */							\
		if (totalTicks <= TIMER_1_MAX_COUNT)				\
		{													\
			/* call TIMER_delayTicks */						\
			TIMER_delayTicks((uint16_t)						\
								(totalTicks - 1), 1, 1);	\
		}													\
		else												\
		{													\
			/* calulate number of iterations required 		\
			 then call TIMER_delayTicks */					\
			counts = (totalTicks / (uint32_t)				\
					(TIMER_1_MAX_COUNT + 1)) + 1;			\
			TIMER_delayTicks((uint16_t)						\
				(totalTicks / counts) - 1, counts, 1);	 	\
		}													\
	}														\
}

#else

#define TIMER_DELAY_MS(time) {								\
	/* check if time is greater than 0 */					\
	if ((float64_t)time > 0)								\
	{														\
		uint32_t counts = 1, totalTicks;					\
		/* calculate total ticks required */				\
		totalTicks = TIME_MS_TO_TICKS(1, (float64_t)		\
					((time) - DELAY_OVERHEAD));				\
		/* check if total ticks fit in 						\
		 compare match register */							\
		if (totalTicks <= TIMER_0_MAX_COUNT)				\
		{													\
			/* call TIMER_delayTicks */						\
			TIMER_delayTicks((uint8_t)						\
								(totalTicks - 1), 1, 1);	\
		}													\
		else												\
		{													\
			/* calulate number of iterations required 		\
			 then call TIMER_delayTicks */					\
			counts = (totalTicks / (uint32_t)				\
					(TIMER_0_MAX_COUNT + 1)) + 1;			\
			TIMER_delayTicks((uint8_t)						\
				(totalTicks / counts) - 1, counts, 1);	 	\
		}													\
	}														\
}

#endif /* DELAY_TIMER == TIMER_1 */

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: TIMER_0_modes
 * [Enum Description]: contains timer 0 available moods
 */
typedef enum
{
	TIMER_0_OVF = 0x00,
	TIMER_0_CTC,
	TIMER_0_CTC_TOGGLE_OC0
}TIMER_0_modes;

/*
 * [Enum Name]: TIMER_1_modes
 * [Enum Description]: contains timer 1 available moods
 */
typedef enum
{
	TIMER_1_OVF = 0x10,
	TIMER_1_CTC,
	TIMER_1_CTC_TOGGLE_OC1A,
	TIMER_1_CTC_TOGGLE_OC1B,
	TIMER_1_CTC_TOGGLE_OC1A_OC1B
}TIMER_1_modes;

/*
 * [Enum Name]: TIMER_2_modes
 * [Enum Description]: contains timer 2 available moods
 */
typedef enum
{
	TIMER_2_OVF = 0x20,
	TIMER_2_CTC,
	TIMER_2_CTC_TOGGLE_OC2
}TIMER_2_modes;

/*
 * [Enum Name]: TIMER_0_prescalers
 * [Enum Description]: contains timer 0 available prescalers
 */
typedef enum
{
	TIMER_0_PRESCALER_1 = 1,
	TIMER_0_PRESCALER_8,
	TIMER_0_PRESCALER_64,
	TIMER_0_PRESCALER_256,
	TIMER_0_PRESCALER_1024,
	TIMER_0_PRESCALER_EXT_FALLIING,
	TIMER_0_PRESCALER_EXT_RISING,
}TIMER_0_prescalers;

/*
 * [Enum Name]: TIMER_1_prescalers
 * [Enum Description]: contains timer 1 available prescalers
 */
typedef enum
{
	TIMER_1_PRESCALER_1 = 1,
	TIMER_1_PRESCALER_8,
	TIMER_1_PRESCALER_64,
	TIMER_1_PRESCALER_256,
	TIMER_1_PRESCALER_1024,
	TIMER_1_PRESCALER_EXT_FALLIING,
	TIMER_1_PRESCALER_EXT_RISING,
}TIMER_1_prescalers;

/*
 * [Enum Name]: TIMER_2_prescalers
 * [Enum Description]: contains timer 2 available prescalers
 */
typedef enum
{
	TIMER_2_PRESCALER_1 = 1,
	TIMER_2_PRESCALER_8,
	TIMER_2_PRESCALER_32,
	TIMER_2_PRESCALER_64,
	TIMER_2_PRESCALER_128,
	TIMER_2_PRESCALER_256,
	TIMER_2_PRESCALER_1024,
}TIMER_2_prescalers;

/*
 * [Struct Name]: TIMER_config
 * [Struct Description]: contains timer configuration
 */
typedef struct
{
	/* timer to be used TIMER_0, TIMER_1, or TIMER_2 */
	uint8_t timer;

	/* timer mode from TIMER_0_modes, TIMER_1_modes, or TIMER_2_modes */
	uint8_t mode;

	/* timer prescaler from TIMER_0_prescalers, TIMER_1_prescalers, or TIMER_2_prescalers */
	uint8_t prescaler;

	/*
	 * ticks of 3 timers in case of CTC or OVF,
	 * can be any 32-bit data except 0
	 */
	uint32_t ticks;

	/* pointer to interrupt handler function */
	void (* volatile ptrToHandler)(void);
}TIMER_config;

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: TIMER_init
 * [Function Description]: Initialize the timer with the passed config.
 * 						   Calling this function, stops the timer and reset any previou
 * 						   config. To start the timer call TIMER_start() after calling it.
 * 						   For small time intervals, Use less prescalers.
 * 						   For ovf. mode, avoid small number of ticks with
 * 						   low F_CPU, because the ISR may take more time
 * 						   than that of the small ticks count so an interrupt is lost,
 * 						   Also avoid ticks = 1 in ovf mode, because it prevents interrupt
 * 						   in the following clock cycle
 * [Args]:
 * [in]: TIMER_config* a_timerConfig
 * 		 pointer to a struct containing the config for the timer
 * [Return]: uint8_t
 * 			 TIMER_SUCCESS or TIMER_ERROR
 */
uint8_t TIMER_init(TIMER_config* a_timerConfig);

/*
 * [Function Name]: TIMER_start
 * [Function Description]: start the passed timer with previously init config
 * 						   must be called after calling TIMER_init()
 * [Args]:
 * [in]: uint8_t a_timer
 * 		 timer to start
 * [Return]: uint8_t
 * 			 TIMER_INIT_SUCCESS or TIMER_INIT_ERROR
 */
uint8_t TIMER_start(uint8_t a_timer);

/*
 * [Function Name]: TIMER_stop
 * [Function Description]: stop the passed timer
 * [Args]:
 * [in]: uint8_t a_timer
 * 		 timer to stop
 * [Return]: uint8_t
 * 			 TIMER_INIT_SUCCESS or TIMER_INIT_ERROR
 */
uint8_t TIMER_stop(uint8_t a_timer);

/*
 * [Function Name]: TIMER_read
 * [Function Description]: gets the value of the current count of a timer
 * [Args]:
 * [in]: uint8_t a_timer
 * 		 timer to read
 * [Return]: uint16_t
 * 			 value of the current timer count
 * 			 1 byte => TIMER_0 or TIMER_2
 * 			 2 bytes => TIMER_1
 */
uint16_t TIMER_read(uint8_t a_timer);

/*
 * [Function Name]: TIMER_delayTicks
 * [Function Description]: delay function using the DELAY_TIMER defined
 * 						   in timer-config.h, It uses ctc mode annd delays
 * 						   for specific number of ticks = ticks * iterations
 * 						   with a defined prescaler, It's also used in
 * 						   TIMER_DELAY_MS macro, the difference is the macro
 * 						   takes time as input, while is can be faster if ticks,
 * 						   iterations and prescaler are known
 * [Args]:
 * [in]: uint16_t ticks
 * 		 timer ticks in one iteration
 * 		 8-bit data in TIMER_0 or TIMER_2, 16-bit data in TIMER_1
 * [in]: uint32_t iterations
 * 		 iterations / loops of compare matchs
 * 		 32-bit data
 * [in]: uint8_t prescaler
 * 		 prescaler to start the timer with, use defined prescalers depending
 * 		 on the DELAY_TIMER used
 * [Return]: void
 */
void TIMER_delayTicks(uint16_t ticks, uint32_t iterations, uint8_t prescaler);

#endif /* __TIMER_H__ */
