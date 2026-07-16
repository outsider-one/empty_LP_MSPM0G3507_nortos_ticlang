#ifndef _GRAYSENSOR_H
#define _GRAYSENSOR_H

#include "ti_msp_dl_config.h"
#include "board.h"

/* ── 传感器引脚 (来自 SysConfig) ── */
#define SENSOR1_PORT    Sensor_Sensor1_PORT
#define SENSOR1_PIN     Sensor_Sensor1_PIN
#define SENSOR2_PORT    Sensor_Sensor2_PORT
#define SENSOR2_PIN     Sensor_Sensor2_PIN
#define SENSOR3_PORT    Sensor_Sensor3_PORT
#define SENSOR3_PIN     Sensor_Sensor3_PIN
#define SENSOR4_PORT    Sensor_Sensor4_PORT
#define SENSOR4_PIN     Sensor_Sensor4_PIN
#define SENSOR5_PORT    Sensor_Sensor5_PORT
#define SENSOR5_PIN     Sensor_Sensor5_PIN
#define SENSOR6_PORT    Sensor_Sensor6_PORT
#define SENSOR6_PIN     Sensor_Sensor6_PIN
#define SENSOR7_PORT    Sensor_Sensor7_PORT
#define SENSOR7_PIN     Sensor_Sensor7_PIN
#define SENSOR8_PORT    Sensor_Sensor8_PORT
#define SENSOR8_PIN     Sensor_Sensor8_PIN

uint8_t Gray_Sensor_Read(void);

#endif
