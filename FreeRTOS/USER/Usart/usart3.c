#include "usart3.h"
#include "led.h"
/*********************************************************************************
**********************************************************************************
* �ļ�����: usart3.c                                                             *
* �ļ�������USART3ʹ��                                                           *
* �������ڣ�2018.08.06                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
* ��    ����                                                                     *
**********************************************************************************
*********************************************************************************/

u8 USART_RX_BUF[USART3_REC_NUM];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 RX_buf[USART3_REC_NUM];

void Usart3_Init(u32 bound)
{
		
	NVIC_InitTypeDef NVIC_InitStructure ;  //�����жϽṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;   //����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure; //���崮�ڽṹ��
	DMA_InitTypeDef DMA_InitStructure;     //����DAM�ṹ��
	
	
	//USART_DeInit(USART3);
	/***********ʱ�ӿ���**************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� 
	
	/***********RX,TX��������*************/
	
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //��ʼ��PB10��PB11

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);                        //��ʼ������3
													//�򿪴���3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //��������ж�
	
	/**********����NVIC ����*************/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                       //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
	/**********����DMA ����*************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);    //����DMAʱ��
	DMA_DeInit(DMA1_Stream1);                                             //����3���գ�DMA1��ͨ��4��������1
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART_RX_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = USART3_REC_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;        
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;      
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);  
											
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); 					//����DMA����
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
       //��USART3�����ж�
	
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); 
		
	DMA_Cmd(DMA1_Stream1,ENABLE);   	    //��DMA������1
	USART_Cmd(USART3, ENABLE); 	
}

void USART3_IRQHandler(void)
{	
	u16 RX_len = 0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
			//���IDLE�ж�
			USART_ReceiveData(USART3);
			RX_len = USART3_REC_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
			
			memcpy(RX_buf,USART_RX_BUF,RX_len);
		  LedSetStatus(500,500,50);
			USART3_SendData(RX_buf,RX_len);
		
		  USART_ClearITPendingBit(USART3,USART_IT_IDLE);     //����жϱ��             
   		DMA_Cmd(DMA1_Stream1,DISABLE);
			DMA_SetCurrDataCounter(DMA1_Stream1,USART3_REC_NUM);  //�ָ�DMAָ��,�ȴ���һ��*/
			DMA_Cmd(DMA1_Stream1,ENABLE);
	}
	if(USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
  {
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE);
  }
}

/*******************************************************************************
* ������  : USART_SendData
* ����    : USART1�������ݻ���������
* ����    : *buff�����ݻ�����ָ�룬len���������ݳ���
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART3_SendData(u8* buff, u16 len)
{    
	u16 i;
	USART_GetFlagStatus(USART3, USART_FLAG_TC);
	for(i=0; i<len; i++)  
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); 
	    USART_SendData(USART3, buff[i]);   
	}
}


