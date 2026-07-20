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

#ifndef __HEADER_H
#define __HEADER_H

//头文件包含
#include "stm32f10x.h"
#include "./SysTick/bsp_SysTick.h"
#include "./Led/bsp_led.h"
#include "./beep/bsp_beep.h"   
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart.h"
#include "./GeneralTim/bsp_GeneralTim.h" 
#include "./Motor/bsp_motor.h" 
#include "./OLED/bsp_oled.h"
#include "stdlib.h"
#include "./OLED/oledfont.h"  	 
#include "./CONTROL/show.h"
#include "./encoder/encoder.h"
#include "./adc/bsp_adc.h"
#include "bluetooth.h"  
#include "./PSTWO/pstwo.h"
#include "./DataScope_DP/DataScope_DP.h"
#include "pid.h"
#include "Lidar.h"
#include "control.h"
#include "capture.h"
#include "ELE_CCD.h"
#include "MPU6050.h"
#include "usartx.h"
#include "./OLED/bsp_oled.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "ioi2c.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "filter.h"
#include "stdio.h"
extern u16 Voltage;						//电压变量，放大100倍储存
extern u8 Flag_Stop;					//电机启停标志位
extern volatile u8 delay_50,delay_flag; 			//延时变量
extern u8 Car_Num;						//车型号码选择
extern u8 PS2_ON_Flag,APP_ON_Flag,ROS_ON_Flag,Remote_ON_Flag;		//默认所有方式不控制
extern u8 Flag_Show ;					//显示标志位，默认开启，长按切换到上位机模式，此时关闭
extern float Perimeter; 
extern float Wheelspacing; 
extern u16  DISTANCE,ANGLE;

extern u8 one_lap_data_success_flag,Lidar_Success_Receive_flag,Lidar_flag_count;
extern int lap_count,PointDataProcess_count,test_once_flag,Dividing_point,Avoid_Flag;
//车型选择的号码
#define    Diff_Car    						  	0			
#define    Akm_Car 							 	1
#define    Small_Tank_Car				  		2
#define    Big_Tank_Car							3



//#define Wheel_spacing         0.162f
//#define Diff_wheelSpacing     0.177f

//位带操作
// 这里只定义了 GPIO ODR和IDR这两个寄存器的位带别名区地址，其他寄存器的没有定义

//SRAM 位带区:    0X2000 0000~0X2010 0000
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X4010 0000
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
/*
 *addr & 0xF0000000，取地址的高4位，看看是2还是4，用于区分SRAM和外设地址，
 *如果是2，+0x02000000则=0X2200 0000，即是SRAM，如果是4，+0x02000000则=0X4200 0000，即是外设
 *
 *addr & 0x000FFFFFF，屏蔽掉高两位，相当于-0X2000 0000或者-0X4000 0000，结果表示偏移位带区多少个字节
 *<<5  等于*8*4，因为位带区一个地址表示一个字节，一个字节有8个bit，一个bit可以膨胀成一个字，即4个字节
 *<<2 等于*4，因为一个位可以膨胀成一个字，即4个字节
 *
 *分解成两条公式应该就是这样：
 *SRAM位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *外设位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 */

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 	//输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 	//输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} 		//输出反转状态


// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 


// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   


// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 



// 单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入  



#endif 
