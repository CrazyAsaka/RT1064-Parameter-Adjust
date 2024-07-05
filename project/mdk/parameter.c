/*
@ Author: 	Asaka
@ Time: 		2024/6/14
@ Version:	0.0
*/
#include "parameter.h"

Param* params_table[MAX_PARAMS];
uint16_t params_count;

Param Param_Create(Device device, const char *name, float value, float foot) {
    Param newParam;
    newParam.device = device;
    strncpy(newParam.name, name, MAX_NAME_LEN - 1);
    newParam.name[MAX_NAME_LEN - 1] = '\0'; /*Make sure the name is end with NULL*/
		newParam.code = params_count;
    newParam.value = value;
    newParam.foot = foot;
		params_table[params_count++] = &newParam;
    return newParam;
}

void Param_Transmit(Param param) {
    char buffer[50]; // Adjust buffer size as needed

    // Format the string according to the specified transmission format
    sprintf(buffer, "@%d,%g;$", param.code, param.value);

    // Send the formatted string via UART
    uart_write_string(TX_UART, buffer);
}

/*Parse string received from UART1*/ 
float parse_float(const char* str) {
    return strtof(str, NULL);
}

int Param_ParseInitStr(const uint8_t* buffer, Param** params, int* param_count) {
    if (buffer == NULL || params == NULL || param_count == NULL) {
        return 0;
    }

    int count = 0;
    char temp[10];
    int index = 0;

    if (buffer[index++] != '@') {
        return 0; // Invalid format
    }

    // Read device ID
    switch (buffer[index++]) {
        case '0':
            params[count]->device = RT1064;
            break;
        case '1':
            params[count]->device = MCX_Vision;
            break;
        case '2':
            params[count]->device = OpenART;
            break;
        default:
            return 0; // Invalid device ID
    }

    // Skip space after device ID
    if (buffer[index++] != ' ') {
        return 0; // Invalid format
    }

    // Start parsing parameters
    while (buffer[index] != '$') {
        // Read param value
        int code_value = count; // Assume code increments with count
        int code_index = 0;

        // Read param value
        while (buffer[index] != ';' && buffer[index] != '$') {
            temp[code_index++] = buffer[index++];
        }
        temp[code_index] = '\0';

        params[count]->code = code_value; // Assign code value
        params[count]->value = parse_float(temp);

        count++;

        if (buffer[index] == ';') {
            index++;
        }
    }

    *param_count = count;

    return 1; // Success
}
