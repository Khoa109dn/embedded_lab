#include "adxl345.h"
#include "uart.h"

// ************ I2C Cấp thấp Helper Functions ************

void delay_us(uint32_t us) {
    for (uint32_t i = 0; i < (us * 8); i++);
}

static uint8_t i2c_wait_for_flag(uint32_t flag) {
    uint32_t timeout = ADXL_TIMEOUT;
    while (!(I2C1->SR1 & flag) && --timeout);
    return (timeout > 0);
}

static void i2c_clear_addr(void) {
    volatile uint32_t temp;
    temp = I2C1->SR1;
    temp = I2C1->SR2;
    (void)temp;
}

static void I2C1_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    I2C1->CR1 |= I2C_CR1_START;
    if (!i2c_wait_for_flag(I2C_SR1_SB)) return;
    I2C1->DR = dev_addr;
    if (!i2c_wait_for_flag(I2C_SR1_ADDR)) return;
    i2c_clear_addr();
    I2C1->DR = reg_addr;
    if (!i2c_wait_for_flag(I2C_SR1_TXE)) return;
    I2C1->DR = data;
    if (!i2c_wait_for_flag(I2C_SR1_BTF)) return;
    I2C1->CR1 |= I2C_CR1_STOP;
}

static void I2C1_ReadRegs(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data_buf, uint8_t len) {
    if (len == 0) return;

    // 1. WRITE: Gửi địa chỉ thanh ghi (Kèm bit 0x80 nếu đọc > 1 byte)
    I2C1->CR1 |= I2C_CR1_START;
    if (!i2c_wait_for_flag(I2C_SR1_SB)) return;
    I2C1->DR = dev_addr;
    if (!i2c_wait_for_flag(I2C_SR1_ADDR)) return;
    i2c_clear_addr();

    if (len > 1) {
        I2C1->DR = reg_addr | 0x80;
    } else {
        I2C1->DR = reg_addr;
    }
    if (!i2c_wait_for_flag(I2C_SR1_TXE)) return;

    // 2. RESTART: Chuyển sang chế độ READ
    I2C1->CR1 |= I2C_CR1_START;
    if (!i2c_wait_for_flag(I2C_SR1_SB)) return;
    I2C1->DR = dev_addr | 0x01; // Địa chỉ READ
    if (!i2c_wait_for_flag(I2C_SR1_ADDR)) return;

    // 3. Đọc Dữ liệu
    if (len == 6) {
        I2C1->CR1 |= I2C_CR1_ACK; // Bật ACK
        i2c_clear_addr(); // Xóa ADDR

        for (uint8_t i = 0; i < 4; i++) {
            if (i2c_wait_for_flag(I2C_SR1_RXNE))
                data_buf[i] = I2C1->DR;
        }

        I2C1->CR1 &= ~I2C_CR1_ACK; // Tắt ACK cho byte cuối
        if (i2c_wait_for_flag(I2C_SR1_BTF)) {
            I2C1->CR1 |= I2C_CR1_STOP; // STOP
            data_buf[4] = I2C1->DR; // Đọc byte 5
            data_buf[5] = I2C1->DR; // Đọc byte 6
        }
    } else {
        // Trường hợp đọc 1 byte (Dùng cho ID check)
        I2C1->CR1 &= ~I2C_CR1_ACK; // NACK
        i2c_clear_addr();
        I2C1->CR1 |= I2C_CR1_STOP;
        if (i2c_wait_for_flag(I2C_SR1_RXNE))
            *data_buf = I2C1->DR;
    }
}


// ************ Hàm Cấu hình I2C và ADXL345 ************

void I2C1_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL &= ~((GPIO_CRL_MODE6) | (GPIO_CRL_MODE7));
    GPIOB->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    I2C1->CR2 = I2C_CLOCK_FREQ / 1000000;
    I2C1->CCR = 80; // Tốc độ I2C 50 KHz (Giảm tốc độ để tăng độ ổn định)
    I2C1->TRISE = 9;
    I2C1->CR1 |= I2C_CR1_PE;
}

void ADXL345_Init_RegLevel(void) {
    uint8_t dev_id;
    I2C1_ReadRegs(ADXL345_I2C_ADDR_W, 0x00, &dev_id, 1);

    if (dev_id != 0xE5) {
        UART1_Transmit_String("ERROR: Khong phai ADXL345 (ID: ");
        UART1_Transmit_Number(dev_id);
        UART1_Transmit_String("). HAY KIEM TRA PULL-UP VA ADO!\r\n");
    } else {
        UART1_Transmit_String("ADXL345 ID OK (0xE5).\r\n");
    }

    I2C1_WriteReg(ADXL345_I2C_ADDR_W, ADXL345_DATA_FORMAT, 0x08);
    I2C1->DR = ADXL345_POWER_CTL;
    I2C1->DR = 0x08; // Chỉ ghi 0x08 vào thanh ghi POWER_CTL
}

void ADXL345_Read_Accel_Raw_RegLevel(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data_rec[6];

    I2C1_ReadRegs(ADXL345_I2C_ADDR_W, ADXL345_DATAX0, data_rec, 6);

    *x = (int16_t)(data_rec[1] << 8 | data_rec[0]);
    *y = (int16_t)(data_rec[3] << 8 | data_rec[2]);
    *z = (int16_t)(data_rec[5] << 8 | data_rec[4]);
}
