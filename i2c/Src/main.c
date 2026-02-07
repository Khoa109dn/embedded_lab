#include "adxl345.h"
#include "uart.h"
#include <stdio.h>

int main(void)
{
    // Cần đảm bảo System Clock (SystemInit) đã được cấu hình

    I2C1_Init();
    UART1_Init();
    ADXL345_Init_RegLevel();

    int16_t acc_raw_x, acc_raw_y, acc_raw_z;

    UART1_Transmit_String("STM32F103C8T6 - ADXL345 Running!\r\n");

    while (1)
    {
        ADXL345_Read_Accel_Raw_RegLevel(&acc_raw_x, &acc_raw_y, &acc_raw_z);

        UART1_Transmit_String("X: ");
        UART1_Transmit_Number(acc_raw_x);
        UART1_Transmit_String(" | Y: ");
        UART1_Transmit_Number(acc_raw_y);
        UART1_Transmit_String(" | Z: ");
        UART1_Transmit_Number(acc_raw_z);
        UART1_Transmit_String("\r\n");

        delay_us(200000); // 200ms delay
    }
}
