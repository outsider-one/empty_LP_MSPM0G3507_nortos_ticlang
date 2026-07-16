#include "motor.h"
float Velcity_Kp=1.0,  Velcity_Ki=0.4,  Velcity_Kd; //����ٶ�PID����

int limit_PWM(int value,int low,int high)
{
	if(value>high) return high;
	else if(value<low) return low;
	else return value;
}


void Set_PWM(int pwmA,int pwmB)
{
	 if(pwmA>0)
    {
        DL_GPIO_setPins(AIN_PORT,AIN_AIN2_PIN);
        DL_GPIO_clearPins(AIN_PORT,AIN_AIN1_PIN);
		DL_Timer_setCaptureCompareValue(PWM_0_INST,ABS(pwmA),GPIO_PWM_0_C0_IDX);
    }
    else
    {
        DL_GPIO_setPins(AIN_PORT,AIN_AIN1_PIN);
        DL_GPIO_clearPins(AIN_PORT,AIN_AIN2_PIN);
		DL_Timer_setCaptureCompareValue(PWM_0_INST,ABS(pwmA),GPIO_PWM_0_C0_IDX);
    }
    if(pwmB>0)
    {
		DL_GPIO_setPins(BIN_PORT,BIN_BIN2_PIN);
        DL_GPIO_clearPins(BIN_PORT,BIN_BIN1_PIN);
        DL_Timer_setCaptureCompareValue(PWM_0_INST,ABS(pwmB),GPIO_PWM_0_C1_IDX);
    }
    else
    {
		DL_GPIO_setPins(BIN_PORT,BIN_BIN1_PIN);
        DL_GPIO_clearPins(BIN_PORT,BIN_BIN2_PIN);
		 DL_Timer_setCaptureCompareValue(PWM_0_INST,ABS(pwmB),GPIO_PWM_0_C1_IDX);
    }
   

}



int Velocity_A(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //������ر���
	static int ControlVelocityA, Last_biasA; //��̬�������������ý�������ֵ��Ȼ����
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
	ControlVelocityA+=Velcity_Ki*(Bias-Last_biasA)+Velcity_Kp*Bias;  //����ʽPI������
                                                               //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                             //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasA=Bias;	
	if(ControlVelocityA>7000) ControlVelocityA=7000;
	else if(ControlVelocityA<-7000) ControlVelocityA=-7000;
	return ControlVelocityA; //�����ٶȿ���ֵ
}


int Velocity_B(int TargetVelocity, int CurrentVelocity)
{  
	int Bias;  //������ر���
	static int ControlVelocityB, Last_biasB; //��̬�������������ý�������ֵ��Ȼ����
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
	ControlVelocityB+=Velcity_Ki*(Bias-Last_biasB)+Velcity_Kp*Bias;  //����ʽPI������
                                                               //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                            //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasB=Bias;	
	if(ControlVelocityB>7000) ControlVelocityB=7000;
	else if(ControlVelocityB<-7000) ControlVelocityB=-7000;
	return ControlVelocityB; //�����ٶȿ���ֵ
}

