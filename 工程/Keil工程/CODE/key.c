#include "key.h"

extern int flag;

//KEY按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOB Peripheral clock enable 使能GPIOA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Configure PB0 in input  PullUp mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;          //引脚编号：0号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //GPIO模式：输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //使用上拉电阻:给IO一个确定的电平
	GPIO_Init(GPIOB, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
}

void EXTILine_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	//初始化按键
	KEY_Init();
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI Line0 to PB0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3;               //中断线编号
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      //模式选择
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   //上升沿触发 从低电平到高电平的跳变
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/*抢占优先和响应优先级决定了中断的执行顺序：
		1、有多个中断触发时抢占优先级高的中断先执行
		2、当抢占优先级别相同时响应优先级别高的中断先执行
		3、抢占优先级别高的中断可打断抢占优先级别低的中断
		4、响应优先级别高的中断不可打断响应优先级别低的中断
	*/
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;		     //中断线
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //抢占优先
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;		     //中断线
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //抢占优先
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;		     //中断线
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //抢占优先
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;		     //中断线
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;    //抢占优先
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//中执行函数中不要有延时函数，并且功能不要写太复杂
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		//printf("KEY1\r\n");
		flag = 1;
		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		//printf("KEY2\r\n");
		flag = 2;
		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

//光照距离
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		//printf("KEY3\r\n");
		flag = 3;
		/* Clear the EXTI line 2 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

//气体
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		//printf("KEY4\r\n");
		flag = 4;
		/* Clear the EXTI line 1 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
