#include "spi.h"
#include "w25qxx.h"  

/*********************************************************************************
************************启明欣欣 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: w25qxx.c                                                             *
* 文件简述：W25Q128使用                                                          *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：25Q128容量为16M,共有128个Block,4096个Sector                          *
            255byte为一页                                                        *
						4Kbytes为一个扇区                                                    *
            16个扇区为一个块                                                     *
*********************************************************************************/	
 
u16 W25QXX_ID;	 
													 
//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //使能GPIOG时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;             //PG8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);                //初始化

	W25QXX_CS=1;			                                    //SPI FLASH不选中
	SPI1_Init();		   			                              //初始化SPI
	SPI1_Setclock(SPI_BaudRatePrescaler_2);		            //设置为42M时钟,高速模式 
	W25QXX_ID=W25QXX_ReadID();	                          //读取FLASH ID.
}

/****************************************************************************
* 名    称: u8 W25QXX_ReadSR(void) 
* 功    能：读取W25QXX的状态寄存器
* 入口参数：无
* 返回参数：状态寄存器的值
* 说    明： 		     
****************************************************************************/
u8 W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	W25QXX_CS=0;                                  //使能器件   
	SPI1_ReadWriteByte(W25X_CMD_ReadStatusReg);   //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);                //读取一个字节  
	W25QXX_CS=1;                                  //取消片选     
	return byte;   
} 

/****************************************************************************
* 名    称: void W25QXX_Write_SR(u8 sr)
* 功    能：写W25QXX状态寄存器
* 入口参数：写入的值
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void W25QXX_Write_SR(u8 sr)   
{   
	W25QXX_CS=0;                                 //使能器件   
	SPI1_ReadWriteByte(W25X_CMD_WriteStatusReg); //发送写取状态寄存器命令    
	SPI1_ReadWriteByte(sr);                      //写入一个字节  
	W25QXX_CS=1;                                 //取消片选     	      
} 

//W25QXX写使能	 
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                               //使能器件   
  SPI1_ReadWriteByte(W25X_CMD_WriteEnable);  //发送写使能  
	W25QXX_CS=1;                               //取消片选     	      
} 

//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);      //等待BUSY位清空
} 

//W25QXX写禁止	 
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                                //使能器件   
  SPI1_ReadWriteByte(W25X_CMD_WriteDisable);  //发送写禁止指令    
	W25QXX_CS=1;                                //取消片选     	      
} 

/****************************************************************************
* 名    称: u16 W25QXX_ReadID(void)
* 功    能：读取芯片ID
* 入口参数：无
* 返回参数：芯片ID
* 说    明：0XEF13,表示芯片型号为W25Q80  
            0XEF14,表示芯片型号为W25Q16    
            0XEF15,表示芯片型号为W25Q32  
            0XEF16,表示芯片型号为W25Q64 
            0XEF17,表示芯片型号为W25Q128      
****************************************************************************/  
u16 W25QXX_ReadID(void)
{
	u16 IDnum = 0;	  
	W25QXX_CS=0;				    
	SPI1_ReadWriteByte(0x90); //发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	IDnum|=SPI1_ReadWriteByte(0xFF)<<8;  
	IDnum|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return IDnum;
} 

/****************************************************************************
* 名    称: void W25QXX_Erase_Chip(void) 
* 功    能：擦除整个芯片		  
* 入口参数：无
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                   
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                             //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_ChipErase);  //发送片擦除命令  
	  W25QXX_CS=1;                             //取消片选     	      
	  W25QXX_Wait_Busy();   				           //等待芯片擦除结束
}  

/****************************************************************************
* 名    称: void W25QXX_Erase_Sector(u32 First_Addr)
* 功    能：擦除某个扇区的首地址	  
* 入口参数：First_Addr:扇区地址
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void W25QXX_Erase_Sector(u32 First_Addr)   
{    
 	  First_Addr*=4096;
    W25QXX_Write_Enable();                  
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                                  //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_SectorErase);     //发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((First_Addr)>>16));   //发送地址    
    SPI1_ReadWriteByte((u8)((First_Addr)>>8));   
    SPI1_ReadWriteByte((u8)First_Addr);  
	  W25QXX_CS=1;                                  //取消片选     	      
    W25QXX_Wait_Busy();   				                //等待擦除完成
} 

//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                             //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_PowerDown);  //发送掉电命令  
	  W25QXX_CS=1;                             //取消片选     	      
    delay_us(3);                             
}  

//唤醒
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                                      //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_ReleasePowerDown);    //发送唤醒命令
	  W25QXX_CS=1;                                      //取消片选     	      
    delay_us(3);                                     
}   

/****************************************************************************
* 名    称: void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
* 功    能：指定地址开始读取指定长度的数据
* 入口参数：DataBuffer:数据存储区
            StartAddress:开始读取的地址(24bit)
            ByteCount:要读取的字节数(最大65535)
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void W25QXX_Read(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{  	
		u16 i;
		W25QXX_CS=0;                                 //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_ReadData);     //发送读取命令   
    SPI1_ReadWriteByte((u8)((StartAddress)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI1_ReadWriteByte((u8)StartAddress);   

		for(i=0;i<ByteCount;i++) 
		{
			 DataBuffer[i] = SPI1_ReadWriteByte(0XFF);
		}		
		W25QXX_CS=1;  				    	      
}  

/****************************************************************************
* 名    称: void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* 功    能：在一页内写入少于256个字节的数据  
* 入口参数：DataBuffer:数据存储区
            StartAddress:开始写入的地址(24bit)
            ByteCount:要写入的字节数(最大256)
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
{  
	  u16 i;
	  W25QXX_Write_Enable();                   
	  W25QXX_CS=0;                                   //使能器件   
    SPI1_ReadWriteByte(W25X_CMD_PageProgram);      //发送写页命令   
    SPI1_ReadWriteByte((u8)((StartAddress)>>16));  //发送地址    
    SPI1_ReadWriteByte((u8)((StartAddress)>>8));   
    SPI1_ReadWriteByte((u8)StartAddress);   
		for (i=0;i<ByteCount;i++)SPI1_ReadWriteByte(DataBuffer[i]);
		W25QXX_CS=1;                   //取消片选 
		W25QXX_Wait_Busy();					   //等待写入结束
}

/****************************************************************************
* 名    称: void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount) 
* 功    能：按页写入数据
* 入口参数：DataBuffer:数据存储区
            StartAddress:开始写入的地址(24bit)
            ByteCount:要写入的字节数(最大256)
* 返回参数：无
* 说    明：有自动换页功能，因为该函数没有在写之前判断要写入的地址上的数据是否为
            空，所以在写入之前必须确保该地址上的数据为空0xFF，否则写入失败 		     
****************************************************************************/
void W25QXX_PageWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)   
{
  u8 NumOfPage = 0, NumOfSingle = 0, Surplus_Addr = 0, Surplus_count = 0, midtemp = 0;

  Surplus_Addr = StartAddress % 256;
  Surplus_count = 256 - Surplus_Addr;
  NumOfPage =  ByteCount / 256;
  NumOfSingle = ByteCount % 256;

  if (Surplus_Addr == 0) //开始写的地址刚好是页开始的地址
  {
    if (NumOfPage == 0)  // ByteCount < 256（一页总的字节数）  
    {
      W25QXX_Write_Page(DataBuffer, StartAddress, ByteCount);
    }
    else                 // ByteCount > 256（一页总的字节数）  
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
  else ///开始写的地址不在页的首地址上
  {
    if (NumOfPage == 0) // ByteCount < 256（一页总的字节数） 
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
    else //ByteCount > 256（一页总的字节数）  
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
* 名    称: void W25QXX_SectorWrite(u8* DataBuffer,u32 StartAddress,u16 ByteCount)
* 功    能：按扇区写入数据
* 入口参数：DataBuffer:数据存储区
            StartAddress:开始写入的地址(24bit)
            ByteCount:要写入的字节数(最大65536)
* 返回参数：无
* 说    明：写入之前判断改地址上的数据是否为空，不是先擦除再写入，该函数参考网上代码http://www.openedv.com      
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
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		  pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		  NumByteToWrite-=secremain;				//字节数递减
		  if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
		  else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};
}
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
