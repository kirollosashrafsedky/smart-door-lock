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

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "external-eeprom.h"

/* For using TWI module */
#include "../../Mcal/Twi/twi.h"

#include "../../Mcal/Dio/dio.h"

/*******************************************************************************
 *                          Functions Definition	                           *
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
uint8_t EEPROM_writeByte(uint16_t a_u16addr, uint8_t a_u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8_t)(0xA0 | ((a_u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8_t)(a_u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* write byte to eeprom */
	TWI_writeByte(a_u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return EEPROM_SUCCESS;
}

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
uint8_t EEPROM_readByte(uint16_t a_u16addr, uint8_t * a_u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8_t)((0xA0) | ((a_u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return EEPROM_ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8_t)(a_u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return EEPROM_ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return EEPROM_ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8_t)((0xA0) | ((a_u16addr & 0x0700) >> 7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return EEPROM_ERROR;

	/* Read Byte from Memory without send ACK */
	*a_u8data = TWI_readByteWithoutACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return EEPROM_ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return EEPROM_SUCCESS;
}
