/******************************************************************************
 *
 * Module: TWI (I2C)
 *
 * File Name: twi.c
 *
 * Description: Source file for the TWI (I2C) driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* module header file */
#include "twi.h"

/* For using mcu registers */
#include "../Mcu/mcu.h"

/*******************************************************************************
 *                          Functions Definition	                           *
 *******************************************************************************/

/*
 * [Function Name]: TWI_init
 * [Function Description]: initializes the TWI
 * [Args]:
 * [in]: ST_TwiConfig * a_twiConfig
 * 		 pointer to structure of the twi config
 * [Return]: void
 */
void TWI_init(ST_TwiConfig * a_twiConfig)
{

	/* copy the value of the prescaler to TWSR_R */
	TWSR_R = a_twiConfig->prescaler;

	/* copy the value of the bit rate to TWBR_R */
	TWBR_R = a_twiConfig->bitRate;

	/* General Call Recognition: Off */
	CLEAR_BIT(TWAR_R, TWGCE);

	/* Two Wire Bus address my address if any master device want to call me:
	 * (used in case this MC is a slave device)
	 * General Call Recognition: Off
	 */
	COPY_BITS(TWAR_R, 0xFE, a_twiConfig->slaveAddress, TWA0);

	/* enable TWI */
	TWCR_R = SELECT_BIT(TWEN);
}

/*
 * [Function Name]: TWI_start
 * [Function Description]: sends the start bit
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_start(void)
{
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR_R = SELECT_BIT(TWINT) | SELECT_BIT(TWSTA) | SELECT_BIT(TWEN);

	/* Wait for TWINT flag set in TWCR_R Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR_R, TWINT));
}

/*
 * [Function Name]: TWI_stop
 * [Function Description]: sends the stop bit
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_stop(void)
{
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR_R = SELECT_BIT(TWINT) | SELECT_BIT(TWSTO) | SELECT_BIT(TWEN);
}

/*
 * [Function Name]: TWI_stop
 * [Function Description]: sends a byte
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_writeByte(uint8_t a_data)
{
	/* Put data On TWI data Register */
	TWDR_R = a_data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR_R = SELECT_BIT(TWINT) | SELECT_BIT(TWEN);

	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while(BIT_IS_CLEAR(TWCR_R, TWINT));
}

/*
 * [Function Name]: TWI_readByteWithACK
 * [Function Description]: receives a byte and enables sending ack after receving
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR_R = SELECT_BIT(TWINT) | SELECT_BIT(TWEN) | SELECT_BIT(TWEA);

	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR_R, TWINT));

	/* Read Data */
	return TWDR_R;
}

/*
 * [Function Name]: TWI_readByteWithoutACK
 * [Function Description]: receives a byte without sending ack
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_readByteWithoutACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR_R = SELECT_BIT(TWINT) | SELECT_BIT(TWEN);

	/* Wait for TWINT flag set in TWCR_R Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR_R, TWINT));

	/* Read Data */
	return TWDR_R;
}

/*
 * [Function Name]: TWI_getStatus
 * [Function Description]: gets the twi status
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_getStatus(void)
{
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	return (TWSR_R & 0xF8);
}
