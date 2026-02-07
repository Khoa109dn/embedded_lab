################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/can.c \
../Src/fonts.c \
../Src/main.c \
../Src/servo.c \
../Src/spi.c \
../Src/ssd1306.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/can.o \
./Src/fonts.o \
./Src/main.o \
./Src/servo.o \
./Src/spi.o \
./Src/ssd1306.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/can.d \
./Src/fonts.d \
./Src/main.d \
./Src/servo.d \
./Src/spi.d \
./Src/ssd1306.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"F:/STM32Cube_FW_F1_V1.8.0/Drivers/CMSIS/Include" -I"F:/STM32Cube_FW_F1_V1.8.0/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/can.cyclo ./Src/can.d ./Src/can.o ./Src/can.su ./Src/fonts.cyclo ./Src/fonts.d ./Src/fonts.o ./Src/fonts.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/ssd1306.cyclo ./Src/ssd1306.d ./Src/ssd1306.o ./Src/ssd1306.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

