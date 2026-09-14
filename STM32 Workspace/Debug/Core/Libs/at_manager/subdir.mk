################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Libs/at_manager/at_manager.c 

OBJS += \
./Core/Libs/at_manager/at_manager.o 

C_DEPS += \
./Core/Libs/at_manager/at_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Libs/at_manager/%.o Core/Libs/at_manager/%.su Core/Libs/at_manager/%.cyclo: ../Core/Libs/at_manager/%.c Core/Libs/at_manager/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/preel/Desktop/projets/Ampli/Ampli-D/STM32 Workspace/Core/Libs" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Libs-2f-at_manager

clean-Core-2f-Libs-2f-at_manager:
	-$(RM) ./Core/Libs/at_manager/at_manager.cyclo ./Core/Libs/at_manager/at_manager.d ./Core/Libs/at_manager/at_manager.o ./Core/Libs/at_manager/at_manager.su

.PHONY: clean-Core-2f-Libs-2f-at_manager

