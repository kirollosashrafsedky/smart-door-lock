 /******************************************************************************
 *
 * Module: EXTERNAL EEPROM - M24c16
 *
 * File Name: external-eeprom.c
 *
 * Description: Source file for the EXTERNAL EEPROM driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __EXTERNAL_EEPROM_H__
#define __EXTERNAL_EEPROM_H__

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* For using std types */
#include "../../Lib/types.h"

/* For using common defines and macros */
#include "../../Lib/common.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* returned if the operation (R/W) encountered an error */
#define EEPROM_ERROR 						0

/* returned if the operation (R/W) completed successfully */
#define EEPROM_SUCCESS 						1

/*******************************************************************************
 *                           Function Prototypes                               *
 *******************************************************************************/

/*
 * [Function Name]: EEPROM_writeByte
 * [Function Description]: writes a byte to the eeprom in the specified address
 * [Args]:
 * [in]: uint16_t a_u16addr
 * 		 the address in the eeprom to write to
 * [in]: uint8_t a_u8data
 * 		 the data to write in the eeprom
 * [Return]: uint8_t
 * 			 EEPROM_ERROR or EEPROM_SUCCESS
 */
uint8_t EEPROM_writeByte(uint16_t a_u16addr, uint8_t a_u8data);

/*
 * [Function Name]: EEPROM_readByte
 * [Function Description]: reades a byte from the eeprom from the specified address
 * [Args]:
 * [in]: uint16_t a_u16addr
 * 		 the address in the eeprom to read from
 * [in]: uint8_t * a_u8data
 * 		 pointer to the location where the data will be saved
 * [Return]: uint8_t
 * 			 EEPROM_ERROR or EEPROM_SUCCESS
 */
uint8_t EEPROM_readByte(uint16_t a_u16addr, uint8_t * a_u8data);

#endif /* __EXTERNAL_EEPROM_H__ */
