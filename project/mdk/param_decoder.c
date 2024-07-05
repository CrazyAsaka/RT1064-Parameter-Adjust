/*
@ Author: 	Asaka
@ Time: 		2024/7/5
@ Version:	0.0
*/
#include "param_decoder.h"


ParamData Parse_Param_String(const char* buffer) {
    ParamData param_data = {0, 0.0};

    // Check for valid start of frame
    if (buffer[0] != '@') {
        printf("Invalid frame start.\n");
        return param_data;
    }

    // Find code and value within the buffer
    char* token = strtok((char*)buffer + 1, ",;");
    if (token == NULL) {
        printf("Invalid format.\n");
        return param_data;
    }

    param_data.code = atoi(token);

    token = strtok(NULL, ",;$");
    if (token == NULL) {
        printf("Invalid format.\n");
        return param_data;
    }

    param_data.value = atof(token);

    return param_data;
}

void Param_Table_Transmit(uint8_t device_id, void **params_table, VarType *param_types, int param_count) {
    // ������Ҫ�Ļ�������С
    int buffer_size = 10 + param_count * 20; // ����ÿ���������ռ��20���ַ�

    // ���仺����
    char *buffer = (char *)malloc(buffer_size);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return;
    }

    // ���������ַ���
    int index = snprintf(buffer, buffer_size, "@%d ", device_id);

    for (int i = 0; i < param_count; ++i) {
        if (i > 0) {
            index += snprintf(buffer + index, buffer_size - index, ";");
        }
        if (param_types[i] == TYPE_UINT8) {
            index += snprintf(buffer + index, buffer_size - index, "%d", *(uint8_t *)params_table[i]);
        } else if (param_types[i] == TYPE_FLOAT) {
            index += snprintf(buffer + index, buffer_size - index, "%.2f", *(float *)params_table[i]);
        }
    }

    snprintf(buffer + index, buffer_size - index, ";$");
    // ͨ�� UART ����
    uart_write_string(SEND_UART, buffer);
    // �ͷŻ�����
    free(buffer);
}
