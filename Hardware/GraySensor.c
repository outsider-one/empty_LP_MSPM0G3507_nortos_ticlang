#include "GraySensor.h"

/**
 * @brief  读取5路灰度传感器, 返回 8 位编码值
 * @return bit0=Sensor1  bit1=Sensor2  ...  bit4=Sensor5
 *         1 = 检测到黑线(高电平), 0 = 白底(低电平)
 */
uint8_t Gray_Sensor_Read(void)
{
    uint8_t val = 0;

    if (DL_GPIO_readPins(SENSOR1_PORT, SENSOR1_PIN)) val |= 0x01;
    if (DL_GPIO_readPins(SENSOR2_PORT, SENSOR2_PIN)) val |= 0x02;
    if (DL_GPIO_readPins(SENSOR3_PORT, SENSOR3_PIN)) val |= 0x04;
    if (DL_GPIO_readPins(SENSOR4_PORT, SENSOR4_PIN)) val |= 0x08;
    if (DL_GPIO_readPins(SENSOR5_PORT, SENSOR5_PIN)) val |= 0x10;
    if (DL_GPIO_readPins(SENSOR6_PORT, SENSOR6_PIN)) val |= 0x20;
    if (DL_GPIO_readPins(SENSOR7_PORT, SENSOR7_PIN)) val |= 0x40;
    if (DL_GPIO_readPins(SENSOR8_PORT, SENSOR8_PIN)) val |= 0x80;

    return val;
}
