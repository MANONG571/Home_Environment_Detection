#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

#include "sys.h"
#include <stdio.h>

#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "dht11.h"
#include "timer.h"
#include "ADC_LDR.h"
#include "display.h"
#include "SR04.h"
#include "beep_init.h"

uint32_t SR04_Getdis(void);

int menu = 1;
int flag = 0;

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	return ch;
}


int main(void)
{
	//确定系统定时器的时钟源 84MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	//确定优先级别分组：选择分组2 2位表示抢占优先级 2位表示响应优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//配置外部中断
	EXTILine_Config();
	//配置串口
	USART_Config();
	//数码管显示初始化
	display_Init();
	//蜂鸣器初始化
	beep_init();
	
	
	while(1)
	{
		switch(flag)
		{
			case 1:
				//获取温度值并处理
				Read_DHT11_Data();
				break;
				
			case 2:
				//获取湿度值
				Read_DHT11_Data();
				break;
				
			case 3:
				//检测距离
				GETdis();
				
				//获取光照值
				ADC_EXE(1);
				break;
				
			case 4:
				//获取气体值
				ADC_EXE(2);
				break;
				
			default:
				if(menu == 1)
				{
					printf("—————————————————\r\n");
					printf("******KEY1:检测温度******\r\n");
					printf("******KEY2:检测湿度******\r\n");
					printf("******KEY3:检测光照******\r\n");
					printf("******KEY4:检测气体******\r\n");
					menu = 0;
				}
				break;
		}
	}
}

