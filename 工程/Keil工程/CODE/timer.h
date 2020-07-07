#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f4xx.h"
#include "sys.h"

void TIM2_OutPWM_Init(void);
void TIM4_config(void);
void TIM4_IRQHandler(void);

#endif
