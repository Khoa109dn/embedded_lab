#ifndef SPI_H
#define SPI_H

#include "stm32f103xb.h"
#include <stdint.h>

void SPI1_Init(void);
void SPI1_Write(uint8_t data);

#endif
