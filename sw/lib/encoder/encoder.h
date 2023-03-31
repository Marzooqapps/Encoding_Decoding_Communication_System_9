#include <stdbool.h>

void BitFifo_Init(void);
int BitFifo_Get(bool *datapt);
int BitFifo_Put(bool data);

void Encoder_Init(void);
void Encoder_ProcessChar(void);
void Encoder_ProcessBit(void);
void Encoder_Test(int option);
