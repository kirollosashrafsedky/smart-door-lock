/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "lcd.h"

/* For using dio functions for pins */
#include "../../Mcal/Dio/dio.h"

/* For using delay function */
#include "../../Mcal/Timer/timer.h"

/*******************************************************************************
 *                      Static Functions Prototypes	                           *
 *******************************************************************************/

/*
 * [Function Name]: LCD_setDataPinsDir
 * [Function Description]: set the direction of the pins/port of the lcd
 * 						   as input or output, input is used to read if the screen
 * 						   is busy, output is used in all writing operations
 * [Args]:
 * [in]: uint8_t a_direction
 * 	  	 direction to be set
 * [Return]: void
 */
static void LCD_setDataPinsDir(uint8_t a_direction);

/*
 * [Function Name]: LCD_waitTillNotBusy
 * [Function Description]: busy wait till the screen is not busy
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void LCD_waitTillNotBusy(void);

/*
 * [Function Name]: LCD_writeToDataPins
 * [Function Description]: write data to data pins/port
 * [Args]:
 * [in]: uint8_t a_data
 * 		 data to be written
 * [Return]: void
 */
static void LCD_writeToDataPins(uint8_t a_data);

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

/*
 * [Function Name]: LCD_init
 * [Function Description]: initializes the lcd
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void LCD_init(void)
{
	/* init RS, R/W, enable pins as output  */
	DIO_pinInit(LCD_RS_PIN, PIN_OUTPUT);
	DIO_pinInit(LCD_RW_PIN, PIN_OUTPUT);
	DIO_pinInit(LCD_ENABLE_PIN, PIN_OUTPUT);

#if LCD_4_BIT_MODE == 1
	/* use 4-bit mode */
	LCD_sendCommand(0x02);
#endif

#if LCD_USE_SINGLE_DATA_PORT == 1
	/* init data port as output */
	LCD_setDataPinsDir(PORT_OUTPUT);

#else
	/* init data pins as output */
	LCD_setDataPinsDir(PIN_OUTPUT);

#endif

	/* use 2-lines with 5*8 font size as default */
	LCD_sendCommand(LCD_2_LINES_SM_FONT);

	/* turn on display and turn off cursor */
	LCD_sendCommand(LCD_DISPLAY_ON_CURSOR_OFF);

	/* clear lcd */
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}

/*
 * [Function Name]: LCD_sendCommand
 * [Function Description]: send command to the lcd
 * [Args]:
 * [in]: uint8_t a_cmd
 * 	  command to be sent
 * [Return]: void
 */
void LCD_sendCommand(uint8_t a_cmd)
{
	/* make sure lcd is not busy */
	LCD_waitTillNotBusy();

	/* make RS=0 to send commmand and R/W=0 to write */
	DIO_writePin(LCD_RS_PIN, RS_CMD);
	DIO_writePin(LCD_RW_PIN, RW_WRITE);

	TIMER_DELAY_MS(1);

	/* write high to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, HIGH);

	TIMER_DELAY_MS(1);

#if LCD_4_BIT_MODE == 1

	/* write higher 4 bits of data to data pins/port */
	LCD_writeToDataPins((a_cmd & 0xF0) >> 4);

	TIMER_DELAY_MS(1);

	/* write low to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, LOW);

	TIMER_DELAY_MS(1);

	/* write high to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, HIGH);

	TIMER_DELAY_MS(1);

	/* write lower 4 bits of data to data pins/port */
	LCD_writeToDataPins(a_cmd & 0x0F);

#else

	/* write 8-bits data to data port/pins */
	LCD_writeToDataPins(a_cmd);

#endif /* LCD_4_BIT_MODE == 1 */

	TIMER_DELAY_MS(1);

	/* write low to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, LOW);

	TIMER_DELAY_MS(1);

}

/*
 * [Function Name]: LCD_sendChar
 * [Function Description]: send char to the lcd
 * [Args]:
 * [in]: uint8_t a_data
 * 	  	 character to be sent
 * [Return]: void
 */
void LCD_sendChar(uint8_t a_data)
{
	/* make sure lcd is not busy */
	LCD_waitTillNotBusy();

	/* make RS=1 to send data and R/W=0 to write */
	DIO_writePin(LCD_RS_PIN, RS_DATA);
	DIO_writePin(LCD_RW_PIN, RW_WRITE);

	TIMER_DELAY_MS(1);

	/* write high to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, HIGH);

	TIMER_DELAY_MS(1);

#if LCD_4_BIT_MODE == 1

	/* write higher 4 bits of data to data pins/port */
	LCD_writeToDataPins((a_data & 0xF0) >> 4);

	TIMER_DELAY_MS(1);

	/* write low to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, LOW);

	TIMER_DELAY_MS(1);

	/* write high to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, HIGH);

	TIMER_DELAY_MS(1);

	/* write lower 4 bits of data to data pins/port */
	LCD_writeToDataPins(a_data & 0x0F);

#else

	/* write 8-bits data to data port/pins */
	LCD_writeToDataPins(a_data);

#endif /* LCD_4_BIT_MODE == 1 */

	TIMER_DELAY_MS(1);

	/* write low to enable pin */
	DIO_writePin(LCD_ENABLE_PIN, LOW);

	TIMER_DELAY_MS(1);

}

/*
 * [Function Name]: LCD_sendChar
 * [Function Description]: send char to the lcd at specific location
 * [Args]:
 * [in]: uint8_t a_row
 * 	  	 index of the row to write in
 * [in]: uint8_t a_col
 * 	  	 index of the column to write in
 * [in]: uint8_t a_data
 * 	  	 character to be sent
 * [Return]: void
 */
void LCD_sendCharAt(uint8_t a_row, uint8_t a_col, uint8_t a_data)
{
	/* set cursor to row and col */
	LCD_setCursor(a_row, a_col);

	/* write character */
	LCD_sendChar(a_data);
}

/*
 * [Function Name]: LCD_sendStr
 * [Function Description]: send string to the lcd
 * [Args]:
 * [in]: const char * a_data
 * 	  	 string to be sent
 * [Return]: void
 */
void LCD_sendStr(const uint8_t * a_data)
{
	/* loop till reach the null char '\0' */
	while(*a_data)
	{
		LCD_sendChar(*a_data);
		a_data ++;
	}
}

/*
 * [Function Name]: LCD_sendStrAt
 * [Function Description]: send string to the lcd at specific location
 * [Args]:
 * [in]: uint8_t a_row
 * 	  	 index of the row to write in
 * [in]: uint8_t a_col
 * 	  	 index of the column to write in
 * [in]: const char * a_data
 * 	  	 string to be sent
 * [Return]: void
 */
void LCD_sendStrAt(uint8_t a_row, uint8_t a_col, const uint8_t * a_data)
{
	/* set cursor to row and col */
	LCD_setCursor(a_row, a_col);

	/* write string */
	LCD_sendStr(a_data);
}

/*
 * [Function Name]: LCD_setCursor
 * [Function Description]: set the cursor location in the lcd
 * [Args]:
 * [in]: uint8_t a_row
 * 	  	 index of the row
 * [in]: uint8_t a_col
 * 	  	 index of the column
 * [Return]: void
 */
void LCD_setCursor(uint8_t a_row, uint8_t a_col)
{
	uint8_t lcdMemoryAddress;

	/* Calculate the required address in the LCD DDRAM */
	switch(a_row)
	{
	case 0:
		lcdMemoryAddress = a_col;
		break;
	case 1:
		lcdMemoryAddress = a_col + 0x40;
		break;
	case 2:
		lcdMemoryAddress = a_col + 0x10;
		break;
	case 3:
		lcdMemoryAddress = a_col + 0x50;
		break;
	default :
		return;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcdMemoryAddress | LCD_SET_CURSOR_BASE_ADDRESS);
}

/*
 * [Function Name]: LCD_sendInteger
 * [Function Description]: send integer to the lcd
 * [Args]:
 * [in]: int32_t a_num
 * 	  	 number to be sent, can be any 32-bit signed number
 * [in]: uint8_t a_minLength
 * 	  	 minimum length to reserve for this number by adding ' ' spaces after it tell
 * 	  	 the number length becomes equal to a_minLength
 * [Return]: void
 */
void LCD_sendInteger(int32_t a_num, uint8_t a_minLength)
{
	/* buffer will hold the string after conversion, bufferPtr is a pointer to buffer */
	uint8_t buffer[16], * bufferPtr;
	/* tempNum will hold the number during parsing it */
	int32_t tempNum;
	/* numLength will hold the number of digits of the number */
	uint8_t numLength, loopCounter;

	bufferPtr = buffer;
	tempNum = a_num;

	/* put '-' if the number is negative */
	if (a_num < 0)
	{
		*bufferPtr++ = '-';
		/* turn the number to positive */
		a_num = -1 * a_num;
	}
	/* reserve space in the buffer array equal to the number of digits + null char */
	do
	{
		tempNum /= 10;
		bufferPtr++;
	}while(tempNum);

	/* make the last position euqal to null char */
	*bufferPtr = '\0';

	/* calculate the length of the number */
	numLength = bufferPtr - buffer;
	tempNum = a_num;

	/* convert each digit to it corresponding ascii */
	do
	{
		*--bufferPtr= tempNum % 10 + '0';
		tempNum /= 10;
	}while(tempNum);

	/* write string to lcd */
	LCD_sendStr(buffer);

	/* check to add spaces ' ' if a_minLength is bigger than numLength */
	/* this is used to override any past numbers */
	if(a_minLength > numLength)
	{
		for(loopCounter = 0; loopCounter < (a_minLength - numLength); loopCounter++)
		{
			LCD_sendChar(' ');
		}
	}
}

/*
 * [Function Name]: LCD_setDataPinsDir
 * [Function Description]: set the direction of the pins/port of the lcd
 * 						   as input or output, input is used to read if the screen
 * 						   is busy, output is used in all writing operations
 * [Args]:
 * [in]: uint8_t a_direction
 * 	  	 direction to be set
 * [Return]: void
 */
static void LCD_setDataPinsDir(uint8_t a_direction)
{
#if LCD_4_BIT_MODE == 1 && LCD_USE_SINGLE_DATA_PORT == 1

	/* init only 4 successive ppins of the port */
	DIO_portInitPartial(LCD_DATA_PORT, a_direction, 0x0F, LCD_DATA_START_PIN);

#elif LCD_4_BIT_MODE == 1 && LCD_USE_SINGLE_DATA_PORT == 0

	/* init pins individually */
	DIO_pinInit(LCD_D4, a_direction);
	DIO_pinInit(LCD_D5, a_direction);
	DIO_pinInit(LCD_D6, a_direction);
	DIO_pinInit(LCD_D7, a_direction);

#elif LCD_4_BIT_MODE == 0 && LCD_USE_SINGLE_DATA_PORT == 1

	/* init 8 successive pins of the port */
	DIO_portInitPartial(LCD_DATA_PORT, a_direction, 0xFF, LCD_DATA_START_PIN);

#elif LCD_4_BIT_MODE == 0 && LCD_USE_SINGLE_DATA_PORT == 0

	/* init pins individually */
	DIO_pinInit(LCD_D0, a_direction);
	DIO_pinInit(LCD_D1, a_direction);
	DIO_pinInit(LCD_D2, a_direction);
	DIO_pinInit(LCD_D3, a_direction);
	DIO_pinInit(LCD_D4, a_direction);
	DIO_pinInit(LCD_D5, a_direction);
	DIO_pinInit(LCD_D6, a_direction);
	DIO_pinInit(LCD_D7, a_direction);

#endif
}

/*
 * [Function Name]: LCD_waitTillNotBusy
 * [Function Description]: busy wait till the screen is not busy
 * [Args]:
 * [in]: void
 * [Return]: void
 */
static void LCD_waitTillNotBusy()
{
	uint8_t isBusy = 0;

	/* make datapins input */
#if LCD_USE_SINGLE_DATA_PORT == 1

	LCD_setDataPinsDir(PORT_INPUT);

#else

	LCD_setDataPinsDir(PIN_INPUT);

#endif

	/* make RS=0 to read cmd, R/W=1 to read */
	DIO_writePin(LCD_RS_PIN, RS_CMD);
	DIO_writePin(LCD_RW_PIN, RW_READ);

	do{
#if (LCD_4_BIT_MODE == 1)

		DIO_writePin(LCD_ENABLE_PIN, HIGH);

		TIMER_DELAY_MS(1);

#if LCD_USE_SINGLE_DATA_PORT == 1
		/* read the 4th pin D7 to check the busy flag */
		isBusy = DIO_readPin(LCD_DATA_START_PIN + 3);
#else
		isBusy = DIO_readPin(LCD_D7);
#endif /* LCD_USE_SINGLE_DATA_PORT == 1 */

		DIO_writePin(LCD_ENABLE_PIN, LOW);

		TIMER_DELAY_MS(1);

		DIO_writePin(LCD_ENABLE_PIN, HIGH);

		TIMER_DELAY_MS(1);

		DIO_writePin(LCD_ENABLE_PIN, LOW);

#else
		DIO_writePin(LCD_ENABLE_PIN, HIGH);

		TIMER_DELAY_MS(1);

#if LCD_USE_SINGLE_DATA_PORT == 1
		/* read the 8th pin D7 to check the busy flag */
		isBusy = DIO_readPin(LCD_DATA_START_PIN + 7);
#else
		isBusy = DIO_readPin(LCD_D7);
#endif /* LCD_USE_SINGLE_DATA_PORT == 1 */

		TIMER_DELAY_MS(1);

		DIO_writePin(LCD_ENABLE_PIN, LOW);
#endif
	}while(isBusy);

	/* return the data pins to output mode */
#if LCD_USE_SINGLE_DATA_PORT == 1

	LCD_setDataPinsDir(PORT_OUTPUT);

#else

	LCD_setDataPinsDir(PIN_OUTPUT);

#endif

}

/*
 * [Function Name]: LCD_writeToDataPins
 * [Function Description]: write data to data pins/port
 * [Args]:
 * [in]: uint8_t a_data
 * 		 data to be written
 * [Return]: void
 */
static void LCD_writeToDataPins(uint8_t a_data)
{
#if LCD_4_BIT_MODE == 1 && LCD_USE_SINGLE_DATA_PORT == 1

	/* write to only 4 successive pins of the port */
	DIO_writePortPartial(LCD_DATA_PORT, a_data, 0x0F, LCD_DATA_START_PIN);

#elif LCD_4_BIT_MODE == 1 && LCD_USE_SINGLE_DATA_PORT == 0

	/* write to indvidual pins */
	DIO_writePin(LCD_D4, GET_BIT(a_data, 0));
	DIO_writePin(LCD_D5, GET_BIT(a_data, 1));
	DIO_writePin(LCD_D6, GET_BIT(a_data, 2));
	DIO_writePin(LCD_D7, GET_BIT(a_data, 3));

#elif LCD_4_BIT_MODE == 0 && LCD_USE_SINGLE_DATA_PORT == 1

	/* write to 8 successive pins of the port */
	DIO_writePortPartial(LCD_DATA_PORT, a_data, 0xFF, LCD_DATA_START_PIN);

#elif LCD_4_BIT_MODE == 0 && LCD_USE_SINGLE_DATA_PORT == 0

	/* write to indvidual pins */
	DIO_writePin(LCD_D0, GET_BIT(a_data, 0));
	DIO_writePin(LCD_D1, GET_BIT(a_data, 1));
	DIO_writePin(LCD_D2, GET_BIT(a_data, 2));
	DIO_writePin(LCD_D3, GET_BIT(a_data, 3));
	DIO_writePin(LCD_D4, GET_BIT(a_data, 4));
	DIO_writePin(LCD_D5, GET_BIT(a_data, 5));
	DIO_writePin(LCD_D6, GET_BIT(a_data, 6));
	DIO_writePin(LCD_D7, GET_BIT(a_data, 7));

#endif

}
