#include <stdbool.h>

void CharFifo_Init(void);
int CharFifo_Get(char *datapt);
int CharFifo_Put(char data);
void BitFifo_Init(void);
int BitFifo_Get(bool *datapt);
int BitFifo_Put(bool data);

void Encoder_Init(void);
void Encoder_ProcessChar(void);
void Encoder_ProcessBit(void);
