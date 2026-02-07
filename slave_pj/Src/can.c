#include "can.h"

void CAN_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    // PA11: CAN_RX (Input Floating), PA12: CAN_TX (AF Push-Pull)
    GPIOA->CRH &= ~(0xFF << 12);
    GPIOA->CRH |=  (0x4 << 12) | (0xB << 16);

    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK));
    CAN1->MCR &= ~CAN_MCR_SLEEP;

    // Baudrate 500kbps (PCLK1 = 36MHz, Prescaler = 4, TS1 = 12, TS2 = 5)
    CAN1->BTR = (3 << 0) | (11 << 16) | (4 << 20);

    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);

    // Filter: Chấp nhận mọi ID
    CAN1->FMR  |= CAN_FMR_FINIT;
    CAN1->FA1R &= ~CAN_FA1R_FACT0;
    CAN1->FS1R |= CAN_FS1R_FSC0;
    CAN1->sFilterRegister[0].FR1 = 0;
    CAN1->sFilterRegister[0].FR2 = 0;
    CAN1->FA1R |= CAN_FA1R_FACT0;
    CAN1->FMR  &= ~CAN_FMR_FINIT;
}

void CAN_SendAngle(uint8_t angle) {
    uint32_t transmit_mailbox = (CAN1->TSR & CAN_TSR_TME) >> 24;
    if (CAN1->TSR & CAN_TSR_TME) {
        int mb = (CAN1->TSR & CAN_TSR_TME0) ? 0 : (CAN1->TSR & CAN_TSR_TME1 ? 1 : 2);
        CAN1->sTxMailBox[mb].TIR  = (0x123 << 21); // Standard ID 0x123
        CAN1->sTxMailBox[mb].TDTR = 1;             // Data length 1 byte
        CAN1->sTxMailBox[mb].TDLR = angle;
        CAN1->sTxMailBox[mb].TIR |= CAN_TI0R_TXRQ;
    }
}

uint8_t CAN_ReadAngle(uint8_t *angle) {
    if (CAN1->RF0R & CAN_RF0R_FMP0) {
        *angle = (uint8_t)(CAN1->sFIFOMailBox[0].RDLR & 0xFF);
        CAN1->RF0R |= CAN_RF0R_RFOM0;
        return 1;
    }
    return 0;
}
