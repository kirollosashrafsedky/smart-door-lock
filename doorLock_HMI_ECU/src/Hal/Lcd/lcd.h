/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __LCD_H__
#define __LCD_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module config file */
#include "lcd-config.h"

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* RS value when sending cmd */
#define RS_CMD											0

/* RS value when sending data */
#define RS_DATA											1

/* R/W value when reading */
#define RW_READ											1

/* R/W value when writing */
#define RW_WRITE										0

/*------------Commands------------*/

/* clear screen */
#define	LCD_CLEAR_SCREEN								0x01
/* return home */
#define	LCD_RETURN_HOME									0x02

/* make screan left to right */
#define	LCD_LTR											0x06
/* make screan right to left */
#define	LCD_RTL											0x04
/* shift next data to the left */
#define	LCD_SHIFT_NEXT_LEFT								0x07
/* shift next data to the right */
#define	LCD_SHIFT_NEXT_RIGHT							0x05

/* turn off display */
#define	LCD_DISPLAY_OFF									0x08
/* turn on display and turn off cursor - default when initializing the lcd*/
#define	LCD_DISPLAY_ON_CURSOR_OFF						0x0C
/* turn on display, cursor and blinking */
#define	LCD_DISPLAY_ON_CURSOR_ON_BLINKING_ON			0x0F
/* turn on display and cursor and turn off blinking */
#define	LCD_DISPLAY_ON_CURSOR_ON_BLINKING_OFF			0x0E

/* move cursor to the left */
#define	LCD_MOVE_CURSOR_LEFT							0x10
/* move cursor to the right */
#define	LCD_MOVE_CURSOR_RIGHT							0x14
/* shift all screen left */
#define	LCD_SHIFT_LEFT									0x18
/* shift all screen right */
#define	LCD_SHIFT_RIGHT									0x1C

/* base address for setting cursor location */
#define LCD_SET_CURSOR_BASE_ADDRESS						0x80

#if LCD_4_BIT_MODE == 0

/* 2 lines, 5x11 font size */
#define	LCD_2_LINES_LG_FONT								0x3C
/* 2 lines, 5x8 font size - default when initializing the lcd */
#define	LCD_2_LINES_SM_FONT								0x38
/* 1 line, 5x11 font size */
#define	LCD_1_LINE_LG_FONT								0x34
/* 1 line, 5x8 font size */
#define	LCD_1_LINE_SM_FONT								0x30

#else

/* 2 lines, 5x11 font size */
#define	LCD_2_LINES_LG_FONT								0x2C
/* 2 lines, 5x8 font size - default when initializing the lcd */
#define	LCD_2_LINES_SM_FONT								0x28
/* 1 line, 5x11 font size */
#define	LCD_1_LINE_LG_FONT								0x24
/* 1 line, 5x8 font size */
#define	LCD_1_LINE_SM_FONT								0x20

#endif	/* LCD_4_BIT_MODE == 0 */

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: LCD_init
 * [Function Description]: initializes the lcd
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void LCD_init(void);

 /*
  * [Function Name]: LCD_sendCommand
  * [Function Description]: send command to the lcd
  * [Args]:
  * [in]: uint8_t a_cmd
  * 	  command to be sent
  * [Return]: void
  */
void LCD_sendCommand(uint8_t a_cmd);

/*
 * [Function Name]: LCD_sendChar
 * [Function Description]: send char to the lcd
 * [Args]:
 * [in]: uint8_t a_data
 * 	  	 character to be sent
 * [Return]: void
 */
void LCD_sendChar(uint8_t a_data);

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
void LCD_sendCharAt(uint8_t a_row, uint8_t a_col, uint8_t a_data);

/*
 * [Function Name]: LCD_sendStr
 * [Function Description]: send string to the lcd
 * [Args]:
 * [in]: const uint8_t * a_data
 * 	  	 string to be sent
 * [Return]: void
 */
void LCD_sendStr(const uint8_t * a_data);

/*
 * [Function Name]: LCD_sendStrAt
 * [Function Description]: send string to the lcd at specific location
 * [Args]:
 * [in]: uint8_t a_row
 * 	  	 index of the row to write in
 * [in]: uint8_t a_col
 * 	  	 index of the column to write in
 * [in]: const uint8_t * a_data
 * 	  	 string to be sent
 * [Return]: void
 */
void LCD_sendStrAt(uint8_t a_row, uint8_t a_col, const uint8_t * a_data);

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
void LCD_setCursor(uint8_t a_row, uint8_t a_col);

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
void LCD_sendInteger(int32_t a_num, uint8_t a_minLength);

#endif /* __LCD_H__ */
