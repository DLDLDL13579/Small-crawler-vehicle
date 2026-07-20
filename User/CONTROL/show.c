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

#include "show.h"

/**************************************************************************
Function: OLED_Show
Input   : none
Output  : none
函数功能：显示屏显示函数
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Show(void)
{
	memset(OLED_GRAM,0, 128*8*sizeof(u8));	//GRAM清零但不立即刷新，防止花屏
	if(Mode==ELE_Line_Patrol_Mode)			//电磁巡线模式显示
	{
		//=============第一行显示小车模式=======================//	
		OLED_ShowNumber(0,0,Sensor_Left,4,12);	
		OLED_ShowNumber(30,0,Sensor_Middle,4,12);
		OLED_ShowNumber(60,0,Sensor_Right,4,12);
		OLED_ShowNumber(90,0,Sensor,4,12);		
		//=============第二行显示X和Z轴的速度===============//	
		OLED_ShowString(00,10,"MoveX:");
		if(Move_X<0)		OLED_ShowString(50,10,"-");
		else				OLED_ShowString(50,10,"+");
							OLED_ShowNumber(60,10,myabs(Move_X),3,12);
		
		if( Move_Z<0)		OLED_ShowString(94,10,"-");
		else				OLED_ShowString(94,10,"+");
							OLED_ShowNumber(106,10,myabs(Move_Z),3,12);

		//=============第三行显示左电机PWM与速度数据=======================//		
								OLED_ShowString(00,20,"L-P:");
		if(MotorA.Motor_Pwm<0)	OLED_ShowString(30,20,"-"),
								OLED_ShowNumber(36,20,MotorA.Motor_Pwm,4,12);
		if(MotorA.Motor_Pwm>=0)	OLED_ShowString(30,20,"+"),
								OLED_ShowNumber(36,20,MotorA.Motor_Pwm,4,12);
										
								OLED_ShowString(66,20,"L-E:");

		if(MotorA.Target_Encoder<0)	  	OLED_ShowString(98,20,"-");
		if(MotorA.Target_Encoder>=0)  	OLED_ShowString(98,20,"+");
									OLED_ShowNumber(104,20,MotorA.Target_Encoder*1000,4,12);

		//=============第四行显示右电机PWM与速度数据=======================//		
								OLED_ShowString(00,30,"R-P:");
		if(MotorB.Motor_Pwm<0)	OLED_ShowString(30,30,"-"),
								OLED_ShowNumber(36,30,MotorB.Motor_Pwm,4,12);
		if(MotorB.Motor_Pwm>=0)	OLED_ShowString(30,30,"+"),
								OLED_ShowNumber(36,30,MotorB.Motor_Pwm,4,12);
										
								OLED_ShowString(66,30,"R-E:");

		if(MotorB.Target_Encoder<0)		OLED_ShowString(98,30,"-");
		if(MotorB.Target_Encoder>=0)  	OLED_ShowString(98,30,"+");
									OLED_ShowNumber(104,30,MotorB.Target_Encoder*1000,4,12);
		//=============第五行显示雷达是否检测障碍物=======================//
		OLED_ShowString(0,40,"Detect:");
		if(Lidar_Detect == Lidar_Detect_ON)
			OLED_ShowString(65,40,"ON ");
		else
			OLED_ShowString(65,40,"OFF");
		//=============第六行显示模式，电压与电机开关=======================//
//		if(Car_Num == Akm_Car)				OLED_ShowString(0,50,"Akm  ");
//		else if(Car_Num == Diff_Car)		OLED_ShowString(0,50,"Diff ");
//		else if(Car_Num == Small_Tank_Car)	OLED_ShowString(0,50,"STank");
//		else 								OLED_ShowString(0,50,"BTank");
								OLED_ShowString(0,50,"ELE ");				//模式显示
								OLED_ShowString(54,50,".");
								OLED_ShowString(78,50,"V");
								OLED_ShowNumber(40,50,Voltage/100,2,12);
								OLED_ShowNumber(62,50,Voltage/10%10,1,12);//显示电压
								OLED_ShowNumber(70,50,Voltage%10,1,12);
		if(Flag_Stop)          	OLED_ShowString(105,50, "OFF");
		if(!Flag_Stop)        	OLED_ShowString(105,50, "ON ");

	}
	else if(Mode == CCD_Line_Patrol_Mode)				//CCD巡线模式显示
	{
		OLED_Show_CCD();
		//=============第二行显示CCD中值=======================//	
								OLED_ShowString(00,10,"Median:");
		if( CCD_Median<0)		OLED_ShowString(90,10,"-"),
								OLED_ShowNumber(100,10,-CCD_Median,3,12);
		else                 	OLED_ShowString(90,10,"+"),
								OLED_ShowNumber(100,10, CCD_Median,3,12);

		
		//=============第三行显示CCD阈值=======================//	
								OLED_ShowString(00,20,"Threshold:");
		if( CCD_Threshold<0)	OLED_ShowString(90,20,"-"),
								OLED_ShowNumber(100,20,-CCD_Threshold,3,12);
		else                 	OLED_ShowString(90,20,"+"),
								OLED_ShowNumber(100,20, CCD_Threshold,3,12);

		//=============第四行显示左电机PWM与速度数据=======================//		
								OLED_ShowString(00,30,"L-P:");
		if(MotorA.Motor_Pwm<0)	OLED_ShowString(30,30,"-"),
								OLED_ShowNumber(36,30,MotorA.Motor_Pwm,4,12);
		if(MotorA.Motor_Pwm>=0)	OLED_ShowString(30,30,"+"),
								OLED_ShowNumber(36,30,MotorA.Motor_Pwm,4,12);
										
								OLED_ShowString(66,30,"L-E:");

		if(MotorA.Target_Encoder<0)	  	OLED_ShowString(98,30,"-");
		if(MotorA.Target_Encoder>=0)  	OLED_ShowString(98,30,"+");
									OLED_ShowNumber(104,30,MotorA.Target_Encoder*1000,4,12);

		//=============第五行显示右电机PWM与速度数据=======================//		
								OLED_ShowString(00,40,"R-P:");
		if(MotorB.Motor_Pwm<0)	OLED_ShowString(30,40,"-"),
								OLED_ShowNumber(36,40,MotorB.Motor_Pwm,4,12);
		if(MotorB.Motor_Pwm>=0)	OLED_ShowString(30,40,"+"),
								OLED_ShowNumber(36,40,MotorB.Motor_Pwm,4,12);
										
								OLED_ShowString(66,40,"R-E:");

		if(MotorB.Target_Encoder<0)		OLED_ShowString(98,40,"-");
		if(MotorB.Target_Encoder>=0)  	OLED_ShowString(98,40,"+");
									OLED_ShowNumber(104,40,MotorB.Target_Encoder*1000,4,12);
		//=============第六行显示模式，电压与电机开关=======================//
		
//		if(Car_Num == Akm_Car)				OLED_ShowString(0,50,"Akm  ");
//		else if(Car_Num == Diff_Car)		OLED_ShowString(0,50,"Diff ");
//		else if(Car_Num == Small_Tank_Car)	OLED_ShowString(0,50,"STank");
//		else 								OLED_ShowString(0,50,"BTank");
								OLED_ShowString(0,50,"CCD ");				//模式显示
								OLED_ShowString(54,50,".");
								OLED_ShowString(78,50,"V");
								OLED_ShowNumber(40,50,Voltage/100,2,12);
								OLED_ShowNumber(62,50,Voltage/10%10,1,12);//显示电压
								OLED_ShowNumber(70,50,Voltage%10,1,12);
		if(Flag_Stop)          	OLED_ShowString(105,50, "OFF");
		if(!Flag_Stop)        	OLED_ShowString(105,50, "ON ");

	}
	else
	{
		//==================第二行显示小车模式方式==================//	
												OLED_ShowString(0,0, "Mode:");	//显示模式
		if(Mode==Normal_Mode)					OLED_ShowString(50,0,"Normal ");
		else if(Mode==Lidar_Avoid_Mode)			OLED_ShowString(50,0,"Avoid  ");
		else if(Mode==Lidar_Follow_Mode)		OLED_ShowString(50,0,"Follow ");
		else if(Mode==Lidar_Along_Mode)		  OLED_ShowString(50,0,"Along  ");
//		else if(Mode==Measure_Distance_Mode)	OLED_ShowString(50,0,"Measure");//超声波测量距离

		//====================第二行显示小车控制方式================//
		if(Mode==Lidar_Avoid_Mode||Mode==Lidar_Follow_Mode||Mode==Lidar_Along_Mode)
		{
			if(Lidar_Success_Receive_flag==1)
			   OLED_ShowString(0,10,"Success   ");
		    else
			   OLED_ShowString(0,10,"Failure   ");
		}
		else if(Mode==Normal_Mode)
		{
		    if(APP_ON_Flag==RC_ON){		
				OLED_ShowString(0,10,"APP   Avoid:");
				if(Avoid_Flag==Lidar_Avoid_ON&&Lidar_Success_Receive_flag) OLED_ShowString(100,10,"O_N");
				else OLED_ShowString(100,10,"OFF");
			}	//蓝牙
			else if(PS2_ON_Flag==RC_ON){	
				OLED_ShowString(0,10,"PS2   Avoid:");
				if(Avoid_Flag==Lidar_Avoid_ON&&Lidar_Success_Receive_flag) OLED_ShowString(100,10,"O_N");
				else OLED_ShowString(100,10,"OFF");
			}	//PS2手柄
		    else if(ROS_ON_Flag==RC_ON) OLED_ShowString   (0,10,"ROS             ");	//ROS
			else if(Remote_ON_Flag==RC_ON) OLED_ShowString(0,10,"R-C             ");
			else						OLED_ShowString    (0,10,"LINKING...      ");	//两个都没有连接，显示待连接中
			
		}
//			if(Mode==Measure_Distance_Mode)
//										OLED_ShowString(110,10,"mm"),		
//										OLED_ShowNumber(84,10,Distance1,4,12);//显示超声波测试的距离
									  
		//================第三行显示速度====================//	
		if(Car_Num == Akm_Car)//阿克曼车型转向速度显示角度
		{
									OLED_ShowString(00,20,"SPEED:");
			if( RC_Velocity<0)		OLED_ShowString(50,20,"-");
			else					OLED_ShowString(50,20,"+");
									OLED_ShowNumber(60,20,myabs(RC_Velocity),4,12);
			
			if(APP_ON_Flag == RC_ON)
				OLED_ShowString(98,20,"Pi/5");
			else if(PS2_ON_Flag == RC_ON)
				OLED_ShowString(98,20,"Pi/4");
			else if(Remote_ON_Flag == RC_ON)
				OLED_ShowString(98,20,"Pi/4");
		}
		else//其他车型转向速度显示差速转向速度
		{
									OLED_ShowString(00,20,"SPEED:");
			if( RC_Velocity<0)		OLED_ShowString(50,20,"-");
			else					OLED_ShowString(50,20,"+");
									OLED_ShowNumber(60,20,myabs(RC_Velocity),4,12);
			if( RC_Turn_Velocity<0)		OLED_ShowString(94,20,"-");
			else					OLED_ShowString(94,20,"+");
									OLED_ShowNumber(106,20,myabs(RC_Turn_Velocity),3,12);
			
		}

		//=============第四行显示左电机PWM与速度数据=======================//		
								OLED_ShowString(00,30,"L-P:");
		if(MotorA.Motor_Pwm<0)	{OLED_ShowString(30,30,"-"),
								OLED_ShowNumber(36,30,-MotorA.Motor_Pwm,4,12);}
		if(MotorA.Motor_Pwm>=0)	{OLED_ShowString(30,30,"+"),
								OLED_ShowNumber(36,30,MotorA.Motor_Pwm,4,12);}
										
								OLED_ShowString(66,30,"L-E:");

		if(MotorA.Target_Encoder<0)	  	{OLED_ShowString(94,30,"-");OLED_ShowNumber(104,30,-MotorA.Target_Encoder*1000,4,12);}
		if(MotorA.Target_Encoder>=0)  	{OLED_ShowString(94,30,"+");
									OLED_ShowNumber(104,30,MotorA.Target_Encoder*1000,4,12);}

		//=============第五行显示右电机PWM与速度数据=======================//		
								OLED_ShowString(00,40,"R-P:");
		if(MotorB.Motor_Pwm<0){	OLED_ShowString(30,40,"-"),
								OLED_ShowNumber(36,40,-MotorB.Motor_Pwm,4,12);}
		if(MotorB.Motor_Pwm>=0)	{OLED_ShowString(30,40,"+"),
								OLED_ShowNumber(36,40,MotorB.Motor_Pwm,4,12);}
										
								OLED_ShowString(66,40,"R-E:");
		if(MotorB.Target_Encoder<0)		{OLED_ShowString(94,40,"-");OLED_ShowNumber(104,40,-MotorB.Target_Encoder*1000,4,12);}
		if(MotorB.Target_Encoder>=0)  	{OLED_ShowString(94,40,"+");
									OLED_ShowNumber(104,40,MotorB.Target_Encoder*1000,4,12);}

		//=============第六行显示车型，电压与电机开关=======================//
		if(Car_Num == Akm_Car)				OLED_ShowString(0,50,"AKM  ");
		else if(Car_Num == Diff_Car)		OLED_ShowString(0,50,"DIFF ");
		else if(Car_Num == Small_Tank_Car)	OLED_ShowString(0,50,"STANK");
		else 								OLED_ShowString(0,50,"BTANK");
	
								OLED_ShowString(62,50,".");
								OLED_ShowString(86,50,"V");
								OLED_ShowNumber(48,50,Voltage/100,2,12);
								OLED_ShowNumber(70,50,Voltage/10%10,1,12);//显示电压
								OLED_ShowNumber(78,50,Voltage%10,1,12);
		if(Flag_Stop)          	OLED_ShowString(105,50, "OFF");
		else        	          OLED_ShowString(105,50, "ON ");
										
	}
	//==================刷新==================//
	OLED_Refresh_Gram();	
}

/**************************************************************************
Function: OLED_Show_CCD
Input   : none
Output  : none
函数功能：CCD模式显示函数，画点
入口参数: 无 
返回  值：无
**************************************************************************/	 	

void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t)
{ 
	u8 i=0;
	OLED_DrawPoint(x,y,t);
	OLED_DrawPoint(x,y,t);
	for(i = 0;i<8; i++)
	{
		OLED_DrawPoint(x,y+i,t);
	}
}

void OLED_Show_CCD(void)
{ 
	u8 i,t;
	for(i = 0;i<128; i++)
	{
		if(CCD_ADV[i]<CCD_Threshold) t=1; else t=0;
		OLED_DrawPoint_Shu(i,0,t);
	}
}

/**************************************************************************
Function: Car_Select_Show
Input   : none
Output  : none
函数功能：车型选择显示
入口参数: 无 
返回  值：无
**************************************************************************/	 	
void Car_Select_Show(void)
{						  
	OLED_ShowString(0,00,"Rotate Resistor");
	OLED_ShowString(0,10,"To Select Car");
	OLED_ShowString(0,20,"Current Car Is");
	if(Car_Num==Diff_Car)         			OLED_ShowString(24,30," Diff Car ");//差速小车
	if(Car_Num==Akm_Car)        			OLED_ShowString(24,30,"  Akm Car ");//阿克曼
	if(Car_Num==Small_Tank_Car)				OLED_ShowString(24,30,"S_Tank Car");//小履带车
	if(Car_Num==Big_Tank_Car)				OLED_ShowString(24,30,"B_Tank Car");//大履带车
	
	OLED_ShowString(0,40,"Press User Key");
	OLED_ShowString(0,50,"To End Selection");
	
	OLED_Refresh_Gram();	
}

/**************************************************************************
Function: Send data to APP
Input   : none
Output  : none
函数功能：向APP发送数据
入口参数：无
返回  值：无
**************************************************************************/
void APP_Show(void)
{    
	static u8 flag;
	int Velocity_Left_Show,Velocity_Right_Show,Voltage_Show;
	Voltage_Show=(Voltage-1000)*2/3;			if(Voltage_Show<0)Voltage_Show=0;if(Voltage_Show>100) Voltage_Show=100;   //对电压数据进行处理
	Velocity_Right_Show=MotorB.Velocity*1.1; 	if(Velocity_Right_Show<0) Velocity_Right_Show=-Velocity_Right_Show;			  //对编码器数据就行数据处理便于图形化
	Velocity_Left_Show= MotorA.Velocity*1.1;  	if(Velocity_Left_Show<0) Velocity_Left_Show=-Velocity_Left_Show;
	flag=!flag;
	if(PID_Send==1)			//发送PID参数,在APP调参界面显示
	{
		 printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)(ELE_KP_Akm*100000),(int)(ELE_KD_Akm*100000),(int)0,(int)0,(int)0,(int)0,(int)0,0,0);//打印到APP上面
		 PID_Send=0;		
  }
   else	if(flag==0)		// 发送电池电压，速度，角度等参数，在APP首页显示
	   printf("{A%d:%d}$",(int)Voltage_Show,(int)0); //打印到APP上面
	 else				//发送小车姿态角，在波形界面显示
	    printf("{B%d:%d}$",(int)MotorA.Current_Encoder,(int)MotorB.Current_Encoder); //显示左右编码器的速度																	   //可按格式自行增加显示波形，最多可显示五个
}


//上位机示波器
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
	u8 i;			//计数变量
	u8 Send_Count;	//串口需要发送的数据个数
	float Vol;		//电压变量
	Vol=(float)Voltage/100;
	DataScope_Get_Channel_Data( MotorA.Velocity, 1 );     	//显示左轮速度，单位mm/s
	DataScope_Get_Channel_Data( MotorB.Velocity, 2 );    	//显示右轮速度
	DataScope_Get_Channel_Data( Vol, 3 );               	//显示电池电压 单位：V
//	DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(0, 7 );
//	DataScope_Get_Channel_Data( 0, 8 ); 
//	DataScope_Get_Channel_Data(0, 9 );  
//	DataScope_Get_Channel_Data( 0 , 10);
	Send_Count = DataScope_Data_Generate(CHANNEL_NUMBER);//CHANNEL_NUMBER可改变通道数量，目前是4
	for( i = 0 ; i < Send_Count; i++) 
	{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
	}
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：uint
作    者：平衡小车之家
**************************************************************************/
u16 myabs(int Input)
{
	int Output;
	if(Input > 0)
		Output = Input;
	else
		Output = -Input;
	return Output;
}



/**************************************************************************
Function: According to the potentiometer switch needs to control the car type
Input   : none
Output  : none
函数功能：根据电位器切换需要控制的小车类型
入口参数：无
返回  值：无
**************************************************************************/
void Robot_Select(void)
{
	u8 Car_Select_Count = 0;
	u32 Car_Select_Sum = 0;				//车型选择ADC相关变量
	//The ADC value is variable in segments, depending on the number of car models. Currently there are 6 car models, CAR_NUMBER=6
  //ADC值分段变量，取决于小车型号数量
	//车型选择电位计，adc每10次取一次平均
		for(Car_Select_Count=0;Car_Select_Count<2;Car_Select_Count++)
		{
			Car_Select_Sum += Get_Adc(CAR_ADC_CHANNEL);
			//delay_ms(1);
		}
		Car_Num = Car_Select_Sum/2/(Max_Car_ADC/Num_Of_Car);	//算出车型的号码
		Car_Select_Sum = 0;
	
}

