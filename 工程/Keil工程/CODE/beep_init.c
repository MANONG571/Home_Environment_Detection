#include "beep_init.h"

void beep_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOD Peripheral clock enable 使能GPIOA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD0 in input  PullUp mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //引脚编号：0号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //使用上拉电阻:给IO一个确定的电平
	GPIO_Init(GPIOD, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
	PDout(0) = 0;
}
