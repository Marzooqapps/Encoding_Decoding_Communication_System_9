#include <stdint.h>

void DecodedBitFifo_Init(void);
int DecodedBitFifo_Get(int *datapt);
int DecodedBitFifo_Put(int data);
void Decoder_Init(void);
void Decoder_ConversionISR(void);
void Decoder_DecodeISR(void);
void Decoder_Test(void);
