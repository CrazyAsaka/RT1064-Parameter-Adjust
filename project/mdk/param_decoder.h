/*
@ Author: 	Asaka
@ Time: 		2024/7/5
@ Version:	0.0
*/

#ifndef PARAM_DECODER_H
#define PARAM_DECODER_H

#define SEND_UART UART_1
#define SEND_UART_TX UART1_TX_B12
#define SEND_UART_RX UART1_RX_B13

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
	
#include "zf_common_headfile.h"

typedef struct {
  uint8_t code;
  float value;
}ParamData;

typedef enum {
    TYPE_UINT8,
    TYPE_FLOAT
} VarType;

ParamData Parse_Param_String(const char* buffer);
void Param_Table_Transmit(uint8_t device_id, void **params_table, VarType *param_types, int param_count);

#endif
