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

#include "./Motor/bsp_motor.h"   

/**************************************************************************
Function: Motor_Init
Input   : TIM_Period,TIM_Prescaler
Output  : none
函数功能：控制电机的PWM初始化
入口参数: 预装载值和预分频器 
返回  值：无
**************************************************************************/	 	
void Motor_Init(u16 arr,u16 psc)
{
	PWM_OutPut_TIM_Init(arr,psc);//初始化定时器
}



/**************************************************************************
Function: Servo_Init
Input   : TIM_Period,TIM_Prescaler
Output  : none
函数功能：舵机pwm输出初始化
入口参数: 预装载值和预分频器 
返回  值：无
**************************************************************************/	 	
void Servo_Init(u16 arr,u16 psc)
{	
	GPIO_InitTypeDef GPIO_InitStruct;                             
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	SERVO_TIM_APBxClock_FUN(SERVO_TIM_CLK,ENABLE);   	//使能定时器1时钟
	RCC_APB2PeriphClockCmd(SERVO_GPIO_CLK,ENABLE);  	//使能GPIOA的时钟

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        //复用推挽输出
	GPIO_InitStruct.GPIO_Pin = SERVO_GPIO_PIN;          //PA8
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SERVO_GPIO_PORT,&GPIO_InitStruct);

	TIM_TimeBaseInitStruct.TIM_Period = arr;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_Prescaler  = psc;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;    //设置时钟分割
	TIM_TimeBaseInit(SERVO_TIM,&TIM_TimeBaseInitStruct);       	//初始化定时器

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             	//选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState  = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStruct.TIM_Pulse = 0;                            	//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     	//设置输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	SERVO_TIM_OCXInit_FUN(SERVO_TIM,&TIM_OCInitStruct);         //初始化输出比较参数，通道1

	SERVO_TIM_OCXPreloadConfig_FUN(SERVO_TIM,TIM_OCPreload_Enable);  	//CH1使能预装载寄存器

	TIM_CtrlPWMOutputs(SERVO_TIM,ENABLE);  							 	//高级定时器输出需要设置这句，通用定时器需去掉

	TIM_ARRPreloadConfig(SERVO_TIM, ENABLE);               			 	//使能TIM1在ARR上的预装载寄存器

	TIM_Cmd(SERVO_TIM,ENABLE);                              			//使能定时器1

		TIM1->CCR1=Servo_PWM;

}

/**************************************************************************
Function: Drive_Motor
Input   : motor_a,motor_b:Indicates the left/right motor PWM duty cycle
Output  : none
函数功能：驱动电机
入口参数: motor_a和motor_b数值指示左右电机PWM占空比，值越大速度越快
返回  值：无
**************************************************************************/	 	
void Set_Pwm(int motor_a,int motor_b)
{
	
	if(Car_Num==Akm_Car)//阿克曼车需要控制舵机
		SERVO_TIM_SetCompareX_FUN(SERVO_TIM,Servo_PWM);
	if(motor_a<0)		PWMA_IN1=7200,PWMA_IN2=7200+motor_a;
	else 	            PWMA_IN2=7200,PWMA_IN1=7200-motor_a;

	if(motor_b<0)		PWMB_IN1=7200,PWMB_IN2=7200+motor_b;
	else 	            PWMB_IN2=7200,PWMB_IN1=7200-motor_b;
}


/************************************END OF FILE*******************************/







