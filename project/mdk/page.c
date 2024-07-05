/*
@ Author: 	Asaka
@ Time: 		2024/6/14
@ Version:	0.0
*/
#include "page.h"

/*********************************************************************/
/*************************** API Functions ***************************/
void Page_Init(void) {
	ips114_init();
	ips114_set_color(TEXT_COLOR, BG_COLOR);
}

Page* Page_Create(const char *name, page_size_t index) {
    Page *page = (Page *)malloc(sizeof(Page));
    if (page == NULL) {
        perror("Failed to allocate memory for new Page");
        return NULL;
    }

    strncpy(page->pageName, name, MAX_PAGE_NAME - 1);
    page->pageName[MAX_PAGE_NAME - 1] = '\0'; // 确保字符串以null结尾
    page->index = index;
    page->curPageParamsNum = 0;
		page->paramIdxToBeEdited = 0;
    for (int i = 0; i < MAX_PARAMS_PER_PAGE; ++i) {
        page->params[i] = NULL;
    }

    return page;
}

int Page_AddParams(Page *page, const Param *param) {
    if (page == NULL || page->params == NULL) {
        return -1; /*Page or Params is NULL*/
    }
    if (page->curPageParamsNum >= MAX_PARAMS_PER_PAGE) {
        return -2; /*Param numbers overflow*/
    }
		
    page->params[page->curPageParamsNum] = param;
    page->curPageParamsNum++;
    return 0;	/*Success*/
}

void Page_ShowParams(const Page *page) {
	char pParam[(int)(0.5*MAX_WORDS_PER_LINE)];
	ips114_show_string(0,0,page->pageName);
	for (int i=0;i<page->curPageParamsNum;i++) {
	 	Param *param = (page->params[i]);
		sprintf(pParam, "%s:%.2f\0", param->name, param->value);
		screen_size_t st_x, st_y;
		if (i % 2 == 1) {		/*the left param*/
			st_x = (MAX_WORDS_PER_LINE / 2) * WORD_W;
		} else {
			st_x = 0;
		}
		st_y = (i / 2+1) * WORD_H;
		ips114_show_string(st_x, st_y, pParam);
	}
	
	uint8_t line_st_x, line_st_y, line_ed_x, line_ed_y;
	line_st_x = (page->paramIdxToBeEdited % 2) * SCREEN_W / 2;
	line_st_y = (page->paramIdxToBeEdited / 2 + 2) * WORD_H - 1;
	line_ed_x = line_st_x + MAX_PARAM_LENGTH-1;
	line_ed_y = line_st_y;
	ips114_draw_line(line_st_x, line_st_y, line_ed_x, line_ed_y, TEXT_COLOR);
}


