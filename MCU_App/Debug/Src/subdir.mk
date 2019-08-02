################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/CANSPI.c \
../Src/ILI9341_GFX.c \
../Src/ILI9341_STM32_Driver.c \
../Src/MCP2515.c \
../Src/main.c \
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f3xx.c 

CPP_SRCS += \
../Src/blinker_sound.cpp \
../Src/can.cpp \
../Src/data.cpp \
../Src/user_main.cpp 

OBJS += \
./Src/CANSPI.o \
./Src/ILI9341_GFX.o \
./Src/ILI9341_STM32_Driver.o \
./Src/MCP2515.o \
./Src/blinker_sound.o \
./Src/can.o \
./Src/data.o \
./Src/main.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f3xx.o \
./Src/user_main.o 

C_DEPS += \
./Src/CANSPI.d \
./Src/ILI9341_GFX.d \
./Src/ILI9341_STM32_Driver.d \
./Src/MCP2515.d \
./Src/main.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f3xx.d 

CPP_DEPS += \
./Src/blinker_sound.d \
./Src/can.d \
./Src/data.d \
./Src/user_main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mthumb -mfloat-abi=soft -g -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mthumb -mfloat-abi=soft -g -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


