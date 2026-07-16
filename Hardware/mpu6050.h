#ifndef _MPU6050_H
#define _MPU6050_H

#include "ti_msp_dl_config.h"
#include "board.h"

/* ── MPU6050 I2C address ── */
#define MPU6050_ADDR              0x68

/* ── Pin mapping: SCL=PA1, SDA=PA0 ── */
#define I2C_SCL_PORT              MPU6050_SCL_PORT
#define I2C_SDA_PORT              MPU6050_SDA_PORT
#define I2C_SCL_PIN               MPU6050_SCL_PIN_0_PIN
#define I2C_SDA_PIN               MPU6050_SDA_PIN_1_PIN
#define I2C_SCL_IOMUX             MPU6050_SCL_PIN_0_IOMUX
#define I2C_SDA_IOMUX             MPU6050_SDA_PIN_1_IOMUX

/* ── Pure Software I2C API ── */
void    I2C_Init(void);
void    I2C_Start(void);
void    I2C_Stop(void);
uint8_t I2C_SendByte(uint8_t dat);    /* returns 0=ACK, 1=NACK */
uint8_t I2C_RecvByte(uint8_t ack);    /* ack=0: ACK, ack=1: NACK */

#endif /* _MPU6050_H */
