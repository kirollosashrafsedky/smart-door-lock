 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer-config.h
 *
 * Description: Config file for the AVR TIMER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __TIMER_CONFIG_H__
#define __TIMER_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Define F_CPU if not defined to calculate time correctly */
#ifndef F_CPU
#define F_CPU 				1000000UL
#endif /* F_CPU */

/* Timer used when calling the TIMER_delay() function */
/* can be TIMER_0, TIMER_1, or TIMER_2 */
#define DELAY_TIMER			TIMER_2

/* overhead delay time in ms, depending on optimization level */
/* should be decreased when increasing clock frequency (F_CPU) */
#define DELAY_OVERHEAD		0.1

#endif /* __TIMER_CONFIG_H__ */
