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


#ifndef __ENCODER_H
#define	__ENCODER_H

#include "stm32f10x.h"


#define 	ENCODER_TIM_PERIOD 				(u16)(65535)  	 	//不可大于65535 因为F103的定时器是16位的。
#define	 	Encoder1  						4
#define 	Encoder2  						8

//编码器1定时器
#define  	ENCODER1_TIM                   	TIM4
#define  	ENCODER1_TIM_APBxClock_FUN    	RCC_APB1PeriphClockCmd
#define  	ENCODER1_TIM_CLK               	RCC_APB1Periph_TIM4


//编码器1端口
#define 	ENCODER1_GPIO_PORT    			GPIOB			              /* GPIO端口 */
#define 	ENCODER1_GPIO_CLK 	   	 		RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define 	ENCODER1_A_GPIO_PIN				GPIO_Pin_7			        /* 连接到A相的GPIO */
#define 	ENCODER1_B_GPIO_PIN				GPIO_Pin_6			        /* 连接到B相的GPIO */

//中断
#define  	ENCODER1_IRQ                	TIM4_IRQn
#define  	ENCODER1_IRQHandler         	TIM4_IRQHandler


//编码器2定时器
#define  	ENCODER2_TIM                   	TIM8
#define  	ENCODER2_TIM_APBxClock_FUN     	RCC_APB2PeriphClockCmd
#define  	ENCODER2_TIM_CLK               	RCC_APB2Periph_TIM8

//编码器2端口
#define	 	ENCODER2_GPIO_PORT    			GPIOC			              /* GPIO端口 */
#define	 	ENCODER2_GPIO_CLK 	    		RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define	 	ENCODER2_A_GPIO_PIN				GPIO_Pin_7			        /* 连接到A相的GPIO */
#define	 	ENCODER2_B_GPIO_PIN				GPIO_Pin_6			        /* 连接到B相的GPIO */


//中断
#define  	ENCODER2_IRQ                	TIM8_UP_IRQn
#define  	ENCODER2_IRQHandler         	TIM8_UP_IRQHandler



void Encoder_GPIO_Config(void);
void Encoder_NVIC_Config(void);
void Encoder_TIM_Mode_Config(void);
void Encoder_Init(void);
int Read_Encoder(u8 TIMX);


#endif
