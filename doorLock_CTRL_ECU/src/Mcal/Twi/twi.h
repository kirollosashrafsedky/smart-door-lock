/******************************************************************************
 *
 * Module: TWI (I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI (I2C) driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __TWI_H__
#define __TWI_H__

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

/* I2C Status Bits in the TWSR Register */

/* start has been sent */
#define TWI_START         0x08

/* repeated start */
#define TWI_REP_START     0x10

/* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_W_ACK  0x18

/* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40

/* Master transmit data and ACK has been received from Slave. */
#define TWI_MT_DATA_ACK   0x28

/* Master received data and send ACK to slave. */
#define TWI_MR_DATA_ACK   0x50

/* Master received data but doesn't send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58

/*******************************************************************************
 *                             Types Declaration                               *
 *******************************************************************************/

/*
 * [Enum Name]: EN_TWI_PRESCALERS
 * [Enum Description]: contains twi available prescalers
 */
typedef enum
{
	TWI_PRESCALER_1,
	TWI_PRESCALER_4,
	TWI_PRESCALER_16,
	TWI_PRESCALER_64
}EN_TWI_PRESCALERS;

/*
 * [Struct Name]: ST_TwiConfig
 * [Struct Description]: contains twi config, note that actual desired SCL frequency of the
 * 						 twi is calculated as follows:
 * 						 SCL freq = F_CPU / (16 + 2*bitRate * 4^prescaler)
 * 						 where bitRate is an 8-bit value passed through the structure,
 * 						 and prescaler is a number from 0 to 4, can be chosen from the
 * 						 EN_TWI_PRESCALERS and passed through the structure
 */
typedef struct
{
	/*address of the mcu when acting as a slave */
	uint8_t slaveAddress;

	/* twi prescaler */
	EN_TWI_PRESCALERS prescaler;

	/* bit rate */
	uint8_t bitRate;

}ST_TwiConfig;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * [Function Name]: TWI_init
 * [Function Description]: initializes the TWI
 * [Args]:
 * [in]: ST_TwiConfig * a_twiConfig
 * 		 pointer to structure of the twi config
 * [Return]: void
 */
void TWI_init(ST_TwiConfig * a_twiConfig);

/*
 * [Function Name]: TWI_start
 * [Function Description]: sends the start bit
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_start(void);

/*
 * [Function Name]: TWI_stop
 * [Function Description]: sends the stop bit
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_stop(void);

/*
 * [Function Name]: TWI_stop
 * [Function Description]: sends a byte
 * [Args]:
 * [in]: void
 * [Return]: void
 */
void TWI_writeByte(uint8_t a_data);

/*
 * [Function Name]: TWI_readByteWithACK
 * [Function Description]: receives a byte and enables sending ack after receving
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_readByteWithACK(void);

/*
 * [Function Name]: TWI_readByteWithoutACK
 * [Function Description]: receives a byte without sending ack
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_readByteWithoutACK(void);

/*
 * [Function Name]: TWI_getStatus
 * [Function Description]: gets the twi status
 * [Args]:
 * [in]: void
 * [Return]: void
 */
uint8_t TWI_getStatus(void);

#endif /* __TWI_H__ */
