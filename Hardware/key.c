#include "key.h"

#define KEY  DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)

unsigned char key_flag=0;

unsigned char Key_Scan(void)      //检测到按键按下返回1，否则返回0
{
    if(KEY==0)
    {
        delay_ms(10);
        key_flag=1;
    }
    if(KEY==1&&key_flag==1)
    {
        key_flag=0;
        return 1;
    }
    else return 0;
}
