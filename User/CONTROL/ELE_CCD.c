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


#include "ELE_CCD.h"

int Sensor_Left,Sensor_Middle,Sensor_Right,Sensor;
u16  CCD_ADV[128]={0};
u8 CCD_Median,CCD_Threshold;                 //线性CCD相关

//电磁巡线ADC端口初始化
/**************************************************************************
Function: ELE_ADC_GPIO_Config
Input   : none
Output  : none
函数功能：初始化电磁巡线GPIO
入口参数: 无
返回  值：无
**************************************************************************/	 	
static void ELE_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	RCC_APB2PeriphClockCmd ( ELE_ADC_L_GPIO_CLK|ELE_ADC_M_GPIO_CLK|ELE_ADC_R_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	//左路端口配置，PA4
	GPIO_InitStructure.GPIO_Pin = ELE_ADC_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ELE_ADC_L_PORT, &GPIO_InitStructure);	

	//中间端口配置，PA5
	GPIO_InitStructure.GPIO_Pin = ELE_ADC_M_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ELE_ADC_M_PORT, &GPIO_InitStructure);				


	//右路端口配置，PC5
	GPIO_InitStructure.GPIO_Pin = ELE_ADC_R_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ELE_ADC_R_PORT, &GPIO_InitStructure);				
	
}

/**************************************************************************
Function: ELE_ADC_Mode_Config
Input   : none
Output  : none
函数功能：初始化电磁巡线ADC
入口参数: 无
返回  值：无
**************************************************************************/	 	

static void ELE_ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// 打开ADC时钟
	ELE_ADC_APBxClock_FUN ( ELE_ADC_CLK, ENABLE );
	
	//复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_DeInit(ELE_ADC); 

	// ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 单次转换
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(ELE_ADC, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的6分频，即12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	
	// 配置 ADC 通道转换顺序和采样时间
	ADC_RegularChannelConfig(ELE_ADC, ELE_ADC_L_CHANNEL, 1, 
	                         ADC_SampleTime_239Cycles5);
							 
	ADC_RegularChannelConfig(ELE_ADC, ELE_ADC_M_CHANNEL, 1, 
	                         ADC_SampleTime_239Cycles5);

	ADC_RegularChannelConfig(ELE_ADC, ELE_ADC_R_CHANNEL, 1, 
	                         ADC_SampleTime_239Cycles5);
		
	// 不中断
	ADC_ITConfig(ELE_ADC, ADC_IT_EOC, DISABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ELE_ADC, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ELE_ADC);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ELE_ADC));
	
	// ADC开始校准
	ADC_StartCalibration(ELE_ADC);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ELE_ADC));
	
}

/**************************************************************************
Function: ELE_ADC_Init
Input   : none
Output  : none
函数功能：初始化电磁巡线ADC
入口参数: 无
返回  值：无
**************************************************************************/	 	
//电磁巡线初始化
void ELE_ADC_Init(void)
{
	ELE_ADC_GPIO_Config();
	ELE_ADC_Mode_Config();

}


/**************************************************************************
Function: CCD_ADC_Mode_Config
Input   : none
Output  : none
函数功能：初始化CCD巡线ADC
入口参数: 无
返回  值：无
**************************************************************************/	 	
static void CCD_ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	

	// 打开ADC时钟
	CCD_ADC_APBxClock_FUN ( CCD_ADC_CLK, ENABLE );
	
	//复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	ADC_DeInit(CCD_ADC); 

	// ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 单次转换
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(CCD_ADC, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的6分频，即12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

	// 配置 ADC 通道转换顺序和采样时间
	ADC_RegularChannelConfig(CCD_ADC, CCD_ADC_CHANNEL, 1, 
	                         ADC_SampleTime_239Cycles5);
		
		
	// 不中断
	ADC_ITConfig(CCD_ADC, ADC_IT_EOC, DISABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(CCD_ADC, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(CCD_ADC);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(CCD_ADC));
	
	// ADC开始校准
	ADC_StartCalibration(CCD_ADC);
	// 等待校准完成

	while(ADC_GetCalibrationStatus(CCD_ADC));
}


/**************************************************************************
Function: CCD_GPIO_Config
Input   : none
Output  : none
函数功能：初始化CCD巡线GPIO
入口参数: 无
返回  值：无
**************************************************************************/	 	
static void CCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开CCD端口时钟
	RCC_APB2PeriphClockCmd ( TSL_CLK_GPIO_CLK|TSL_SI_GPIO_CLK|CCD_ADC_GPIO_CLK, ENABLE );
	
	// CLK,SI配置为输出	
	GPIO_InitStructure.GPIO_Pin = TSL_SI_PIN;				//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TSL_SI_PORT, &GPIO_InitStructure);	


	GPIO_InitStructure.GPIO_Pin = TSL_CLK_PIN;				//PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TSL_CLK_PORT, &GPIO_InitStructure);				


	//配置ADC输入模式
	GPIO_InitStructure.GPIO_Pin = CCD_ADC_PIN;				//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(CCD_ADC_PORT, &GPIO_InitStructure);				
	
}


/**************************************************************************
Function: CCD_Init
Input   : none
Output  : none
函数功能：初始化CCD巡线
入口参数: 无
返回  值：无
**************************************************************************/	 	
void CCD_Init(void)
{
	CCD_GPIO_Config();
	CCD_ADC_Mode_Config();

}

/**************************************************************************
Function: ELE_Mode
Input   : none
Output  : none
函数功能：电磁巡线模式运行
入口参数: 无
返回  值：无
**************************************************************************/	 	
void ELE_Mode(void)
{
	int Sum = 0;
	Sensor_Left = Get_Adc(ELE_ADC_L_CHANNEL);
	Sensor_Middle = Get_Adc(ELE_ADC_M_CHANNEL);
	Sensor_Right = Get_Adc(ELE_ADC_R_CHANNEL);
	Sum = Sensor_Left*1+Sensor_Middle*100+Sensor_Right*199;			
	Sensor = Sum/(Sensor_Left+Sensor_Middle+Sensor_Right);
	if(Detect_Barrier() == No_Barrier)		//检测到无障碍物
	{
		Move_X = ELE_Move_X;				//巡线的时候速度,默认是0.25m
		if(Car_Num == Diff_Car|| Car_Num == Akm_Car)
			Move_Z = ELE_PID(Sensor,92);		//目标值92
		else
			Move_Z = ELE_PID(Sensor,100);		//目标值100
		Buzzer_Alarm(0);
    }											
	else									//有障碍物
	{
		if(!Flag_Stop)
			Buzzer_Alarm(100);				//当电机使能的时候，有障碍物则蜂鸣器报警
		else 
			Buzzer_Alarm(0);
		Move_X = 0;
		Move_Z = 0;
	}
}

/**************************************************************************
Function: Detect_Barrier
Input   : none
Output  : 1or0(Barrier_Detected or No_Barrier)
函数功能：电磁巡线模式雷达检测障碍物
入口参数: 无
返回  值：1或0(检测到障碍物或无障碍物)
**************************************************************************/	 	
//检测障碍物
u8 Detect_Barrier(void)
{
	int i;
	u8 point_count = 0;
	if(Lidar_Detect == Lidar_Detect_ON)
	{
		for(i=0;i<390;i++)	//检测是否有障碍物
		{
			if((Dataprocess[i].angle>300)||(Dataprocess[i].angle<60))
			{
				if(0<Dataprocess[i].distance&&Dataprocess[i].distance<700)//700mm内是否有障碍物
					point_count++;
		  }
		}
		if(point_count > 3)//有障碍物
			return Barrier_Detected;
		else
			return No_Barrier;
	}
	else
		return No_Barrier;
}
/**************************************************************************
函数功能：软件延时
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void Dly_us(void)
{
   int ii;    
   for(ii=0;ii<10;ii++);      
}

/**************************************************************************
Function: Read_TSL
Input   : none
Output  : none
函数功能：读取CCD模块的数据
入口参数: 无
返回  值：无
**************************************************************************/	 	
//读取CCD模块的数据
void RD_TSL(void) 
{
	u8 i=0,tslp=0;
	
	TSL_CLK_LOW;
	TSL_SI_LOW;
	Dly_us();

	TSL_SI_HIGH;
	Dly_us();

	TSL_CLK_LOW;
	Dly_us();
	
	TSL_CLK_HIGH;
	Dly_us();
	
	TSL_SI_LOW;
	Dly_us();
	
	for(i=0;i<128;i++)
	{ 
		TSL_CLK_LOW; 
		Dly_us();  //调节曝光时间
		CCD_ADV[tslp]=(Get_Adc(CCD_ADC_CHANNEL))>>4;		
		++tslp;
		TSL_CLK_HIGH;
		Dly_us();  
	}
}


/**************************************************************************
函数功能：线性CCD取中值
入口参数：无
返回  值：无
**************************************************************************/
void  Find_CCD_Median(void)
{ 
	static u8 i,j,Left,Right,Last_CCD_Median;
	static u16 value1_max,value1_min;

	value1_max=CCD_ADV[0];  //动态阈值算法，读取最大和最小值
	for(i=5;i<123;i++)   //两边各去掉5个点
	{
		if(value1_max<=CCD_ADV[i])
		value1_max=CCD_ADV[i];
	}
	value1_min=CCD_ADV[0];  //最小值
	for(i=5;i<123;i++) 
	{
		if(value1_min>=CCD_ADV[i])
		value1_min=CCD_ADV[i];
	}
	CCD_Threshold=(value1_max+value1_min)/2;	  //计算出本次中线提取的阈值
	
//	//CCD巡单线
//	for(i = 15;i<118; i++)   //寻找左边跳变沿
//	{
//		if(CCD_ADV[i]>CCD_Threshold&&CCD_ADV[i+1]>CCD_Threshold&&CCD_ADV[i+2]>CCD_Threshold&&CCD_ADV[i+3]<CCD_Threshold&&CCD_ADV[i+4]<CCD_Threshold&&CCD_ADV[i+5]<CCD_Threshold)
//		{	
//			Left=i;
//			break;	
//		}
//	}
//	for(j = 118;j>15; j--)//寻找右边跳变沿
//	{
//		if(CCD_ADV[j]<CCD_Threshold&&CCD_ADV[j+1]<CCD_Threshold&&CCD_ADV[j+2]<CCD_Threshold&&CCD_ADV[j+3]>CCD_Threshold&&CCD_ADV[j+4]>CCD_Threshold&&CCD_ADV[j+5]>CCD_Threshold)
//		{	
//			Right=j;
//			break;	
//		}
//	}
	 //CCD巡双线，这是找到左边黑线的右边值，找到右边黑线的左边值，然后确定中值
    for(i=59;i>5;i--)
	{
		if(CCD_ADV[i]<CCD_Threshold&&CCD_ADV[i+1]<CCD_Threshold&&CCD_ADV[i+2]<CCD_Threshold&&CCD_ADV[i+3]>CCD_Threshold&&CCD_ADV[i+4]>CCD_Threshold&&CCD_ADV[i+5]>CCD_Threshold)
		{
			Left=i;
			break;
		}
	}
	for(j=60;j<118;j++)
  {
	  if(CCD_ADV[j]<CCD_Threshold&&CCD_ADV[j+1]<CCD_Threshold&&CCD_ADV[j+2]<CCD_Threshold&&CCD_ADV[j+3]>CCD_Threshold&&CCD_ADV[j+4]>CCD_Threshold&&CCD_ADV[j+5]>CCD_Threshold)
	  {
		  Right=j;
		  break;
	  }
  }
	CCD_Median=(Right+Left)/2;//计算中线位置
	if(myabs(CCD_Median-Last_CCD_Median)>90)   //计算中线的偏差，如果太大
		CCD_Median=Last_CCD_Median;    //则取上一次的值
	Last_CCD_Median=CCD_Median;  //保存上一次的偏差

}	
/**************************************************************************
Function: CCD_Mode
Input   : none
Output  : none
函数功能：CCD巡线模式运行
入口参数: 无
返回  值：无
**************************************************************************/	 	
void CCD_Mode(void)
{
	RD_TSL();			//读取CCD模块数据
	Find_CCD_Median();	//找中值
	
	Move_X = CCD_Move_X;			//CCD巡线速度，默认0.3m
	Move_Z = CCD_PID(CCD_Median,64);//PID调节，目标值64
	
}


