#ifndef ADXL345_H_
#define ADXL345_H_

#include "stm32f103xb.h"
#include <stdint.h>

// --- Định nghĩa cho ADXL345 ---
#define ADXL345_I2C_ADDR_W  0xA6  // Địa chỉ 8-bit WRITE (0x53 << 1, ADO=GND)
#define ADXL345_POWER_CTL   0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0      0x32

// --- Cấu hình I2C Cấp thấp ---
#define I2C_CLOCK_FREQ      8000000 // APB1 Clock (Giả định 8MHz)
#define ADXL_TIMEOUT        100000

// --- Khai báo Hàm ---
void I2C1_Init(void);
void delay_us(uint32_t us);
void ADXL345_Init_RegLevel(void);
void ADXL345_Read_Accel_Raw_RegLevel(int16_t *x, int16_t *y, int16_t *z);

#endif /* ADXL345_H_ */
