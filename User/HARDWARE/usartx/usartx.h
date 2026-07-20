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
#ifndef __USRATX_H
#define __USRATX_H 

#include "Header.h"


#define FRAME_HEADER      0X7B //Frame_header //帧头
#define FRAME_TAIL        0X7D //Frame_tail   //帧尾
#define SEND_DATA_SIZE    24
#define RECEIVE_DATA_SIZE 11

//串口2-USART2
//ROS串口

#define  ROS_USARTx                   USART1
#define  ROS_USART_CLK                RCC_APB2Periph_USART1
#define  ROS_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  ROS_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  ROS_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  ROS_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  ROS_USART_TX_GPIO_PORT       GPIOA   
#define  ROS_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  ROS_USART_RX_GPIO_PORT       GPIOA
#define  ROS_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  ROS_USART_IRQ                USART2_IRQn
#define  ROS_USART_IRQHandler         USART2_IRQHandler

//阿克曼车型的最小转弯半径，由机械结构决定：轮距、轴距、前轮最大转角
#define MINI_AKM_MIN_TURN_RADIUS 0.350f 

/*****A structure for storing triaxial data of a gyroscope accelerometer*****/
/*****用于存放陀螺仪加速度计三轴数据的结构体*********************************/
typedef struct __Mpu6050_Data_ 
{
	short X_data; //2 bytes //2个字节
	short Y_data; //2 bytes //2个字节
	short Z_data; //2 bytes //2个字节
}Mpu6050_Data;

/*******The structure of the serial port sending data************/
/*******串口发送数据的结构体*************************************/
typedef struct _SEND_DATA_  
{
	unsigned char buffer[SEND_DATA_SIZE];
	struct _Sensor_Str_
	{
		unsigned char Frame_Header; //1个字节
		short X_speed;	            //2 bytes //2个字节
		short Y_speed;              //2 bytes //2个字节
		short Z_speed;              //2 bytes //2个字节
		short Power_Voltage;        //2 bytes //2个字节
		Mpu6050_Data Accelerometer; //6 bytes //6个字节
		Mpu6050_Data Gyroscope;     //6 bytes //6个字节	
		unsigned char Frame_Tail;   //1 bytes //1个字节
	}Sensor_Str;
}SEND_DATA;

typedef struct _RECEIVE_DATA_  
{
	unsigned char buffer[RECEIVE_DATA_SIZE];
	struct _Control_Str_
	{
		unsigned char Frame_Header; //1 bytes //1个字节
		float X_speed;	            //4 bytes //4个字节
		float Y_speed;              //4 bytes //4个字节
		float Z_speed;              //4 bytes //4个字节
		unsigned char Frame_Tail;   //1 bytes //1个字节
	}Control_Str;
}RECEIVE_DATA;


void data_transition(void);
void USART1_SEND(void);
float float_abs(float insert);

int USART1_IRQHandler(void);

float Vz_to_Akm_Angle(float Vx, float Vz);
float XYZ_Target_Speed_transition(u8 High,u8 Low);

void usart1_send(u8 data);


u8 Check_Sum(unsigned char Count_Number,unsigned char Mode);


#endif
