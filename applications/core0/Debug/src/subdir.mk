################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/diode_core0_functions.c \
../src/diode_uartlite.c \
../src/main_core0.c \
../src/mutex.c \
../src/others.c \
../src/platform.c 

OBJS += \
./src/diode_core0_functions.o \
./src/diode_uartlite.o \
./src/main_core0.o \
./src/mutex.o \
./src/others.o \
./src/platform.o 

C_DEPS += \
./src/diode_core0_functions.d \
./src/diode_uartlite.d \
./src/main_core0.d \
./src/mutex.d \
./src/others.d \
./src/platform.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../core0_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


