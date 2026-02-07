#include "uart.h"

// ************ Hàm Khởi tạo UART1 ************

void UART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PA9 (TX) là Alternate Function Push-Pull
    GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
    GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1);

    // PA10 (RX) là Input Floating
    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    GPIOA->CRH |= (GPIO_CRH_CNF10_0);

    // Cấu hình Tốc độ Baud Rate
    uint32_t baud_div = APB2_CLOCK / UART1_BAUD_RATE;
    USART1->BRR = baud_div;

    // Bật Transmitter (TE), Receiver (RE), và USART (UE)
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

// ************ Hàm Truyền Dữ liệu ************

void UART1_Transmit_Char(uint8_t data) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = data;
    while (!(USART1->SR & USART_SR_TC));
}

void UART1_Transmit_String(char *str) {
    while (*str) {
        UART1_Transmit_Char(*str++);
    }
}

void UART1_Transmit_Number(int32_t num) {
    char buffer[12];
    int i = 0;

    if (num == 0) {
        UART1_Transmit_Char('0');
        return;
    }

    if (num < 0) {
        UART1_Transmit_Char('-');
        num = -num;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        UART1_Transmit_Char(buffer[--i]);
    }
}
