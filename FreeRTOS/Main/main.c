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
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: ����1 LED�����������main()                                          *
* �ļ�������LED�����                                                            *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
* ��    ���������̴��������ѧϰ�ο�                                             *
**********************************************************************************
*********************************************************************************/

/*******************���������ͨ��λ������ʵ��IO�ڿ���***************************/

volatile uint32_t ulHighFrequencyTimerTicks;


//�������ȼ�
#define START_TASK_PRIO			1
//�����ջ��С	
#define START_STK_SIZE 			256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

int main(void)
{ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����2
	
	delay_init();		  //��ʼ����ʱ����
	Tim2_Init(50-1,720-1);//Timer2��ʼ��������LED����
	LED_Init();		    //��ʼ��LED�˿�
	KEY_Init();
	W25QXX_Init();			//W25QXX��ʼ�� 
	LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
	
	Usart3_Init(230400);
	
		//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();        										  //�����������
 
}



