/**
 * @file switches.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PF0   (*((volatile uint32_t *)0x40025004)) // Left Button (toggle)
#define PF4   (*((volatile uint32_t *)0x40025040)) // Right Button (menu)


void CharFifo_Init(void);
int CharFifo_Get(char *datapt);
int CharFifo_Put(char data);


void DelayWait10ms(uint32_t n);
bool Switches_CheckButton0(void);
bool Switches_CheckButton1(void);
void Swithces_Routine(void);
