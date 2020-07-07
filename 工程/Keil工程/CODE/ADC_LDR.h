#ifndef _ADCLDR_H
#define _ADCLDR_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "delay.h"
#include "display.h"

void LDR_Init1(void);
void LDR_Init2(void);

void ADC_EXE(uint16_t num);

#endif
