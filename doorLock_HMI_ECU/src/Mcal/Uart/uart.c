/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the AVR UART driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "uart.h"

/* For using mcu registers */
#include "../Mcu/mcu.h"

/*******************************************************************************
 *                            Global Variables	                               *
 *******************************************************************************/

/* pointer to rx interrupt handler */
static void (* volatile g_uartRxPtrToHandler)(void) = NULL;

/* pointer to tx interrupt handler */
static void (* volatile g_uartTxPtrToHandler)(void) = NULL;

/*******************************************************************************
 *                          Functions Definition	                           *
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
void UART_init(const ST_UartConfig * a_uartConfig)
{
	uint16_t ubrrValue = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA_R = SELECT_BIT(U2X);

	/************************** UCSRB Description **************************
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * RXB8 & TXB8 = 0 not used
	 * UCSZ2 = 0 all available character size options (5, 6, 7, 8) has UCSZ2 = 0
	 ***********************************************************************/
	UCSRB_R = SELECT_BIT(RXEN) | SELECT_BIT(TXEN);

	/* insert the value of rx interrupt in RXCIE */
	COPY_BITS(UCSRB_R, 0x01, a_uartConfig->rxInterrupt, RXCIE);

	/* insert the value of tx interrupt in TXCIE */
	COPY_BITS(UCSRB_R, 0x01, a_uartConfig->txInterrupt, TXCIE);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC_R = SELECT_BIT(URSEL);

	/* insert the value of the parity in UPM0, UPM1 */
	COPY_BITS(UCSRC_R, 0x03, a_uartConfig->parity, UPM0);

	/* insert the value of the stop bit(s) number in USBS */
	COPY_BITS(UCSRC_R, 0x01, a_uartConfig->stopBit, USBS);

	/* insert the value of the character size in UCSZ0, UCSZ1 */
	COPY_BITS(UCSRC_R, 0x03, a_uartConfig->charSize, UCSZ0);

	/* Calculate the UBRR register value */
	ubrrValue = (uint16_t)(((F_CPU / (a_uartConfig->baudRate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH_R = (uint8_t)(ubrrValue >> 8);
	UBRRL_R = (uint8_t)(ubrrValue);
}

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
void UART_setRxInterruptCallback(void (* volatile a_ptrToHandler)(void))
{
	/* save the rx interrupt pointer to callback */
	g_uartRxPtrToHandler = a_ptrToHandler;
}

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
void UART_setTxInterruptCallback(void (* volatile a_ptrToHandler)(void))
{
	/* save the tx interrupt pointer to callback */
	g_uartTxPtrToHandler = a_ptrToHandler;
}

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
void UART_sendByteBlocking(const uint8_t a_data)
{
	/*
	 * wait till the transfer buffer is empty
	 */
	while(!UART_TxIsEmpty());

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR_R = a_data;
}

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
void UART_sendByteNonBlocking(const uint8_t a_data)
{
	/*
	 * check if the transfer buffer is empty and ready to receive new data
	 */
	if(UART_TxIsEmpty())
	{
		/* send data */
		UDR_R = a_data;
	}
}

/*
 * [Function Name]: UART_receiveByteBlocking
 * [Function Description]: The function uses busy wait till a byte is received
 * 						   It can be used whether Rx interrupt is enabled or not
 * 						   If rx interrupt is enabled it will be disabled temporarely
 * 						   during the function time then re-enabled again
 * 						   It will not generate Rx interrupt after receiving even if Rx
 * 						   interrupt is enabled
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 the received byte
 */
uint8_t UART_receiveByteBlocking(void)
{
	uint8_t data;
	boolean rxInterruptEnabled = FALSE;

	/* check if the rx interrput is enabled */
	if(BIT_IS_SET(UCSRB_R, RXCIE))
	{
		/* disable it temporarely */
		CLEAR_BIT(UCSRB_R, RXCIE);
		rxInterruptEnabled = TRUE;
	}

	/* wait till data is available in receive buffer */
	while(!UART_DataIsAvailable());

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	data = UDR_R;

	/* re-enable rx enterrupt if it was enabled before entering the function */
	if(rxInterruptEnabled == TRUE)
	{
		/* re-enable it */
		SET_BIT(UCSRB_R, RXCIE);
	}

	return data;
}

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
uint8_t UART_receiveByteNonBlocking(void)
{
	/* check if data is available in the receive buffer */
	if(UART_DataIsAvailable())
	{
		/*
		 * Read the received data from the Rx buffer (UDR)
		 */
		return UDR_R;
	}

	/* return 0 if the receive buffer is empty */
	return 0;
}

/*
 * [Function Name]: UART_sendString
 * [Function Description]: The function sends the given string till it reaches the null character
 * 						   It uses polling techinque "busy wait"
 * [Args]:
 * [in]: const char * a_str
 * 		 string to be sent
 * [Return]: void
 */
void UART_sendString(const char * a_str)
{
	while(*a_str)
	{
		/* send char by char */
		UART_sendByteBlocking(*a_str);

		/* increment the pointer */
		a_str ++;
	}
}

/*
 * [Function Name]: UART_receiveString
 * [Function Description]: The function receives string using busy wait
 * 						   "polling technique" till
 * 						   1. the UART_RECEIVE_STRING_TILL is found
 * 						   	  this value can be modified in the config file
 * 						   2. reaching the maximum value a_maxSize
 * [Args]:
 * [in]: uint8_t * a_str
 * 		 array to store received string
 * [in]: uint8_t a_maxSize
 * 		 maximum size of the passed array to receive till
 * 		 it reached.
 * 		 It's the total size considering the null terminator, i.e if
 * 		 max size of 20 is passed, only 19 char will be saved and the 20th is '\0'
 * [Return]: void
 */
void UART_receiveString(uint8_t * a_str, uint8_t a_maxSize)
{
	/* counter to keep track of the size */
	uint8_t counter = 0;

	/* return if size is 0 or 1 */
	if(a_maxSize == 0)
	{
		return;
	}
	else if(a_maxSize == 1)
	{
		*a_str = '\0';
		return;
	}

	/* get the first character */
	*a_str = UART_receiveByteBlocking();

	/* loop till the UART_RECEIVE_STRING_TILL is found */
	while(*a_str != UART_RECEIVE_STRING_TILL)
	{
		/* increment the counter and the pointer */
		counter ++;
		a_str ++;

		/* break the loop if the length reached the max size - 1 to leave space for '\0' */
		if(counter >= a_maxSize - 1) break;

		/* get the next character */
		*a_str = UART_receiveByteBlocking();
	}

	/* add null terminator */
	*a_str = '\0';
}

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
uint8_t UART_DataIsAvailable(void)
{
	/* check if RXC flag is set (the UART receive data) */
	return BIT_IS_SET(UCSRA_R, RXC);
}

/*
 * [Function Name]: UART_TxIsEmpty
 * [Function Description]: The function checks if the transfer buffer is empty or not
 * [Args]:
 * [in]: void
 * [Return]: uint8_t
 * 			 TRUE if the tx buffer is empty
 * 			 FALSE otherwise
 */
uint8_t UART_TxIsEmpty(void)
{
	/* check if transfer buffer is empty */
	return BIT_IS_SET(UCSRA_R, UDRE);
}

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

/* ISR for uart Rx */
ISR(USART_RXC_vect)
{
	if(g_uartRxPtrToHandler != NULL)
	{
		(*g_uartRxPtrToHandler)();
	}

	/* The RX flag is cleared only when data is read
	 * so, this condition checks if it's not cleared (read) in the
	 * callback, it reads the data to clear it
	 */
	if(UART_DataIsAvailable())
	{
		UART_receiveByteNonBlocking();
	}
}

/* ISR for uart Tx */
ISR(USART_TXC_vect)
{
	if(g_uartTxPtrToHandler != NULL)
	{
		(*g_uartTxPtrToHandler)();
	}
}
