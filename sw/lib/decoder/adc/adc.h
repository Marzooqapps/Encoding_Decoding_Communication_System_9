/**
 * @file adc.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

void SampleFifo_Init(void);
int SampleFifo_Get(uint32_t *datapt);
int SampleFifo_Put(uint32_t data);


int ADC0_InitTimer0ATriggerSeq0(uint32_t channelNum, uint32_t fs, void(*task)(uint32_t));
void ADC0Seq0_Handler(void);
void ADC0_InitTimer0ATriggerSeq3PD3(uint32_t period);
void ADC0Seq3_Handler(void);
