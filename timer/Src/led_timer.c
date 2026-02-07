#include "led_timer.h"

void LED_Timer_Init(void) {
    /* 1. Kích hoạt Clock cho ngoại vi */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   // Bật Clock Port C
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Bật Clock Timer 2

    /* 2. Cấu hình GPIO PC13: Output Push-Pull, Max Speed 2MHz */
    // Xóa 4 bit cấu hình của chân 13 (vị trí bit 20-23 trong thanh ghi CRH)
    LED_PORT->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    // Thiết lập MODE13 = 10 (Output 2MHz), CNF13 = 00 (Push-Pull)
    LED_PORT->CRH |= GPIO_CRH_MODE13_1;

    /* 3. Cấu hình Timer 2 (Giả sử xung nhịp hệ thống 72MHz) */
    TIM2->PSC = 7199;   // Prescaler: 72MHz / 7200 = 10kHz
    TIM2->ARR = 9999;   // Auto-reload: Đếm 10,000 xung = 1 giây

    /* 4. Cấu hình Ngắt */
    TIM2->DIER |= TIM_DIER_UIE;   // Enable Update Interrupt
    NVIC_EnableIRQ(TIM2_IRQn);    // Kích hoạt ngắt TIM2 trong NVIC
    NVIC_SetPriority(TIM2_IRQn, 1); // Đặt mức ưu tiên ngắt

    /* 5. Cho phép Timer chạy */
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief Trình xử lý ngắt Timer 2
 */
void TIM2_IRQHandler(void) {
    // Kiểm tra cờ ngắt Update
    if (TIM2->SR & TIM_SR_UIF) {
        // Đảo trạng thái PC13 bằng thanh ghi ODR (Output Data Register)
        LED_PORT->ODR ^= (1 << LED_PIN);

        // Xóa cờ ngắt sau khi xử lý
        TIM2->SR &= ~TIM_SR_UIF;
    }
}
