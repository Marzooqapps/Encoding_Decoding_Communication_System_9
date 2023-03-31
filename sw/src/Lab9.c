/**
 * @file Lab9.c
 * @author your name (you@domain.com), Jonathan Valvano, Matthew Yu, Jared McArthur
 *    <TA NAME and LAB SECTION # HERE>
 * @brief
 *    Possible main programs to run Lab 9. Feel free to edit this to match your
 *    specifications.
 * 
 *    For this lab, the students must implement the following communication pipeline:
 *     - Input -> Encoder -> DAC -> Speaker -> AIR GAP -> Microphone -> ADC ->
 *       Decoder -> Display.
 *     Teams are allowed to choose an encoding scheme to transmit their
 *     information over the speaker. They need to be able to discuss and profile
 *     the execution latency, bandwidth, and SNR of their implementation.
 * @version 0.2
 * @date 2023-03-28
 * 
 * @copyright Copyright (c) 2022
 */

/**
 * @note Modify __MAIN__ on L28 to determine which main method is executed.
 *   __MAIN__ = 0 or ENCODER - run the encoder main
 *              1 or DECODER - run the decoder main
 */


/*
encoding:
1 start bit: 0
8 data bits: xxxxxxxx
1 parity bit: x
1 stop bit: 1

total: 11 bits per character
 */

#define ENCODER 0
#define DECODER 1
#define __MAIN__ ENCODER

#include "./inc/PLL.h"

/* Include relevant files for the encoder depending on the value of __MAIN__ */
/* You can include/uninclude encoder files as you see fit here. */
#include "./lib/encoder/switches/switches.h"
#include "./lib/encoder/tlv5616/tlv5616.h"
#include "./lib/encoder/encoder.h"
/* You can include/uninclude encoder files as you see fit here. */
#include "./lib/decoder/adc/adc.h"
#include "./lib/decoder/display/display.h"
#include "./lib/decoder/fft/fft.h"
#include "./lib/decoder/decoder.h"
/* Filters are also provided in /lib/Filters. */


// Timer0A used by ADC
// Timer1A used by Decoder
// Timer2A used by Encoder

int main(void) {
    PLL_Init(4);

    Encoder_Init();
    Decoder_Init();

    Encoder_Test(1);

    while (1) {
//        Swithces_Routine();
//        Display_Routine();
    }
}
