################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Hal/Seven-Segment/seven-segment.c 

OBJS += \
./src/Hal/Seven-Segment/seven-segment.o 

C_DEPS += \
./src/Hal/Seven-Segment/seven-segment.d 


# Each subdirectory must supply rules for building sources it contributes
src/Hal/Seven-Segment/%.o: ../src/Hal/Seven-Segment/%.c src/Hal/Seven-Segment/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


