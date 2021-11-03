/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the AVR UART driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __UART_H__
#define __UART_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "uart-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_UartCharacterSize
 * [Enum Description]: contains uart character size options
 */
typedef enum
{
	UART_5_BITS,
	UART_6_BITS,
	UART_7_BITS,
	UART_8_BITS
}EN_UartCharacterSize;

/*
 * [Enum Name]: EN_UartParity
 * [Enum Description]: contains uart parity options
 */
typedef enum
{
	UART_PARITY_DISABLED,
	UART_PARITY_EVEN = 2,
	UART_PARITY_ODD
}EN_UartParity;

/*
 * [Enum Name]: EN_UartStopBit
 * [Enum Description]: contains uart stop bit options
 */
typedef enum
{
	UART_STOP_BIT_1,
	UART_STOP_BIT_2
}EN_UartStopBit;

/*
 * [Enum Name]: EN_UartTxInterrupt
 * [Enum Description]: contains uart Tx interrupt options
 */
typedef enum
{
	UART_TX_INTERRUPT_DISABLED,
	UART_TX_INTERRUPT_ENABLED
}EN_UartTxInterrupt;

/*
 * [Enum Name]: EN_UartRxInterrupt
 * [Enum Description]: contains uart Rx interrupt options
 */
typedef enum
{
	UART_RX_INTERRUPT_DISABLED,
	UART_RX_INTERRUPT_ENABLED
}EN_UartRxInterrupt;

/*
 * [Struct Name]: ST_UartConfig
 * [Struct Description]: contains uart initialization config
 */
typedef struct
{

	/* data size 5, 6, 7 or 8 */
	EN_UartCharacterSize charSize;

	/* type of parity check, disabled, even or odd */
	EN_UartParity parity;

	/* number of stop bit(s) 1 or 2 */
	EN_UartStopBit stopBit;

	/* uart baud rate */
	uint32_t baudRate;

	/* tx interrupt enabled or disabled */
	EN_UartTxInterrupt txInterrupt;

	/* rx interrupt enabled or disabled */
	EN_UartRxInterrupt rxInterrupt;

}ST_UartConfig;

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: UART_init
 * [Function Description]: initializes the uart with the given configuration
 * 						   1. sets the character size
 * 						   2. control parity check
 * 						   3. specify stop bit(s) number
 * 						   4. specify baud rate
 * 						   5. enable/disable Tx/Rx interrupts
 * [Args]:
 * [in]: const ST_UartConfig * a_uartConfig
 * 		 the configuration of the uart
 * [Return]: void
 */
void UART_init(const ST_UartConfig * a_uartConfig);

/*
 * [Function Name]: UART_setRxInterruptCallback
 * [Function Description]: sets the callback function of the receive interrupt
 * 						   should be used only if the rx interrupt is enabled.
 * 						   Otherwise it's meaningless
 * [Args]:
 * [in]: void (* volatile a_ptrToHandler)(void)
 * 		 pointer to the callback function
 * [Return]: void
 */
void UART_setRxInterruptCallback(void (* volatile a_ptrToHandler)(void));

/*
 * [Function Name]: UART_setTxInterruptCallback
 * [Function Description]: sets the callback function of the send interrupt
 * 						   should be used only if the tx interrupt is enabled.
 * 						   Otherwise it's meaningless
 * [Args]:
 * [in]: void (* volatile a_ptrToHandler)(void)
 * 		 pointer to the callback function
 * [Return]: void
 */
void UART_setTxInterruptCallback(void (* volatile a_ptrToHandler)(void));

/*
 * [Function Name]: UART_sendByteBlocking
 * [Function Description]: This function uses busy wait till the send buffer
 * 						   is empty to send the byte. It can be used whether
 * 						   Tx interrupt is enabled or not.
 * 						   If Tx interrupt is enabled,
 * 						   It will generate Tx interrupt after sending
 * [Args]:
 * [in]: const uint8_t a_data
 * 		 data to be sent
 * [Return]: void
 */
void UART_sendByteBlocking(const uint8_t a_data);

/*
 * [Function Name]: UART_sendByteNonBlocking
 * [Function Description]: The function checks if the send buffer is ready,
 * 						   It will send the byte, otherwise it will do nothing.
 * 						   It doesn't use any busy wait.
 * 						   The best use of this function is to wait till a flag is changed
 * 						   in the callback of tx interrupt then send data using this function
 * 						   to be sure data is sent. otherwise, you can use
 * 						   UART_sendBytePolling to send data safely
 * 						   If Tx interrupt is enabled,
 * 						   It will generate Tx interrupt after sending
 * [Args]:
 * [in]: const uint8_t a_data
 * 		 data to be sent
 * [Return]: void
 */
void UART_sendByteNonBlocking(const uint8_t a_data);

/*
 * [Function Name]: UART_receiveByteBlocking
 * [Function Description]: The function uses busy wait till a byte is received
 * 						   It can be used whether Rx interrupt is enabled or not
 * 						   If rx interrupt is enabled it will be disabled temporarely
 * 						   during the function time then re-enabled again
 * 						   It will not generate Rx interrupt after recieving even if Rx
 * 						   interrupt is enabled
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 the received byte
 */
uint8_t UART_receiveByteBlocking(void);

/*
 * [Function Name]: UART_receiveByteNonBlocking
 * [Function Description]: The function checks if the receive buffer has data,
 * 						   It will return the data, otherwise it will return 0.
 * 						   It doesn't use any busy wait.
 * 						   The best use of this function is to be called in callback
 * 						   function after an Rx interrupt has occurred to get the received byte.
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 the received byte
 */
uint8_t UART_receiveByteNonBlocking(void);

/*
 * [Function Name]: UART_sendString
 * [Function Description]: The function sends the given string till it reaches the null character
 * 						   It uses polling techinque "busy wait"
 * [Args]:
 * [in]: const char * a_str
 * 		 string to be sent
 * [Return]: void
 */
void UART_sendString(const char * a_str);

/*
 * [Function Name]: UART_receiveString
 * [Function Description]: The function receives string using busy wait
 * 						   "polling technique" till
 * 						   1. the UART_RECEIVE_STRING_TILL is found
 * 						   	  this value can be modified in the config file
 * 						   2. reaching the maximum value a_maxSize
 * [Args]:
 * [in]: uint8_t * a_str
 * 		 array to store recived string
 * [in]: uint8_t a_maxSize
 * 		 maximum size of the passed array to receive till
 * 		 it reached
 * [Return]: void
 */
void UART_receiveString(uint8_t * a_str, uint8_t a_maxSize);

/*
 * [Function Name]: UART_DataIsAvailable
 * [Function Description]: The function checks if the receive buffer has data
 * 						   available in it or not
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 TRUE if there is data available
 * 			 FALSE otherwise
 */
uint8_t UART_DataIsAvailable(void);

/*
 * [Function Name]: UART_TxIsEmpty
 * [Function Description]: The function checks if the transfer buffer is empty or not
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 TRUE if the tx buffer is empty
 * 			 FALSE otherwise
 */
uint8_t UART_TxIsEmpty(void);

#endif /* __UART_H__ */
