#include "adc_sensor.h"

void ADC_Init_Bare(void) {
    /* 1. Bật Clock cho ADC1 và GPIOA */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;

    /* 2. Cấu hình chân PA0, PA1 là Analog Input */
    // CNF=00, MODE=00 (Analog mode)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 | GPIO_CRL_MODE1 | GPIO_CRL_CNF1);

    /* 3. Thiết lập ADC */
    ADC1->CR2 |= ADC_CR2_ADON;       // Bật bộ ADC

    // Đợi ADC ổn định (Calibration)
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    // Cấu hình Sample Time cho kênh 0 và 1 (Chu kỳ lấy mẫu)
    ADC1->SMPR2 |= (ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP1_2); // ~55.5 cycles
}

uint16_t ADC_Read(uint8_t channel) {
    // Chọn kênh cần đọc vào nhóm Regular (chỉ đọc 1 kênh tại 1 thời điểm ở hàm này)
    ADC1->SQR3 = channel;

    // Bắt đầu chuyển đổi
    ADC1->CR2 |= ADC_CR2_ADON;

    // Đợi cờ EOC (End Of Conversion)
    while (!(ADC1->SR & ADC_SR_EOC));

    return (uint16_t)ADC1->DR; // Trả về giá trị 12-bit (0-4095)
}

float LM35_GetTemp(uint16_t adc_val) {
    // ADC 12-bit, Vref = 3.3V => 1 đơn vị ADC = 3300mV / 4096
    // LM35: 10mV = 1 độ C
    float voltage = (adc_val * 3300.0f) / 4096.0f;
    return voltage / 10.0f;
}
