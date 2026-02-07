#include "led_timer.h"

int main(void) {
    // Lưu ý: Đảm bảo SystemInit() đã cấu hình Clock (HSE -> PLL -> 72MHz)

    LED_Timer_Init();

    while (1) {
        // CPU rảnh hoàn toàn để làm việc khác
        // Hoặc ngủ để tiết kiệm điện
        __WFI();
    }
}
