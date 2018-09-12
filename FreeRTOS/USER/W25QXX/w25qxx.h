#ifndef __W25QXX_H
#define __W25QXX_H			    
#include "common.h"  

//////////////////////////////////////////////////////////////////////////////////	 

//W25Qϵ��оƬID�б�	   
#define W25Q08 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//ָ���
#define W25X_CMD_WriteEnable		  0x06 
#define W25X_CMD_WriteDisable		  0x04 
#define W25X_CMD_ReadStatusReg		0x05 
#define W25X_CMD_WriteStatusReg		0x01 
#define W25X_CMD_ReadData			    0x03 
#define W25X_CMD_FastReadData		  0x0B 
#define W25X_CMD_FastReadDual		  0x3B 
#define W25X_CMD_PageProgram		  0x02 
#define W25X_CMD_BlockErase			  0xD8 
#define W25X_CMD_SectorErase		  0x20 
#define W25X_CMD_ChipErase			  0xC7 
#define W25X_CMD_PowerDown			  0xB9 
#define W25X_CMD_ReleasePowerDown	0xAB 
#define W25X_CMD_DeviceID			    0xAB 
#define W25X_CMD_ManufactDeviceID	0x90 
#define W25X_CMD_JedecDeviceID		0x9F 
#define	W25QXX_CS PGout(8)   //W25QXX��Ƭѡ�ź�


typedef struct
{
	u8 Data_Len;
  u8 Data_Buffer[255];
}Data_Save;

extern u16 W25QXX_ID;			   //��¼W25QXXоƬ�ͺ�		   



void W25QXX_Init(void);
u16  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
u8	 W25QXX_ReadSR(void);        		//��ȡ״̬�Ĵ��� 
void W25QXX_Write_SR(u8 sr);  			//д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		//дʹ�� 
void W25QXX_Write_Disable(void);		//д����

void W25QXX_Erase_Chip(void);    	  	  //��Ƭ����
void W25QXX_Erase_Sector(u32 First_Addr);	//��������
void W25QXX_Wait_Busy(void);           	//�ȴ�����
void W25QXX_PowerDown(void);        	  //�������ģʽ
void W25QXX_WAKEUP(void);				        //����

void W25QXX_Read(u8* DataBuffer,u32 StartAddress,u16 ByteCount);      //��ָ����ַ����ָ���ֽ���
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//������д��Flash

void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount);
void W25QXX_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);  //��ҳд��Flash
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
#endif















