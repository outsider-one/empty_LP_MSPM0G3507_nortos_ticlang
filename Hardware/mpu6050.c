#include "mpu6050.h"
#include "board.h"

/* ============================================================
 *  Software I2C on PA1(SCL) / PA0(SDA)
 *
 *  关键: 用 GPIO 寄存器直接翻 SDA 方向 (单条指令),
 *        而不是每次调用重量级的 DL_GPIO_initDigital*()
 * ============================================================ */

/* ── GPIOA 寄存器地址 (来自 MSPM0 头文件) ── */
#define GPIOA_DOE    (*(volatile uint32_t *)(GPIOA_BASE + 0x04))  /* Data Output Enable */
#define GPIOA_DOUT   (*(volatile uint32_t *)(GPIOA_BASE + 0x08))  /* Data Output        */
#define GPIOA_DIN    (*(volatile uint32_t *)(GPIOA_BASE + 0x10))  /* Data Input         */

/* ── SDA 位掩码 ── */
#define SDA_BIT      (1U << 0)    /* PA0 = bit 0 */

/* ── 宏: 快速方向切换 ── */
#define SDA_OUT()   (GPIOA_DOE |=  SDA_BIT)   /* PA0 → 输出 */
#define SDA_IN()    (GPIOA_DOE &= ~SDA_BIT)   /* PA0 → 输入 (Hi-Z, 外部上拉) */

/* ── 宏: SCL / SDA 值操作 ── */
#define SCL_H()     DL_GPIO_setPins(I2C_SCL_PORT, I2C_SCL_PIN)
#define SCL_L()     DL_GPIO_clearPins(I2C_SCL_PORT, I2C_SCL_PIN)
#define SDA_L()     DL_GPIO_clearPins(I2C_SDA_PORT, I2C_SDA_PIN)
#define SDA_READ()  DL_GPIO_readPins(I2C_SDA_PORT, I2C_SDA_PIN)

/* ── 延时 (100kHz I2C → 半周期 ~5μs) ── */
static void i2c_delay(void)
{
    delay_us(3);
}

/* ============================================================
 *  I2C 时序
 * ============================================================ */

static void I2C_GPIO_Init(void)
{
    /* SCL = PA1 始终输出, 初始高 */
    DL_GPIO_initDigitalOutput(I2C_SCL_IOMUX);
    SCL_H();

    /* SDA = PA0 初始为输入 (Hi-Z + 上拉 = 高) */
    DL_GPIO_initDigitalInputFeatures(I2C_SDA_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    i2c_delay();
}

static void I2C_Start(void)
{
    SDA_OUT();          /* SDA 输出模式          */
    SDA_L();            /* SDA 拉低 (SCL 仍高) — START! */
    i2c_delay();
    SCL_L();            /* SCL 低, 准备发数据   */
    i2c_delay();
}

static void I2C_Stop(void)
{
    SDA_OUT();          /* SDA 输出              */
    SDA_L();            /* SDA 拉低              */
    i2c_delay();
    SCL_H();            /* SCL 拉高              */
    i2c_delay();
    SDA_IN();           /* SDA 释放 → 上拉 → STOP! */
    i2c_delay();
}

static uint8_t I2C_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            SDA_IN();           /* 释放 = 1 (上拉) */
        else
        {
            SDA_OUT();
            SDA_L();            /* 拉低 = 0         */
        }
        i2c_delay();
        SCL_H();                /* 上升沿, 从机采样 */
        i2c_delay();
        SCL_L();
        dat <<= 1;
        i2c_delay();
    }

    /* 读 ACK */
    SDA_IN();                   /* 释放, 等待从机拉低 */
    i2c_delay();
    SCL_H();
    i2c_delay();
    {
        uint8_t ack = SDA_READ() ? 1 : 0;
        SCL_L();
        i2c_delay();
        return ack;
    }
}

static uint8_t I2C_RecvByte(uint8_t ack)
{
    uint8_t i, dat = 0;
    SDA_IN();

    for (i = 0; i < 8; i++)
    {
        i2c_delay();
        SCL_H();
        i2c_delay();
        dat <<= 1;
        if (SDA_READ())
            dat |= 0x01;
        SCL_L();
        i2c_delay();
    }

    /* 主机回 ACK/NACK */
    if (ack)
        SDA_IN();
    else
    {
        SDA_OUT();
        SDA_L();
    }
    i2c_delay();
    SCL_H();
    i2c_delay();
    SCL_L();
    i2c_delay();
    SDA_IN();

    return dat;
}

/* ============================================================
 *  MPU6050 读写
 * ============================================================ */

static void MPU6050_WriteReg(uint8_t reg, uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(MPU6050_I2C_ADDR << 1);       /* 0xD0 */
    I2C_SendByte(reg);
    I2C_SendByte(dat);
    I2C_Stop();
}

static uint8_t MPU6050_ReadReg(uint8_t reg)
{
    uint8_t dat;
    I2C_Start();
    I2C_SendByte(MPU6050_I2C_ADDR << 1);       /* 0xD0 */
    I2C_SendByte(reg);
    I2C_Start();
    I2C_SendByte((MPU6050_I2C_ADDR << 1) | 1);  /* 0xD1 */
    dat = I2C_RecvByte(1);                      /* NACK  */
    I2C_Stop();
    return dat;
}

/* ============================================================
 *  MPU6050 应用层
 * ============================================================ */

void MPU6050_Init(void)
{
    I2C_GPIO_Init();

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);   /* 唤醒        */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);   /* 全轴不待机  */
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);   /* 100Hz       */
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);        /* DLPF ~5Hz   */
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);   /* ±2000°/s   */
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);  /* ±16g        */
}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (int16_t)((DataH << 8) | DataL);

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (int16_t)((DataH << 8) | DataL);

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (int16_t)((DataH << 8) | DataL);

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (int16_t)((DataH << 8) | DataL);

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (int16_t)((DataH << 8) | DataL);

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (int16_t)((DataH << 8) | DataL);
}
