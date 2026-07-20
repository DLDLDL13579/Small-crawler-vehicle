#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"
#include "Header.h"


/*JTAG模式设置定义*/
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	


/* 定义蜂鸣器连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的蜂鸣器引脚 */
#define BEEP_GPIO_PORT    	GPIOA			              	/* GPIO端口 */
#define BEEP_GPIO_CLK 	    RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define BEEP_GPIO_PIN		GPIO_Pin_15			        	/* 连接到蜂鸣器的GPIO */


/* 带参宏，可以像内联函数一样使用 */
#define BEEP(a)	if (a)	\
					GPIO_SetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN);\
					else		\
					GPIO_ResetBits(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

					
/* 定义控制IO的宏 */
#define BEEP_TOGGLE		    digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON		   		digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF			digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


					
void BEEP_GPIO_Config(void);
void JTAG_Set(u8 mode);
void Buzzer_Alarm(u16 count);

					
#endif /* __BEEP_H */
