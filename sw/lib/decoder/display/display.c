/**
 * @file display.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "./lib/decoder/display/display.h"
#include "./inc/ST7735.h"
#include "./inc/FIFO.h"

int16_t CursorX;
int16_t CursorY;

AddIndexFifo(DecodedChar,16,char,1,0);


void Display_Init(void) {
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0, 0);

    DecodedCharFifo_Init();

    CursorX = 0;
    CursorY = 0;
}

void Display_PrintChar(char c) {
    ST7735_DrawChar(CursorX, CursorY, c, ST7735_GREEN, ST7735_BLACK, 1);
    CursorX += 5;
    if(CursorX >= 128) {
        CursorY += 8;
        CursorX = 0;
    }
}

void Display_Error(void) {
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawString(0, 0, "error", ST7735_GREEN);

    CursorX = 0;
    CursorY = 0;
    while(1);
}

void Display_ParityError(void) {
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_DrawString(0, 0, "parity error", ST7735_GREEN);

    CursorX = 0;
    CursorY = 0;
    while(1);
}

void Display_Routine(void) {
    char c;
    if(!DecodedCharFifo_Get(&c)) {
        return;
    }
    Display_PrintChar(c);
}
