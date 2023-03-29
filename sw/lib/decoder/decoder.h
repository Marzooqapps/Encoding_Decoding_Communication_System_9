#include <stdint.h>

void SampleFifo_Init(void);
int SampleFifo_Get(uint16_t *datapt);
int SampleFifo_Put(uint16_t data);
void Decoder_Init(void);
void Decoder_ISR(void);
