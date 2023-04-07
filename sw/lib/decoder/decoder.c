#include <stdint.h>
#include <stdbool.h>

#include "./lib/decoder/decoder.h"
#include "./inc/FIFO.h"
#include "./inc/dsp.h"
#include "./lib/decoder/adc/adc.h"
#include "./lib/decoder/display/display.h"
#include "./inc/Timer1A.h"
#include "./inc/Timer4A.h"
#include "SoundlessTest.h"


#define SOUND_THRESHOLD 3131
#if SOUNDLESS_TEST
    #define SOUND_THRESHOLD 0
#endif
#define ADC_PERIOD 5000
#define ADC_FREQ 16000
#define CONVERSION_PERIOD 5000
#define DECODE_PERIOD 80000

AddIndexFifo(DecodedBit,256,int,1,0);  // -1 means off, 0 means lowfreq, 1 means highfreq

uint8_t i;
bool decoding_msg;
uint8_t data_bits_acquired;
uint8_t data;
bool parity;
bool parity_checked;
int cooldown;

int32_t Dump1[512];
int32_t Dump2[512];
int DumpIndex1;
int DumpIndex2;

uint32_t RawDump1[512];
int RawDumpIndex1;

extern uint32_t SOUND;

void Decoder_Init(void) {
    i = 0;
    decoding_msg = false;
    data_bits_acquired = 0;
    data = 0;
    parity = 0;
    parity_checked = false;
    cooldown = 0;
    DumpIndex1 = 0;
    DumpIndex2 = 0;

    DecodedBitFifo_Init();

    DFT_Init();
    Display_Init();

//    ADC0_InitTimer0ATriggerSeq3PD3(ADC_PERIOD);
    ADC0_InitTimer0ATriggerSeq0(0, ADC_FREQ, &Decoder_ConversionISR);

    // TODO: arm Decoder_ConversionISR with period CONVERSION_PERIOD
//    Timer1A_Init(&Decoder_ConversionISR, CONVERSION_PERIOD, 3);

    // TODO: arm Decoder_DecodeISR with period DECODE_PERIOD
    Timer4A_Init(&Decoder_DecodeISR, DECODE_PERIOD, 3);




}

void Decoder_ConversionISR(uint32_t data) {  // Converts output of ADC to tristate value (off/lowfreq/highfreq)
    #if SOUNDLESS_TEST
        uint32_t x = SOUND;
    #else
        uint32_t x = data;
    #endif


//    if(x < SOUND_THRESHOLD) {
//        return;
//    }
    DFT(i, x);
    i++;
    if(i == 16) {
        int32_t amp1 = Mag1();
        int32_t amp2 = Mag2();
        i = 0;

        if(cooldown) {
            cooldown--;
            return;
        }

        if(amp1 - amp2 > 10000000) {  // low freq
            while(!DecodedBitFifo_Put(0));
            cooldown = 9;
        } else if(amp2 - amp1 > 10000000){  // high freq
            while(!DecodedBitFifo_Put(1));
            cooldown = 9;
        }
    }
}


void Decoder_DecodeISR(void) { // Converts sequence of bits to character
    int bit;
    if(!DecodedBitFifo_Get(&bit)) {
        return;
    }
//    if(bit == -1) {
//        if(decoding_msg) {  // something went wrong, the whole message couldn't be decoded
//            Display_Error();
//            data = 0;
//            decoding_msg = false;
//            data_bits_acquired = 0;
//            parity = 0;
//        }
//        return;
//    }

    if(bit == 0 && !decoding_msg) {  // start bit
        decoding_msg = true;
        return;
    }
    if(bit == 1 && parity_checked) {  // stop bit
        decoding_msg = false;
        data_bits_acquired = 0;
        parity = 0;
        parity_checked = false;
        while(!DecodedCharFifo_Put((char) data));
        data = 0;
        return;
    }
    if(data_bits_acquired == 8 && !parity_checked) {  // parity bit
        if(parity ^ bit) {  // parity check failure
            Display_ParityError();
        }
        parity_checked = true;
        return;
    }

    // at this point, the current bit should be a data bit
    data_bits_acquired++;
    data = (data << 1) + bit;
    parity ^= bit;
}

void Decoder_Test(void) {
    // When running this, adc should be armed

    uint32_t x;
    if(!SampleFifo_Get(&x)) {
        return;
    }

    if(RawDumpIndex1 < 512) {
        RawDump1[RawDumpIndex1] = x;
        RawDumpIndex1++;
    }

    DFT(i, x);
    i++;
    if(i == 16 && DumpIndex1 < 512) {
        int32_t amp1 = Mag1();
        int32_t amp2 = Mag2();
        i = 0;

        Dump1[DumpIndex1] = amp1;
        Dump2[DumpIndex2] = amp2;
        DumpIndex1 = (DumpIndex1 + 1);
        DumpIndex2 = (DumpIndex2 + 1);
    }

    int64_t avg_1 = 0;
    int64_t avg_2 = 0;
}












