################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/product-genx-ctrl/src/genx320.c \
../Drivers/product-genx-ctrl/src/genx320_issd_cpi.c 

C_DEPS += \
./Drivers/product-genx-ctrl/src/genx320.d \
./Drivers/product-genx-ctrl/src/genx320_issd_cpi.d 

OBJS += \
./Drivers/product-genx-ctrl/src/genx320.o \
./Drivers/product-genx-ctrl/src/genx320_issd_cpi.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/product-genx-ctrl/src/%.o Drivers/product-genx-ctrl/src/%.su Drivers/product-genx-ctrl/src/%.cyclo: ../Drivers/product-genx-ctrl/src/%.c Drivers/product-genx-ctrl/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BPP=16 -DSTM32F746xx -DDEBUG -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Drivers/BSP/STM32746G-Discovery -I../../Drivers/BSP/Components/ft5336 -I../../TouchGFX/generated/videos/include -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Drivers/product-genx-ctrl/src" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Middlewares/FreeRTOS" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/rtos" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/Core" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/firmware" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-product-2d-genx-2d-ctrl-2f-src

clean-Drivers-2f-product-2d-genx-2d-ctrl-2f-src:
	-$(RM) ./Drivers/product-genx-ctrl/src/genx320.cyclo ./Drivers/product-genx-ctrl/src/genx320.d ./Drivers/product-genx-ctrl/src/genx320.o ./Drivers/product-genx-ctrl/src/genx320.su ./Drivers/product-genx-ctrl/src/genx320_issd_cpi.cyclo ./Drivers/product-genx-ctrl/src/genx320_issd_cpi.d ./Drivers/product-genx-ctrl/src/genx320_issd_cpi.o ./Drivers/product-genx-ctrl/src/genx320_issd_cpi.su

.PHONY: clean-Drivers-2f-product-2d-genx-2d-ctrl-2f-src

