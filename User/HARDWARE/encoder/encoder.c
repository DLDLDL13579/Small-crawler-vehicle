/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2023-03-02

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2023-03-02

All rights reserved
***********************************************/

#include "./encoder/encoder.h"

/**************************************************************************
Function: Encoder_GPIO_Config
Input   : none
Output  : none
函数功能：编码器端口配置
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Encoder_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//开启编码器的GPIO的端口时钟
	RCC_APB2PeriphClockCmd( ENCODER1_GPIO_CLK|ENCODER2_GPIO_CLK, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = ENCODER1_A_GPIO_PIN|ENCODER1_B_GPIO_PIN;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 					//浮空输入
	GPIO_Init(ENCODER1_GPIO_PORT, &GPIO_InitStructure); 					//根据设定参数初始化

	GPIO_InitStructure.GPIO_Pin = ENCODER2_A_GPIO_PIN|ENCODER2_B_GPIO_PIN;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 					//浮空输入
	GPIO_Init(ENCODER2_GPIO_PORT, &GPIO_InitStructure); 					//根据设定参数初始化

}

/**************************************************************************
Function: Encoder_NVIC_Config
Input   : none
Output  : none
函数功能：编码器中断配置
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Encoder_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = ENCODER1_IRQ;  		//定时器4中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  			//使能IRQ通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级1 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;       	//响应优先级3
	NVIC_Init(&NVIC_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = ENCODER2_IRQ;  		//定时器8中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  			//使能IRQ通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//抢占优先级1 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;       	//响应优先级3
	NVIC_Init(&NVIC_InitStruct);
	
}
/**************************************************************************
Function: Encoder_TIM_Mode_Config
Input   : none
Output  : none
函数功能：编码器模式配置
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Encoder_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  

	ENCODER1_TIM_APBxClock_FUN(ENCODER1_TIM_CLK,ENABLE);		//编码器1时钟

	ENCODER2_TIM_APBxClock_FUN(ENCODER2_TIM_CLK,ENABLE);		//编码器2时钟

	/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00; 				// 预分频器 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; 		//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//边沿计数模式 
	
	TIM_TimeBaseInit(ENCODER1_TIM, &TIM_TimeBaseStructure);  	//初始化定时器
	TIM_TimeBaseInit(ENCODER2_TIM, &TIM_TimeBaseStructure);  	//初始化定时器

	/*--------------------编码器模式接口配置-------------------------*/
	//编码器1
	TIM_EncoderInterfaceConfig(ENCODER1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3

	TIM_ICStructInit(&TIM_ICInitStructure); 					//把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  					//设置滤波器长度
	
	TIM_ICInit(ENCODER1_TIM, &TIM_ICInitStructure);				//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
	TIM_ClearFlag(ENCODER1_TIM, TIM_FLAG_Update);				//清除TIM的更新标志位
	TIM_ITConfig(ENCODER1_TIM, TIM_IT_Update, ENABLE);			//使能定时器中断
	TIM_Cmd(ENCODER1_TIM, ENABLE); 								//使能定时器



	//编码器2
	TIM_EncoderInterfaceConfig(ENCODER2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3

	TIM_ICStructInit(&TIM_ICInitStructure); 					//把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  					//设置滤波器长度
	
	TIM_ICInit(ENCODER2_TIM, &TIM_ICInitStructure);				//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
	TIM_ClearFlag(ENCODER2_TIM, TIM_FLAG_Update);				//清除TIM的更新标志位
	TIM_ITConfig(ENCODER2_TIM, TIM_IT_Update, ENABLE);			//使能定时器中断
	TIM_Cmd(ENCODER2_TIM, ENABLE); 								//使能定时器

}

/**************************************************************************
Function: Encoder_Init
Input   : none
Output  : none
函数功能：编码器配置
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Encoder_Init(void)
{
	Encoder_GPIO_Config();			//端口配置
	Encoder_NVIC_Config();			//中断配置
	Encoder_TIM_Mode_Config();		//模式配置
}

/**************************************************************************
Function: Read_Encoder
Input   : TIMX:Timer Number
Output  : Encoder data
函数功能：读取编码器
入口参数: TIMX: 编码器序号
返回  值：编码器读数
**************************************************************************/	 	
int Read_Encoder(u8 TIMX)
{
	int Encoder_TIM;    
	switch(TIMX)
	{
		case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0; break;
		case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0; break;	
		case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0; break;	
		case 8:  Encoder_TIM= (short)TIM8 -> CNT;  TIM8 -> CNT=0; break;	
		default:  Encoder_TIM=0;
	}
	return Encoder_TIM;
}

/**************************************************************************
函数功能：编码器1中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void ENCODER1_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
{ 		    		  			    
	if(TIM_GetFlagStatus(ENCODER1_TIM,TIM_FLAG_Update)==SET)//溢出中断
	{
	 
	} 
	TIM_ClearITPendingBit(ENCODER1_TIM,TIM_IT_Update); 	//清除中断标志位 	
}
/**************************************************************************
函数功能：编码器2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void ENCODER2_IRQHandler(void)//中断处理函数为空，清除中断标志位后结束中断
{ 		    		  			    
	if(TIM_GetFlagStatus(ENCODER2_TIM,TIM_FLAG_Update)==SET)//溢出中断
	{
	 
	} 
	TIM_ClearITPendingBit(ENCODER2_TIM,TIM_IT_Update); 	//清除中断标志位 	  
}

