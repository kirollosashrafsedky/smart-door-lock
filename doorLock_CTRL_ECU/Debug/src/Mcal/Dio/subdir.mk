################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Mcal/Dio/dio.c 

OBJS += \
./src/Mcal/Dio/dio.o 

C_DEPS += \
./src/Mcal/Dio/dio.d 


# Each subdirectory must supply rules for building sources it contributes
src/Mcal/Dio/%.o: ../src/Mcal/Dio/%.c src/Mcal/Dio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


