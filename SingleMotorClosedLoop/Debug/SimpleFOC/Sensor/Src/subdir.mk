################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SimpleFOC/Sensor/Src/sensor.c 

OBJS += \
./SimpleFOC/Sensor/Src/sensor.o 

C_DEPS += \
./SimpleFOC/Sensor/Src/sensor.d 


# Each subdirectory must supply rules for building sources it contributes
SimpleFOC/Sensor/Src/%.o SimpleFOC/Sensor/Src/%.su SimpleFOC/Sensor/Src/%.cyclo: ../SimpleFOC/Sensor/Src/%.c SimpleFOC/Sensor/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Sensor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Utils" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/PIDController/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/LowPassFilter/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Devices/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SimpleFOC-2f-Sensor-2f-Src

clean-SimpleFOC-2f-Sensor-2f-Src:
	-$(RM) ./SimpleFOC/Sensor/Src/sensor.cyclo ./SimpleFOC/Sensor/Src/sensor.d ./SimpleFOC/Sensor/Src/sensor.o ./SimpleFOC/Sensor/Src/sensor.su

.PHONY: clean-SimpleFOC-2f-Sensor-2f-Src

