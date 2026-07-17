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

uint8_t MPUID;
uint8_t word[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char system=1;

/* ============================================================
 *  TI C Runtime hooks
 * ============================================================ */
void __mpu_init(void) {}
void _system_post_cinit(void) {}

/* 灰度传感器读取巡线误差 */
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
		MPUID = MPU6050_GetID();

		/* 8-bit binary */
		for (int i = 0; i < 8; i++)
			OLED_ShowChar(i * 16, 0,
				(MPUID & (0x80 >> i)) ? '1' : '0', 12, 1);

		/* decimal (expect 104 = 0x68) */
		OLED_ShowString(0, 16, (const uint8_t *)"ID:");
		OLED_ShowNumber(24, 16, MPUID, 3, 12);
		OLED_ShowNumber(0,32,1,1,12);
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
