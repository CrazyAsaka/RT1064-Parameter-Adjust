/*
@ Author: 	Asaka
@ Time: 		2024/6/14
@ Version:	0.0
*/

#ifndef PAGE_H
#define PAGE_H

/*---------------------------------------*/
/*---------------Page Name---------------*/
/*{PARAM_1}:  {Value}|{PARAM_2}:  {Value}*/
/*{PARAM_3}:  {Value}|{PARAM_4}:  {Value}*/
/*{PARAM_5}:  {Value}|{PARAM_6}:  {Value}*/
/*{PARAM_7}:  {Value}|{PARAM_8}:  {Value}*/
/*{PARAM_9}:  {Value}|{PARAM_10}: {Value}*/
/*{PARAM_11}: {Value}|{PARAM_12}: {Value}*/
/*---------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include "parameter.h"

/*LCD Interface Include*/
#include "zf_common_headfile.h"

#define TEXT_COLOR RGB565_BLACK
#define BG_COLOR RGB565_WHITE

/*The max pages that every device can support*/
#define MAX_PARAMS_PER_DEVICE 100

#define MAX_PAGE_NAME 12
#define MAX_PARAMS_PER_DEVICE 100

#define SCREEN_H 135
#define SCREEN_W 240

#define WORD_H 18
#define WORD_W 8

#define MAX_PARAM_PER_LINE 2
#define MAX_PARAM_LENGTH (SCREEN_W / MAX_PARAM_PER_LINE)
#define MAX_LINES ((int)(SCREEN_H / WORD_H))		// About 7 lines max
#define MAX_WORDS_PER_LINE ((int)(SCREEN_W / WORD_W))
#define MAX_PARAMS_PER_PAGE ((MAX_LINES-1) * 2)
#define MAX_PAGES_PER_DEVICE (MAX_PARAMS_PER_DEVICE / MAX_PARAMS_PER_PAGE + 1)

typedef uint16_t screen_size_t;
typedef uint8_t page_size_t;
typedef uint8_t param_size_t;


typedef struct Page{
	char pageName[MAX_PAGE_NAME];
	Device device;													/*belong to which device*/
	page_size_t index;											/*page index*/
	Param *params[ MAX_PARAMS_PER_PAGE];			/*parameters*/
	param_size_t curPageParamsNum;					/*current parameterS NUM*/
	param_size_t paramIdxToBeEdited;
}Page;

typedef struct Device_t {
	Page *pages[MAX_PAGES_PER_DEVICE];
	
}Device_t;

/*high level function*/
void pages_params_register(void);
/*Screen Interface Function*/

/*Page Functions*/
void Page_Init(void);
Page* Page_Create(const char *name, page_size_t index);
int Page_AddParams(Page *page, const Param *param);
/*Display Parameters*/
void Page_ShowParams(const Page *page);

#endif
