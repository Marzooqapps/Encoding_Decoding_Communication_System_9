#include <stdint.h>
#include <stdbool.h>

#include "./lib/decoder/decoder.h"
#include "./inc/FIFO.h"
#include "./inc/dsp.h"
#include "./lib/decoder/adc/adc.h"

#define SOUND_THRESHOLD 3131
#define ADC_PERIOD 31313131
#define CONVERSION_PERIOD 31313131
#define DECODE_PERIOD 31313131

AddIndexFifo(DecodedBit,256,int,1,0);  // -1 means off, 0 means lowfreq, 1 means highfreq

uint8_t i;
bool decoding_msg;
uint8_t data_bits_acquired;
uint8_t data;
bool parity;
bool parity_checked;

void Decoder_Init(void) {
    DFT_Init();
    i = 0;
    decoding_msg = false;
    data_bits_acquired = 0;
    data = 0;
    parity = 0;
    parity_checked = false;

    ADC0_InitTimer0ATriggerSeq3PD3(ADC_PERIOD);
    // TODO: arm Decoder_ConversionISR with period CONVERSION_PERIOD
    // TODO: arm Decoder_DecodeISR with period DECODE_PERIOD
}

void Decoder_ConversionISR(void) {  // Converts output of ADC to tristate value (off/lowfreq/highfreq)
    uint32_t x;
    if(!SampleFifo_Get(&x)) {
        return;
    }
    if(x < SOUND_THRESHOLD) {
        while(!DecodedBitFifo_Put(-1));
        return;
    }
    DFT(i, x);
    i++;
    if(i == 16) {
        int32_t amp1 = Mag1();
        int32_t amp2 = Mag2();
        i = 0;

        if(amp1 > amp2) {  // low freq
            while(!DecodedBitFifo_Put(0));
        } else {  // high freq
            while(!DecodedBitFifo_Put(1));
        }
    }
}


void Decoder_DecodeISR(void) { // Converts sequence of bits to character
    int bit;
    if(!DecodedBitFifo_Get(&bit)) {
        return;
    }
    if(bit == -1) {
        if(decoding_msg) {  // something went wrong, the whole message couldn't be decoded
            // TODO: implement display(error message)
            data = 0;
            decoding_msg = false;
            data_bits_acquired = 0;
            parity = 0;
        }
        return;
    }

    if(bit == 0 && !decoding_msg) {  // start bit
        decoding_msg = true;
        return;
    }
    if(bit == 1 && parity_checked) {  // stop bit
        // TODO: implement graphics_output((char) data);
        decoding_msg = false;
        data_bits_acquired = 0;
        data = 0;
        parity = 0;
        parity_checked = false;
        return;
    }
    if(data_bits_acquired == 8 && !parity_checked) {  // parity bit
        if(parity ^ bit) {  // parity check failure
            // TODO: implement display(error message)
        }
        parity_checked = true;
        return;
    }

    // at this point, the current bit should be a data bit
    data_bits_acquired++;
    data = (data << 1) + bit;
    parity ^= bit;
}













