#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

#include "board.h"
#include "stdio.h"
#include <ti/driverlib/dl_i2c.h>

/* SSD1306 I2C slave address (7-bit) */
#define OLED_I2C_ADDR  0x3C

uint8_t OLED_GRAM[128][8];
/**************************************************************************
Function: Refresh the OLED screen
Input   : none
Output  : none
�������ܣ�ˢ��OLED��Ļ
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //Set page address (0~7) //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //Set the display location - column low address //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //Set the display location - column height address //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
}
/**************************************************************************
Function: Write one byte to OLED via I2C
Input   : dat: data/command to write, cmd: data/command flag 0=command;1=data
Output  : none
�������ܣ�ͨ��I2C��OLEDд��һ���ֽ�
��ڲ�����dat:Ҫд�������/���cmd:����/�����־ 0=����;1=����
����  ֵ����
**************************************************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	uint8_t tx_buf[2];
	tx_buf[0] = cmd ? 0x40 : 0x00;  /* control byte: 0x00=command, 0x40=data */
	tx_buf[1] = dat;                 /* payload byte */

	/* Wait for I2C controller to be idle */
	while ((DL_I2C_getControllerStatus(OLED_INST) &
	        DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
	       == DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

	/* Pre-fill TX FIFO with 2 bytes */
	DL_I2C_fillControllerTXFIFO(OLED_INST, tx_buf, 2);

	/* Start I2C transfer: send 2 bytes, auto-generate STOP */
	DL_I2C_startControllerTransfer(OLED_INST, OLED_I2C_ADDR,
		DL_I2C_CONTROLLER_DIRECTION_TX, 2);

	/* Wait for transfer to complete (BUSY flag clears) */
	while ((DL_I2C_getControllerStatus(OLED_INST) &
	        DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
	       == DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
}
/**************************************************************************
Function: Turn on the OLED display
Input   : none
Output  : none
�������ܣ�����OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC command //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
/**************************************************************************
Function: Turn off the OLED display
Input   : none
Output  : none
�������ܣ��ر�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC command //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
/**************************************************************************
Function: Screen clear function, clear the screen, the entire screen is black
Input   : none
Output  : none
�������ܣ���������,������,������Ļ�Ǻ�ɫ��
��ڲ�������
����  ֵ����
**************************************************************************/
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
	OLED_Refresh_Gram(); //Update the display //������ʾ
}
/**************************************************************************
Function: Draw point
Input   : x,y: starting coordinate;T :1, fill,0, empty
Output  : none
�������ܣ�����
��ڲ�����x,y :�������; t:1,���,0,���
����  ֵ����
**************************************************************************/
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}
/**************************************************************************
Function: Displays a character at the specified position
Input   : x,y: starting coordinate;Len: The number of digits;Size: font size;Mode :0, anti-white display,1, normal display
Output  : none
�������ܣ���ָ��λ����ʾһ���ַ�
��ڲ�����x,y :�������; len :���ֵ�λ��; size:�����С; mode:0,������ʾ,1,������ʾ
����  ֵ����
**************************************************************************/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr=chr-' '; //Get the offset value //�õ�ƫ�ƺ��ֵ
    for(t=0;t<size;t++)
    {
		if(size==12)temp=oled_asc2_1206[chr][t];  //Invoke 1206 font   //����1206����
		else temp=oled_asc2_1608[chr][t];		      //Invoke the 1608 font //����1608����
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
    }
}
/**************************************************************************
Function: Find m to the NTH power
Input   : m: base number, n: power number
Output  : none
�������ܣ���m��n�η��ĺ���
��ڲ�����m��������n���η���
����  ֵ����
**************************************************************************/
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;
	return result;
}

/**************************************************************************
Function: Displays 2 numbers
Input   : x,y: starting coordinate;Len: The number of digits;Size: font size;Mode: mode;Num: value (0 ~ 4294967295);
Output  : none
�������ܣ���ʾ2������
��ڲ�����x,y :�������; len :���ֵ�λ��; size:�����С; mode:ģʽ; num:��ֵ(0~4294967295);
����  ֵ����
**************************************************************************/
void OLED_ShowNumber(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1;

		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
	}
}
/**************************************************************************
Function: Display string
Input   : x,y: starting coordinate;*p: starting address of the string
Output  : none
�������ܣ���ʾ�ַ���
��ڲ�����x,y :�������; *p:�ַ�����ʼ��ַ
����  ֵ����
**************************************************************************/
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p!='\0')
    {
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,12,1);
        x+=8;
        p++;
    }
}
/**************************************************************************
Function: Initialize the OLED
Input   : none
Output  : none
�������ܣ���ʼ��OLED
��ڲ���: ��
����  ֵ����
**************************************************************************/
void OLED_Init(void)
{
	/* OLED hardware reset via I2C command (no RST pin needed) */
	OLED_WR_Byte(0xAE,OLED_CMD); //Display OFF
	delay_ms(120);

	OLED_WR_Byte(0xD5,OLED_CMD); //Set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(80,OLED_CMD);   //[3:0], divide ratio;[7:4], oscillator freq
	OLED_WR_Byte(0xA8,OLED_CMD); //Set multiplex ratio
	OLED_WR_Byte(0X3F,OLED_CMD); //Default 0x3F (1/64 duty)
	OLED_WR_Byte(0xD3,OLED_CMD); //Set display offset
	OLED_WR_Byte(0X00,OLED_CMD); //Default is 0

	OLED_WR_Byte(0x40,OLED_CMD); //Set display start line [5:0]

	OLED_WR_Byte(0x8D,OLED_CMD); //Charge pump setting
	OLED_WR_Byte(0x14,OLED_CMD); //Bit2, enable charge pump
	OLED_WR_Byte(0x20,OLED_CMD); //Set memory addressing mode
	OLED_WR_Byte(0x02,OLED_CMD); //Page addressing mode
	OLED_WR_Byte(0xA1,OLED_CMD); //Segment re-map, column 127 = SEG0
	OLED_WR_Byte(0xC0,OLED_CMD); //COM scan direction, normal mode
	OLED_WR_Byte(0xDA,OLED_CMD); //Set COM pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD); //Alternative COM pin config

	OLED_WR_Byte(0x81,OLED_CMD); //Contrast control
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255; Default 0x7F (brightness)
	OLED_WR_Byte(0xD9,OLED_CMD); //Set pre-charge period
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //Set VCOMH deselect level
	OLED_WR_Byte(0x30,OLED_CMD); //~0.83 x VCC

	OLED_WR_Byte(0xA4,OLED_CMD); //Entire display ON follows RAM content
	OLED_WR_Byte(0xA6,OLED_CMD); //Normal display (not inverted)
	OLED_WR_Byte(0xAF,OLED_CMD); //Display ON
	OLED_Clear();
}

/**************************************************************************
Function: Display Chinese character
Input   : x: horizontal coordinate; y: vertical coordinate;
          no: index in Hzk16 array; font_width: font width; font_height: font height
Output  : none
�������ܣ���ʾ����
��ڲ���: x,y :�������; no: ������Hzk16�����е��к�; font_width: �����; font_height: �����
����  ֵ����
**************************************************************************/
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no,uint8_t font_width,uint8_t font_height)
{
	 uint8_t t, i;
   for(i=0;i<(font_height/8);i++)
	 {
			OLED_Set_Pos(x,y+i);
			for(t=0;t<font_width;t++)
			{
					OLED_WR_Byte(Hzk16[(font_height/8)*no+i][t],OLED_DATA);
			}
	 }
}
/**************************************************************************
Function: Set the display coordinates (position)
Input   : x, y: starting point coordinates
Output  : none
�������ܣ����ú�������Ļ����ʾ�����꣨λ�ã�
��ڲ���: x,y :�������
����  ֵ����
**************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	 OLED_WR_Byte(0xb0+y,OLED_CMD);
	 OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	 OLED_WR_Byte((x&0x0f),OLED_CMD);
}
