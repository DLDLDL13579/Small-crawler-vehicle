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
  
#include "./key/bsp_key.h"  

/**************************************************************************
Function: Key_GPIO_Config
Input   : none
Output  : none
函数功能：按键配置
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Key_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
}

/**************************************************************************
Function: Key_Scan
Input   : none
Output  : none
函数功能：按键检测
入口参数: 无 
返回  值：无
**************************************************************************/	 
//检测是否单击按下
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}

/**************************************************************************
Function: User_Key_Scan
Input   : none
Output  : Key status
函数功能：用户按键检测（单击,长按或双击）
入口参数: 无 
返回  值：返回按键状态
**************************************************************************/	 
//放在5ms中断中调用
uint8_t User_Key_Scan(void)
{
	static u16 count_time = 0;					//计算按下的时间，每5ms加1
	static u8 key_step = 0;						//记录此时的步骤
	switch(key_step)
	{
		case 0:
			if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
				key_step++;						//检测到有按键按下，进入下一步
			break;
		case 1:
			if((++count_time) == 5)				//延时消抖
			{
				if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )//按键确实按下了
					key_step++,count_time = 0;	//进入下一步
				else
					count_time = 0,key_step = 0;//否则复位
			}
			break;
		case 2:
			if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )
				count_time++;					//计算按下的时间
			else 								//此时已松开手
				key_step++;						//进入下一步
			break;
		case 3:									//此时看按下的时间，来判断是长按还是短按
			if(count_time > 400)				//在5ms中断中调用，故按下时间若大于400*5 = 2000ms（大概值）
			{							
				key_step = 0;					//标志位复位
				count_time = 0;
				return Long_Press;				//返回 长按 的状态 
 			}
			else if(count_time > 5)				//此时是单击了一次
			{
				key_step++;						//此时进入下一步，判断是否是双击
				count_time = 0;					//按下的时间清零
			}
			else
			{
				key_step = 0;
				count_time = 0;	
			}
			break;
		case 4:									//判断是否是双击或单击
			if(++count_time == 38)				//5*38 = 190ms内判断按键是否按下
			{
				if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )	//按键确实按下了
				{																	//这里双击不能按太快，会识别成单击
					key_step++;														//进入下一步，需要等松手才能释放状态
					count_time = 0;
				}
				else																//190ms内无按键按下，此时是单击的状态
				{
					key_step = 0;				//标志位复位
					count_time = 0;					
					return Click;				//返回单击的状态
				}
			}
			break;
		case 5:
			if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )//按键还在按着
			{
				count_time++;
			}
			else								//按键已经松手
			{
//				if(count_time>400)				//这里第二次的单击也可以判断时间的，默认不判断时间，全部都返回双击
//				{
//				}
				count_time = 0;
				key_step = 0;
				return Double_Click;
			}
			break;
		default:break;
	}
	return No_Action;							//无动作

}

/**************************************************************************
Function: User_Key_Val
Input   : none
Output  : Key status
函数功能：按键值检测
入口参数: 无 
返回  值：无
**************************************************************************/	 
//放在5ms中断中调用
void Get_KeyVal(void)
{
	switch(User_Key_Scan())								//按键扫描，单击，双击或长按
		{
			//单击按键可以切换到
			//1.普通遥控模式
			//2.雷达巡航模式
			//3.雷达跟随模式
			//4.电磁巡线模式
			//5.CCD巡线模式
			//长按按键进入上位机
			case Click:
				Mode++;
				if(Mode == ELE_Line_Patrol_Mode)
					ELE_ADC_Init();							//初始化电磁巡线模式
				
				else if(Mode == CCD_Line_Patrol_Mode)		//CCD巡线模式
				{
					CCD_Init();								//CCD初始化，CCD模块和电磁巡线模块共用一个接口，两个不能同时使用
					BEEP_OFF;								//关闭蜂鸣器
				}
				
				else if(Mode == 6)							//6种模式循环切换
				{
					Mode = Normal_Mode;
					Move_X = Move_Z = 0;
					MotorA.Motor_Pwm = MotorB.Motor_Pwm = 0;
					BEEP_OFF;
				}
				break;
			case Long_Press:
				Flag_Show = !Flag_Show;								//长按 进入/退出 上位机模式
				break;
			case Double_Click:										
				if(Mode == ELE_Line_Patrol_Mode)					//电磁巡线状态时，双击可以打开/关闭雷达检测障碍物，默认打开
				{
					Lidar_Detect = !Lidar_Detect;
					if(Lidar_Detect == Lidar_Detect_OFF)
						memset(Dataprocess,0, sizeof(PointDataProcessDef)*390);		//用于雷达检测障碍物的数组清零
				}else if(Mode == Normal_Mode)					//普通遥控时，双击可以打开/关闭雷达检测障碍物，默认关闭
				{
					Avoid_Flag = !Avoid_Flag;
					if(Avoid_Flag == Lidar_Avoid_OFF)
						memset(Dataprocess,0, sizeof(PointDataProcessDef)*390);		//用于雷达检测障碍物的数组清零
				}
				break;
			default:break;
		}
}

/************************************END OF FILE*******************************/
