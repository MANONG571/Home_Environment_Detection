#include "timer.h"

void TIM2_OutPWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//初始化结构体成员
	TIM_TimeBaseStructure.TIM_Period = 100-1;         //时钟周期 0x0000-0xffff 1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;          //预分频值 0x0000-0xffff 84000000/840=100000hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟第二次分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数方式向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //定时器常规结构体初始化
	
	/*通道初始化输出PWM波*/
	/* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //定时器模式选择：PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//比较值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //有效电平：低电平有效
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//配置通道1
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);//通道1自动重载，不断输出
	
	//比较器使能
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	/* TIM2 counter enable */
	TIM_Cmd(TIM2, ENABLE);
	
	/* GPIOA Peripheral clock enable 使能GPIOA时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure PA0 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //引脚编号：0号 1号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //输出模式：推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //不使用上/下拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2); //引脚复用映射
}


void TIM4_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//初始化结构体成员
	TIM_TimeBaseStructure.TIM_Period = 10000/10-1;         //时钟周期 0x0000-0xffff 100ms
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;          //预分频值 0x0000-0xffff 84000000/8400=10000hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟第二次分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //计数方式向上计数
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;		         //中断线
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //抢占优先
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable TIM2 Update interrupts *///允许更新中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	/* TIM2 enable counters *///使能定时器
	TIM_Cmd(TIM4, ENABLE);

}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		PAout(1) ^= 1;
		/* Clear TIM4 COM pending bit */
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
	
}


