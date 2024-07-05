# RT1064调参GUI

大体逻辑：每次修改参数时，发送端发送参数给接收端；在初始化的时候接收端将所有参数的值发送给发送端

## 发送端

### 使用

#### 1.创建参数基本信息

```C
//  device:隶属于哪个设备，如RT1064,MCX_VISION,OPENART
//	name:参数名字，七个字符以内
//  value:初始值，可以设置，也可以通过串口自动初始化
//  foot：步长
Param Param_Create(Device device, const char *name, float value, float foot)
```

#### 2.页面创建

```C
// name:页面名字，显示在第一行
// index:序号，按需填写
Page* Page_Create(const char *name, page_size_t index)
```

#### 3.将参数添加到页面中

```C
// page:参数添加到哪个页面
// param:参数结构体
int Page_AddParams(Page *page, const Param *param)
```

#### 4.将page页面添加到遍历数组内

示例：

```C
Page *pages[DEVICE_NUM] = {page_RT1064, page_OpenART, page_MCX_Vision};
```

### Example

```C
/****************************** START REGISTER PARAMETERS ******************************/
/*参数注册表初始化。注: paramx中的x代表其code编码号（最好这样命名），且一定要按编码大小顺序初始化！！！*/
/*initialize parameters table*/
Param param0 = Param_Create(RT1064, "PARA1", 128, 10);		// 该参数编码为0
Param param1 = Param_Create(RT1064, "PARA2", 91, 5);
Param param2 = Param_Create(RT1064, "PARA3", 32, 1);
Param param3 = Param_Create(RT1064, "PARA4", 11.4, 0.1);
Param param4 = Param_Create(RT1064, "PARA5", 51.04, 0.01);

Param param5 = Param_Create(RT1064, "PARAM_1", 12, 10);
Param param6 = Param_Create(RT1064, "PARAM2", 9, 5);
Param param7 = Param_Create(RT1064, "PARA3", 32.0, 0.2);
Param param8 = Param_Create(RT1064, "PARA4", 4.05, 0.02);
Param param9 = Param_Create(RT1064, "PARA5", 1, 3);
Param param10 = Param_Create(RT1064, "PARA3", 32, 9);
Param param11 = Param_Create(RT1064, "PARA4", 4.9, 0.5);
Param param12 = Param_Create(RT1064, "PARA5", 5.07, 0.05);	// 该参数编码为12
/*create pages and add parameters to pages*/
/*创建页面，并将参数添加到页面上*/
Page *page_RT1064_0 = Page_Create("RT1064_0", 0);		/*创建一个名字叫RT1064_0的页面*/
Page_AddParams(page_RT1064_0, &param0);					/*向页面添加参数（一个页面最多添加12个参数）*/
Page_AddParams(page_RT1064_0, &param1);
Page_AddParams(page_RT1064_0, &param2);
Page_AddParams(page_RT1064_0, &param3);
Page_AddParams(page_RT1064_0, &param4);

Page *page_RT1064_1 = Page_Create("RT1064_1", 1);
Page_AddParams(page_RT1064_1, &param5);
Page_AddParams(page_RT1064_1, &param6);
Page_AddParams(page_RT1064_1, &param7);
Page_AddParams(page_RT1064_1, &param8);

Page *page_RT1064_2 = Page_Create("RT1064_2", 2);
Page_AddParams(page_RT1064_2, &param9);
Page_AddParams(page_RT1064_2, &param10);
Page_AddParams(page_RT1064_2, &param11);
Page_AddParams(page_RT1064_2, &param12);

#define PAGE_NUM 3		/* How many pages */
Page *pages[PAGE_NUM] = {page_RT1064_0, page_RT1064_1, page_RT1064_2};

/******************************* END REGISTER PARAMETERS  *******************************/
```

只需要在发送端例程上进行修改即可,完整main.c如下

```C
/*调参板子程序*/
#include "zf_common_headfile.h"
#include "page.h"
#include "keyboard.h"

#define MAX_BUFFER_LEN 65535
#define MAX_PARAMS_RCV 200

void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void uart_rcv_frame_handle(void);

lpuart_handle_t rcv_lpuartHandle;
lpuart_transfer_t rcv_transfer;
uint8_t rx_buffer[MAX_BUFFER_LEN];
uint8_t rx_char;

uint8_t kv;

Param params_rcv[MAX_PARAMS_RCV];
uint8_t param_count = 0;
int main()
{
	clock_init(SYSTEM_CLOCK_600M); 
	timer_init(GPT_TIM_1, TIMER_MS);
	timer_start(GPT_TIM_1);
	/*initialize screen*/
	Page_Init();
	
	/****************************** START REGISTER PARAMETERS ******************************/
	/*参数注册表初始化。注: paramx中的x代表其code编码号（最好这样命名），且一定要按编码大小顺序初始化！！！*/
	/*initialize parameters table*/
	Param param0 = Param_Create(RT1064, "PARA1", 128, 10);
	Param param1 = Param_Create(RT1064, "PARA2", 91, 5);
	Param param2 = Param_Create(RT1064, "PARA3", 32, 1);
	Param param3 = Param_Create(RT1064, "PARA4", 11.4, 0.1);
	Param param4 = Param_Create(RT1064, "PARA5", 51.04, 0.01);
	
	Param param5 = Param_Create(RT1064, "PARAM_1", 12, 10);
	Param param6 = Param_Create(RT1064, "PARAM2", 9, 5);
	Param param7 = Param_Create(RT1064, "PARA3", 32.0, 0.2);
	Param param8 = Param_Create(RT1064, "PARA4", 4.05, 0.02);

	Param param9 = Param_Create(RT1064, "PARA5", 1, 3);
	Param param10 = Param_Create(RT1064, "PARA3", 32, 9);
	Param param11 = Param_Create(RT1064, "PARA4", 4.9, 0.5);
	Param param12 = Param_Create(RT1064, "PARA5", 5.07, 0.05);

	/*create pages and add parameters to pages*/
	/*创建页面，并将参数添加到页面上*/
	Page *page_RT1064_0 = Page_Create("RT1064_0", 0);		/*创建一个名字叫RT1064_0的页面*/
	Page_AddParams(page_RT1064_0, &param0);							/*向页面添加参数（一个页面最多添加12个参数）*/
	Page_AddParams(page_RT1064_0, &param1);
	Page_AddParams(page_RT1064_0, &param2);
	Page_AddParams(page_RT1064_0, &param3);
	Page_AddParams(page_RT1064_0, &param4);
	
	Page *page_RT1064_1 = Page_Create("RT1064_1", 1);
	Page_AddParams(page_RT1064_1, &param5);
	Page_AddParams(page_RT1064_1, &param6);
	Page_AddParams(page_RT1064_1, &param7);
	Page_AddParams(page_RT1064_1, &param8);
	
	Page *page_RT1064_2 = Page_Create("RT1064_2", 2);
	Page_AddParams(page_RT1064_2, &param9);
	Page_AddParams(page_RT1064_2, &param10);
	Page_AddParams(page_RT1064_2, &param11);
	Page_AddParams(page_RT1064_2, &param12);

	#define PAGE_NUM 3		/* How many pages */
	Page *pages[PAGE_NUM] = {page_RT1064_0, page_RT1064_1, page_RT1064_2};
	
	/******************************* END REGISTER PARAMETERS  *******************************/
	
	uart_init(UART_1, 115200, UART1_TX_B12, UART1_RX_B13);
	uart_rx_interrupt(UART_1, 1);
	rcv_transfer.dataSize = 1;
	rcv_transfer.data = &rx_char;
	uart_set_handle(UART_1, &rcv_lpuartHandle, uart_callback, NULL, 0, rcv_transfer.data, 1);
	
	keyboard_init();
	
	while (1) {
		static int cur_page_idx = 0;
		Page_ShowParams(pages[cur_page_idx]);
		
		kv = keyboardscan();
		static uint16_t tick;
		/*move index*/
		if (kv == 2) {
			if (__Now_Tick - tick > 300) {		/*above 300ms*/
				pages[cur_page_idx]->paramIdxToBeEdited = (pages[cur_page_idx]->paramIdxToBeEdited + 1)
														% pages[cur_page_idx]->curPageParamsNum;
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		
		if (kv == 10) {
			if (__Now_Tick - tick > 300) {		/*above 300ms*/
				pages[cur_page_idx]->paramIdxToBeEdited = (pages[cur_page_idx]->paramIdxToBeEdited + pages[cur_page_idx]->curPageParamsNum - 1)% pages[cur_page_idx]->curPageParamsNum;
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		/*change pages*/
		if (kv == 1) {
			if (__Now_Tick - tick > 300) {		/*above 300ms*/
				cur_page_idx = (cur_page_idx + 1) % PAGE_NUM;
				
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		
		if (kv == 9) {
			if (__Now_Tick - tick > 300) {		/*above 300ms*/
				cur_page_idx = (cur_page_idx - 1 + PAGE_NUM) % PAGE_NUM;
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		/*edit parameter's value*/
		if (kv == 5) {
			if (__Now_Tick - tick > 200) {		/*above 200ms*/
				pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->value
				+= pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->foot;	/*increase value*/
				
				Param_Transmit(*pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]);
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		
		if (kv == 7) {
			if (__Now_Tick - tick > 200) {		/*above 200ms*/
				pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->value
				-= pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->foot;	/*decrease value*/
				
				Param_Transmit(*pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]);
				ips114_clear();
				tick = __Now_Tick;
			}
		}
		/*A constant period to reload timer*/
		if(__Now_Tick >= 60000) {
			__Clear_Tick;
		}
		
	}
	
	return 0;
}

void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData){
	static uint16_t count;
	static uint8_t rx_state;
	if(kStatus_LPUART_RxIdle == status) {
		if (rx_char == '@') {
			rx_state = 1;
		}
		else if (rx_char == '$') {		/*Successfully receive a UART frame*/
			rx_buffer[count++] = rx_char;
			uart_rcv_frame_handle();
			rx_state = 0;
			count = 0;
		}
		
		if (rx_state) {
			rx_buffer[count++] = rx_char;
			if (count >= MAX_BUFFER_LEN) {	/*Overload*/
				count = 0;
				rx_state = 0;
			}
		}
	}
	handle->rxDataSize = rcv_transfer.dataSize;  
	handle->rxData = rcv_transfer.data; 
}

void uart_rcv_frame_handle(void) {
	Param_ParseInitStr(rx_buffer, params_table, &param_count);
	for (int i = 0; i < param_count; i++) {
		printf("Parameter %d: Device ID=%d, Name=%s, Value=%.2f, Foot=%.2f\n",
           i, params_table[i]->device, params_table[i]->name, params_table[i]->value, params_table[i]->foot);
	}
}
```



### 注意事项

* 阶段1使用Param_Create创建参数的时候，parameter的编码是按照创建顺序编码的，第一个创建的编码是0，后面依次。因此在接收端建立参数注册表的时候**一定要按照Param_Create创建的顺序添加！！**

## 接收端

### 使用

#### 1.参数变量声明（支持Uint8_t类型和float类型）

例如：

```C
uint8_t param0 = 1;
uint8_t param1 = 244;
float param2 = 3.14;
float param3 = 114.4;
uint8_t param4 = 1;
uint8_t param5 = 4;
float param6 = 0.01;
float param7 = 1.07;
uint8_t param8 = 100;
uint8_t param9 = 41;
float param10 = 0.1;
float param11 = 0.07;
float param12 = 0.07;
```

#### 2. 建立参数注册表（一定要对应发送端的Param_Create的顺序添加！！！）

例如：

```C
// 建立包含指向变量的指针的数组
void *_params_table[] = {
		&param0, &param1, &param2, &param3, &param4, &param5,
		&param6, &param7, &param8, &param9, &param10,&param11,
		&param12
};
// 建立标识变量类型的数组
VarType _param_types[] = {
		TYPE_UINT8, TYPE_UINT8, TYPE_FLOAT, TYPE_FLOAT, TYPE_UINT8, TYPE_UINT8,
		TYPE_FLOAT, TYPE_FLOAT, TYPE_UINT8, TYPE_UINT8, TYPE_FLOAT, TYPE_FLOAT,
		TYPE_FLOAT
};
```

#### 3.在串口初始化结束后，将参数初始化值发送给发送方

例如：

```C
Param_Table_Transmit(0, _params_table, _param_types, ARRAY_SIZE(_params_table));
//其中0为设备信息（RT1064就写0），其余是刚刚创立的注册表信息
```

使用时只需要修改1、2步骤的数组元素对照表就行

#### 4.串口中断回调

见下方例程，使用时添加进去就行

#### 5.修改UART口

在paramdecoder.h里，将这三行修改为你自己的UART接口

```C
#define SEND_UART UART_1
#define SEND_UART_TX UART1_TX_B12
#define SEND_UART_RX UART1_RX_B13
```

### Example

完整例程如下

```C
#include "zf_common_headfile.h"
#include "param_decoder.h"

#define MAX_BUFFER_LEN 100

void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
void uart_rcv_frame_handle(void);

lpuart_handle_t rcv_lpuartHandle;
lpuart_transfer_t rcv_transfer;
uint8_t rx_buffer[MAX_BUFFER_LEN];
uint8_t rx_char;

/****************************************************************************************************/
/*建立参数注册表*/
/*make parameters table to be adjusted*/
uint8_t param0 = 1;
uint8_t param1 = 244;
float param2 = 3.14;
float param3 = 114.4;
uint8_t param4 = 1;
uint8_t param5 = 4;
float param6 = 0.01;
float param7 = 1.07;
uint8_t param8 = 100;
uint8_t param9 = 41;
float param10 = 0.1;
float param11 = 0.07;
float param12 = 0.07;

// 建立包含指向变量的指针的数组
void *_params_table[] = {
		&param0, &param1, &param2, &param3, &param4, &param5,
		&param6, &param7, &param8, &param9, &param10,&param11,
		&param12
};

// 建立标识变量类型的数组
VarType _param_types[] = {
		TYPE_UINT8, TYPE_UINT8, TYPE_FLOAT, TYPE_FLOAT, TYPE_UINT8, TYPE_UINT8,
		TYPE_FLOAT, TYPE_FLOAT, TYPE_UINT8, TYPE_UINT8, TYPE_FLOAT, TYPE_FLOAT,
		TYPE_FLOAT
};
/****************************************************************************************************/

int main()
{
	uart_init(SEND_UART, 115200, SEND_UART_TX, SEND_UART_RX);
	uart_rx_interrupt(SEND_UART, 1);
	rcv_transfer.dataSize = 1;
	rcv_transfer.data = &rx_char;
	uart_set_handle(SEND_UART, &rcv_lpuartHandle, uart_callback, NULL, 0, rcv_transfer.data, 1);
	
	Param_Table_Transmit(0, _params_table, _param_types, ARRAY_SIZE(_params_table));
	
	while (1) {
		
	}
	return 0;
}

void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData){
	static uint16_t count;
	static uint8_t rx_state;
	if(kStatus_LPUART_RxIdle == status) {
		if (rx_char == '@') {
			rx_state = 1;
		}
		else if (rx_char == '$') {		/*Successfully receive a UART frame*/
			rx_buffer[count++] = rx_char;
			uart_rcv_frame_handle();
			rx_state = 0;
			count = 0;
		}
		
		if (rx_state) {
			rx_buffer[count++] = rx_char;
			if (count >= MAX_BUFFER_LEN) {	/*Overload*/
				count = 0;
				rx_state = 0;
			}
		}
	}
	handle->rxDataSize = rcv_transfer.dataSize;  
	handle->rxData = rcv_transfer.data; 
}

ParamData rcvParam;
void uart_rcv_frame_handle(void) {
	rcvParam = Parse_Param_String(rx_buffer);
	if(_param_types[rcvParam.code] == TYPE_UINT8) {
		*(uint8_t *)_params_table[rcvParam.code] = (uint8_t)rcvParam.value;
		printf("after edited:\nparam code:%d, param value:%d\n", rcvParam.code, *(uint8_t *)_params_table[rcvParam.code]);
	} else if(_param_types[rcvParam.code] == 	TYPE_FLOAT) {
		*(float *)_params_table[rcvParam.code] = (float)rcvParam.value;
		printf("after edited:\nparam code:%d, param value:%.2f\n", rcvParam.code, *(float *)_params_table[rcvParam.code]);
	}
}
```

### 注意事项

* 上电时，**一定要发送方（调参的RT1064）先上电**，不然发送方接收不到来自接收方的参数初始化信息

## 发送端按键操作

key5: 参数 + 1*foot

key7: 参数 - 1*foot

key2: 指向下一个参数（不切换页面）

key10: 指向上一个参数（不切换页面）

key1: 下一个页面

key9: 上一个页面
