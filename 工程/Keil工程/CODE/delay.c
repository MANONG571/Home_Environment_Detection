#include "delay.h"

void delay_us(uint32_t nus) //最大定时时间199728us
{
	SysTick->CTRL = 0;      // 控制寄存器：失能定时器
	SysTick->LOAD = 84*nus; // 装置寄存器：装置值 - 1
	SysTick->VAL = 0;       // 计数寄存器：清空计数器 
	SysTick->CTRL = 1;      // 使能定时器定时计数
	while ((SysTick->CTRL & 0x00010000)==0);// 检测计数值是否到达0
	SysTick->CTRL = 0;      // 关闭定时器
}

void delay_ms(uint32_t nms)
{
	for(;nms>0; nms--)
	{
		delay_us(1000);
	}
}
