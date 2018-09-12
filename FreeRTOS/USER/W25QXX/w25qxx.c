#include "spi.h"
#include "w25qxx.h"  

/*********************************************************************************
************************�������� STM32F407���Ŀ�����******************************
**********************************************************************************
* �ļ�����: w25qxx.c                                                             *
* �ļ�������W25Q128ʹ��                                                          *
* �������ڣ�2017.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����25Q128����Ϊ16M,����128��Block,4096��Sector                          *
            255byteΪһҳ                                                        *
						4KbytesΪһ������                                                    *
            16������Ϊһ����                                                     *
*********************************************************************************/	
 
u16 W25QXX_ID;	 
													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //ʹ��GPIOGʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             //PG8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //����
  GPIO_Init(GPIOG, &GPIO_InitStructure);                //��ʼ��

	W25QXX_CS=1;			                                    //SPI FLASH��ѡ��
	SPI1_Init();		   			                              //��ʼ��SPI
	SPI1_Setclock(SPI_BaudRatePrescaler_2);		            //����Ϊ42Mʱ��,����ģʽ 
	W25QXX_ID=W25QXX_ReadID();	                          //��ȡFLASH ID.
}

/****************************************************************************
* ��    ��: u8 W25QXX_ReadSR(void) 
* ��    �ܣ���ȡW25QXX��״̬�Ĵ���
* ��ڲ�������
* ���ز�����״̬�Ĵ�����ֵ
* ˵    ���� 		     
****************************************************************************/
u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	W25QXX_CS=0;                                  //ʹ������   
	SPI1_ReadWriteByte(W25X_CMD_ReadStatusReg);   //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);                //��ȡһ���ֽ�  
	W25QXX_CS=1;                                  //ȡ��Ƭѡ     
	return byte;   
} 

/****************************************************************************
* ��    ��: void W25QXX_Write_SR(u8 sr)
* ��    �ܣ�дW25QXX״̬�Ĵ���
* ��ڲ�����д���ֵ
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Write_SR(u8 sr)   
{   
	W25QXX_CS=0;                                 //ʹ������   
	SPI1_ReadWriteByte(W25X_CMD_WriteStatusReg); //����дȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sr);                      //д��һ���ֽ�  
	W25QXX_CS=1;                                 //ȡ��Ƭѡ     	      
} 

//W25QXXдʹ��	 
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                               //ʹ������   
  SPI1_ReadWriteByte(W25X_CMD_WriteEnable);  //����дʹ��  
	W25QXX_CS=1;                               //ȡ��Ƭѡ     	      
} 

//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);      //�ȴ�BUSYλ���
} 

//W25QXXд��ֹ	 
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                                //ʹ������   
  SPI1_ReadWriteByte(W25X_CMD_WriteDisable);  //����д��ָֹ��    
	W25QXX_CS=1;                                //ȡ��Ƭѡ     	      
} 

/****************************************************************************
* ��    ��: u16 W25QXX_ReadID(void)
* ��    �ܣ���ȡоƬID
* ��ڲ�������
* ���ز�����оƬID
* ˵    ����0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
            0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
            0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
            0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
            0XEF17,��ʾоƬ�ͺ�ΪW25Q128      
****************************************************************************/  
u16 W25QXX_ReadID(void)
{
	u16 IDnum = 0;	  
	W25QXX_CS=0;				    
	SPI1_ReadWriteByte(0x90); //���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	IDnum|=SPI1_ReadWriteByte(0xFF)<<8;  
	IDnum|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return IDnum;
} 

/****************************************************************************
* ��    ��: void W25QXX_Erase_Chip(void) 
* ��    �ܣ���������оƬ		  
* ��ڲ�������
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                   
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                             //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_ChipErase);  //����Ƭ��������  
	  W25QXX_CS=1;                             //ȡ��Ƭѡ     	      
	  W25QXX_Wait_Busy();   				           //�ȴ�оƬ��������
}  

/****************************************************************************
* ��    ��: void W25QXX_Erase_Sector(u32 First_Addr)
* ��    �ܣ�����ĳ���������׵�ַ	  
* ��ڲ�����First_Addr:������ַ
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Erase_Sector(u32 First_Addr)   
{    
 	  First_Addr*=4096;
    W25QXX_Write_Enable();                  
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                                  //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_SectorErase);     //������������ָ�� 
    SPI1_ReadWriteByte((u8)((First_Addr)>>16));   //���͵�ַ    
    SPI1_ReadWriteByte((u8)((First_Addr)>>8));   
    SPI1_ReadWriteByte((u8)First_Addr);  
	  W25QXX_CS=1;                                  //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				                //�ȴ��������
} 

//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                             //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_PowerDown);  //���͵�������  
	  W25QXX_CS=1;                             //ȡ��Ƭѡ     	      
    delay_us(3);                             
}  

//����
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                                      //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_ReleasePowerDown);    //���ͻ�������
	  W25QXX_CS=1;                                      //ȡ��Ƭѡ     	      
    delay_us(3);                                     
}   

/****************************************************************************
* ��    ��: void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
* ��    �ܣ�ָ����ַ��ʼ��ȡָ�����ȵ�����
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼ��ȡ�ĵ�ַ(24bit)
            ByteCount:Ҫ��ȡ���ֽ���(���65535)
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Read(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{  	
		u16 i;
		W25QXX_CS=0;                                 //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_ReadData);     //���Ͷ�ȡ����   
    SPI1_ReadWriteByte((u8)((StartAddress)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI1_ReadWriteByte((u8)StartAddress);   

		for(i=0;i<ByteCount;i++) 
		{
			 DataBuffer[i] = SPI1_ReadWriteByte(0XFF);
		}		
		W25QXX_CS=1;  				    	      
}  

/****************************************************************************
* ��    ��: void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* ��    �ܣ���һҳ��д������256���ֽڵ�����  
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���256)
* ���ز�������
* ˵    ���� 		     
****************************************************************************/
void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
{  
	  u16 i;
	  W25QXX_Write_Enable();                   
	  W25QXX_CS=0;                                   //ʹ������   
    SPI1_ReadWriteByte(W25X_CMD_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((u8)((StartAddress)>>16));  //���͵�ַ    
    SPI1_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI1_ReadWriteByte((u8)StartAddress);   
		for (i=0;i<ByteCount;i++)SPI1_ReadWriteByte(DataBuffer[i]);
		W25QXX_CS=1;                   //ȡ��Ƭѡ 
		W25QXX_Wait_Busy();					   //�ȴ�д�����
}

/****************************************************************************
* ��    ��: void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount) 
* ��    �ܣ���ҳд������
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���256)
* ���ز�������
* ˵    �������Զ���ҳ���ܣ���Ϊ�ú���û����д֮ǰ�ж�Ҫд��ĵ�ַ�ϵ������Ƿ�Ϊ
            �գ�������д��֮ǰ����ȷ���õ�ַ�ϵ�����Ϊ��0xFF������д��ʧ�� 		     
****************************************************************************/
void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{
  u8 NumOfPage = 0, NumOfSingle = 0, Surplus_Addr = 0, Surplus_count = 0, midtemp = 0;

  Surplus_Addr = StartAddress % 256;
  Surplus_count = 256 - Surplus_Addr;
  NumOfPage =  ByteCount / 256;
  NumOfSingle = ByteCount % 256;

  if (Surplus_Addr == 0) //��ʼд�ĵ�ַ�պ���ҳ��ʼ�ĵ�ַ
  {
    if (NumOfPage == 0)  // ByteCount < 256��һҳ�ܵ��ֽ�����  
    {
      W25QXX_Write_Page(DataBuffer, StartAddress, ByteCount);
    }
    else                 // ByteCount > 256��һҳ�ܵ��ֽ�����  
    {
      while (NumOfPage--)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, 256);
        StartAddress +=  256;
        DataBuffer += 256;
      }
      W25QXX_Write_Page(DataBuffer, StartAddress, NumOfSingle);
    }
  }
  else ///��ʼд�ĵ�ַ����ҳ���׵�ַ��
  {
    if (NumOfPage == 0) // ByteCount < 256��һҳ�ܵ��ֽ����� 
    {
      if (NumOfSingle > Surplus_count)  
      {
        midtemp = NumOfSingle - Surplus_count;
        W25QXX_Write_Page(DataBuffer, StartAddress, Surplus_count);
        StartAddress +=  Surplus_count;
        DataBuffer += Surplus_count;
        W25QXX_Write_Page(DataBuffer, StartAddress, midtemp);
      }
      else
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, ByteCount);
      }
    }
    else //ByteCount > 256��һҳ�ܵ��ֽ�����  
    {
      ByteCount -= Surplus_count;
      NumOfPage =  ByteCount / 256;
      NumOfSingle = ByteCount % 256;

      W25QXX_Write_Page(DataBuffer, StartAddress, Surplus_count);
      StartAddress +=  Surplus_count;
      DataBuffer += Surplus_count;
      while (NumOfPage--)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, 256);
        StartAddress +=  256;
        DataBuffer += 256;
      }
      if (NumOfSingle != 0)
      {
        W25QXX_Write_Page(DataBuffer, StartAddress, NumOfSingle);
      }
    }
  }
} 

/****************************************************************************
* ��    ��: void W25QXX_SectorWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* ��    �ܣ�������д������
* ��ڲ�����DataBuffer:���ݴ洢��
            StartAddress:��ʼд��ĵ�ַ(24bit)
            ByteCount:Ҫд����ֽ���(���65536)
* ���ز�������
* ˵    ����д��֮ǰ�жϸĵ�ַ�ϵ������Ƿ�Ϊ�գ������Ȳ�����д�룬�ú����ο����ϴ���http://www.openedv.com      
****************************************************************************/
u8 TS_BUFFER[4096];	
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   W25QXX_BUF=TS_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		  pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		  NumByteToWrite-=secremain;				//�ֽ����ݼ�
		  if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
		  else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};
}
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
