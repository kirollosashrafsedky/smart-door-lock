################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Mcal/Twi/twi.c 

OBJS += \
./src/Mcal/Twi/twi.o 

C_DEPS += \
./src/Mcal/Twi/twi.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mcal/Twi/%.o: ../src/Mcal/Twi/%.c src/Mcal/Twi/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


