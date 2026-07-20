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

#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"
#include "Header.h"

/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
//只需修改宏定义
//这里输出PWM
//使用定时器TIM3
#define            PWM_OutPut_TIM                   TIM3
#define            PWM_OutPut_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            PWM_OutPut_TIM_CLK               RCC_APB1Periph_TIM3
//#define            GENERAL_TIM_Period            9
//#define            GENERAL_TIM_Prescaler         71
// TIM3 输出比较通道1
#define            PWM_OutPut_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            PWM_OutPut_TIM_CH1_PORT          GPIOA
#define            PWM_OutPut_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 输出比较通道2
#define            PWM_OutPut_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            PWM_OutPut_TIM_CH2_PORT          GPIOA
#define            PWM_OutPut_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 输出比较通道3
#define            PWM_OutPut_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWM_OutPut_TIM_CH3_PORT          GPIOB
#define            PWM_OutPut_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 输出比较通道4
#define            PWM_OutPut_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWM_OutPut_TIM_CH4_PORT          GPIOB
#define            PWM_OutPut_TIM_CH4_PIN           GPIO_Pin_1



//这里使用定时功能
//使用定时器TIM5
#define 			TIMING_TIM					 	TIM5
#define            	TIMING_TIM_APBxClock_FUN     	RCC_APB1PeriphClockCmd
#define            	TIMING_TIM_CLK               	RCC_APB1Periph_TIM5
#define            	TIMING_TIM_IRQ               	TIM5_IRQn
#define           	TIMING_TIM_IRQHandler        	TIM5_IRQHandler



/***************************函数声明*******************************/

void PWM_OutPut_TIM_Init(u16 arr,u16 psc);
void TIMING_TIM_Init(u16 arr,u16 psc);

#endif	/* __BSP_GENERALTIME_H */


