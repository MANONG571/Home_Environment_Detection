#include "ADC_LDR.h"

extern int flag;

void LDR_Init1(void)
{
	//定义结构体
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	/* Configure ADC1 Channel5 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入功能
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//配置ADC常规结构体
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;       //ADC模式：独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;    //时钟分频2分频 84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //禁止DMA功能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //采样间隔
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;      //12位精度，量化范围0~4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								      //启用扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      					//转换器开启,连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //无触发方式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 						//低位对齐，右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //获取1个结果
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel5 configuration **************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //ADC1通道5没3个时钟采样1次

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

}

void LDR_Init2(void)
{
	//定义结构体
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	/* Configure ADC1 Channel5 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //模拟输入功能
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//配置ADC常规结构体
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;       //ADC模式：独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;    //时钟分频2分频 84/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //禁止DMA功能
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //采样间隔
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;      //12位精度，量化范围0~4095
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								      //启用扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      					//转换器开启,连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //无触发方式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 						//低位对齐，右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;                                  //获取1个结果
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel5 configuration **************************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_3Cycles); //ADC1通道5没3个时钟采样1次

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

}

void ADC_EXE(uint16_t num)
{
	  
		switch(num)
		{
			case 1:
					LDR_Init1();
					uint16_t	LDR_tmp, LDR_value;
					
					/* Start ADC1 Software Conversion */ 
					ADC_SoftwareStartConv(ADC1);

					while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//等待转换结束

					LDR_tmp = ADC_GetConversionValue(ADC1); //获取转换值
					LDR_value = LDR_tmp * 3300/4096;
					
					//光照越强电压越小，光照越若电压越大
					printf("转换值是：%d 实际电压：%dmv \r\n",LDR_tmp, LDR_value);
					
					//失能ADC1
					ADC_Cmd(ADC1, DISABLE);
					//循环数码管显示电压
					while(flag == 3)
					{
						display(1,LDR_value/1000%10);
						delay_ms(20);
						display(2,LDR_value/100%10);
						delay_ms(20);
						display(3,LDR_value/10%10);
						delay_ms(20);
						display(4,LDR_value%10);
						delay_ms(20);
					}
					break;
			
			case 2:
					LDR_Init2();
					uint16_t GAS_tmp, GAS_value;
			
					/* Start ADC1 Software Conversion */ 
					ADC_SoftwareStartConv(ADC1);

					while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//等待转换结束

					GAS_tmp = ADC_GetConversionValue(ADC1) ; //获取转换值
					GAS_value = GAS_tmp * 10000/4096;
			
					printf("—————————————————\r\n");
					//光照越强电压越小，光照越若电压越大
					printf("气体检测结果为：%d  \r\n",GAS_value);
					
					//失能ADC1
					ADC_Cmd(ADC1, DISABLE);
					//循环数码管显示气体检测结果
					while(flag == 4)
					{
						display(1,GAS_value/1000%10);
						delay_ms(20);
						display(2,GAS_value/100%10);
						delay_ms(20);
						display(3,GAS_value/10%10);
						delay_ms(20);
						display(4,GAS_value%10);
						delay_ms(20);
					}
					break;
		}
}
