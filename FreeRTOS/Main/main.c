#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
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
	LED_Init();		    //��ʼ��LED�˿�
		//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
 
}

void start_task(void *pvParameters)
{ 
	while(1)
	{
    LED0=0;     //LED0��
    LED1=1;     //LED1��
    LED2=1;     //LED2��
		
    delay_ms(500);
		LED0=1;     //LED0��
    LED1=0;     //LED1��
    LED2=1;     //LED2��
		 
		delay_ms(500);
		LED0=1;     //LED0��
    LED1=1;     //LED1��
    LED2=0;     //LED2��
    delay_ms(500);
	}
}


