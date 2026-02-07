#ifndef __LED_TIMER_H
#define __LED_TIMER_H

#include "stm32f103xb.h" // Thư viện định nghĩa cụ thể cho dòng F103 Medium Density

// Định nghĩa chân LED (PC13)
#define LED_PORT      GPIOC
#define LED_PIN       13

// Hàm khởi tạo
void LED_Timer_Init(void);

#endif
