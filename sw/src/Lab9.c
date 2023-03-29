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
#define ENCODER 0
#define DECODER 1
#define __MAIN__ ENCODER

/* Include relevant files for the encoder depending on the value of __MAIN__ */
#if __MAIN__ == ENCODER
    /* You can include/uninclude encoder files as you see fit here. */
    #include "./lib/encoder/switches/switches.h"
    #include "./lib/encoder/tlv5616/tlv5616.h"
    #include "./lib/encoder/encoder.h"
#elif __MAIN__ == DECODER
    /* You can include/uninclude encoder files as you see fit here. */
    #include "./lib/decoder/adc/adc.h"
    #include "./lib/decoder/display/display.h"
    #include "./lib/decoder/fft/fft.h"
    #include "./lib/decoder/decoder.h"
    /* Filters are also provided in /lib/Filters. */
#endif

#if __MAIN__ == ENCODER
    /**
     * @brief Runs the encoder. Flash this code to one system.
     */
    int main(void) {
        /* TODO: Set up the required initializations here. */

        Encoder_Init();

        while (1) {
            /* TODO: Encoder main loop. */
        }
    }
#elif __MAIN__ == DECODER
    /**
     * @brief Runs the decoder. Flash this code to a different system
     * than the encoder's system.
     */
    int main(void) {
        /* TODO: Set up the required initializations here. */

        Decoder_Init();

        while (1) {
            /* TODO: Decoder main loop. */
        }
    }
#endif
