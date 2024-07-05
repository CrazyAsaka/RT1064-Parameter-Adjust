/*
@ Author: 	Asaka
@ Time: 		2024/6/14
@ Version:	0.0
*/

/*
UART frame:
接受参数初始化数据帧格式: "@{device ID} {param0 value};{param1 value};...{paramx value};$"
其中， 
device ID:设备ID，RT1064:0;MCX_VISION:1;OPENART:2;(注意设备ID后加一个空格)
value: 参数值
				
例如: "@0 3.14;255;0.19$"
表明Device0（RT1064）的参数0为3.14，参数1为255，参数2为0.19

发送格式: "@{param0 code},{param0 value};$"
例如: "@0,3.14;$"
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <zf_common_headfile.h>

#define TX_UART UART_1

#define MAX_NAME_LEN 10

#define MAX_PARAMS 300

#define MAX_TX_BUFFER_LEN 100

typedef enum Device{
	RT1064 = 0,
	MCX_Vision,
	OpenART
}Device;

typedef struct Param{
	Device device;
	char name[MAX_NAME_LEN];
	uint8_t code;
	float value;
	float foot;
}Param;

extern Param* params_table[MAX_PARAMS];
extern uint16_t params_count;

Param Param_Create(Device device, const char *name, float value, float foot);
/*Advanced API Function*/
int Param_ParseInitStr(const uint8_t* buffer, Param** params, int* param_count);
void Param_Transmit(Param param);

#endif
