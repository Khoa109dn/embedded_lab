#ifndef UART_H_
#define UART_H_

#include "stm32f103xb.h"
#include <stdint.h>

// --- Cấu hình UART ---
#define UART1_BAUD_RATE 115200
#define APB2_CLOCK 8000000 // Tần số APB2 (Giả định 8MHz)

// --- Khai báo Hàm UART ---
void UART1_Init(void);
void UART1_Transmit_Char(uint8_t data);
void UART1_Transmit_String(char *str);
void UART1_Transmit_Number(int32_t num);

#endif /* UART_H_ */
