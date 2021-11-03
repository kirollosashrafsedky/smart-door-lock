################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Mcal/Timer/timer.c 

OBJS += \
./src/Mcal/Timer/timer.o 

C_DEPS += \
./src/Mcal/Timer/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mcal/Timer/%.o: ../src/Mcal/Timer/%.c src/Mcal/Timer/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


