/**
 * @file DAC.c
 * @author your name (you@domain.com)
 * @brief Low level driver for the DAC. Documentation comments are specifically
 *        for the TLV5618 12-bit SPI DAC. Modify for your particular DAC.
 * @version 0.3.0
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 * @note Reference datasheet:
 *      https://www.ti.com/lit/ds/symlink/tlv5618a.pdf?ts=1676400608127&ref_url=https%253A%252F%252Fwww.google.com%252F
 */

#include <stdbool.h>

#include "./DAC.h"
#include "./tm4c123gh6pm.h"
#include "SoundlessTest.h"

#define TFT_CS                  (*((volatile uint32_t *)0x40007020))
#define TFT_CS_LOW              0           // CS normally controlled by hardware
#define TFT_CS_HIGH             0x08
#define DC                      (*((volatile uint32_t *)0x40007100))
#define DC_COMMAND              0
#define DC_DATA                 0x40
#define RESET                   (*((volatile uint32_t *)0x40007200))
#define RESET_LOW               0
#define RESET_HIGH              0x80

const unsigned short Wave[32] = {
    2047, 2448, 2829, 3185, 3497, 3751, 3939, 4054, 4094, 4054, 3939, 3751, 3497,
    3185, 2829, 2448, 2047, 1646, 1265, 909, 597, 343, 155, 40, 0, 40, 155, 343,
    597, 909, 1265, 1646
};

bool dac_on;
bool high_freq;
uint8_t wave_index;
bool wave_parity;

extern uint32_t SOUND;

int dac_init() {
    SYSCTL_RCGCSSI_R |= 0x02;       // activate SSI1
    SYSCTL_RCGCGPIO_R |= 0x08;      // activate port D

    while((SYSCTL_PRGPIO_R&0x08) == 0){}; // ready?
    SSI1_CR1_R = 0x00000000;        // disable SSI, master mode
    GPIO_PORTD_AFSEL_R |= 0x0B;     // enable alt funct on PD3,1,0
    GPIO_PORTD_DEN_R |= 0x0B;       // enable digital I/O on PD3,1,0
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R
                       & 0xFFFF0F00)
                       + 0x00002022;
    GPIO_PORTD_AMSEL_R &= ~0x0B;    // disable analog functionality on PD
    SSI1_CPSR_R = 0x08;             // 80MHz/8 = 10 MHz SSIClk (should work up to 20 MHz)
    SSI1_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 1 Freescale
    SSI1_CR0_R += 0x40;             // SPO = 1
    SSI1_CR0_R |= 0x0F;             // DSS = 16-bit data
//    SSI1_DR_R = data;               // load 'data' into transmit FIFO
    SSI1_CR1_R |= 0x00000002;       // enable SSI

    dac_on = false;
    high_freq = false;
    wave_index = 0;
    wave_parity = 0;

    return 0;
}

int dac_output(uint16_t data) {  // data should be the 12-bit value that will be the new dac value
    // An example of how to send data via SSI is found in L534 of inc/ST7735.c.
    // Remember that 4 out of the 16 bits is for DAC operation. The last 12 bits
    // are for data. Read the datasheet! 

    while((SSI1_SR_R&0x00000002)==0){};// wait until room in FIFO
    SSI1_DR_R = data; // data out
    return 0;
}

void dac_ISR(void) {
    if(!dac_on) {
        return;
    }

    #if SOUNDLESS_TEST
        SOUND = Wave[wave_index];
    #else
        dac_output(Wave[wave_index]);
    #endif

    if(high_freq || wave_parity) {
        wave_index = (wave_index + 1) % 32;
    }
    wave_parity ^= 1;
}


void dac_off(void) {
    dac_on = false;
}

void dac_setLowFreq(void) {
    high_freq = false;
    dac_on = true;
}

void dac_setHighFreq(void) {
    high_freq = true;
    dac_on = true;
}
