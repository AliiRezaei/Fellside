################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SimpleFOC/PIDController/Src/pid.c 

OBJS += \
./SimpleFOC/PIDController/Src/pid.o 

C_DEPS += \
./SimpleFOC/PIDController/Src/pid.d 


# Each subdirectory must supply rules for building sources it contributes
SimpleFOC/PIDController/Src/%.o SimpleFOC/PIDController/Src/%.su SimpleFOC/PIDController/Src/%.cyclo: ../SimpleFOC/PIDController/Src/%.c SimpleFOC/PIDController/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Sensor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Utils" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/PIDController/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/LowPassFilter/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Devices/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SimpleFOC-2f-PIDController-2f-Src

clean-SimpleFOC-2f-PIDController-2f-Src:
	-$(RM) ./SimpleFOC/PIDController/Src/pid.cyclo ./SimpleFOC/PIDController/Src/pid.d ./SimpleFOC/PIDController/Src/pid.o ./SimpleFOC/PIDController/Src/pid.su

.PHONY: clean-SimpleFOC-2f-PIDController-2f-Src

