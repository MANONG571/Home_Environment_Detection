#include "dht11.h"

uint8_t DHT11_data[5];
extern int flag;

void DHT11_IOout(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOD Peripheral clock enable 使能GPIOD时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PA0 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;   //引脚编号：12号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      //GPIO模式：输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //输出模式：推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //不使用上/下拉电阻
	GPIO_Init(GPIOD, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
	
	GPIO_SetBits(GPIOD, GPIO_Pin_12);//输出高电平 
	delay_ms(20);//让总线空闲一会儿
}

void DHT11_IOin(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO结构体 
	
	/* GPIOD Peripheral clock enable 使能GPIOD时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PA0 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;   //引脚编号：12号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      //GPIO模式：输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;   //IO速度：高速
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure); //按初始化结构体的要求对GPIO进行初始化
}

void DHT11_Start(void)
{
	//GPIO输出模式
	DHT11_IOout();
	//输出低电平至少18ms
	PDout(12) = 0;
	delay_ms(20);
	//输出高电平20-40us
	PDout(12) = 1;
	delay_us(30);
}


int DHT11_Response(void)
{
	int t1=0,t2=0,t3=0;
	//GPIO输入模式
	DHT11_IOin();
	//等待80us低电平到来
	while(1)
	{
		delay_us(10);
		if(PDin(12) == 0)
			break;
		if(++t1>100)
		{
			printf("—————————————————\r\n");
			printf("唤醒DHT11失败1\r\n");
			return -1;
		}
	}
	//等待80us高电平到来
	while(1)
	{
		delay_us(10);
		if(PDin(12) == 1)
			break;
		if(++t2>100)
		{
			printf("—————————————————\r\n");
			printf("唤醒DHT11失败2\r\n");
			return -1;
		}
	}
	//等待数据时隙低电平到来
	while(1)
	{
		delay_us(10);
		if(PDin(12) == 0)
			break;
		if(++t3>100)
		{
			printf("—————————————————\r\n");
			printf("唤醒DHT11失败3\r\n");
			return -1;
		}
	}
	return 0;
}

uint8_t Read_DHT11_DataByte(void)
{
	//定义保存数据变量
	uint8_t i,tmpe = 0; //0000 0000
	
	for(i=0; i<8; i++)
	{
		//等待时隙电平结束
		while(PDin(12) == 0);
		delay_us(30);
		if(PDin(12) == 1)
		{
			//读取数据1 （读8bit）
			tmpe |= 1<<(7-i);//tmpe = tmpe | (1<<(7-i));
			while(PDin(12) == 1); //等待高电平持续时间结束
		}
	}
	//返回数据
	return tmpe;
}

void Read_DHT11_Data(void)
{
	uint8_t ret,i,DHT11_data[5];

	//1、主机发送起始信号
	DHT11_Start();
	//2、从机应答
	ret = DHT11_Response();
	if(ret == 0)
	{
		//3、读取数据(读5次)
		for(i=0; i<5; i++)
		{
			DHT11_data[i] = Read_DHT11_DataByte();
		}
		//4、校验数据
		if(DHT11_data[0]+DHT11_data[1]+DHT11_data[2]+DHT11_data[3] == DHT11_data[4])
		{	
			if(flag == 1)
			{
				printf("—————————————————\r\n");
				printf("当前环境温度：%d ℃\r\n", DHT11_data[2]);
				while(flag == 1)
				{
					//温度超于28或低于18，蜂鸣器响
					if( !(18<=DHT11_data[2] && DHT11_data[2]<=28) )
					{
						PDout(0) = 1;
					}
					
					display(1,DHT11_data[2]/10);
					delay_ms(20);
					display(2,DHT11_data[2]%10);
					delay_ms(20);
					display(3,DHT11_data[3]/10);
					delay_ms(20);
					display(4,DHT11_data[3]%10);
					delay_ms(20);
				}
				PDout(0) = 0;
			}
			
			if(flag == 2)
			{
				printf("—————————————————\r\n");
				printf("当前环境湿度：%d %%\r\n", DHT11_data[0]);
				while(flag == 2)
				{
					//湿度高于70%或低于30%，蜂鸣器响
					if( !(30<=DHT11_data[0]&&DHT11_data[0]<=70) )
					{
						PDout(0) = 1;
					}
					
					display(1,DHT11_data[0]/10);
					delay_ms(20);
					display(2,DHT11_data[0]%10);
					delay_ms(20);
					display(3,DHT11_data[1]/10);
					delay_ms(20);
					display(4,DHT11_data[1]%10);
					delay_ms(20);
				}
				PDout(0) = 0;
			}
		}
		else
		{
			printf("—————————————————\r\n");
			printf("DHT11数据不正确\r\n");
		}
			
	}
	//6、将总线拉高--总线变为空闲模式
	DHT11_IOout();
	PDout(12)=1;
	
}
