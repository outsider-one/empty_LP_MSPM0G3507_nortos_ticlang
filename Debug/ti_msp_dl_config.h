/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000
/* Defines for SYSPLL_ERR_01 Workaround */
/* Represent 1.000 as 1000 */
#define FLOAT_TO_INT_SCALE                                               (1000U)
#define FCC_EXPECTED_RATIO                                                  2500
#define FCC_UPPER_BOUND                       (FCC_EXPECTED_RATIO * (1 + 0.003))
#define FCC_LOWER_BOUND                       (FCC_EXPECTED_RATIO * (1 - 0.003))

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);


/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA1
#define PWM_0_INST_IRQHandler                                   TIMA1_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA1_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             80000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOB
#define GPIO_PWM_0_C0_PIN                                          DL_GPIO_PIN_2
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM15)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM15_PF_TIMA1_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOB
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_3
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM16)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM16_PF_TIMA1_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (12499U)




/* Defines for OLED */
#define OLED_INST                                                           I2C0
#define OLED_INST_IRQHandler                                     I2C0_IRQHandler
#define OLED_INST_INT_IRQN                                         I2C0_INT_IRQn
#define OLED_BUS_SPEED_HZ                                                1000000
#define GPIO_OLED_SDA_PORT                                                 GPIOA
#define GPIO_OLED_SDA_PIN                                         DL_GPIO_PIN_28
#define GPIO_OLED_IOMUX_SDA                                       (IOMUX_PINCM3)
#define GPIO_OLED_IOMUX_SDA_FUNC                        IOMUX_PINCM3_PF_I2C0_SDA
#define GPIO_OLED_SCL_PORT                                                 GPIOA
#define GPIO_OLED_SCL_PIN                                         DL_GPIO_PIN_31
#define GPIO_OLED_IOMUX_SCL                                       (IOMUX_PINCM6)
#define GPIO_OLED_IOMUX_SCL_FUNC                        IOMUX_PINCM6_PF_I2C0_SCL



/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for key: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_key_PIN                                             (DL_GPIO_PIN_21)
#define KEY_key_IOMUX                                            (IOMUX_PINCM49)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for led: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED_led_PIN                                             (DL_GPIO_PIN_22)
#define LED_led_IOMUX                                            (IOMUX_PINCM50)
/* Port definition for Pin Group MPU6050_SCL */
#define MPU6050_SCL_PORT                                                 (GPIOA)

/* Defines for PIN_0: GPIOA.1 with pinCMx 2 on package pin 34 */
#define MPU6050_SCL_PIN_0_PIN                                    (DL_GPIO_PIN_1)
#define MPU6050_SCL_PIN_0_IOMUX                                   (IOMUX_PINCM2)
/* Port definition for Pin Group MPU6050_SDA */
#define MPU6050_SDA_PORT                                                 (GPIOA)

/* Defines for PIN_1: GPIOA.0 with pinCMx 1 on package pin 33 */
#define MPU6050_SDA_PIN_1_PIN                                    (DL_GPIO_PIN_0)
#define MPU6050_SDA_PIN_1_IOMUX                                   (IOMUX_PINCM1)
/* Port definition for Pin Group BIN */
#define BIN_PORT                                                         (GPIOA)

/* Defines for BIN1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define BIN_BIN1_PIN                                            (DL_GPIO_PIN_16)
#define BIN_BIN1_IOMUX                                           (IOMUX_PINCM38)
/* Defines for BIN2: GPIOA.24 with pinCMx 54 on package pin 25 */
#define BIN_BIN2_PIN                                            (DL_GPIO_PIN_24)
#define BIN_BIN2_IOMUX                                           (IOMUX_PINCM54)
/* Port definition for Pin Group AIN */
#define AIN_PORT                                                         (GPIOB)

/* Defines for AIN1: GPIOB.17 with pinCMx 43 on package pin 14 */
#define AIN_AIN1_PIN                                            (DL_GPIO_PIN_17)
#define AIN_AIN1_IOMUX                                           (IOMUX_PINCM43)
/* Defines for AIN2: GPIOB.19 with pinCMx 45 on package pin 16 */
#define AIN_AIN2_PIN                                            (DL_GPIO_PIN_19)
#define AIN_AIN2_IOMUX                                           (IOMUX_PINCM45)
/* Port definition for Pin Group ENCODERA */
#define ENCODERA_PORT                                                    (GPIOA)

/* Defines for E1A: GPIOA.25 with pinCMx 55 on package pin 26 */
// groups represented: ["ENCODERB","ENCODERA"]
// pins affected: ["E2A","E2B","E1A","E1B"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define ENCODERA_E1A_IIDX                                   (DL_GPIO_IIDX_DIO25)
#define ENCODERA_E1A_PIN                                        (DL_GPIO_PIN_25)
#define ENCODERA_E1A_IOMUX                                       (IOMUX_PINCM55)
/* Defines for E1B: GPIOA.15 with pinCMx 37 on package pin 8 */
#define ENCODERA_E1B_IIDX                                   (DL_GPIO_IIDX_DIO15)
#define ENCODERA_E1B_PIN                                        (DL_GPIO_PIN_15)
#define ENCODERA_E1B_IOMUX                                       (IOMUX_PINCM37)
/* Port definition for Pin Group ENCODERB */
#define ENCODERB_PORT                                                    (GPIOA)

/* Defines for E2A: GPIOA.26 with pinCMx 59 on package pin 30 */
#define ENCODERB_E2A_IIDX                                   (DL_GPIO_IIDX_DIO26)
#define ENCODERB_E2A_PIN                                        (DL_GPIO_PIN_26)
#define ENCODERB_E2A_IOMUX                                       (IOMUX_PINCM59)
/* Defines for E2B: GPIOA.27 with pinCMx 60 on package pin 31 */
#define ENCODERB_E2B_IIDX                                   (DL_GPIO_IIDX_DIO27)
#define ENCODERB_E2B_PIN                                        (DL_GPIO_PIN_27)
#define ENCODERB_E2B_IOMUX                                       (IOMUX_PINCM60)
/* Defines for Sensor1: GPIOB.14 with pinCMx 31 on package pin 2 */
#define Sensor_Sensor1_PORT                                              (GPIOB)
#define Sensor_Sensor1_PIN                                      (DL_GPIO_PIN_14)
#define Sensor_Sensor1_IOMUX                                     (IOMUX_PINCM31)
/* Defines for Sensor2: GPIOB.11 with pinCMx 28 on package pin 63 */
#define Sensor_Sensor2_PORT                                              (GPIOB)
#define Sensor_Sensor2_PIN                                      (DL_GPIO_PIN_11)
#define Sensor_Sensor2_IOMUX                                     (IOMUX_PINCM28)
/* Defines for Sensor3: GPIOB.10 with pinCMx 27 on package pin 62 */
#define Sensor_Sensor3_PORT                                              (GPIOB)
#define Sensor_Sensor3_PIN                                      (DL_GPIO_PIN_10)
#define Sensor_Sensor3_IOMUX                                     (IOMUX_PINCM27)
/* Defines for Sensor4: GPIOB.1 with pinCMx 13 on package pin 48 */
#define Sensor_Sensor4_PORT                                              (GPIOB)
#define Sensor_Sensor4_PIN                                       (DL_GPIO_PIN_1)
#define Sensor_Sensor4_IOMUX                                     (IOMUX_PINCM13)
/* Defines for Sensor5: GPIOB.0 with pinCMx 12 on package pin 47 */
#define Sensor_Sensor5_PORT                                              (GPIOB)
#define Sensor_Sensor5_PIN                                       (DL_GPIO_PIN_0)
#define Sensor_Sensor5_IOMUX                                     (IOMUX_PINCM12)
/* Defines for Sensor6: GPIOA.30 with pinCMx 5 on package pin 37 */
#define Sensor_Sensor6_PORT                                              (GPIOA)
#define Sensor_Sensor6_PIN                                      (DL_GPIO_PIN_30)
#define Sensor_Sensor6_IOMUX                                      (IOMUX_PINCM5)
/* Defines for Sensor7: GPIOB.18 with pinCMx 44 on package pin 15 */
#define Sensor_Sensor7_PORT                                              (GPIOB)
#define Sensor_Sensor7_PIN                                      (DL_GPIO_PIN_18)
#define Sensor_Sensor7_IOMUX                                     (IOMUX_PINCM44)
/* Defines for Sensor8: GPIOB.25 with pinCMx 56 on package pin 27 */
#define Sensor_Sensor8_PORT                                              (GPIOB)
#define Sensor_Sensor8_PIN                                      (DL_GPIO_PIN_25)
#define Sensor_Sensor8_IOMUX                                     (IOMUX_PINCM56)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_OLED_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
