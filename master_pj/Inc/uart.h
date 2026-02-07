#ifndef UART_H
#define UART_H
#include "stm32f103xb.h"
#include <stdint.h>

void UART1_Init(void);
void UART1_SendChar(char c);
void UART1_SendString(char *str);
uint8_t UART1_Receive(void);

#endif
