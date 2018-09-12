#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"
#include "usart3.h"
#include "task.h"
#include "lcd.h"
#include "w25qxx.h" 
#include "key.h"
/*********************************************************************************
*********************启明欣欣 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程1 LED跑马灯主函数main()                                          *
* 文件简述：LED跑马灯                                                            *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

/*******************下面代码是通过位带操作实现IO口控制***************************/

volatile uint32_t ulHighFrequencyTimerTicks;


//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

int main(void)
{ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组2
	
	delay_init();		  //初始化延时函数
	Tim2_Init(50-1,720-1);//Timer2初始化，用于LED控制
	LED_Init();		    //初始化LED端口
	KEY_Init();
	W25QXX_Init();			//W25QXX初始化 
	LCD_Init();           //初始化LCD FSMC接口和显示驱动
	
	Usart3_Init(230400);
	
		//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();        										  //开启任务调度
 
}



