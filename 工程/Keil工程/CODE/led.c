#include "led.h"

//LED初始化函数
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOA Peripheral clock enable 使能GPIOA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure PA0 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;   //引脚编号：0号 1号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //输出模式：推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //不使用上/下拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);//输出高电平 灯灭
}
