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

#include "pid.h"

//float Velocity_KP = 0.18f,Velocity_KI = 0.17f;	//增量式PI参数，用于电机速度控制，光电编码器参数
float Velocity_KP = 200,Velocity_KI = 140;			//增量式PI参数，用于电机速度控制，霍尔编码器参数

//雷达小车走直线各车型PID参数
float Diff_Along_Distance_KP = 0.00080f,Diff_Along_Distance_KD = 120.0f,Diff_Along_Distance_KI = 0.00001f;
float Akm_Along_Distance_KP = 0.000415f,Akm_Along_Distance_KD = 1000.245f,Akm_Along_Distance_KI = 0.00001f;	
float STank_Along_Distance_KP = 0.018,STank_Along_Distance_KD = 6.8880f,STank_Along_Distance_KI = 0.00001f;
float BTank_Along_Distance_KP = 0.018,BTank_Along_Distance_KD = 2.8880f,BTank_Along_Distance_KI = 0.00001f;


//雷达跟随各车型PID参数
float Follow_KP_BTank = 0.01072f,Follow_KD_BTank = 0.0461f,Follow_KI_BTank = 0.0001f;	
float Follow_KP_Akm = 0.015f,Follow_KD_Akm = 0.0182f,Follow_KI_Akm = -0.0001f;			
float Follow_KP_STank = 0.0138f,Follow_KD_STank = 0.0446f,Follow_KI_STank = 0.0001f;	
float Follow_KP_Diff = 0.01f,Follow_KD_Diff = 0.025f,Follow_KI_Diff = 0.0001f;		

float Distance_KP =0.001685,Distance_KD = 0.25557 ,Distance_KI = 0.00001;	//距离调整PID参数


//各车型电磁巡线PID参数,其中K是非常规PID的一项参数，调节非线性变化
float ELE_KP_Diff = 0.01642f,ELE_KD_Diff = 3.0f,ELE_KI_Diff = 0.00012f,ELE_K_Diff = 0.00018f;
float ELE_KP_Akm = 0.00508f,ELE_KD_Akm = 0.008f,ELE_KI_Akm = 0.00012f,ELE_K_Akm = 0.00028f;
float ELE_KP_STank = 0.04368,ELE_KD_STank = 3.41,ELE_KI_STank = 0.0002,ELE_K_STank = 0.0002;
float ELE_KP_BTank = 0.01058,ELE_KD_BTank = 4.182,ELE_KI_BTank = 0.0005,ELE_K_BTank = 0.0004; 


//各车型CCD巡线PID参数
float CCD_KP_Akm = 0.0126,CCD_KD_Akm = 0.0262,CCD_KI_Akm = 0.0001;
float CCD_KP_Diff = 0.02166,CCD_KD_Diff = 0.3100,CCD_KI_Diff = 0.0001;
float CCD_KP_STank = 0.05772,CCD_KD_STank = 0.01186,CCD_KI_STank = 0.0001;
float CCD_KP_BTank = 0.03345,CCD_KD_BTank = 0.01902,CCD_KI_BTank = 0.0001;


/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/

int Incremental_PI_Left (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                					//计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   	//增量式PI控制器
//	 if(Pwm>7200)Pwm=7200;
//	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   					//保存上一次偏差 
	 return Pwm;                         					//增量输出
}


int Incremental_PI_Right (float Encoder,float Target)
{ 	
	 static float Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                					//计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   	//增量式PI控制器
//	 if(Pwm>7200)Pwm=7200;
//	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   					//保存上一次偏差 
	 return Pwm;                         					//增量输出
}

/**************************************************************************
Function: Distance_Adjust_PID
Input   : Current_Distance;Target_Distance
Output  : OutPut
函数功能：走直线雷达距离pid
入口参数: 当前距离和目标距离
返回  值：电机目标速度
**************************************************************************/	 	
//走直线雷达距离调整pid

float Along_Adjust_PID(float Current_Distance,float Target_Distance)//距离调整PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Distance-Current_Distance;                          	//计算偏差
	Integral_bias+=Bias;	                                 			//求出偏差的积分
	if(Integral_bias>1000) Integral_bias=1000;
	else if(Integral_bias<-1000) Integral_bias=-1000;
	if(Car_Num == Diff_Car)
		OutPut=-Diff_Along_Distance_KP*Bias-Diff_Along_Distance_KI*Integral_bias-Diff_Along_Distance_KD*(Bias-Last_Bias);//位置式PID控制器
	else if(Car_Num == Akm_Car)
		OutPut=-Akm_Along_Distance_KP*Bias-Akm_Along_Distance_KI*Integral_bias-Akm_Along_Distance_KD*(Bias-Last_Bias);//位置式PID控制器
	else if(Car_Num == Small_Tank_Car)
		OutPut=-STank_Along_Distance_KP*Bias-STank_Along_Distance_KI*Integral_bias-STank_Along_Distance_KD*(Bias-Last_Bias);
	else
		OutPut=-BTank_Along_Distance_KP*Bias-BTank_Along_Distance_KI*Integral_bias-BTank_Along_Distance_KD*(Bias -Last_Bias);
	Last_Bias=Bias;                                       		 			//保存上一次偏差
	if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)								//电机关闭，此时积分清零
		Integral_bias = 0;
	return OutPut;                                           					 	//输出                                        	
}


/**************************************************************************
Function: Follow_Turn_PID
Input   : Current_Angle;Target_Angle
Output  : OutPut
函数功能：雷达转向pid
入口参数: 当前角度和目标角度
返回  值：电机转向速度
**************************************************************************/	 	
//雷达转向pid
float Follow_Turn_PID(float Current_Angle,float Target_Angle)
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Angle-Current_Angle;                         				 //计算偏差
	Integral_bias+=Bias;	                                 				 //求出偏差的积分
	if(Integral_bias>1000) Integral_bias=1000;
	else if(Integral_bias<-1000) Integral_bias=-1000;
	if(Car_Num == Akm_Car)//阿克曼车专用参数
		OutPut=Follow_KP_Akm*Bias+Follow_KI_Akm*Integral_bias+Follow_KD_Akm*(Bias-Last_Bias);//位置式PID控制器
	else if(Car_Num == Small_Tank_Car)
		OutPut=Follow_KP_STank*Bias+Follow_KI_STank*Integral_bias+Follow_KD_STank*(Bias-Last_Bias);	//位置式PID控制器
	else if(Car_Num == Diff_Car)
		OutPut=Follow_KP_Diff*Bias+Follow_KI_Diff*Integral_bias+Follow_KD_Diff*(Bias-Last_Bias);	//位置式PID控制器
	else
		OutPut=Follow_KP_BTank*Bias+Follow_KI_BTank*Integral_bias+Follow_KD_BTank*(Bias-Last_Bias);	//位置式PID控制器
	Last_Bias=Bias;                                       					 		//保存上一次偏差
	if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)								//电机关闭，此时积分清零
		Integral_bias = 0;
	return OutPut;                                           					 	//输出
	
}

/**************************************************************************
Function: Distance_Adjust_PID
Input   : Current_Distance;Target_Distance
Output  : OutPut
函数功能：雷达转向pid
入口参数: 当前距离和目标距离
返回  值：电机目标速度
**************************************************************************/	 	
//雷达距离调整pid
float Distance_Adjust_PID(float Current_Distance,float Target_Distance)//距离调整PID
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Distance-Current_Distance;                          	//计算偏差
	Integral_bias+=Bias;	                                 			//求出偏差的积分
	if(Integral_bias>1000) Integral_bias=1000.0;
	else if(Integral_bias<-1000) Integral_bias=-1000.0;
	OutPut=-Distance_KP*Bias-Distance_KI*Integral_bias-Distance_KD*(Bias-Last_Bias);//位置式PID控制器
	Last_Bias=Bias;                                       		 			//保存上一次偏差
	if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)						//电机关闭，此时积分清零
		Integral_bias = 0;
	return OutPut;                                          	
}

/**************************************************************************
Function: ELE_PID
Input   : Current_ELE_ADC;Target_ELE_ADC
Output  : OutPut
函数功能：电磁巡线PID
入口参数: 当前电磁巡线ADC和目标ADC
返回  值：电机目标速度
**************************************************************************/	 	
float ELE_PID(int Current_ELE_ADC,int Target_ELE_ADC )
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_ELE_ADC-Current_ELE_ADC;                        //计算偏差
	Integral_bias+=Bias;	                                 	//求出偏差的积分
//	if(Integral_bias>5000) Integral_bias=5000;
//	else if(Integral_bias<-5000) Integral_bias=-5000;
	if(Car_Num == Diff_Car)										//车型不同，参数不同
		OutPut=-ELE_KP_Diff*Bias-ELE_KI_Diff*Integral_bias-ELE_KD_Diff*(Bias-Last_Bias)-ELE_K_Diff*myabs(Bias)*Bias;//位置式PID控制器
	else if(Car_Num == Akm_Car)
		OutPut=-ELE_KP_Akm*Bias-ELE_KD_Akm*(Bias-Last_Bias);//-ELE_K_Akm*myabs(Bias)*Bias;
	else if(Car_Num == Small_Tank_Car)
		OutPut=-ELE_KP_STank*Bias-ELE_KI_STank*Integral_bias-ELE_KD_STank*(Bias-Last_Bias)-ELE_K_STank*myabs(Bias)*Bias;
	else																					
		OutPut=-ELE_KP_BTank*Bias-ELE_KI_BTank*Integral_bias-ELE_KD_BTank*(Bias-Last_Bias)-ELE_K_BTank*myabs(Bias)*Bias;
	Last_Bias=Bias;                                       		//保存上一次偏差
	if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)			//电机关闭，此时积分清零
		Integral_bias = 0;
	return OutPut;                                          	//输出
}
/**************************************************************************
Function: CCD_PID
Input   : Current_Value;Target_Value
Output  : OutPut
函数功能：CCD巡线PID
入口参数: 当前CCD的值和目标值
返回  值：电机目标速度
**************************************************************************/	 	
float CCD_PID(float Current_Value,float Target_Value )
{
	static float Bias,OutPut,Integral_bias,Last_Bias;
	Bias=Target_Value-Current_Value;                         	 	//计算偏差
	Integral_bias+=Bias;	                                 		//求出偏差的积分
	if(Integral_bias>5000) Integral_bias=5000;
	else if(Integral_bias<-5000) Integral_bias=-5000;
	if(Car_Num == Akm_Car)
		OutPut=(CCD_KP_Akm)*Bias+(CCD_KI_Akm)*Integral_bias+(CCD_KD_Akm)*(Bias-Last_Bias);//位置式PID控制器
	else if(Car_Num == Diff_Car)
		OutPut=CCD_KP_Diff*Bias+CCD_KI_Diff*Integral_bias+CCD_KD_Diff*(Bias-Last_Bias);
	else if(Car_Num == Small_Tank_Car)
		OutPut=CCD_KP_STank*Bias+CCD_KI_STank*Integral_bias+CCD_KD_STank*(Bias-Last_Bias);
	else
		OutPut=CCD_KP_BTank*Bias+CCD_KI_BTank*Integral_bias+CCD_KD_BTank*(Bias-Last_Bias);
	Last_Bias=Bias;                                       		//保存上一次偏差
	if(MotorA.Motor_Pwm == 0 && MotorB.Motor_Pwm == 0)			//电机关闭，此时积分清零
		Integral_bias = 0;
	return OutPut;                                       
}



