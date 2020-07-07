//条件编译 
#ifndef _DELAY_H   
#define _DELAY_H

//其他功能头文件调用
#include "stm32f4xx.h"

//结构体定义

//函数声明
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
#endif
