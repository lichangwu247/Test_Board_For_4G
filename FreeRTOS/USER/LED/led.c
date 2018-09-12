#include "led.h" 

/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: led.c                                                                *
* �ļ�������LED��ʼ��                                                            *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����LED��ӦIO�ڳ�ʼ��                                                    * 
* �Ա����̣�https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

//LED��ӦIO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

  //PG13��PG14��PG15��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;         //LED0��LED1��LED2��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //����
  GPIO_Init(GPIOE, &GPIO_InitStructure);                         //��ʼ��GPIO
	
	GPIO_SetBits(GPIOE, GPIO_Pin_3 | GPIO_Pin_4);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOG, GPIO_Pin_9);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/*����LED*/
void LED_ON(void) 
{
	GPIO_ResetBits(LED_PORT3,LED3);
}

/*�ر�LED.PA81*/
void LED_OFF(void)
{
	GPIO_SetBits(LED_PORT3,LED3);
}

LED_TYPE	ledCtr;	

void LedSetStatus(uint32_t onTime,uint32_t offTime,uint32_t times)		
{
	ledCtr.ledOnTime = onTime;
	ledCtr.ledOffTime = offTime;
	ledCtr.ledCounter = 0;
	ledCtr.ledEnable = LED_TURN_ON;
	ledCtr.ledTimes = times;
}													


/*LED ��˸����*/
void LedCtr(LED_TYPE *ledCtr, uint32_t times)	
{
	if(ledCtr->ledEnable == LED_TURN_ON) 
	{
			if(ledCtr->ledCounter > times)
				ledCtr->ledCounter -= times;
			else ledCtr->ledCounter = 0;
				
			if(ledCtr->ledCounter == 0) 
			{
				if(ledCtr->ledTimes) 
				{
					ledCtr->ledTimes--;
					ledCtr->ledCounter = ledCtr->ledOffTime + ledCtr->ledOnTime;
					ledCtr->ledStatus = LEDON;
				}
			}
			
			if(ledCtr->ledCounter <= ledCtr->ledOffTime) 
				ledCtr->ledStatus = LEDOFF;
	}
}


void CtrLed(uint32_t timeInterval) //�ڶ�ʱ��������ִ�д˺�����
{
	LedCtr(&ledCtr, timeInterval);
	
	if(ledCtr.ledStatus == LEDON)
	{
		LED_ON();
	}
	else 
	{
		LED_OFF();
	}
}	
