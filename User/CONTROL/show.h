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


#ifndef __SHOW_H
#define	__SHOW_H

#include "stm32f10x.h"
#include "./OLED/bsp_oled.h"


#define CHANNEL_NUMBER    3				//示波器通道数，更改这个显示通道数


void Show(void);
void Car_Select_Show(void);

u16 myabs(int Input);
void DataScope(void);
void APP_Show(void);
void OLED_DrawPoint_Shu(u8 x,u8 y,u8 t);
void OLED_Show_CCD(void);
void Robot_Select(void);


#endif
