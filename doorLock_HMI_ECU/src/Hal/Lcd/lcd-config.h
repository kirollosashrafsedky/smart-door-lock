/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd-config.h
 *
 * Description: Config file for the LCD driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* 0 => 8-bit mode
 * 1 => 4-bit mode
 */
#define LCD_4_BIT_MODE						0

/* RS pin */
#define LCD_RS_PIN							PB2

/* R/W pin */
#define LCD_RW_PIN							PB1

/* Enable pin */
#define LCD_ENABLE_PIN						PB0

/* whether to use a single port for the data bits or not
 * 0 => you should determine below the pins for each data bit separately
 * 1 => you should connect the 4 or 8 data bits all in one port and
 * 		specify their start location
 * the second case is faster and more effecient so only use the first case in need
 */
#define LCD_USE_SINGLE_DATA_PORT			1

#if LCD_USE_SINGLE_DATA_PORT == 1

/* the port of the data bits */
#define LCD_DATA_PORT						PORTA

/* the start location of the data bits
 * in case of 4-bit mode, it should be the D4 bit
 * in case of 8-bit mode, it should be the D0 bit
 */
#define LCD_DATA_START_PIN					PA0

#else

#if LCD_4_BIT_MODE == 0

/* lcd data bits */
#define LCD_D0								PD0
#define LCD_D1								PD1
#define LCD_D2								PD2
#define LCD_D3								PD3

#endif /* LCD_4_BIT_MODE == 1 */

#define LCD_D4								PD4
#define LCD_D5								PD5
#define LCD_D6								PD6
#define LCD_D7								PD7

#endif /* LCD_USE_SINGLE_DATA_PORT == 1 */

#endif /* __LCD_CONFIG_H__ */
