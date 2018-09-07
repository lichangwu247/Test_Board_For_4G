#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "common.h" 
#include "w25qxx.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define USART3_REC_NUM  			300  	//定义最大接收字节数 300  小于300后会引起丢包
#define TX_LEN_GSM            300

extern u8  Tx_Buf_Gsm[TX_LEN_GSM];
extern u8  USART_RX_BUF[USART3_REC_NUM]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记		
extern u8 RX_buf[USART3_REC_NUM];	
extern u8 uart_byte_count;          //uart_byte_count要小于USART_REC_LEN 
extern u8 RX_Finish;
extern uint16_t RX_len;
void uart3_init(u32 bound);

void uart3SendChars(u8 *str, u16 strlen);
	
void Uart3_DMA_Init(u32 bound);
void Drv_Tx(uint8_t *data,uint16_t size);
void inf_gsm_deal_irq_dma_tx(void);
void gsm_dma_tx_irq_handler(void);


#endif


