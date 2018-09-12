#include "usart3.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "lcd.h"
#include "beep.h" 
#include "LinkList.h"

/*********************************************************************************
**********************�������� STM32F407������(�����)****************************
**********************************************************************************
* �ļ�����: usart3.c                                                             *
* �ļ�������USART3ʹ��                                                           *
* �������ڣ�2015.03.06                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    �������ô��ڵ������־���USART1����LED��������������                    * 
* ��    ���������̴��������ѧϰ�ο�                                             *
**********************************************************************************
*********************************************************************************/	

u8 uart_byte_count=0;
u8 Tx_Buf_Gsm[TX_LEN_GSM];
u8 USART_RX_BUF[USART3_REC_NUM];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 RX_buf[USART3_REC_NUM];


/****************************************************************************
* ��    ��: void uart3_init(u32 bound)
* ��    �ܣ�USART1��ʼ��
* ��ڲ�����bound��������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart3_init(u32 bound)
{   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ�� 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //��ʼ��PB10��PB11
   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //��������ж�
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
void Uart3_DMA_Init(u32 bound)
{
	
	//�����жϽṹ��
	NVIC_InitTypeDef NVIC_InitStructure ;
	//����IO��ʼ���ṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;
	//���崮�ڽṹ��
	USART_InitTypeDef USART_InitStructure;
	//����DAM�ṹ��
	DMA_InitTypeDef DMA_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  //����DMAʱ��
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;//DMA�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//ͨ������
	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
	
	//�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//����3���͵�ַ
	//
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Tx_Buf_Gsm;//����BUFF�ĵ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//��DMA���ⲿ���ڴ���
	DMA_InitStructure.DMA_BufferSize = TX_LEN_GSM;//���ͻ�������С
  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����DMA���ڴ�Ϊ����ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//����ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//DMA���ȼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
  // ���FIFOģʽ��ֱ�ӽ�����ָ����������ʹ��FIFO
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
	//ָ��FIFO��ֵˮƽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        
	//ָ��Burstת�������ڴ洫��
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       
	//ָ����Burstת��������Χת��
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//����DMA1ͨ�� 
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);  
	//ʹ���ж�
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  
  
	
	//���ڽ�������
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream1);     //����3���գ�DMA1��ͨ��4��������1
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART_RX_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = USART3_REC_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
	//
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        
	//
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       
	//  
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	//       
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);  
	//
	DMA_Cmd(DMA1_Stream1,ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ�� 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //��ʼ��PB10��PB11
	
   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1	
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //��������ж�
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	
	
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); 
	USART_Cmd(USART3, ENABLE); 
}
int Flag_Tx_Gsm_Busy=0;

//�������ݺ���
void Drv_Tx(uint8_t *data,uint16_t size)
{
	//
	while (Flag_Tx_Gsm_Busy);
	Flag_Tx_Gsm_Busy = 1;
	//
	memcpy(Tx_Buf_Gsm,data,size);
	//
	DMA_SetCurrDataCounter(DMA1_Stream3,size);
	//
 	DMA_Cmd(DMA1_Stream3,ENABLE);
}

//DMA ��������жϴ���
uint8_t drv_deal_irq_tx_end(void)
{
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
    {
			//
			USART_ITConfig(USART3,USART_IT_TC,DISABLE);
			//
			Flag_Tx_Gsm_Busy = 0;
			return 1;
    } 
	return 0;
}


void DMA1_Stream3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF6) != RESET) 
	{
		//
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF6);
		//
		DMA_Cmd(DMA1_Stream3,DISABLE);
		//
		USART_ITConfig(USART3,USART_IT_TC,ENABLE);
	}
}

//����1����һ���ַ�
void uart3SendChar(u8 ch)
{      
	while((USART3->SR&0x40)==0);  
    USART3->DR = (u8) ch;      
}
/****************************************************************************
* ��    ��: void uart1SendChars(u8 *str, u16 strlen)
* ��    �ܣ�����1����һ�ַ���
* ��ڲ�����*str�����͵��ַ���
            strlen���ַ�������
* ���ز�������
* ˵    ���� 
****************************************************************************/
void uart3SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart3SendChar(*(str + k)); k++; }   //ѭ������,ֱ���������   
    while (k < strlen); 
} 

//����1�жϷ������
void USART3_IRQHandler(void)  
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
			USART3->SR;
			USART3->DR; 
			//
			DMA_Cmd(DMA1_Stream1,DISABLE);
			//
			DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF5);
			//
			RX_len = USART3_REC_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
			memcpy(RX_buf,USART_RX_BUF,RX_len);
			//
			DMA_SetCurrDataCounter(DMA1_Stream1,USART3_REC_NUM);
			//
			DMA_Cmd(DMA1_Stream1,ENABLE);
	    //	 
		  RX_Finish=1;
  } 
} 


