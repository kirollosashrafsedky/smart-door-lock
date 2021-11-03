#ifndef __ATMEGA32_H__
#define __ATMEGA32_H__

#include "./Lib/types.h"

/** General **/
#define SFIOR_R 	(*(volatile uint8_t*)(0x50))

/** DIO **/
/* DDRx Registers */
#define DDRA_R 		(*(volatile uint8_t*)(0x3A))
#define DDRB_R 		(*(volatile uint8_t*)(0x37))
#define DDRC_R 		(*(volatile uint8_t*)(0x34))
#define DDRD_R 		(*(volatile uint8_t*)(0x31))

/* PORTx Registers */
#define PORTA_R		(*(volatile uint8_t*)(0x3B))
#define PORTB_R		(*(volatile uint8_t*)(0x38))
#define PORTC_R 	(*(volatile uint8_t*)(0x35))
#define PORTD_R 	(*(volatile uint8_t*)(0x32))

/* PINx Registers */
#define PINA_R 		(*(volatile uint8_t*)(0x39))
#define PINB_R 		(*(volatile uint8_t*)(0x36))
#define PINC_R 		(*(volatile uint8_t*)(0x33))
#define PIND_R 		(*(volatile uint8_t*)(0x30))

/** External Interrupts **/
#define MCUCR_R 	(*(volatile uint8_t*)(0x55))
#define MCUCSR_R 	(*(volatile uint8_t*)(0x54))
#define GICR_R 		(*(volatile uint8_t*)(0x5B))
#define GIFR_R 		(*(volatile uint8_t*)(0x5A))

/** Timers **/
#define TCCR0_R 	(*(volatile uint8_t*)(0x53))
#define TCNT0_R 	(*(volatile uint8_t*)(0x52))
#define OCR0_R 		(*(volatile uint8_t*)(0x5C))
#define TIMSK_R 	(*(volatile uint8_t*)(0x59))
#define TIFR_R 		(*(volatile uint8_t*)(0x58))
#define TCCR1A_R 	(*(volatile uint8_t*)(0x4F))
#define TCCR1B_R 	(*(volatile uint8_t*)(0x4E))
#define TCNT1L_R 	(*(volatile uint8_t*)(0x4C))
#define TCNT1H_R 	(*(volatile uint8_t*)(0x4D))
#define TCNT1_R 	(*(volatile uint16_t*)(0x4C))
#define OCR1AL_R 	(*(volatile uint8_t*)(0x4A))
#define OCR1AH_R 	(*(volatile uint8_t*)(0x4B))
#define OCR1A_R 	(*(volatile uint16_t*)(0x4A))
#define OCR1BL_R 	(*(volatile uint8_t*)(0x48))
#define OCR1BH_R 	(*(volatile uint8_t*)(0x49))
#define OCR1B_R 	(*(volatile uint16_t*)(0x48))
#define ICR1L_R 	(*(volatile uint8_t*)(0x46))
#define ICR1H_R 	(*(volatile uint8_t*)(0x47))
#define ICR1_R 		(*(volatile uint16_t*)(0x46))
#define TCCR2_R 	(*(volatile uint8_t*)(0x45))
#define TCNT2_R 	(*(volatile uint8_t*)(0x44))
#define OCR2_R 		(*(volatile uint8_t*)(0x43))
#define ASSR_R 		(*(volatile uint8_t*)(0x42))

/** WATCH DOG **/
#define WDTCR_R 	(*(volatile uint8_t*)(0x41))

/** ADC **/
#define ADMUX_R 	(*(volatile uint8_t*)(0x27))
#define ADCSRA_R 	(*(volatile uint8_t*)(0x26))
#define ADCH_R 		(*(volatile uint8_t*)(0x25))
#define ADCL_R 		(*(volatile uint8_t*)(0x24))
#define ADC_R 		(*(volatile uint16_t*)(0x24))

/** USART **/
#define UDR_R 		(*(volatile uint8_t*)(0x2C))
#define UCSRA_R 	(*(volatile uint8_t*)(0x2B))
#define UCSRB_R 	(*(volatile uint8_t*)(0x2A))
#define UCSRC_R 	(*(volatile uint8_t*)(0x40))
#define UBRRL_R 	(*(volatile uint8_t*)(0x29))
#define UBRRH_R 	(*(volatile uint8_t*)(0x40))

/** SPI **/
#define SPCR_R 		(*(volatile uint8_t*)(0x2D))
#define SPSR_R 		(*(volatile uint8_t*)(0x2E))
#define SPDR_R 		(*(volatile uint8_t*)(0x2F))

/** TWI **/
#define TWBR_R 		(*(volatile uint8_t*)(0x20))
#define TWCR_R 		(*(volatile uint8_t*)(0x56))
#define TWSR_R 		(*(volatile uint8_t*)(0x21))
#define TWDR_R 		(*(volatile uint8_t*)(0x23))
#define TWAR_R 		(*(volatile uint8_t*)(0x22))

/* start address of PORTx = PORTA address */
#define PORT_START_LOC		(0x3B)
/* start address of DDRx = DDRA address */
#define DDR_START_LOC		(0x3A)
/* start address of PINx = PINA address */
#define PIN_START_LOC		(0x39)

#define PORTS_OFFSET		0x03

/** Register bits **/

/* SFIOR */
#define PSR10			0
#define PSR2			1
#define PUD				2
#define ACME			3
#define ADTS0			5
#define ADTS1			6
#define ADTS2			7

/* External Interrupts */
#define ISC00			0
#define ISC01			1
#define ISC10			2
#define ISC11			3
#define ISC2			6

#define INT0			6
#define INT1			7
#define INT2			5

#define INTF0			6
#define INTF1			7
#define INTF2			5

#define INT0_PIN		PD2
#define INT1_PIN		PD3
#define INT2_PIN		PB2

/** Timers **/

/* TCCR0 */
#define CS00			0
#define CS01			1
#define CS02			2
#define WGM01			3
#define COM00			4
#define COM01			5
#define WGM00			6
#define FOC0			7

/* TIMSK */
#define TOIE0			0
#define OCIE0			1
#define TOIE1			2
#define OCIE1B			3
#define OCIE1A			4
#define TICIE1			5
#define TOIE2			6
#define OCIE2			7

/* TIFR */
#define TOV0			0
#define OCF0			1
#define TOV1			2
#define OCF1B			3
#define OCF1A			4
#define ICF1			5
#define TOV2			6
#define OCF2			7

/* TCCR1A */
#define WGM10			0
#define WGM11			1
#define FOC1B			2
#define FOC1A			3
#define COM1B0			4
#define COM1B1			5
#define COM1A0			6
#define COM1A1			7

/* TCCR1B */
#define CS10			0
#define CS11			1
#define CS12			2
#define WGM12			3
#define WGM13			4

#define ICES1			6
#define ICNC1			7

/* TCCR2 */
#define CS20			0
#define CS21			1
#define CS22			2
#define WGM21			3
#define COM20			4
#define COM21			5
#define WGM20			6
#define FOC2			7

/* ASSR */
#define TCR2UB			0
#define OCR2UB			1
#define TCN2UB			2
#define AS2				3

/* TIMER PINS */
#define OC0				PB3
#define PWM0			PB3

#define OC1A			PD5
#define PWM1A			PD5

#define OC1B			PD4
#define PWM1B			PD4

#define OC2				PD7
#define PWM2			PD7

/* WDTCR */
#define WDP0			0
#define WDP1			1
#define WDP2			2
#define WDE				3
#define WDTOE			4

/* ADMUX */
#define MUX0			0
#define MUX1			1
#define MUX2			2
#define MUX3			3
#define MUX4			4
#define ADLAR			5
#define REFS0			6
#define REFS1			7

/* ADCSRA */
#define ADPS0			0
#define ADPS1			1
#define ADPS2			2
#define ADIE			3
#define ADIF			4
#define ADATE			5
#define ADSC			6
#define ADEN			7

/** USART **/

/* UCSRA */
#define MPCM			0
#define U2X				1
#define PE				2
#define DOR				3
#define FE				4
#define UDRE			5
#define TXC				6
#define RXC				7

/* UCSRB */
#define TXB8			0
#define RXB8			1
#define UCSZ2			2
#define TXEN			3
#define RXEN			4
#define UDRIE			5
#define TXCIE			6
#define RXCIE			7

/* UCSRC */
#define UCPOL			0
#define UCSZ0			1
#define UCSZ1			2
#define USBS			3
#define UPM0			4
#define UPM1			5
#define UMSEL			6
#define URSEL			7

/** SPI **/

/* SPCR */
#define SPR0			0
#define SPR1			1
#define CPHA			2
#define CPOL			3
#define MSTR			4
#define DORD			5
#define SPE				6
#define SPIE			7

/* SPSR */
#define SPI2X			0
#define WCOL			6
#define SPIF			7

/** TWI **/

/* TWCR */
#define TWIE			0
#define TWEN			2
#define TWWC			3
#define TWSTO			4
#define TWSTA			5
#define TWEA			6
#define TWINT			7

/* TWSR */
#define TWPS0			0
#define TWPS1			1
#define TWS3			3
#define TWS4			4
#define TWS5			5
#define TWS6			6
#define TWS7			7

/* TWAR */
#define TWGCE			0
#define TWA0			1
#define TWA1			2
#define TWA2			3
#define TWA3			4
#define TWA4			5
#define TWA5			6
#define TWA6			7

/* Vector Table */

/* External Interrupt Request 0 */
#define INT0_vect				_VECTOR(1)

/* External Interrupt Request 1 */
#define INT1_vect				_VECTOR(2)

/* External Interrupt Request 2 */
#define INT2_vect				_VECTOR(3)

/* Timer/Counter2 Compare Match */
#define TIMER2_COMP_vect		_VECTOR(4)

/* Timer/Counter2 Overflow */
#define TIMER2_OVF_vect			_VECTOR(5)

/* Timer/Counter1 Capture Event */
#define TIMER1_CAPT_vect		_VECTOR(6)

/* Timer/Counter1 Compare Match A */
#define TIMER1_COMPA_vect		_VECTOR(7)

/* Timer/Counter1 Compare Match B */
#define TIMER1_COMPB_vect		_VECTOR(8)

/* Timer/Counter1 Overflow */
#define TIMER1_OVF_vect			_VECTOR(9)

/* Timer/Counter0 Compare Match */
#define TIMER0_COMP_vect		_VECTOR(10)

/* Timer/Counter0 Overflow */
#define TIMER0_OVF_vect			_VECTOR(11)

/* Serial Transfer Complete */
#define SPI_STC_vect			_VECTOR(12)

/* USART, Rx Complete */
#define USART_RXC_vect			_VECTOR(13)

/* USART Data Register Empty */
#define USART_UDRE_vect			_VECTOR(14)

/* USART, Tx Complete */
#define USART_TXC_vect			_VECTOR(15)

/* ADC Conversion Complete */
#define ADC_vect				_VECTOR(16)

/* EEPROM Ready */
#define EE_RDY_vect				_VECTOR(17)

/* Analog Comparator */
#define ANA_COMP_vect			_VECTOR(18)

/* 2-wire Serial Interface */
#define TWI_vect				_VECTOR(19)

/* Store Program Memory Ready */
#define SPM_RDY_vect			_VECTOR(20)

#define _VECTORS_SIZE 84

#endif /* __ATMEGA32_H__ */
