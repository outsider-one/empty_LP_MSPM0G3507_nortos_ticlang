#include "mpu6050.h"

/* ============================================================
 *  软件 I2C — PA1=SCL, PA0=SDA
 *
 *  原理: SDA 输出 1 = 切输入(Hi-Z), 靠外部上拉 → 高
 *        SDA 输出 0 = 切输出, 推挽拉低
 *
 *  注意: MPU6050 模块必须有外部上拉电阻 (通常模块自带 4.7kΩ)
 *        不要用内部上拉 — 40kΩ 太弱, 上升沿太慢
 * ============================================================ */

/* ── GPIOA 寄存器 ── */
#define SDA_BIT      (1U << 0)       /* PA0 = bit 0 */

/* SDA 输出 0: DOUT 写 0 → 开输出 → 强下拉 */
#define SDA_0()      do {                             \
        GPIOA->DOUTCLR31_0 = SDA_BIT;                 \
        GPIOA->DOESET31_0  = SDA_BIT;                 \
    } while(0)

/* SDA 输出 1: 关输出 → Hi-Z → 外部上拉 = 高 */
#define SDA_1()      (GPIOA->DOECLR31_0 = SDA_BIT)

/* SDA 读 */
#define SDA_RD()     DL_GPIO_readPins(I2C_SDA_PORT, I2C_SDA_PIN)

/* SCL */
#define SCL_H()      DL_GPIO_setPins   (I2C_SCL_PORT, I2C_SCL_PIN)
#define SCL_L()      DL_GPIO_clearPins (I2C_SCL_PORT, I2C_SCL_PIN)

/* ── 延时 (50 kHz) ── */
static void i2c_delay(void) { delay_us(10); }

/* ============================================================
 *  I2C 原语
 * ============================================================ */

static void I2C_Init(void)
{
    /* SCL PA1: 推挽输出, 初始高 */
    DL_GPIO_initDigitalOutput(I2C_SCL_IOMUX);
    SCL_H();

    /* SDA PA0: 输入, 不上拉 (靠模块外部 4.7kΩ) */
    DL_GPIO_initDigitalInputFeatures(I2C_SDA_IOMUX,
        DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_NONE,          /* 不用内部上拉 */
        DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);

    delay_us(100);                      /* 等总线稳定 */
}

/* START: SCL 高 + SDA 拉低 → SCL 拉低 */
static void I2C_Start(void)
{
    SDA_1();  i2c_delay();             /* SDA = 1 (释放) */
    SCL_H();  i2c_delay();             /* SCL = 1         */
    SDA_0();  i2c_delay();             /* SDA ↓ (START!)   */
    SCL_L();  i2c_delay();             /* SCL = 0         */
}

/* STOP: SCL 高 + SDA 上升沿 */
static void I2C_Stop(void)
{
    SDA_0();  i2c_delay();             /* SDA = 0         */
    SCL_H();  i2c_delay();             /* SCL = 1         */
    SDA_1();  i2c_delay();             /* SDA ↑ (STOP!)   */
}

/* 写 1 字节 → 返回 0=ACK, 非0=NACK */
static uint8_t I2C_WriteByte(uint8_t dat)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)  SDA_1();      /* bit=1: 释放     */
        else             SDA_0();      /* bit=0: 拉低     */
        i2c_delay();
        SCL_H();                        /* ↑ 从机采样      */
        i2c_delay();
        SCL_L();
        dat <<= 1;
        i2c_delay();
    }

    /* ACK */
    SDA_1();
    i2c_delay();
    SCL_H();
    i2c_delay();
    {
        uint8_t ack = (SDA_RD() != 0);
        SCL_L();
        i2c_delay();
        return ack;                     /* 0=OK, 1=FAIL    */
    }
}

/* 读 1 字节, ack=0 回 ACK; ack=1 回 NACK */
static uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t i, dat = 0;

    SDA_1();                            /* 释放 → 从机驱动 */

    for (i = 0; i < 8; i++)
    {
        i2c_delay();
        SCL_H();                        /* ↑ 从机输出 */
        i2c_delay();
        dat <<= 1;
        if (SDA_RD()) dat |= 0x01;
        SCL_L();
        i2c_delay();
    }

    /* ACK/NACK */
    if (ack) SDA_1();
    else     SDA_0();
    i2c_delay();
    SCL_H();
    i2c_delay();
    SCL_L();
    i2c_delay();
    SDA_1();

    return dat;
}

/* ============================================================
 *  MPU6050 读写
 * ============================================================ */

static uint8_t MPU_WriteReg(uint8_t reg, uint8_t dat)
{
    uint8_t err = 0;

    I2C_Start();
    err |= I2C_WriteByte(MPU6050_ADDR << 1);
    err |= I2C_WriteByte(reg);
    err |= I2C_WriteByte(dat);
    I2C_Stop();

    return err;                         /* 0=全部ACK, 非0=有NACK */
}

static uint8_t MPU_ReadReg(uint8_t reg)
{
    uint8_t dat;

    I2C_Start();
    I2C_WriteByte(MPU6050_ADDR << 1);           /* 0xD0 */
    I2C_WriteByte(reg);
    I2C_Start();
    I2C_WriteByte((MPU6050_ADDR << 1) | 1);     /* 0xD1 */
    dat = I2C_ReadByte(1);
    I2C_Stop();

    return dat;
}

/* ============================================================
 *  公开接口
 * ============================================================ */

void MPU6050_Init(void)
{
    I2C_Init();

    /* 先尝试唤醒 — 写入 PWR_MGMT_1 */
    MPU_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    /* 其他配置... */
    MPU_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    MPU_WriteReg(MPU6050_CONFIG,      0x06);
    MPU_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    MPU_WriteReg(MPU6050_ACCEL_CONFIG,0x18);
}

uint8_t MPU6050_GetID(void)
{
    return MPU_ReadReg(MPU6050_WHO_AM_I);
}

uint8_t MPU6050_ReadRawReg(uint8_t reg)
{
    return MPU_ReadReg(reg);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    int16_t raw;

    /* ── 加速度 cm/s²: raw / 2048 * 980 = raw * 245 / 512 ── */
    raw = (int16_t)((MPU_ReadReg(MPU6050_ACCEL_XOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_ACCEL_XOUT_L));
    *AccX = (int16_t)((int32_t)raw * 245 / 512);

    raw = (int16_t)((MPU_ReadReg(MPU6050_ACCEL_YOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_ACCEL_YOUT_L));
    *AccY = (int16_t)((int32_t)raw * 245 / 512);

    raw = (int16_t)((MPU_ReadReg(MPU6050_ACCEL_ZOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_ACCEL_ZOUT_L));
    *AccZ = (int16_t)((int32_t)raw * 245 / 512);

    /* ── 角速度 milli-rad/s: raw / 16.4 * π/180 * 1000 = raw * 213 / 200 ── */
    raw = (int16_t)((MPU_ReadReg(MPU6050_GYRO_XOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_GYRO_XOUT_L));
    *GyroX = (int16_t)((int32_t)raw * 213 / 200);

    raw = (int16_t)((MPU_ReadReg(MPU6050_GYRO_YOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_GYRO_YOUT_L));
    *GyroY = (int16_t)((int32_t)raw * 213 / 200);

    raw = (int16_t)((MPU_ReadReg(MPU6050_GYRO_ZOUT_H) << 8) |
                     MPU_ReadReg(MPU6050_GYRO_ZOUT_L));
    *GyroZ = (int16_t)((int32_t)raw * 213 / 200);
}
