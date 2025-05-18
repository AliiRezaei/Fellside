################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SimpleFOC/Motor/Src/BLDCMotor.c 

OBJS += \
./SimpleFOC/Motor/Src/BLDCMotor.o 

C_DEPS += \
./SimpleFOC/Motor/Src/BLDCMotor.d 


# Each subdirectory must supply rules for building sources it contributes
SimpleFOC/Motor/Src/%.o SimpleFOC/Motor/Src/%.su SimpleFOC/Motor/Src/%.cyclo: ../SimpleFOC/Motor/Src/%.c SimpleFOC/Motor/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorOpenLoop/SimpleFOC/Utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SimpleFOC-2f-Motor-2f-Src

clean-SimpleFOC-2f-Motor-2f-Src:
	-$(RM) ./SimpleFOC/Motor/Src/BLDCMotor.cyclo ./SimpleFOC/Motor/Src/BLDCMotor.d ./SimpleFOC/Motor/Src/BLDCMotor.o ./SimpleFOC/Motor/Src/BLDCMotor.su

.PHONY: clean-SimpleFOC-2f-Motor-2f-Src

