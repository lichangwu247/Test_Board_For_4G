#ifndef __LED_H
#define __LED_H
#include "common.h"

////////////////////////////////////////////////////////////////////////////////////	

//LED端口定义

#define LED0 PEout(3)	 
#define LED1 PEout(4)	 
#define LED2 PGout(9)	  

typedef struct _ledDEF
{
    uint32_t ledOnTime;
		uint32_t ledOffTime;
		uint32_t ledCounter;
		uint8_t ledStatus;
		uint8_t ledEnable;
		uint32_t ledTimes;
} LED_TYPE;

#define LED_PORT3 GPIOE
#define LED3			 GPIO_Pin_3
#define LED_PORT4 GPIOE
#define LED4			 GPIO_Pin_4
#define LED_PORT5 GPIOG
#define LED5			 GPIO_Pin_9

#define LED_TURN_ON	 1
#define LED_TURN_OFF 0

#define LEDON			  1
#define LEDOFF			0

//函数声明
void LED_Init(void);//初始化	
void CtrLed(uint32_t timeInterval); //在定时器中周期执行此函数，
void LedSetStatus(uint32_t onTime,uint32_t offTime,uint32_t times);		

#endif

