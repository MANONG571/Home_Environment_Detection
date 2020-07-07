#ifndef _KEY_H
#define _KEY_H
#include "stm32f4xx.h"
#include <stdio.h>

void KEY_Init(void);
void EXTILine_Config(void);
void EXTI0_IRQHandler(void);

#endif
