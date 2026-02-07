#include "servo.h"
#include "stm32f103xb.h"

void Servo_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_TIM1EN;

    GPIOA->CRH &= ~(0xF << 0);
    GPIOA->CRH |=  (0xB << 0);   // PA8 AF PP

    TIM1->PSC = 71;
    TIM1->ARR = 19999;

    TIM1->CCMR1 = (6 << 4);
    TIM1->CCER  |= TIM_CCER_CC1E;
    TIM1->BDTR  |= TIM_BDTR_MOE;

    TIM1->CCR1 = 1500;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void Servo_SetAngle(uint8_t angle)
{
    if (angle > 180) angle = 180;
    TIM1->CCR1 = 500 + (angle * 2000) / 180;
}
/*
 * servo.c
 *
 *  Created on: Feb 7, 2026
 *      Author: DELL
 */


