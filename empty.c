#include "board.h"
#include "oled.h"
#include "mpu6050.h"
#include "GraySensor.h"

//#define KEY  DL_GPIO_readPins(KEY_PORT,KEY_key_PIN);

int32_t encoderA_cnt,PWMA,encoderB_cnt,PWMB;    //从编码器获得的相位输入，pwm输出值
int Flag_Stop=0;                                //电机运行标识位，0则停止，1则运行
int target_speed=30;                            //电机目标速度
int16_t AX,AY,AZ,GX,GY,GZ;                      //mpu6050获得的加速度和角速度值
uint8_t sensor; 
signed char Error=0;

unsigned char system=1;

/* ============================================================
 *  TI C Runtime hooks — 编译器启动流程所需的弱函数实现
 * ============================================================ */
void __mpu_init(void) {}

void _system_post_cinit(void) {}

//灰度传感器读取巡线误差

signed char ERROR_GET()
{
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
      //
}

int main(void)
{
	int i=0;
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
		sensor=Gray_Sensor_Read();
		OLED_ShowString(0, 0, (const uint8_t *)"Z:");   OLED_ShowNumber(16, 0,  (int32_t)AZ*100/2048, 4, 12);   /* centi-g */
		OLED_ShowString(0,16, (const uint8_t *)"G:");   OLED_ShowNumber(16,16, (int32_t)GX*10/164,   4, 12);   /* °/s */
		OLED_Refresh_Gram();

		/* KEY=PB21, 上拉输入, 按下=低电平, 松开=高电平 */
		if (DL_GPIO_readPins(KEY_PORT, KEY_key_PIN) == 0)
			LED_ON();
		else
			LED_OFF();
	    //Velocity_A(-15,encoderA_cnt);
		//printf("%d  %d\n\r",encoderA_cnt,encoderB_cnt);
		
    }
}

//10ms
void TIMER_0_INST_IRQHandler(void)
{
    if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
    {
        if(DL_TIMER_IIDX_ZERO)
        {
			encoderA_cnt = Get_Encoder_countA;//���������װ�෴������һ��������ֵ��Ҫ�෴
			encoderB_cnt = -Get_Encoder_countB;
			Get_Encoder_countA=Get_Encoder_countB=0;
			if(Flag_Stop)//����BLS������رյ��
			{
				PWMA = -Velocity_A(target_speed,encoderA_cnt);
				PWMB = -Velocity_B(target_speed,encoderB_cnt);
				//޷(-7999~7999),Ƶ10khz
				PWMA=limit_PWM(PWMA,-7999,7999);
				PWMB=limit_PWM(PWMB,-7999,7999);
				Set_PWM(PWMA,PWMB);//PWM���������
			}else Set_PWM(0,0);//�رյ��
			
		}
    }
}

