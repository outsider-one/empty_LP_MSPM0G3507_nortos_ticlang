#include "board.h"
#include "oled.h"
#include "mpu6050.h"
#include "GraySensor.h"

int32_t encoderA_cnt,PWMA,encoderB_cnt,PWMB;
int Flag_Stop=0;
int target_speed=30;
int16_t AX,AY,AZ,GX,GY,GZ;
uint8_t sensor;
signed char Error=0;

/* TI C Runtime hooks */
void __mpu_init(void) {}
void _system_post_cinit(void) {}

/* Gray sensor error */
signed char ERROR_GET()
{
    signed char Error=0;
    if (sensor & 0x01) Error= -2;
    else if (sensor & 0x02) Error= -8;
    else if (sensor & 0x04) Error= -5;
    else if (sensor & 0x08) Error= -3;
    else if (sensor & 0x10) Error= -1;
    else if (sensor & 0x20) Error= 1;
    else if (sensor & 0x40) Error= 3;
    else if (sensor & 0x80) Error= 5;
    else if (sensor & 0xC0) Error= 8;
    else Error=0;
    return Error;
}

/* signed value → show sign-char + abs(value) at (x,y) */
static void OLED_ShowSigned(uint8_t x, uint8_t y, int16_t val, uint8_t len, uint8_t size)
{
    if (val < 0) {
        OLED_ShowChar(x, y, '-', size, 1);
        OLED_ShowNumber(x + size/2, y, (uint32_t)(-val), len, size);
    } else {
        OLED_ShowChar(x, y, ' ', size, 1);
        OLED_ShowNumber(x + size/2, y, (uint32_t)val, len, size);
    }
}

int main(void)
{
    SYSCFG_DL_init();
    DL_Timer_startCounter(PWM_0_INST);
    NVIC_ClearPendingIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    OLED_Init();
    MPU6050_Init();

    while (1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

        /* Line 0: Accel cm/s² (±9999) */
        OLED_ShowString(0, 0, (const uint8_t *)"A:");     /* title    */
        OLED_ShowSigned(24, 0, AX, 4, 12);                /* "  -21"  */
        OLED_ShowSigned(56, 0, AY, 4, 12);
        OLED_ShowSigned(88, 0, AZ, 4, 12);

        /* Line 2: Gyro milli-rad/s (±9999) */
        OLED_ShowString(0, 16, (const uint8_t *)"G:");
        OLED_ShowSigned(24,16, GX, 4, 12);
        OLED_ShowSigned(56,16, GY, 4, 12);
        OLED_ShowSigned(88,16, GZ, 4, 12);

        OLED_Refresh_Gram();
    }
}

/* 10ms */
void TIMER_0_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
        if(DL_TIMER_IIDX_ZERO)
        {
            encoderA_cnt = Get_Encoder_countA;
            encoderB_cnt = -Get_Encoder_countB;
            Get_Encoder_countA=Get_Encoder_countB=0;
            if(Flag_Stop)
            {
                PWMA = -Velocity_A(target_speed,encoderA_cnt);
                PWMB = -Velocity_B(target_speed,encoderB_cnt);
                PWMA=limit_PWM(PWMA,-7999,7999);
                PWMB=limit_PWM(PWMB,-7999,7999);
                Set_PWM(PWMA,PWMB);
            }else Set_PWM(0,0);
        }
    }
}
