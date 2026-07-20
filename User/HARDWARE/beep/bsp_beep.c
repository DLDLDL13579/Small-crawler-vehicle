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
  
#include "./beep/bsp_beep.h"   

/**************************************************************************
Function: Buzzer initialization
Input   : none
Output  : none
函数功能：蜂鸣器初始化
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void BEEP_GPIO_Config(void)
{		

	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*关闭JTAG接口*/
	JTAG_Set(JTAG_SWD_DISABLE);    

	/*打开SWD接口 可以利用主板的SWD接口调试*/
	JTAG_Set(SWD_ENABLE);           

	/*开启控制蜂鸣器的GPIO的端口时钟*/
	RCC_APB2PeriphClockCmd( BEEP_GPIO_CLK, ENABLE); 

	/*选择要控制蜂鸣器的GPIO*/															   
	GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;	

	/*设置GPIO模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置GPIO速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

	/*调用库函数，初始化控制蜂鸣器的GPIO*/
	GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 

	/* 关闭蜂鸣器*/
	GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);	 
}


/**************************************************************************
Function: Set JTAG mode
Input   : mode:JTAG, swd mode settings；00，all enable；01，enable SWD；10，Full shutdown
Output  : none
函数功能：设置JTAG模式
入口参数：mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;	
返回  值：无
**************************************************************************/
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00	
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
} 



/**************************************************************************
Function: Buzzer_Alarm
Input   : Indicates the count of frequencies
Output  : none
函数功能：蜂鸣器报警
入口参数: 指示频率的计数 
返回  值：无
**************************************************************************/	 	
//在中断函数调用
void Buzzer_Alarm(u16 count)
{
	static int count_time;
	if(0 == count)
	{
		BEEP_OFF;
	}
	else if(++count_time >= count)
	{
		BEEP_TOGGLE;
		count_time = 0;	
	}
}




/*********************************************END OF FILE**********************/
