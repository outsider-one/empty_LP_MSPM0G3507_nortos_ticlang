#include "mpu6050.h"

/* ============================================================
 *  软件 I2C 底层 — PA1=SCL, PA0=SDA
 *
 *  GPIOA 寄存器  (来自 msp.h)
 *    BASE = 0x400A0000
 *    DOE  = BASE + 0x04  方向寄存器 (1=输出, 0=输入)
 *    DOUT = BASE + 0x08  输出数据
 *    DIN  = BASE + 0x10  输入数据
 *
 *  SDA 方向:
 *    DOE |=  BIT → 输出 (用于拉低)
 *    DOE &= ~BIT → 输入 (Hi-Z → 外部/内部上拉 = 高)
 * ============================================================ */

#define SDA_BIT      (1U << 0)

/* 输出低: 先写 0 到 DOUT 锁存器, 再使能输出 (避免电平毛刺) */
#define SDA_OUT()    do {                            \
        GPIOA->DOUTCLR31_0 = SDA_BIT;                \
        GPIOA->DOESET31_0  = SDA_BIT;                \
    } while(0)

/* 输入: 关 DOE → Hi-Z → 外部上拉 = 高 */
#define SDA_IN()     (GPIOA->DOECLR31_0 = SDA_BIT)

#define SCL_H()      DL_GPIO_setPins   (I2C_SCL_PORT, I2C_SCL_PIN)
#define SCL_L()      DL_GPIO_clearPins (I2C_SCL_PORT, I2C_SCL_PIN)
#define SDA_L()      DL_GPIO_clearPins (I2C_SDA_PORT, I2C_SDA_PIN)
#define SDA_READ()   DL_GPIO_readPins  (I2C_SDA_PORT, I2C_SDA_PIN)

/* ── 延时 ~100 kHz ── */
static void i2c_delay(void)
{
    delay_us(5);
}

/* ============================================================
 *  I2C 原语
 * ============================================================ */

/* 总线初始化: SCL 输出高, SDA 输入上拉 */
static void I2C_Init(void)
{
    DL_GPIO_initDigitalOutput(I2C_SCL_IOMUX);
    SCL_H();

    DL_GPIO_initDigitalInputFeatures(I2C_SDA_IOMUX,
        DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);

    delay_us(10);
}

/* START: SCL 高时 SDA ↓ */
static void I2C_Start(void)
{
    SDA_OUT();              /* 拉低    */
    SDA_L();
    i2c_delay();
    SCL_L();                /* SCL 也拉低 → 开始传输 */
    i2c_delay();
}

/* STOP: SCL 高时 SDA ↑ */
static void I2C_Stop(void)
{
    SDA_OUT();
    SDA_L();                /* 先确保 SDA 为低        */
    i2c_delay();
    SCL_H();                /* SCL 拉高               */
    i2c_delay();
    SDA_IN();               /* SDA 释放 → 上升沿 STOP */
    i2c_delay();
}

/* 写一个字节, 返回 0=ACK / 1=NACK */
static uint8_t I2C_WriteByte(uint8_t dat)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            SDA_IN();       /* 释放 = 1 (上拉) */
        else
        {
            SDA_OUT();
            SDA_L();        /* 拉低 = 0        */
        }
        i2c_delay();
        SCL_H();            /* ↑ 从机锁存       */
        i2c_delay();
        SCL_L();            /* ↓ 准备下一位     */
        dat <<= 1;
        i2c_delay();
    }

    /* 第 9 拍 — 从机回 ACK */
    SDA_IN();
    i2c_delay();
    SCL_H();
    i2c_delay();
    {
        uint8_t ack = (SDA_READ() != 0);   /* SDA 低 = ACK */
        SCL_L();
        i2c_delay();
        return ack;
    }
}

/* 读一个字节, ack=0 主机回 ACK, ack=1 主机回 NACK */
static uint8_t I2C_ReadByte(uint8_t ack)
{
    uint8_t i, dat = 0;

    SDA_IN();

    for (i = 0; i < 8; i++)
    {
        i2c_delay();
        SCL_H();            /* ↑ 从机输出数据到 SDA */
        i2c_delay();
        dat <<= 1;
        if (SDA_READ())
            dat |= 0x01;
        SCL_L();
        i2c_delay();
    }

    /* 第 9 拍 — 主机回 ACK/NACK */
    if (ack)
        SDA_IN();           /* NACK → 释放 */
    else
    {
        SDA_OUT();
        SDA_L();            /* ACK → 拉低 */
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
 *  MPU6050 寄存器读写
 * ============================================================ */

/* 写寄存器 */
static void MPU_WriteReg(uint8_t reg, uint8_t dat)
{
    I2C_Start();
    I2C_WriteByte(MPU6050_ADDR << 1);          /* 0xD0 */
    I2C_WriteByte(reg);
    I2C_WriteByte(dat);
    I2C_Stop();
}

/* 读寄存器 */
static uint8_t MPU_ReadReg(uint8_t reg)
{
    uint8_t dat;

    I2C_Start();
    I2C_WriteByte(MPU6050_ADDR << 1);          /* 0xD0      */
    I2C_WriteByte(reg);
    I2C_Start();                               /* repeated   */
    I2C_WriteByte((MPU6050_ADDR << 1) | 1);     /* 0xD1      */
    dat = I2C_ReadByte(1);                     /* NACK      */
    I2C_Stop();

    return dat;
}

/* ============================================================
 *  公开接口
 * ============================================================ */

/**
 * @brief  MPU6050 初始化
 *         1. 初始化软件 I2C (PA1=SCL, PA0=SDA)
 *         2. 唤醒传感器并配置量程
 */
void MPU6050_Init(void)
{
    I2C_Init();

    MPU_WriteReg(MPU6050_PWR_MGMT_1,  0x01);  /* 退出休眠, 时钟=PLL  */
    MPU_WriteReg(MPU6050_PWR_MGMT_2,  0x00);  /* 全部轴工作          */
    MPU_WriteReg(MPU6050_SMPLRT_DIV,  0x09);  /* 100Hz (1kHz / 10)  */
    MPU_WriteReg(MPU6050_CONFIG,      0x06);  /* DLPF ~5Hz 带宽      */
    MPU_WriteReg(MPU6050_GYRO_CONFIG, 0x18);  /* 陀螺仪 ±2000°/s   */
    MPU_WriteReg(MPU6050_ACCEL_CONFIG,0x18);  /* 加速度 ±16g        */
}

/**
 * @brief  读取 WHO_AM_I 寄存器
 * @return 设备 ID (应为 0x68)
 */
uint8_t MPU6050_GetID(void)
{
    return MPU_ReadReg(MPU6050_WHO_AM_I);
}

/**
 * @brief  读取六轴原始数据
 *
 *  加速度 满量程 ±16g   → 原始值 / 2048 = g
 *  陀螺仪 满量程 ±2000°/s → 原始值 / 16.4 = °/s
 *
 * @param[out] AccX/Y/Z   加速度 X/Y/Z 轴
 * @param[out] GyroX/Y/Z  陀螺仪 X/Y/Z 轴
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;

    /* ── 加速度 ── */
    DataH = MPU_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (int16_t)((DataH << 8) | DataL);

    DataH = MPU_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (int16_t)((DataH << 8) | DataL);

    DataH = MPU_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (int16_t)((DataH << 8) | DataL);

    /* ── 陀螺仪 ── */
    DataH = MPU_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (int16_t)((DataH << 8) | DataL);

    DataH = MPU_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (int16_t)((DataH << 8) | DataL);

    DataH = MPU_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (int16_t)((DataH << 8) | DataL);
}
