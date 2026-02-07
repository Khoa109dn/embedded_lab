#include "ssd1306.h"
#include "spi.h"
#include "fonts.h"

#define CS_LOW()    GPIOA->BRR  = (1<<4)
#define CS_HIGH()   GPIOA->BSRR = (1<<4)
#define DC_CMD()    GPIOB->BRR  = (1<<1)
#define DC_DATA()   GPIOB->BSRR = (1<<1)
#define RES_LOW()   GPIOB->BRR  = (1<<0)
#define RES_HIGH()  GPIOB->BSRR = (1<<0)

static void delay(volatile uint32_t t)
{
    while(t--);
}

static void OLED_Cmd(uint8_t cmd)
{
    DC_CMD();
    CS_LOW();
    SPI1_Write(cmd);
    CS_HIGH();
}

static void OLED_Data(uint8_t data)
{
    DC_DATA();
    CS_LOW();
    SPI1_Write(data);
    CS_HIGH();
}

void SSD1306_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // PA4 CS output
    GPIOA->CRL &= ~(0xF << 16);
    GPIOA->CRL |=  (0x3 << 16);

    // PB0 RES, PB1 DC output
    GPIOB->CRL &= ~((0xF<<0)|(0xF<<4));
    GPIOB->CRL |=  ((0x3<<0)|(0x3<<4));

    CS_HIGH();

    // RESET OLED (BẮT BUỘC)
    RES_LOW();
    delay(100000);
    RES_HIGH();
    delay(100000);

    OLED_Cmd(0xAE); // display off
    OLED_Cmd(0x20); OLED_Cmd(0x00); // horizontal addressing
    OLED_Cmd(0xB0);
    OLED_Cmd(0xC8);
    OLED_Cmd(0x00);
    OLED_Cmd(0x10);
    OLED_Cmd(0x40);
    OLED_Cmd(0x81); OLED_Cmd(0x7F);
    OLED_Cmd(0xA1);
    OLED_Cmd(0xA6);
    OLED_Cmd(0xA8); OLED_Cmd(0x3F);
    OLED_Cmd(0xA4);
    OLED_Cmd(0xD3); OLED_Cmd(0x00);
    OLED_Cmd(0xD5); OLED_Cmd(0x80);
    OLED_Cmd(0xD9); OLED_Cmd(0xF1);
    OLED_Cmd(0xDA); OLED_Cmd(0x12);
    OLED_Cmd(0xDB); OLED_Cmd(0x40);
    OLED_Cmd(0x8D); OLED_Cmd(0x14);
    OLED_Cmd(0xAF); // display ON

    SSD1306_Clear();
}

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{
    OLED_Cmd(0xB0 + y);
    OLED_Cmd(0x00 + (x & 0x0F));
    OLED_Cmd(0x10 + (x >> 4));
}

void SSD1306_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++)
    {
        SSD1306_SetCursor(0, page);
        for (uint8_t col = 0; col < 128; col++)
        {
            OLED_Data(0x00);
        }
    }
}

void SSD1306_Print(char *str)
{
    while (*str)
    {
        for (int i = 0; i < 5; i++)
        {
            OLED_Data(Font5x7[*str - 32][i]);
        }
        OLED_Data(0x00);
        str++;
    }
}
/*
 * ssd1306.c
 *
 *  Created on: Feb 7, 2026
 *      Author: DELL
 */


