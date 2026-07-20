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

#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include <stdio.h>
#include "Header.h"

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
//串口1-USART1
//调试串口

#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler



 //串口2-USART2
 //空闲串口
#define  UNUSED_USARTx                   USART2
#define  UNUSED_USART_CLK                RCC_APB1Periph_USART2
#define  UNUSED_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  UNUSED_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  UNUSED_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  UNUSED_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  UNUSED_USART_TX_GPIO_PORT       GPIOA   
#define  UNUSED_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  UNUSED_USART_RX_GPIO_PORT       GPIOA
#define  UNUSED_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  UNUSED_USART_IRQ                USART2_IRQn
#define  UNUSED_USART_IRQHandler         USART2_IRQHandler




// 串口3-USART3
//蓝牙的串口
#define  BLUETOOTH_USARTx                   USART3
#define  BLUETOOTH_USART_CLK                RCC_APB1Periph_USART3
#define  BLUETOOTH_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  BLUETOOTH_USART_BAUDRATE           230400

// USART GPIO 引脚宏定义
#define  BLUETOOTH_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  BLUETOOTH_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  BLUETOOTH_USART_TX_GPIO_PORT       GPIOB   
#define  BLUETOOTH_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  BLUETOOTH_USART_RX_GPIO_PORT       GPIOB
#define  BLUETOOTH_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  BLUETOOTH_USART_IRQ                USART3_IRQn
#define  BLUETOOTH_USART_IRQHandler         USART3_IRQHandler



//// 串口4-UART4
////预留openmv
//#define  OPENMV_USARTx                   UART4
//#define  OPENMV_USART_CLK                RCC_APB1Periph_UART4
//#define  OPENMV_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  OPENMV_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  OPENMV_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  OPENMV_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  OPENMV_USART_TX_GPIO_PORT       GPIOC   
//#define  OPENMV_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  OPENMV_USART_RX_GPIO_PORT       GPIOC
//#define  OPENMV_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  OPENMV_USART_IRQ                UART4_IRQn
//#define  OPENMV_USART_IRQHandler         UART4_IRQHandler




//串口5-UART5
//雷达串口
#define  LIDAR_USARTx                   UART5
#define  LIDAR_USART_CLK                RCC_APB1Periph_UART5
#define  LIDAR_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  LIDAR_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  LIDAR_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  LIDAR_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  LIDAR_USART_TX_GPIO_PORT       GPIOC   
#define  LIDAR_USART_TX_GPIO_PIN        GPIO_Pin_12
#define  LIDAR_USART_RX_GPIO_PORT       GPIOD
#define  LIDAR_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  LIDAR_USART_IRQ                UART5_IRQn
#define  LIDAR_USART_IRQHandler         UART5_IRQHandler




void DEBUG_USART_Init(void);
void BLUETOOTH_USART_Init(void);
void LIDAR_USART_Init(void);

#endif /* __USART_H */
