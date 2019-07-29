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
../Src/user_main.cpp 

OBJS += \
./Src/CANSPI.o \
./Src/ILI9341_GFX.o \
./Src/ILI9341_STM32_Driver.o \
./Src/MCP2515.o \
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
./Src/user_main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xC -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Inc" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/STM32F3xx_HAL_Driver/Inc" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xC -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Inc" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/STM32F3xx_HAL_Driver/Inc" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"D:/Programs/STM32/Workspace/STM32F3Discovery_Cluster/MCU_App/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


