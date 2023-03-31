/**
 * @file switches.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdbool.h>
#include <stdint.h>

#include "./lib/encoder/switches/switches.h"
#include "./inc/FIFO.h"

AddIndexFifo(Char,8,char,1,0);


// PF0 is bit 0
// PF4 is bit 1

uint8_t data;
uint8_t data_bits_acquired;

void DelayWait10ms(uint32_t n) {
    uint32_t volatile time;
    while (n){
        time = 727240 * 2 / 91;  // 10msec
        while (time){
            --time;
        }
        --n;
    }
}


void Switches_Init(void) {
    data = 0;
    data_bits_acquired = 0;

    CharFifo_Init();
}


bool Switches_CheckButton0(void) {
    if(PF0 == 0x00 && PF4 == 0x10) {
        while(PF0 == 0x00) {
            if(PF4 == 0x00){
                return 0;
            }
            DelayWait10ms(10);
        }
        return 1;
    }
    return 0;
}


bool Switches_CheckButton1(void) {
    if(PF4 == 0x00 && PF0 == 0x01) {
        while(PF4 == 0x00) {
            if(PF0 == 0x00) {
                return 0;
            }
            DelayWait10ms(10);
        }
        return 1;
    }
    return 0;
}

void Swithces_Routine(void) {
    if(Switches_CheckButton0()) {
        data <<= 1;
        data_bits_acquired++;
        if(data_bits_acquired == 8) {
            data_bits_acquired = 0;
            while(!CharFifo_Put((char) data));
            data = 0;
        }
    } else if(Switches_CheckButton1()) {
        data <<= 1;
        data++;
        data_bits_acquired++;
        if(data_bits_acquired == 8) {
            data_bits_acquired = 0;
            while(!CharFifo_Put((char) data));
            data = 0;
        }
    }
}
