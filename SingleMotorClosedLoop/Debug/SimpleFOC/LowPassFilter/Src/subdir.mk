################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SimpleFOC/LowPassFilter/Src/lpf.c 

OBJS += \
./SimpleFOC/LowPassFilter/Src/lpf.o 

C_DEPS += \
./SimpleFOC/LowPassFilter/Src/lpf.d 


# Each subdirectory must supply rules for building sources it contributes
SimpleFOC/LowPassFilter/Src/%.o SimpleFOC/LowPassFilter/Src/%.su SimpleFOC/LowPassFilter/Src/%.cyclo: ../SimpleFOC/LowPassFilter/Src/%.c SimpleFOC/LowPassFilter/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Sensor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Utils" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/PIDController/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/LowPassFilter/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Devices/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SimpleFOC-2f-LowPassFilter-2f-Src

clean-SimpleFOC-2f-LowPassFilter-2f-Src:
	-$(RM) ./SimpleFOC/LowPassFilter/Src/lpf.cyclo ./SimpleFOC/LowPassFilter/Src/lpf.d ./SimpleFOC/LowPassFilter/Src/lpf.o ./SimpleFOC/LowPassFilter/Src/lpf.su

.PHONY: clean-SimpleFOC-2f-LowPassFilter-2f-Src

