################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -c -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Core/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/CMSIS/Include" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Peripherals/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Driver/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Motor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Sensor/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/Utils" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/PIDController/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/SimpleFOC/LowPassFilter/Inc" -I"C:/Users/Asus/armcortex/Fellside/SingleMotorClosedLoop/Devices/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

