################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripherals/Src/rcc.c \
../Peripherals/Src/tim.c 

OBJS += \
./Peripherals/Src/rcc.o \
./Peripherals/Src/tim.o 

C_DEPS += \
./Peripherals/Src/rcc.d \
./Peripherals/Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Peripherals/Src/%.o Peripherals/Src/%.su Peripherals/Src/%.cyclo: ../Peripherals/Src/%.c Peripherals/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Peripherals-2f-Src

clean-Peripherals-2f-Src:
	-$(RM) ./Peripherals/Src/rcc.cyclo ./Peripherals/Src/rcc.d ./Peripherals/Src/rcc.o ./Peripherals/Src/rcc.su ./Peripherals/Src/tim.cyclo ./Peripherals/Src/tim.d ./Peripherals/Src/tim.o ./Peripherals/Src/tim.su

.PHONY: clean-Peripherals-2f-Src

