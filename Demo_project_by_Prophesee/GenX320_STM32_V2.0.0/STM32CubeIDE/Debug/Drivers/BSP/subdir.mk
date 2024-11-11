################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_qspi.c 

C_DEPS += \
./Drivers/BSP/stm32746g_discovery_qspi.d 

OBJS += \
./Drivers/BSP/stm32746g_discovery_qspi.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/stm32746g_discovery_qspi.o: C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_qspi.c Drivers/BSP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BPP=16 -DSTM32F746xx -DDEBUG -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Drivers/BSP/STM32746G-Discovery -I../../Drivers/BSP/Components/ft5336 -I../../TouchGFX/generated/videos/include -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Drivers/product-genx-ctrl/src" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Middlewares/FreeRTOS" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/rtos" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/Core" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/firmware" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP

clean-Drivers-2f-BSP:
	-$(RM) ./Drivers/BSP/stm32746g_discovery_qspi.cyclo ./Drivers/BSP/stm32746g_discovery_qspi.d ./Drivers/BSP/stm32746g_discovery_qspi.o ./Drivers/BSP/stm32746g_discovery_qspi.su

.PHONY: clean-Drivers-2f-BSP

