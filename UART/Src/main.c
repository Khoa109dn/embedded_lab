#include "stm32f103xb.h"
#include <string.h>
/* UART1 - APB2
   TX = PA9
   RX = PA10
*/

#define GPIOAEN   (1U << 2)
#define UART1EN   (1U << 14)
#define SR_TXE    (1U << 7)
#define SR_RXNE   (1U << 5)

void uart1_init(void);
void uart1_write(int ch);
char uart1_read(void);
void uart1_write_string(const char *str);
void uart1_read_string(char *buf, int maxlen);

void uart1_init(void)
{
    /* 1. Enable clock cho GPIOA và USART1 */
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= UART1EN;

    /* 2. Cấu hình PA9 TX & PA10 RX */
    GPIOA->CRH &= ~(0xFF << 4);    // clear PA9 PA10
    GPIOA->CRH |=  (0xB << 4);     // PA9 TX: AF push-pull (0xB)
    GPIOA->CRH |=  (0x4 << 8);     // PA10 RX: input floating (0x4)

    /* 3. Baud = 9600 @ 8 MHz */
    USART1->BRR = 0x341;

    /* 4. Enable UART */
    USART1->CR1 = 0;
    USART1->CR1 |= (1U << 13);   // UE
    USART1->CR1 |= (1U << 3);    // TE
    USART1->CR1 |= (1U << 2);    // RE
}

void uart1_write(int ch)
{
    while (!(USART1->SR & SR_TXE)) {}
    USART1->DR = (ch & 0xFF);
}

char uart1_read(void)
{
    while (!(USART1->SR & SR_RXNE)) {}
    return (char)(USART1->DR & 0xFF);
}

void uart1_write_string(const char *str){
    while (*str){
        uart1_write(*str++);
    }
}

void uart1_read_string(char *buf, int maxlen){
    int i = 0;
    while (i < maxlen - 1){
        char c = uart1_read();
        	uart1_write(c);


        if (c == '\r' || c == '\n'){
            buf[i] = '\0';
            	break;
        }

        buf[i++] = c;
    }
}

int main(void)
{
    uart1_init();

    uart1_write_string("Nhap MSSV: ");

    char buff[20];
    uart1_read_string(buff, 20);

    uart1_write_string("\r\n");

    if (strcmp(buff, "24146199") == 0){
        uart1_write_string("Nguyen Dang Khoa\r\n");
    }
    else{
        uart1_write_string("Sai MSSV!\r\n");
    }

    while (1) {}
}
