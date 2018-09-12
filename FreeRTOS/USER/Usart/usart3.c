#include "usart3.h"
#include "led.h"
/*********************************************************************************
**********************************************************************************
* 文件名称: usart3.c                                                             *
* 文件简述：USART3使用                                                           *
* 创建日期：2018.08.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 声    明：                                                                     *
**********************************************************************************
*********************************************************************************/

u8 USART_RX_BUF[USART3_REC_NUM];     //接收缓冲,最大USART_REC_LEN个字节.
u8 RX_buf[USART3_REC_NUM];

void Usart3_Init(u32 bound)
{
		
	NVIC_InitTypeDef NVIC_InitStructure ;  //定义中断结构体
 	GPIO_InitTypeDef GPIO_InitStructure;   //定义IO初始化结构体
	USART_InitTypeDef USART_InitStructure; //定义串口结构体
	DMA_InitTypeDef DMA_InitStructure;     //定义DAM结构体
	
	
	//USART_DeInit(USART3);
	/***********时钟开启**************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
	
	/***********RX,TX引脚配置*************/
	
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               //初始化PB10，PB11

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);                        //初始化串口3
													//打开串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //开启相关中断
	
	/**********串口NVIC 配置*************/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                       //根据指定的参数初始化VIC寄存器、
	
	
	/**********串口DMA 配置*************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);    //启动DMA时钟
	DMA_DeInit(DMA1_Stream1);                                             //串口3接收，DMA1的通道4，数据流1
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
											
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); 					//开启DMA接收
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
       //打开USART3空闲中断
	
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); 
		
	DMA_Cmd(DMA1_Stream1,ENABLE);   	    //打开DMA数据流1
	USART_Cmd(USART3, ENABLE); 	
}

void USART3_IRQHandler(void)
{	
	u16 RX_len = 0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
			//清除IDLE中断
			USART_ReceiveData(USART3);
			RX_len = USART3_REC_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
			
			memcpy(RX_buf,USART_RX_BUF,RX_len);
		  LedSetStatus(500,500,50);
			USART3_SendData(RX_buf,RX_len);
		
		  USART_ClearITPendingBit(USART3,USART_IT_IDLE);     //清除中断标记             
   		DMA_Cmd(DMA1_Stream1,DISABLE);
			DMA_SetCurrDataCounter(DMA1_Stream1,USART3_REC_NUM);  //恢复DMA指针,等待下一次*/
			DMA_Cmd(DMA1_Stream1,ENABLE);
	}
	if(USART_GetITStatus(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
  {
		USART_ClearITPendingBit(USART3, USART_IT_PE | USART_IT_FE | USART_IT_NE);
  }
}

/*******************************************************************************
* 函数名  : USART_SendData
* 描述    : USART1发送数据缓冲区数据
* 输入    : *buff：数据缓冲区指针，len：发送数据长度
* 输出    : 无
* 返回    : 无 
* 说明    : 无
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


