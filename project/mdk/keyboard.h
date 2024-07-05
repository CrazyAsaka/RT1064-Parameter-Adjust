#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <zf_common_headfile.h>

#define N 4

#define __Now_Tick 			(timer_get(GPT_TIM_1))
#define __Clear_Tick 		(timer_clear(GPT_TIM_1))

void keyboard_init();
int keyboardscan(void); 

uint8_t GetKeyVal(uint8_t keynum); 

#endif