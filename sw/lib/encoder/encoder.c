#include <stdint.h>
#include <stdbool.h>

#include "./lib/encoder/encoder.h"
#include "./inc/FIFO.h"
#include "./inc/DAC.h"
#include "./lib/encoder/switches/switches.h"
#include "inc/Timer2A.h"
#include "inc/Timer5A.h"
#include "inc/Timer1A.h"

#define ENCODER_PROCESSCHAR_PERIOD 1600000
#define ENCODER_PROCESSBIT_PERIOD 8000000
#define DAC_PERIOD 1250

AddIndexFifo(Bit,88,bool,1,0);

void Encoder_Init(void) {
    BitFifo_Init();
    dac_init();
    Switches_Init();

    Timer1A_Init(&Encoder_ProcessChar, ENCODER_PROCESSCHAR_PERIOD, 3);

    Timer5A_Init(&Encoder_ProcessBit, ENCODER_PROCESSBIT_PERIOD, 4);

    Timer2A_Init(&dac_ISR, DAC_PERIOD, 5);

}

void Encoder_ProcessChar(void) {
    char c;
    if(!CharFifo_Get(&c)) {
        return;
    }

    uint8_t mask = 0x80;
    uint8_t parity = 0;

    while(!BitFifo_Put(0));  // start bit
    while(mask) {
        bool bit = c & mask;
        parity ^= bit;
        mask >>= 1;
        while(!BitFifo_Put(bit));
    }
    while(!BitFifo_Put(parity));
    while(!BitFifo_Put(1));  // stop bit
}

void Encoder_ProcessBit(void) {
    bool bit;
    if(!BitFifo_Get(&bit)) {
        dac_off();
        return;
    }

    if(!bit) {
        dac_setLowFreq();
    } else {
        dac_setHighFreq();
    }

}

void Encoder_Test(int option) {  // -1 means off, 0 means lowfreq, 1 means highfreq
    if(option == -1) {
        dac_off();
    } else if(option == 0) {
        dac_setLowFreq();
    } else if(option == 1) {
        dac_setHighFreq();
    }
}
