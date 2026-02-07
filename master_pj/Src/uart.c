#include "uart.h"

void UART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    // PA9: TX (AFPP), PA10: RX (Input Floating)
    GPIOA->CRH &= ~(0xFF << 4);
    GPIOA->CRH |= (0xB << 4) | (0x4 << 8);

    // 9600 Baud @ 72MHz -> BRR = 72MHz / 9600 = 7500 (0x1D4C)
    USART1->BRR = 0x1D4C;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

uint8_t UART1_Receive(void) {
    while (!(USART1->SR & USART_SR_RXNE));
    return (uint8_t)USART1->DR;
}
