#include "spi.h"

void SPI1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;

    // PA5 SCK, PA7 MOSI = AF PP 50MHz
    GPIOA->CRL &= ~((0xF<<20)|(0xF<<28));
    GPIOA->CRL |=  ((0xB<<20)|(0xB<<28));

    SPI1->CR1 =
        SPI_CR1_MSTR |      // Master
        SPI_CR1_SSM  |
        SPI_CR1_SSI  |      // Software NSS
        SPI_CR1_BR_1;       // fPCLK/8

    // CPOL=0, CPHA=0 (MODE 0) ← SSD1306 CẦN
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
    while (SPI1->SR & SPI_SR_BSY);
}
/*
 * spi.c
 *
 *  Created on: Feb 7, 2026
 *      Author: DELL
 */


