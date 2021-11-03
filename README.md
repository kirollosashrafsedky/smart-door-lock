# Door Lock System

A door lock system implemented using 2 **ATMEGA 16** MCUs, using layered structured architecture.

## List of used Drivers

> MCAL (Microcontroller Abstraction Layer)

	- DIO
	- TIMER
	- PWM
	- TWI
	- UART

> HAL (Hardware Abstraction Layer)

	- KEYPAD
	- LCD
	- BUZZER
	- DC-MOTOR
	- EXTERNAL_EEPROM (MC24C16)

## Features

**The Door Lock supports**

 1.  entering 5-char password to unlock the door, and can be changed to any number of chars.
2.  delete last entered character from the password '-'
3.  clear the screen during entering the password 'c'
4.  submiting the password '='
5.  password is required to unlock door or change password
6.  for a first-time user, a new password must be entered and confirmed, then it's
   save in the eeprom so the next time, it shows the menu directly
7.  password is saved in an external eeprom 'M24C16'
8.  password can be entered wrong for only 3-times successively, then a buzzer will
  beep for 1 minute.
9.  A new password can mismatch its confirmation for 5 times and after that the system
 returns to home screen
10. A list of helping messages that appear on the lcd
11. Can be easily modified to implement one of the sleep modes to reduce power consumption

## Author

> **Kirollos Ashraf Sedky**

## Schematic
![Proteus Schematic](https://drive.google.com/uc?export=view&id=1nIcbLenPjF3u5lop_lQFtLxfunQh3-fe)

## Repo Link
> [Git Hub Repo Link](https://github.com/kirollosashrafsedky/smart-door-lock)
