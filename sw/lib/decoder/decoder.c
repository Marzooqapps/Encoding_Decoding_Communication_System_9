#include <stdint.h>

#include "./lib/decoder/decoder.h"
#include "./inc/FIFO.h"
#include "./inc/dsp.h"

AddIndexFifo(Sample,256,uint16_t,1,0);
uint8_t i;

void Decoder_Init(void) {
    SampleFifo_Init();
    DFT_Init();
    i = 0;
}

void Decoder_ISR(void) {
    uint16_t x;
    if(!SampleFifo_Get(&x)) {
        return;
    }
    DFT(i, x);
    i++;
    if(i == 16) {
        int32_t amp1 = Mag1();
        int32_t amp2 = Mag2();
        i = 0;
    }
}
