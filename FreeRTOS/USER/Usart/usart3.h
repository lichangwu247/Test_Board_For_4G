#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "common.h" 
#include "string.h"
#include "stdlib.h"  

#define USART3_REC_NUM  			300  	//�����������ֽ��� 300  С��300������𶪰�
#define TX_LEN_GSM            300

extern u8  Tx_Buf_Gsm[TX_LEN_GSM];
extern u8  USART_RX_BUF[USART3_REC_NUM]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���		
extern u8 RX_buf[USART3_REC_NUM];	
extern u8 uart_byte_count;          //uart_byte_countҪС��USART_REC_LEN 

void Usart3_Init(u32 bound);
void USART3_SendData(u8* buff, u16 len);

#endif


