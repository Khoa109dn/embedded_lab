#include "stm32f103xb.h"
#include "can.h"
#include "servo.h"
#include "ssd1306.h"
#include "spi.h"
#include <stdint.h>

// Hàm bổ trợ hiển thị
void int_to_str(uint8_t num, char *str) {
    if (num == 0) { str[0] = '0'; str[1] = '\0'; return; }
    uint8_t i = 0, temp = num;
    while (temp > 0) { temp /= 10; i++; }
    str[i] = '\0';
    while (num > 0) { str[--i] = (num % 10) + '0'; num /= 10; }
}

void SystemClock_Config(void) {
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

int main(void) {
    SystemClock_Config();
    SPI1_Init();
    SSD1306_Init();
    Servo_Init();
    CAN_Init();

    uint8_t cmd = 0;
    int16_t current_angle = 0; //
    char buffer[20];


    SSD1306_Clear();
    SSD1306_SetCursor(0, 0);
    SSD1306_Print("System Ready!");

    while (1) {
        if (CAN_ReadAngle(&cmd)) {

            if (cmd == '1') {
                current_angle += 10; // Tiến 10 độ
            }
            else if (cmd == ' ') {
                current_angle -= 10; // Lùi 10 độ
            }
            else if (cmd == 'f') {
                current_angle = 0;   // Reset về 0
            }

            // Giới hạn vật lý cho Servo (0 đến 180)
            if (current_angle > 180) current_angle = 180;
            if (current_angle < 0)   current_angle = 0;

            // Điều khiển Servo quay đến góc mới
            Servo_SetAngle((uint8_t)current_angle);

            // Cập nhật màn hình OLED
            SSD1306_Clear();
            SSD1306_SetCursor(0, 0);
            SSD1306_Print("Action: ");
            if (cmd == '1') SSD1306_Print("FORWARD");
            else if (cmd == ' ') SSD1306_Print("BACKWARD");
            else if (cmd == 'f') SSD1306_Print("RESET");

            SSD1306_SetCursor(0, 3);
            SSD1306_Print("Current Angle:");

            SSD1306_SetCursor(0, 5);
            int_to_str((uint8_t)current_angle, buffer);
            SSD1306_Print(buffer);
            SSD1306_Print(" deg");
        }
    }
}
