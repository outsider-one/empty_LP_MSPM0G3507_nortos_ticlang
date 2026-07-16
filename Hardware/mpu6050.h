#ifndef _MPU6050_H
#define _MPU6050_H

#include "ti_msp_dl_config.h"
#include "board.h"

/* ============================================================
 *  MPU6050 I2C slave address (7-bit): 0x68 (AD0 = LOW)
 * ============================================================ */
#define MPU6050_I2C_ADDR         0x68

/* ============================================================
 *  Software I2C pin mapping (from ti_msp_dl_config.h)
 *  SCL → PA1 (IOMUX_PINCM2),  SDA → PA0 (IOMUX_PINCM1)
 * ============================================================ */
#define I2C_SCL_PORT             MPU6050_SCL_PORT
#define I2C_SDA_PORT             MPU6050_SDA_PORT
#define I2C_SCL_PIN              MPU6050_SCL_PIN_0_PIN
#define I2C_SDA_PIN              MPU6050_SDA_PIN_1_PIN
#define I2C_SCL_IOMUX            MPU6050_SCL_PIN_0_IOMUX
#define I2C_SDA_IOMUX            MPU6050_SDA_PIN_1_IOMUX

/* ============================================================
 *  MPU6050 Register Map
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
#define MPU6050_TEMP_OUT_H        0x41
#define MPU6050_TEMP_OUT_L        0x42
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
 *  Function prototypes
 * ============================================================ */
void    MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void    MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif /* _MPU6050_H */
