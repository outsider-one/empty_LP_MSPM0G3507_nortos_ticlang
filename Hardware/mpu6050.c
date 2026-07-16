#include "mpu6050.h"

/* ============================================================
 *  纯软件 I2C — PA1=SCL, PA0=SDA
 *
 *  关键设计:
 *    1. 寄存器直接写 DOE 翻方向 (单周期), 不用 DL API
 *    2. delay_us(5) 留足裕量
 *    3. 内部上拉 PULL_UP
 * ============================================================ */


#define GPIOA_DOE    (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

#define SDA_BIT      (1U << 0)

#define SDA_OUT()    (GPIOA_DOE |=  SDA_BIT)
#define SDA_IN()     (GPIOA_DOE &= ~SDA_BIT)

#define SCL_H()      DL_GPIO_setPins   (I2C_SCL_PORT, I2C_SCL_PIN)
#define SCL_L()      DL_GPIO_clearPins (I2C_SCL_PORT, I2C_SCL_PIN)
#define SDA_L()      DL_GPIO_clearPins (I2C_SDA_PORT, I2C_SDA_PIN)
#define SDA_READ()   DL_GPIO_readPins  (I2C_SDA_PORT, I2C_SDA_PIN)

static void i2c_delay(void) { delay_us(5); }

/* ────────────────────────────────────────────────────────── */

void I2C_Init(void)
{
    DL_GPIO_initDigitalOutput(I2C_SCL_IOMUX);
    SCL_H();

    DL_GPIO_initDigitalInputFeatures(I2C_SDA_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    i2c_delay();
}

void I2C_Start(void)
{
    SDA_OUT();
    SDA_L();
    i2c_delay();
    SCL_L();
    i2c_delay();
}

void I2C_Stop(void)
{
    SDA_OUT();
    SDA_L();
    i2c_delay();
    SCL_H();
    i2c_delay();
    SDA_IN();
    i2c_delay();
}

uint8_t I2C_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80) SDA_IN();
        else           { SDA_OUT(); SDA_L(); }
        i2c_delay();
        SCL_H();
        i2c_delay();
        SCL_L();
        dat <<= 1;
        i2c_delay();
    }

    SDA_IN();
    i2c_delay();
    SCL_H();
    i2c_delay();
    {
        uint8_t ack = (SDA_READ() != 0);
        SCL_L();
        i2c_delay();
        return ack;
    }
}

uint8_t I2C_RecvByte(uint8_t ack)
{
    uint8_t i, dat = 0;
    SDA_IN();

    for (i = 0; i < 8; i++)
    {
        i2c_delay();
        SCL_H();
        i2c_delay();
        dat <<= 1;
        if (SDA_READ()) dat |= 0x01;
        SCL_L();
        i2c_delay();
    }

    if (ack) SDA_IN();
    else    { SDA_OUT(); SDA_L(); }
    i2c_delay();
    SCL_H();
    i2c_delay();
    SCL_L();
    i2c_delay();
    SDA_IN();

    return dat;
}
