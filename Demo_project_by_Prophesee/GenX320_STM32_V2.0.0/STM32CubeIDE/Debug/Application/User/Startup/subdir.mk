################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/User/Startup/startup_stm32f746nghx.s 

S_DEPS += \
./Application/User/Startup/startup_stm32f746nghx.d 

OBJS += \
./Application/User/Startup/startup_stm32f746nghx.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Startup/%.o: ../Application/User/Startup/%.s Application/User/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User" -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Drivers/product-genx-ctrl/src" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/gui" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Middlewares/FreeRTOS" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/rtos" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/Core" -I"C:/FINALPROJECT/genx320/GENX320_demo_project_by_Prophesee/GenX320_STM32_V2.0.0/STM32CubeIDE/Application/User/firmware" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Application-2f-User-2f-Startup

clean-Application-2f-User-2f-Startup:
	-$(RM) ./Application/User/Startup/startup_stm32f746nghx.d ./Application/User/Startup/startup_stm32f746nghx.o

.PHONY: clean-Application-2f-User-2f-Startup

