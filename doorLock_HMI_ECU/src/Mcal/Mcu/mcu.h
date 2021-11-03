#ifndef __MCU_H__
#define __MCU_H__

#if defined (__AVR_ATmega16__)
#include "Mcus/atmega16.h"
#elif defined (__AVR_ATmega32__)
#include "Mcus/atmega32.h"
#endif

#include "avr-common.h"

#endif /* __MCU_H__ */
