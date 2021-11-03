/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart-config.h
 *
 * Description: Config file for the AVR UART driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* used in UART_receiveString function to receive string
 * till this character is found before reaching the maximum size of the array
 */
#define UART_RECEIVE_STRING_TILL			'\r'

/* Define F_CPU if not defined to calculate baud rate correctly */
#ifndef F_CPU
#define F_CPU 								1000000UL
#endif /* F_CPU */


#endif /* __UART_CONFIG_H__ */
