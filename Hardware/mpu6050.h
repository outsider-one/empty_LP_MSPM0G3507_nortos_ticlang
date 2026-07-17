#ifndef _MPU6050_H
#define _MPU6050_H

#include "ti_msp_dl_config.h"
#include "board.h"

/* ============================================================
 *  MPU6050 I2C 地址
 * ============================================================ */
#define MPU6050_ADDR              0x68

/* ============================================================
 *  引脚 (来自 SysConfig)
 *  SCL → PA1   SDA → PA0
 * ============================================================ */
#define I2C_SCL_PORT              MPU6050_SCL_PORT
#define I2C_SDA_PORT              MPU6050_SDA_PORT
#define I2C_SCL_PIN               MPU6050_SCL_PIN_0_PIN
#define I2C_SDA_PIN               MPU6050_SDA_PIN_1_PIN
#define I2C_SCL_IOMUX             MPU6050_SCL_PIN_0_IOMUX
#define I2C_SDA_IOMUX             MPU6050_SDA_PIN_1_IOMUX

/* ============================================================
 *  MPU6050 寄存器
 * ============================================================ */
#define MPU6050_SMPLRT_DIV        0x19
#define MPU6050_CONFIG            0x1A
#define MPU6050_GYRO_CONFIG       0x1B
#define MPU6050_ACCEL_CONFIG      0x1C
#define MPU6050_ACCEL_XOUT_H      0x3B
#define MPU6050_ACCEL_XOUT_L      0x3C
#define MPU6050_ACCEL_YOUT_H      0x3D
#define MPU6050_ACCEL_YOUT_L      0x3E
#define MPU6050_ACCEL_ZOUT_H      0x3F
#define MPU6050_ACCEL_ZOUT_L      0x40
#define MPU6050_GYRO_XOUT_H       0x43
#define MPU6050_GYRO_XOUT_L       0x44
#define MPU6050_GYRO_YOUT_H       0x45
#define MPU6050_GYRO_YOUT_L       0x46
#define MPU6050_GYRO_ZOUT_H       0x47
#define MPU6050_GYRO_ZOUT_L       0x48
#define MPU6050_PWR_MGMT_1        0x6B
#define MPU6050_PWR_MGMT_2        0x6C
#define MPU6050_WHO_AM_I          0x75

/* ============================================================
 *  函数声明 — 全部通过指针返回数据
 * ============================================================ */

/* 初始化软件 I2C + MPU6050 */
void MPU6050_Init(void);

/* 读取六轴原始数据 (每个 out 指针指向一个 int16_t) */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

/* 读取 WHO_AM_I (应为 0x68) */
uint8_t MPU6050_GetID(void);

#endif /* _MPU6050_H */
