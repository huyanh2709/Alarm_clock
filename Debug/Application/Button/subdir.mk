################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Button/button.c 

OBJS += \
./Application/Button/button.o 

C_DEPS += \
./Application/Button/button.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Button/%.o Application/Button/%.su Application/Button/%.cyclo: ../Application/Button/%.c Application/Button/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32 cube/stm32f1_register/alarmClock/Application/DS3231" -I"D:/STM32 cube/stm32f1_register/alarmClock/Application/LCD" -I"D:/STM32 cube/stm32f1_register/alarmClock/Application/Button" -I"D:/STM32 cube/stm32f1_register/alarmClock/Application/AlarmClock" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-Button

clean-Application-2f-Button:
	-$(RM) ./Application/Button/button.cyclo ./Application/Button/button.d ./Application/Button/button.o ./Application/Button/button.su

.PHONY: clean-Application-2f-Button

