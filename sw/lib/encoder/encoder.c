#include <stdint.h>
#include <stdbool.h>

#include "./lib/encoder/encoder.h"
#include "./inc/FIFO.h"
#include "./inc/DAC.h"

AddIndexFifo(Char,8,char,1,0);
AddIndexFifo(Bit,88,bool,1,0);

void Encoder_Init(void) {
    CharFifo_Init();
    BitFifo_Init();
    dac_init();
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

    if(bit) {
        dac_setLowFreq();
    } else {
        dac_setHighFreq();
    }

}
