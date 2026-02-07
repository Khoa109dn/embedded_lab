#ifndef __ADC_SENSOR_H
#define __ADC_SENSOR_H

#include "stm32f103xb.h"

void ADC_Init_Bare(void);
uint16_t ADC_Read(uint8_t channel);
float LM35_GetTemp(uint16_t adc_val);

#endif
