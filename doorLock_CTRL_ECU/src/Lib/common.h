#ifndef __COMMON_H__
#define __COMMON_H__

#define NULL 				(void *) (0x0)

#define TRUE				1
#define FALSE				0

#define HIGH 				1
#define LOW 				0

#define ALL_HIGH 			0xFF
#define ALL_LOW 			0x00

/** PORTS **/
#define PORTS_NO 4

#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3

/** PINS **/
/* PORTA PINS */
#define PA0 (0x00)
#define PA1 (0x01)
#define PA2 (0x02)
#define PA3 (0x03)
#define PA4 (0x04)
#define PA5 (0x05)
#define PA6 (0x06)
#define PA7 (0x07)

/* PORTB PINS */
#define PB0 (0x10)
#define PB1 (0x11)
#define PB2 (0x12)
#define PB3 (0x13)
#define PB4 (0x14)
#define PB5 (0x15)
#define PB6 (0x16)
#define PB7 (0x17)

/* PORTC PINS */
#define PC0 (0x20)
#define PC1 (0x21)
#define PC2 (0x22)
#define PC3 (0x23)
#define PC4 (0x24)
#define PC5 (0x25)
#define PC6 (0x26)
#define PC7 (0x27)

/* PORTD PINS */
#define PD0 (0x30)
#define PD1 (0x31)
#define PD2 (0x32)
#define PD3 (0x33)
#define PD4 (0x34)
#define PD5 (0x35)
#define PD6 (0x36)
#define PD7 (0x37)

/* Common Macros */
#define SET_BIT(reg,bit) ((reg) |= (1 << (bit)))

#define CLEAR_BIT(reg,bit) ((reg) &= ~(1 << (bit)))

#define TOGGLE_BIT(reg,bit) ((reg) ^= (1 << (bit)))

#define COPY_BITS(reg,mask,src,shift) ((reg) = ((reg) & (0xFF ^ ((mask) << (shift)))) | (((src) & (mask)) << (shift)))

#define BIT_IS_SET(reg,bit) ((reg) & (1 << (bit)))

#define BIT_IS_CLEAR(reg,bit) (!((reg) & (1 << (bit))))

#define GET_BIT(reg,bit) (((reg) & (1 << (bit))) >> (bit))

#define GET_PIN_NO(pin) ((pin) & 0x0F)

#define GET_PORT_NO(pin) ((pin) >> 4)

#define SELECT_BIT(bit) (1 << (bit))

#define SELECT_INV_BIT(bit) (~(1 << (bit)))

/* Interrupts */
#define ENABLE_GLOBAL_INTERRUPT()  __asm__ __volatile__ ("sei" ::)
#define DISABLE_GLOBAL_INTERRUPT()  __asm__ __volatile__ ("cli" ::)


#endif /* __COMMON_H__*/
