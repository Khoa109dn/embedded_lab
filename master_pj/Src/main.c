#include "stm32f103xb.h"
#include "can.h"
#include "uart.h"

// Hàm cấu hình Clock 72MHz
void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

int main(void) {
    SystemClock_Config();
    UART1_Init();
    CAN_Init();

    while (1) {
        // Kiểm tra xem có phím nào được nhấn từ máy tính không
        if (USART1->SR & USART_SR_RXNE) {
            uint8_t key = (uint8_t)USART1->DR;

            // Chỉ gửi đi nếu là phím hợp lệ để tránh nhiễu mạng CAN
            if (key == '1' || key == ' ' || key == 'f') {
                CAN_SendAngle(key);
            }
        }
    }
}
