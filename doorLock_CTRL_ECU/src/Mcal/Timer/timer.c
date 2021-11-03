/******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR TIMER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "timer.h"

/* For using mcu registers */
#include "../Mcu/mcu.h"

/* For using DIO module */
#include "../Dio/dio.h"

/*******************************************************************************
 *                      Static Functions Prototypes	                           *
 *******************************************************************************/

/*
 * [Function Name]: ticksPerIteration
 * [Function Description]: calculate the actual ticks from the passed ticks
 * 						   and setting global interrupt counts for the passed
 * 						   timer
 * [Args]:
 * [in]: uint8_t timer
 * 		 time to delay for in ms
 * [in]: uint32_t ticks
 * 		 total number of ticks, any 32-bit unsigned number
 * [in]: uint16_t maxCounts
 * 		 max number of ticks available for each timer
 * [Return]: uint16_t
 * 			 number of ticks that can fit in the timer count register (in OVF mood)
 * 			 or in OCRx (in CTC mood)
 */
static uint16_t ticksPerIteration(uint8_t a_timer, uint32_t a_ticks, uint16_t a_maxCounts);

/*******************************************************************************
 *                            Global Variables	                               *
 *******************************************************************************/

/* array for holding number of interrups before calling the callback function */
static uint32_t g_timersInterruptCount[TIMERS_COUNT_SUPPORTED] = { 0 };

/* array for actual number of interrups before calling the callback function */
static volatile uint32_t g_timersInterruptActualCount[TIMERS_COUNT_SUPPORTED] = { 0 };

/* pointer to callback functions of the available timers */
static void (* volatile g_timerInterruptHandler[TIMERS_COUNT_SUPPORTED])(void);

/* store the value of overflow start to init the TCNTx register with it in the begining of every ISR */
static uint8_t g_timer0_ovf_start, g_timer2_ovf_start;
static uint16_t g_timer1_ovf_start = 0;

/* store the prescalers after init to use them when TIMER_start() is called*/
static uint8_t g_timers_init_prescaler[TIMERS_COUNT_SUPPORTED] = { 0 };

/*******************************************************************************
 *                          Functions Definition	                           *
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
uint8_t TIMER_init(TIMER_config* a_timerConfig){
	/* Check if ticks is equal to zero */
	if (a_timerConfig->ticks == 0) {
		return TIMER_ERROR;
	}
	/* Select TIMER_0, TIMER_1, or TIMER_2 */
	switch (a_timerConfig->timer) {
	case TIMER_0:
		switch (a_timerConfig->mode) {
		case TIMER_0_OVF:
			/* set start value of the timer to be equal 256 - ticks per interrupt */
			g_timer0_ovf_start = TIMER_0_MAX_COUNT + 1 - ticksPerIteration(a_timerConfig->timer, \
					a_timerConfig->ticks, TIMER_0_MAX_COUNT);
			TCNT0_R = g_timer0_ovf_start;

			/* set timer pointer to handler */
			g_timerInterruptHandler[TIMER_0] = a_timerConfig->ptrToHandler;

			/* select ovf mode
			 * FOC0 = 1
			 * WGM00 = 0, WGM01 = 0
			 * COM00 = 0, COM01 = 0
			 * */
			TCCR0_R = SELECT_BIT(FOC0);

			/* enable timer0 ovf interrupt */
			SET_BIT(TIMSK_R, TOIE0);
			break;
		case TIMER_0_CTC:
		case TIMER_0_CTC_TOGGLE_OC0:
			/* set  timer start to 0 */
			TCNT0_R = 0;

			/* compare on the value of ticks per interrupt */
			OCR0_R = ticksPerIteration(a_timerConfig->timer, a_timerConfig->ticks, TIMER_0_MAX_COUNT) - 1;

			/* set timer pointer to handler */
			g_timerInterruptHandler[TIMER_0] = a_timerConfig->ptrToHandler;

			/* select comp mode
			 * FOC0 = 1
			 * WGM00 = 0, WGM01 = 1
			 * COM00 = 0, COM01 = 0
			 *  */
			TCCR0_R = SELECT_BIT(FOC0) | SELECT_BIT(WGM01);

			/* enable OC0 if mode is TIMER_0_CTC_TOGGLE_OC0  */
			if (a_timerConfig->mode == TIMER_0_CTC_TOGGLE_OC0) {
				DIO_pinInit(OC0, PIN_OUTPUT);
				SET_BIT(TCCR0_R, COM00);
			}

			/* enable timer0 comp interrupt */
			SET_BIT(TIMSK_R, OCIE0);
			break;
		default:
			return TIMER_ERROR;
		}

		/* store timer0 prescaler to start the timer with it when calling TIMER_start() */
		g_timers_init_prescaler[TIMER_0] = a_timerConfig->prescaler;

		break;
		case TIMER_1:
			switch (a_timerConfig->mode) {

			case TIMER_1_OVF:
				/* set start value of the timer to be equal 65536 - ticks per interrupt */
				g_timer1_ovf_start = TIMER_1_MAX_COUNT + 1 - ticksPerIteration(a_timerConfig->timer, \
						a_timerConfig->ticks, TIMER_1_MAX_COUNT);
				TCNT1_R = g_timer1_ovf_start;

				/* set timer pointer to handler */
				g_timerInterruptHandler[TIMER_1] = a_timerConfig->ptrToHandler;

				/* select comp mode
				 * FOC1A = 1, FOC1B = 1
				 * WGM10 = 0, WGM11 = 0, WGM12 = 0, WGM13 = 0,
				 * COM1A0 = 0, COM1A1 = 0, COM1B0 = 0, COM1B1 = 0
				 * ICNC1 = 0, ICES1 = 0
				 */
				TCCR1A_R = SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
				TCCR1B_R = 0;

				/* enable timer1 ovf interrupt */
				SET_BIT(TIMSK_R, TOIE1);
				break;
			case TIMER_1_CTC:
			case TIMER_1_CTC_TOGGLE_OC1A:
			case TIMER_1_CTC_TOGGLE_OC1B:
			case TIMER_1_CTC_TOGGLE_OC1A_OC1B:
				/* set  timer start to 0 */
				TCNT1_R = 0;

				/* compare on the value of ticks per interrupt */
				OCR1A_R = ticksPerIteration(a_timerConfig->timer, a_timerConfig->ticks, TIMER_1_MAX_COUNT) - 1;

				/* set timer pointer to handler */
				g_timerInterruptHandler[TIMER_1] = a_timerConfig->ptrToHandler;

				/* select comp mode
				 * FOC1A = 1, FOC1B = 1
				 * WGM10 = 0, WGM11 = 0, WGM12 = 1, WGM13 = 0
				 * COM1A0 = 0, COM1B0 = 0, COM1A1 = 0, COM1B1 = 0
				 *  */
				TCCR1A_R = SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
				TCCR1B_R = SELECT_BIT(WGM12);

				/* enable timer1 compA interrupt */
				SET_BIT(TIMSK_R, OCIE1A);

				/* enable OC1A if mode is TIMER_1_CTC_TOGGLE_OC1A  */
				if (a_timerConfig->mode == TIMER_1_CTC_TOGGLE_OC1A) {
					DIO_pinInit(OC1A, PIN_OUTPUT);
					SET_BIT(TCCR1A_R, COM1A0);
				}

				/* enable OC1B if mode is TIMER_1_CTC_TOGGLE_OC1B  */
				else if(a_timerConfig->mode == TIMER_1_CTC_TOGGLE_OC1B) {
					DIO_pinInit(OC1B, PIN_OUTPUT);
					SET_BIT(TCCR1A_R, COM1B0);
				}

				/* enable OC1B if mode is TIMER_1_CTC_TOGGLE_OC1B  */
				else if(a_timerConfig->mode == TIMER_1_CTC_TOGGLE_OC1A_OC1B) {
					DIO_pinInit(OC1A, PIN_OUTPUT);
					DIO_pinInit(OC1B, PIN_OUTPUT);
					SET_BIT(TCCR1A_R, COM1A0);
					SET_BIT(TCCR1A_R, COM1B0);
				}

				break;
			default:
				return TIMER_ERROR;
			}

			/* store timer1 prescaler to start the timer with it when calling TIMER_start() */
			g_timers_init_prescaler[TIMER_1] = a_timerConfig->prescaler;

			break;
			case TIMER_2:
				switch (a_timerConfig->mode) {
				case TIMER_2_OVF:
					/* set start value of the timer to be equal 256 - ticks per interrupt */
					g_timer2_ovf_start = TIMER_2_MAX_COUNT + 1 - ticksPerIteration(a_timerConfig->timer, a_timerConfig->ticks, \
							TIMER_2_MAX_COUNT);
					TCNT2_R = g_timer2_ovf_start;

					/* set timer pointer to handler */
					g_timerInterruptHandler[TIMER_2] = a_timerConfig->ptrToHandler;

					/* select ovf mode
					 * FOC2 = 1
					 * WGM20 = 0, WGM21 = 0
					 * COM20 = 0, COM21 = 0
					 * */
					TCCR2_R = SELECT_BIT(FOC2);

					/* enable timer2 ovf interrupt */
					SET_BIT(TIMSK_R, TOIE2);

					break;
				case TIMER_2_CTC:
				case TIMER_2_CTC_TOGGLE_OC2:
					/* set  timer start to 0 */
					TCNT2_R = 0;

					/* compare on the value of ticks per interrupt */
					OCR2_R = ticksPerIteration(a_timerConfig->timer, a_timerConfig->ticks, TIMER_2_MAX_COUNT) - 1;

					/* set timer pointer to handler */
					g_timerInterruptHandler[TIMER_2] = a_timerConfig->ptrToHandler;

					/* select comp mode
					 * FOC2 = 1
					 * WGM20 = 0, WGM21 = 1
					 * COM20 = 0, COM21 = 0
					 *  */
					TCCR2_R = SELECT_BIT(FOC2) | SELECT_BIT(WGM21);

					/* enable OC2 if mode is TIMER_2_CTC_TOGGLE_OC2  */
					if (a_timerConfig->mode == TIMER_2_CTC_TOGGLE_OC2) {
						DIO_pinInit(OC2, PIN_OUTPUT);
						SET_BIT(TCCR2_R, COM20);
					}

					/* enable timer0 comp interrupt */
					SET_BIT(TIMSK_R, OCIE2);
					break;
				default:
					return TIMER_ERROR;
				}
				/* store timer2 prescaler to start the timer with it when calling TIMER_start() */
				g_timers_init_prescaler[TIMER_2] = a_timerConfig->prescaler;

				break;
				default:
					/* timer not defined */
					return TIMER_ERROR;
	}
	/* return success */
	return TIMER_SUCCESS;
}

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
uint8_t TIMER_start(uint8_t a_timer) {
	switch(a_timer)
	{
	case TIMER_0:
		COPY_BITS(TCCR0_R, 0b00000111, g_timers_init_prescaler[TIMER_0], 0);
		break;
	case TIMER_1:
		COPY_BITS(TCCR1B_R, 0b00000111, g_timers_init_prescaler[TIMER_1], 0);
		break;
	case TIMER_2:
		COPY_BITS(TCCR2_R, 0b00000111, g_timers_init_prescaler[TIMER_2], 0);
		break;
	default:
		return TIMER_ERROR;
	}
	return TIMER_SUCCESS;
}

/*
 * [Function Name]: TIMER_stop
 * [Function Description]: stop the passed timer
 * [Args]:
 * [in]: uint8_t a_timer
 * 		 timer to stop
 * [Return]: uint8_t
 * 			 TIMER_INIT_SUCCESS or TIMER_INIT_ERROR
 */
uint8_t TIMER_stop(uint8_t a_timer){
	switch(a_timer)
	{
	case TIMER_0:
		COPY_BITS(TCCR0_R, 0b00000111, 0, 0);
		break;
	case TIMER_1:
		COPY_BITS(TCCR1B_R, 0b00000111, 0, 0);
		break;
	case TIMER_2:
		COPY_BITS(TCCR2_R, 0b00000111, 0, 0);
		break;
	default:
		return TIMER_ERROR;
	}
	return TIMER_SUCCESS;
}

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
uint16_t TIMER_read(uint8_t a_timer)
{
	uint16_t count;
	switch(a_timer)
	{
	case TIMER_0:
		count = TCNT0_R;
		break;
	case TIMER_1:
		count = TCNT1_R;
		break;
	case TIMER_2:
		count = TCNT2_R;
		break;
	default:
		return 0;
	}
	return count;
}

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
void TIMER_delayTicks(uint16_t ticks, uint32_t iterations, uint8_t prescaler)
{
#if DELAY_TIMER == TIMER_0
	/* loop counter */
	uint32_t loopCounter;

	/* make sure count is initially = 0 */
	TCNT0_R = 0;

	/* make sure interrupt is closed */
	CLEAR_BIT(TIMSK_R, OCIE0);

	/* put ticks in OCR0_R */
	OCR0_R = ticks;

	/* set the timer to work in CTC mode */
	TCCR0_R = SELECT_BIT(FOC0) | SELECT_BIT(WGM01);

	/* start the timer with the input prescaler */
	COPY_BITS(TCCR0_R, 0b00000111, prescaler, 0);

	for(loopCounter = 0; loopCounter < iterations; loopCounter ++)
	{
		/* wait till the flag is 1, then clear it */
		while(BIT_IS_CLEAR(TIFR_R, OCF0));
		SET_BIT(TIFR_R, OCF0);
	}

	/* stop the timer */
	TCCR0_R = 0;

#elif DELAY_TIMER == TIMER_1
	/* loop counter */
	uint32_t loopCounter;

	/* make sure count is initially = 0 */
	TCNT1_R = 0;

	/* make sure interrupt is closed */
	CLEAR_BIT(TIMSK_R, OCIE1A);

	/* put ticks in OCR1A_R */
	OCR1A_R = ticks;

	/* set the timer to work in CTC mode */
	TCCR1A_R = SELECT_BIT(FOC1A) | SELECT_BIT(FOC1B);
	TCCR1B_R = SELECT_BIT(WGM12);

	/* start the timer with the input prescaler */
	COPY_BITS(TCCR1B_R, 0b00000111, prescaler, 0);

	for(loopCounter = 0; loopCounter < iterations; loopCounter ++)
	{
		/* wait till the flag is 1, then clear it */
		while(BIT_IS_CLEAR(TIFR_R, OCF1A));
		SET_BIT(TIFR_R, OCF1A);
	}

	/* stop the timer */
	TCCR1B_R = 0;

#elif DELAY_TIMER == TIMER_2
	/* loop counter */
	uint32_t loopCounter;

	/* make sure count is initially = 0 */
	TCNT2_R = 0;

	/* make sure interrupt is closed */
	CLEAR_BIT(TIMSK_R, OCIE2);

	/* put ticks in OCR2_R */
	OCR2_R = ticks;

	/* set the timer to work in CTC mode */
	TCCR2_R = SELECT_BIT(FOC2) | SELECT_BIT(WGM21);

	/* start the timer with the input prescaler */
	COPY_BITS(TCCR2_R, 0b00000111, prescaler, 0);

	for(loopCounter = 0; loopCounter < iterations; loopCounter ++)
	{
		/* wait till the flag is 1, then clear it */
		while(BIT_IS_CLEAR(TIFR_R, OCF2));
		SET_BIT(TIFR_R, OCF2);
	}

	/* stop the timer */
	TCCR2_R = 0;
#endif /* DELAY_TIMER == TIMER_0 */
}

/*
 * [Function Name]: ticksPerIteration
 * [Function Description]: calculate the actual ticks from the passed ticks
 * 						   and setting global interrupt counts for the passed
 * 						   timer
 * [Args]:
 * [in]: uint8_t timer
 * 		 time to delay for in ms
 * [in]: uint32_t ticks
 * 		 total number of ticks, any 32-bit unsigned number
 * [in]: uint16_t maxCounts
 * 		 max number of ticks available for each timer
 * [Return]: uint16_t
 * 			 number of ticks that can fit in the timer count register (in OVF mood)
 * 			 or in OCRx (in CTC mood)
 */
static uint16_t ticksPerIteration(uint8_t a_timer, uint32_t a_ticks, uint16_t a_maxCounts) {
	uint32_t counts;									/* number of ticks in each interrupt */
	float64_t countsTemp;								/* used to calculate  */
	g_timersInterruptActualCount[a_timer] = 1;			/* reset the actual interrupt count to 1 */
	if (a_ticks <= a_maxCounts) {						/* if the ticks <= maxCounts there will be only one interrupt */
		g_timersInterruptCount[a_timer] = 1;
		return (uint16_t) (a_ticks);
	}
	/* counts will  be equal to countsTemp or countsTemp + 1
	   depending on whether countsTemp has fractions or not */
	countsTemp = (float64_t) a_ticks / ((uint32_t)a_maxCounts + 1);
	counts = (uint32_t) countsTemp;
	if (counts != countsTemp) {
		counts++;
	}
	g_timersInterruptCount[a_timer] = counts;
	return (uint16_t) (((float64_t)a_ticks / counts) + 0.5);			/* number of ticks in each interrupt */
}

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* ISR for timer 0 CTC */
ISR(TIMER0_COMP_vect) {
	if (g_timersInterruptActualCount[TIMER_0] == g_timersInterruptCount[TIMER_0]) {
		(*g_timerInterruptHandler[TIMER_0])();
		g_timersInterruptActualCount[TIMER_0] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_0]++;
	}
}

/* ISR for timer 0 OVF */
ISR(TIMER0_OVF_vect) {
	TCNT0_R = g_timer0_ovf_start;
	if (g_timersInterruptActualCount[TIMER_0] == g_timersInterruptCount[TIMER_0]) {
		(*g_timerInterruptHandler[TIMER_0])();
		g_timersInterruptActualCount[TIMER_0] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_0]++;
	}
}

/* ISR for timer 2 CTC */
ISR(TIMER2_COMP_vect) {
	if (g_timersInterruptActualCount[TIMER_2] == g_timersInterruptCount[TIMER_2]) {
		(*g_timerInterruptHandler[TIMER_2])();
		g_timersInterruptActualCount[TIMER_2] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_2]++;
	}
}

/* ISR for timer 2 OVF */
ISR(TIMER2_OVF_vect) {
	TCNT2_R = g_timer2_ovf_start;
	if (g_timersInterruptActualCount[TIMER_2] == g_timersInterruptCount[TIMER_2]) {
		(*g_timerInterruptHandler[TIMER_2])();
		g_timersInterruptActualCount[TIMER_2] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_2]++;
	}
}

/* ISR for timer 1 OVF */
ISR(TIMER1_OVF_vect) {
	TCNT1_R = g_timer1_ovf_start;
	if (g_timersInterruptActualCount[TIMER_1] == g_timersInterruptCount[TIMER_1]) {
		(*g_timerInterruptHandler[TIMER_1])();
		g_timersInterruptActualCount[TIMER_1] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_1]++;
	}
}

/* ISR for timer 1A CTC */
ISR(TIMER1_COMPA_vect) {
	if (g_timersInterruptActualCount[TIMER_1] == g_timersInterruptCount[TIMER_1]) {
		(*g_timerInterruptHandler[TIMER_1])();
		g_timersInterruptActualCount[TIMER_1] = 1;
	} else {
		g_timersInterruptActualCount[TIMER_1]++;
	}
}
