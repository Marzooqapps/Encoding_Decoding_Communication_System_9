/**
 * @file display.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

void DecodedCharFifo_Init(void);
int DecodedCharFifo_Get(char *datapt);
int DecodedCharFifo_Put(char data);

void Display_Init(void);
void Display_PrintChar(char c);
void Display_Error(void);
void Display_ParityError(void);
void Display_Routine(void);
