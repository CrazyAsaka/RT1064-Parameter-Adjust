///*调参板子程序*/
//#include "zf_common_headfile.h"
//#include "page.h"
//#include "keyboard.h"

//#define MAX_BUFFER_LEN 65535
//#define MAX_PARAMS_RCV 200

//void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);
//void uart_rcv_frame_handle(void);

//lpuart_handle_t rcv_lpuartHandle;
//lpuart_transfer_t rcv_transfer;
//uint8_t rx_buffer[MAX_BUFFER_LEN];
//uint8_t rx_char;

//uint8_t kv;

//Param params_rcv[MAX_PARAMS_RCV];
//uint8_t param_count = 0;
//int main()
//{
//	clock_init(SYSTEM_CLOCK_600M); 
//	timer_init(GPT_TIM_1, TIMER_MS);
//	timer_start(GPT_TIM_1);
//	/*initialize screen*/
//	Page_Init();
//	
//	/****************************** START REGISTER PARAMETERS ******************************/
//	/*参数注册表初始化。注: paramx中的x代表其code编码号（最好这样命名），且一定要按编码大小顺序初始化！！！*/
//	/*initialize parameters table*/
//	Param param0 = Param_Create(RT1064, "PARA1", 128, 10);
//	Param param1 = Param_Create(RT1064, "PARA2", 91, 5);
//	Param param2 = Param_Create(RT1064, "PARA3", 32, 1);
//	Param param3 = Param_Create(RT1064, "PARA4", 11.4, 0.1);
//	Param param4 = Param_Create(RT1064, "PARA5", 51.04, 0.01);
//	
//	Param param5 = Param_Create(RT1064, "PARAM_1", 12, 10);
//	Param param6 = Param_Create(RT1064, "PARAM2", 9, 5);
//	Param param7 = Param_Create(RT1064, "PARA3", 32.0, 0.2);
//	Param param8 = Param_Create(RT1064, "PARA4", 4.05, 0.02);

//	Param param9 = Param_Create(RT1064, "PARA5", 1, 3);
//	Param param10 = Param_Create(RT1064, "PARA3", 32, 9);
//	Param param11 = Param_Create(RT1064, "PARA4", 4.9, 0.5);
//	Param param12 = Param_Create(RT1064, "PARA5", 5.07, 0.05);

//	/*create pages and add parameters to pages*/
//	/*创建页面，并将参数添加到页面上*/
//	Page *page_RT1064_0 = Page_Create("RT1064_0", 0);		/*创建一个名字叫RT1064_0的页面*/
//	Page_AddParams(page_RT1064_0, &param0);							/*向页面添加参数（一个页面最多添加12个参数）*/
//	Page_AddParams(page_RT1064_0, &param1);
//	Page_AddParams(page_RT1064_0, &param2);
//	Page_AddParams(page_RT1064_0, &param3);
//	Page_AddParams(page_RT1064_0, &param4);
//	
//	Page *page_RT1064_1 = Page_Create("RT1064_1", 1);
//	Page_AddParams(page_RT1064_1, &param5);
//	Page_AddParams(page_RT1064_1, &param6);
//	Page_AddParams(page_RT1064_1, &param7);
//	Page_AddParams(page_RT1064_1, &param8);
//	
//	Page *page_RT1064_2 = Page_Create("RT1064_2", 2);
//	Page_AddParams(page_RT1064_2, &param9);
//	Page_AddParams(page_RT1064_2, &param10);
//	Page_AddParams(page_RT1064_2, &param11);
//	Page_AddParams(page_RT1064_2, &param12);

//	#define PAGE_NUM 3		/* How many pages */
//	Page *pages[PAGE_NUM] = {page_RT1064_0, page_RT1064_1, page_RT1064_2};
//	
//	/******************************* END REGISTER PARAMETERS  *******************************/
//	
//	uart_init(UART_1, 115200, UART1_TX_B12, UART1_RX_B13);
//	uart_rx_interrupt(UART_1, 1);
//	rcv_transfer.dataSize = 1;
//	rcv_transfer.data = &rx_char;
//	uart_set_handle(UART_1, &rcv_lpuartHandle, uart_callback, NULL, 0, rcv_transfer.data, 1);
//	
//	keyboard_init();
//	
//	while (1) {
//		static int cur_page_idx = 0;
//		Page_ShowParams(pages[cur_page_idx]);
//		
//		kv = keyboardscan();
//		static uint16_t tick;
//		/*move index*/
//		if (kv == 2) {
//			if (__Now_Tick - tick > 300) {		/*above 300ms*/
//				pages[cur_page_idx]->paramIdxToBeEdited = (pages[cur_page_idx]->paramIdxToBeEdited + 1)
//																								% pages[cur_page_idx]->curPageParamsNum;
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		
//		if (kv == 10) {
//			if (__Now_Tick - tick > 300) {		/*above 300ms*/
//				pages[cur_page_idx]->paramIdxToBeEdited = (pages[cur_page_idx]->paramIdxToBeEdited
//																								+ pages[cur_page_idx]->curPageParamsNum - 1)
//																								% pages[cur_page_idx]->curPageParamsNum;
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		/*change pages*/
//		if (kv == 1) {
//			if (__Now_Tick - tick > 300) {		/*above 300ms*/
//				cur_page_idx = (cur_page_idx + 1) % PAGE_NUM;
//				
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		
//		if (kv == 9) {
//			if (__Now_Tick - tick > 300) {		/*above 300ms*/
//				cur_page_idx = (cur_page_idx - 1 + PAGE_NUM) % PAGE_NUM;
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		/*edit parameter's value*/
//		if (kv == 5) {
//			if (__Now_Tick - tick > 200) {		/*above 200ms*/
//				pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->value
//				+= pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->foot;	/*increase value*/
//				
//				Param_Transmit(*pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]);
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		
//		if (kv == 7) {
//			if (__Now_Tick - tick > 200) {		/*above 200ms*/
//				pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->value
//				-= pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]->foot;	/*decrease value*/
//				
//				Param_Transmit(*pages[cur_page_idx]->params[pages[cur_page_idx]->paramIdxToBeEdited]);
//				ips114_clear();
//				tick = __Now_Tick;
//			}
//		}
//		/*A constant period to reload timer*/
//		if(__Now_Tick >= 60000) {
//			__Clear_Tick;
//		}
//		
//	}
//	
//	return 0;
//}

//void uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData){
//	static uint16_t count;
//	static uint8_t rx_state;
//	if(kStatus_LPUART_RxIdle == status) {
//		if (rx_char == '@') {
//			rx_state = 1;
//		}
//		else if (rx_char == '$') {		/*Successfully receive a UART frame*/
//			rx_buffer[count++] = rx_char;
//			uart_rcv_frame_handle();
//			rx_state = 0;
//			count = 0;
//		}
//		
//		if (rx_state) {
//			rx_buffer[count++] = rx_char;
//			if (count >= MAX_BUFFER_LEN) {	/*Overload*/
//				count = 0;
//				rx_state = 0;
//			}
//		}
//	}
//	handle->rxDataSize = rcv_transfer.dataSize;  
//	handle->rxData = rcv_transfer.data; 
//}

//void uart_rcv_frame_handle(void) {
//	Param_ParseInitStr(rx_buffer, params_table, &param_count);
//	for (int i = 0; i < param_count; i++) {
//		printf("Parameter %d: Device ID=%d, Name=%s, Value=%.2f, Foot=%.2f\n",
//           i, params_table[i]->device, params_table[i]->name, params_table[i]->value, params_table[i]->foot);
//	}
//}


/*控制主板Example*/
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
