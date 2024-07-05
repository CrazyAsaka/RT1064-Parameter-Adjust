#include "keyboard.h"

gpio_pin_enum pin_in[N] = {
	D0, D1, D2, D3
};
gpio_pin_enum pin_out[N] = {
	D12, D13, D14, D15
};

static uint16_t keyval = 0;

void keyboard_init() {
	for (int i=0; i<4; i++) {
		gpio_init(pin_out[i], GPO, 0, GPO_PUSH_PULL);
	}
	for (int i=0; i<4; i++) {
		gpio_init(pin_in[i], GPI, 0, GPI_PULL_DOWN);
	}
	  
}

/**
 * @brief 扫描矩阵键盘并将结果储存在一个静态的16位数据中
 * 
 */
int keyboardscan(void)
{
    uint16_t __keyval = 0; //用一个16位的数据来表示16个按键
    for (int j = 0; j < 4; j++) //按序读取按键值
    {
//        HAL_GPIO_WritePin(__PinOutput[j].PinPort, __PinOutput[j].Pinnum, GPIO_PIN_SET);
			gpio_set_level(pin_out[j], 1);
      for (int k = 0; k < 4; k++)
//				__keyval += HAL_GPIO_ReadPin(__PinInput[k].PinPort, __PinInput[k].Pinnum) << (j + k * 4);
				__keyval += gpio_get_level(pin_in[k]) << (j + k * 4);
//      HAL_GPIO_WritePin(__PinOutput[j].PinPort, __PinOutput[j].Pinnum, GPIO_PIN_RESET);
			gpio_set_level(pin_out[j], 0);
    }
    keyval = __keyval;

    for(int i = 0;i<16;i++)
    {
        if(GetKeyVal(i))
            return i+1;
    }
    return 0;
}

/**
 * @brief 获取按键值
 * 
 * @param keynum 按键序号，从0到15
 * @return 1表示按下，0表示松开
 */
uint8_t GetKeyVal(uint8_t keynum)
{
    return (keyval & (1 << keynum)) >> keynum;
}

