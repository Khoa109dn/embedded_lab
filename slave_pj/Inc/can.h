#ifndef CAN_H
#define CAN_H
#include "stm32f103xb.h"

void CAN_Init(void);
void CAN_SendAngle(uint8_t angle);
uint8_t CAN_ReadAngle(uint8_t *angle);

#endif
