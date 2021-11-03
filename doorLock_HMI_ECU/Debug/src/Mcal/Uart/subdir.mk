################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Mcal/Uart/uart.c 

OBJS += \
./src/Mcal/Uart/uart.o 

C_DEPS += \
./src/Mcal/Uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mcal/Uart/%.o: ../src/Mcal/Uart/%.c src/Mcal/Uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


