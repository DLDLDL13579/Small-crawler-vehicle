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

#include "bluetooth.h"  
u8 PID_Send;			//用于手机app获取参数界面的变量
u8 Flag_Direction;		//用于手机app的方向变量，顺时针一圈共8个方向，数值是1--8,停止时数值为0
/**************************************************************************
Function: BLUETOOTH_USART_IRQHandler
Input   : none
Output  : none
函数功能：蓝牙接收中断函数，接收来自手机的数据
入口参数: 无 
返回  值：无
**************************************************************************/	 
//软复位进BootLoader区域
static void _System_Reset_(u8 uart_recv)
{
	static u8 res_buf[5];
	static u8 res_count=0;
	
	res_buf[res_count]=uart_recv;
	
	if( uart_recv=='r'||res_count>0 )
		res_count++;
	else
		res_count = 0;
	
	if(res_count==5)
	{
		res_count = 0;
		//接受到上位机请求的复位字符“reset”，执行软件复位
		if( res_buf[0]=='r'&&res_buf[1]=='e'&&res_buf[2]=='s'&&res_buf[3]=='e'&&res_buf[4]=='t' )
		{
			NVIC_SystemReset();//进行软件复位，复位后执行 BootLoader 程序
		}
	}
}

//蓝牙接收中断，串口3
void BLUETOOTH_USART_IRQHandler(void)
{
	static int temp_count = 0;				//用于记录前进的指令的次数，第一次连接蓝牙的时候需要用到
	static	int bluetooth_receive=0;		//蓝牙接收相关变量
	static u8 Flag_PID,i,j,Receive[50];
	static float Data;						//app界面接收参数用到的变量
	if(USART_GetITStatus(BLUETOOTH_USARTx, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		bluetooth_receive=USART_ReceiveData(BLUETOOTH_USARTx); 
		USART_ClearITPendingBit(BLUETOOTH_USARTx,USART_IT_RXNE);
		_System_Reset_(bluetooth_receive);
		if(APP_ON_Flag == RC_OFF)						//未开启蓝牙控制，只接收数据进行简单的分析
		{
			if(bluetooth_receive == 0x41)
			{
				if((++temp_count) == 5)					//需要连续发送5次前进的指令，上拉转盘一段时间可开始app控制
				{
					temp_count = 0;
					APP_ON_Flag = RC_ON;		
					PS2_ON_Flag = RC_OFF;
					ROS_ON_Flag=RC_OFF;
					Remote_ON_Flag = RC_OFF;
					RC_Velocity = Default_Velocity;		//此时速度恢复默认速度
					if(Car_Num == Akm_Car)				//阿克曼车的转向为角度，蓝牙控制时转向角度默认不变
						RC_Turn_Velocity = Bluetooth_Turn_Angle;
					else								//其他车型是两轮差速
						RC_Turn_Velocity = Default_Turn_Bias;
				}
			}
			else 
				temp_count = 0;
		}
		else 
		{
			if(bluetooth_receive>=0x41&&bluetooth_receive<=0x48) 	//默认使用，8个方向手机发送的数值是0x41--0x48
				Flag_Direction=bluetooth_receive-0x40;
			
			else if(bluetooth_receive==0X5A)  						//停止时发送0x5a
				Flag_Direction=0;

			else if(bluetooth_receive<=10)  //备用app
				Flag_Direction=bluetooth_receive;	
			
			else if(bluetooth_receive==0x59) 						//减速按键
			{
				if((RC_Velocity -= X_Step)<MINI_RC_Velocity)
					RC_Velocity = MINI_RC_Velocity;
				if(Car_Num != Akm_Car)								//非阿克曼车转向速度可更改
				{
					if((RC_Turn_Velocity -= Z_Step)<MINI_RC_Turn_Velocity)
					RC_Turn_Velocity = MINI_RC_Turn_Velocity;
				}
			}			 
			else if(bluetooth_receive==0x58)						//加速
			{
				if((RC_Velocity += X_Step)>MAX_RC_Velocity)
					RC_Velocity = MAX_RC_Velocity;
				if(Car_Num != Akm_Car)
				{
					if((RC_Turn_Velocity += Z_Step)>MAX_RC_Turn_Bias)
						RC_Turn_Velocity = MAX_RC_Turn_Bias;
				}
			}			
			else if(bluetooth_receive==0x7B) Flag_PID=1;   			//APP参数指令起始位
			else if(bluetooth_receive==0x7D) Flag_PID=2;   			//APP参数指令停止位
			else if(bluetooth_receive=='a') Avoid_Flag=!Avoid_Flag;  //app开启雷达避障
			if(Flag_PID==1)  //采集数据
			{
				Receive[i]=bluetooth_receive;
				i++;
			}
			if(Flag_PID==2)  //分析数据
			{
				if(Receive[3]==0x50) 	 PID_Send=1;
				else if(Receive[1]!=0x23) 
				{								
					for(j=i;j>=4;j--)
					{
						Data+=(Receive[j-1]-48)*pow(10,i-j);
					}
								switch(Receive[1])//调参界面
								{  
								case 0x30:  ELE_KP_Akm =Data/100000;break;
								case 0x31:  ELE_KD_Akm =Data/100000;break;//mm/s化为m/s
								case 0x32:  ELE_KD_Akm =Data/100000;break;
								case 0x33:  break; //预留
								case 0x34:  break; //预留 
								case 0x35:  break; //预留
								case 0x36:  break; //预留
								case 0x37:  break; //预留
								case 0x38:  break; //预留
								default:break;
								}
				}				 
				Flag_PID=0;
				i=0;
				j=0;
				Data=0;
				memset(Receive, 0, sizeof(u8)*50);//数组清零
			} 
		}
	}  											 
}



