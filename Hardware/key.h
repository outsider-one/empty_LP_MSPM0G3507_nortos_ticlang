#ifndef _KEY_H
#define _KEY_H
#include "ti_msp_dl_config.h"
#include "board.h"

#define KEY  DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)
unsigned char Key_Scan(void);

#endif
