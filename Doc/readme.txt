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



MiniBalance:
├─STARTUP
│  └─startup_stm32f10x_hd.s			//stm32启动文件
│
├─CMSIS								//Cortex微处理器接口
│  ├─core_cm3.c
│  ├─core_cm3.h
│  └─......
│
├─FWIB								//stm32标准库文件
│  ├─misc.c
│  ├─stm32f10x_adc.c
│  ├─stm32f10x_bkp.c
│  └─......
│
├─USER								//主要是用户文件，main函数入口
│  ├─main.c							//main函数入口
│  ├─stm32f10x_conf.h					//配置文件等
│  ├─stm32f10x_it.c
│  ├─stm32f10x_it.h
│  └─Header.h							//公共头文件
│  
├─HARDWARE							//主要是硬件部分的代码
│  ├─bsp_beep.c				
│  ├─bsp_key.c
│  ├─bsp_led.c
│  ├─bsp_SysTick.c
│  └─......
│
├─CONTROL								//控制逻辑部分代码
│  ├─control.c						//控制函数在这，5ms中断
│  ├─show.c							//显示屏函数，app显示函数和上位机示波器
│  ├─bluetooth.c						//蓝牙部分控制代码
│  ├─pid.c							//pid反馈
│  ├─Lidar.c							//雷达串口接口和处理代码
│  ├─capture.c						//主要是捕获功能，未使用
│  └─ELE_CCD.c						//电磁巡线和CCD巡线
│
├─MPU6050								//MPU6050
│  ├─inv_mpu.c						//MPU6050相关库文件
│  ├─inv_mpu_dmp_motion_driver.c		//MPU6050相关库文件
│  ├─IOI2C.c							//I2C驱动文件
│  ├─MPU6050.c						//MPU6050驱动文件
│  ├─filter.c							//滤波文件，卡尔曼和互补滤波
│  └─MPU6050_Test.c					//MPU6050测试程序
│
└─DOC									//文档说明
   └─readme.txt

