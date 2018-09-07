#include "usart3.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "lcd.h"
#include "beep.h" 
#include "LinkList.h"

/*********************************************************************************
**********************启明欣欣 STM32F407开发板(高配版)****************************
**********************************************************************************
* 文件名称: usart3.c                                                             *
* 文件简述：USART3使用                                                           *
* 创建日期：2015.03.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手经过USART1控制LED亮灭与蜂鸣器响闭                    * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/	

u8 uart_byte_count=0;
u8 Tx_Buf_Gsm[TX_LEN_GSM];
u8 USART_RX_BUF[USART3_REC_NUM];     //接收缓冲,最大USART_REC_LEN个字节.
u8 RX_buf[USART3_REC_NUM];


/****************************************************************************
* 名    称: void uart3_init(u32 bound)
* 功    能：USART1初始化
* 入口参数：bound：波特率
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart3_init(u32 bound)
{   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //初始化PB10，PB11
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
}
void Uart3_DMA_Init(u32 bound)
{
	
	//定义中断结构体
	NVIC_InitTypeDef NVIC_InitStructure ;
	//定义IO初始化结构体
 	GPIO_InitTypeDef GPIO_InitStructure;
	//定义串口结构体
	USART_InitTypeDef USART_InitStructure;
	//定义DAM结构体
	DMA_InitTypeDef DMA_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  //启动DMA时钟
	
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;//DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//通道配置
	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4; 
	
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//串口3发送地址
	//
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Tx_Buf_Gsm;//发送BUFF的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//从DMA到外部串口传送
	DMA_InitStructure.DMA_BufferSize = TX_LEN_GSM;//发送缓冲区大小
  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存为递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//DMA优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
  // 如果FIFO模式或直接将用于指定的流，不使能FIFO
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
	//指定FIFO阈值水平
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;        
	//指定Burst转移配置内存传输
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;       
	//指定的Burst转移配置外围转移
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//配置DMA1通道 
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);  
	//使能中断
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);  
  
	
	//串口接收配置
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream1);     //串口3接收，DMA1的通道4，数据流1
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //初始化PB10，PB11
	
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1	
	
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	
	
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); 
	USART_Cmd(USART3, ENABLE); 
}
int Flag_Tx_Gsm_Busy=0;

//发送数据函数
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

//DMA 发送完成中断处理
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

//串口1发送一个字符
void uart3SendChar(u8 ch)
{      
	while((USART3->SR&0x40)==0);  
    USART3->DR = (u8) ch;      
}
/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u16 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart3SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart3SendChar(*(str + k)); k++; }   //循环发送,直到发送完毕   
    while (k < strlen); 
} 

//串口1中断服务程序
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


