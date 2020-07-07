#include "SR04.h"

void HCSR04_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOA Peripheral clock enable 使能GPIOA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //引脚编号：6号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //输出模式：推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //不使用上/下拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   //引脚编号：6号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //不使用上/下拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
}

uint32_t SR04_Getdis(void)
{
	uint32_t dis;
	uint32_t tmp=0;

	//超声波测距模块初始化
	HCSR04_Init();
	
	PDout(6) = 1;
	delay_us(10);
	
	while(PDin(7) == 0);//容易卡住
	
	while(PDin(7) == 1)
	{
		delay_us(8); //约 3mm
		tmp++;
	}

	dis = tmp*3/2;
	
	return dis;
}	

void GETdis(void)
{
	uint32_t dis;
	
	dis = SR04_Getdis();

	printf("—————————————————\r\n");
	printf("距离值是：%d \r\n",dis);
	
}
