
#include "usart3.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "w25qxx.h" 
#include "key.h"
#include "LinkList.h"

/*********************************************************************************
**********************************************************************************
* �ļ�����: main.c                                                               *
* �ļ�������                                                                     *
* �������ڣ�2018.07.10                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�                                                                     *
* ˵    ����                                                                     * 
*                                                                                *
**********************************************************************************
*********************************************************************************/
void 	Device_Param_Load(void); 

u8 RX_Finish=0;
uint16_t RX_len = 0;
u8 Display_Location_R=0,Display_Location_S=0,Current_Display_Secrent=0;
u8 Display_Location_Pub=0;
u8  i=0;

int Back_Color=0;

u32 FLASH_SIZE =16*1024*1024;

u32 SysInfo_Save_Location=0;
u32 Save_Flag_Save_Location=256;

u32 Save_Sector_Area=256;

u32 Receive_Data_Save_StartAdd=512;
u32 Receive_Data_Save_EndAdd=5632;
u32 Receive_Max_Save=20;
u32 Current_Receive_Save_Address=0;

u32 Send_Data_Save_StartAdd=10240;
u32 Send_Data_Save_EndAdd=15360;
u32 Send_Max_Save=20;
u32 Current_Send_Save_Address=0;

u32 Pub_Data_Save_StartAdd=20480;
u32 Pub_Data_Save_EndAdd=33280;
u32 Pub_Max_Save=50;
u32 Current_Pub_Save_Address=0;

u32 Line_Toal=0,Send_Displeay_Line_Toal=0;
u32 Displeay_Send_Line_SUM=0,Displeay_Recive_Line_SUM=0,Displeay_Pub_Line_SUM=0;

Device_Save_Flag  Save_flag;
Device_Infomation SysInfo;
Data_Save Receive_Data_Buffer;
Data_Save Send_Data_Buffer;
Data_Save Pub_Data_Buffer;

/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Delay(int nms)
{
	int i,j;
	for(i=0;i<nms;i++)
	{
	    for(j=100000;j>0;j--);
	}
}

/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Device_Recive_Info_Display()
{
//	u16 id;
	LCD_Clear_Area(0,0,240,360,GREEN);
	
	LCD_Clear_Area(0,0,240,360,BLUE);//�������	
	LCD_Draw_Rectangle(1,1,238,318,GREEN);//��һ�����	
	LCD_Draw_Straight_Line(1,25,238,25,GREEN);//����һ������
	LCD_Draw_Vertical_Line(45,1,45,25,GREEN);
	LCD_Draw_Vertical_Line(135,1,135,25,GREEN);
	LCD_Draw_Vertical_Line(186,1,186,25,GREEN);
//	LCD_Draw_Vertical_Line(220,25,220,318,GREEN);
//  LCD_Draw_Straight_Line(1,170,238,170,GREEN);
  LCD_DisplayString_color(5,30,12,"Receive from to Device:",RED,BLUE);
	
//	id=W25QXX_ReadID();
//	LCD_DisplayNum_color(5,10,id,6,12,1,RED,BLUE);
	
//  LCD_DisplayString_color(5,10,12,ID,RED,GREEN);
	LCD_DisplayString_color(50,10,12,"              ",RED,BLUE);
//	LCD_DisplayString_color(140,10,12,"offline",RED,BLUE);
	LCD_DisplayString_color(190,10,12,"Wifi:  ",RED,BLUE);

	Device_Param_Load();  
	
}

/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Device_Pub_Info_Display()
{

	LCD_Clear_Area(0,0,240,360,GREEN);
	
	LCD_Clear_Area(0,0,240,360,BLUE);//�������	
	LCD_Draw_Rectangle(1,1,238,318,GREEN);//��һ�����	
	LCD_Draw_Straight_Line(1,25,238,25,GREEN);//����һ������
	LCD_Draw_Vertical_Line(45,1,45,25,GREEN);
	LCD_Draw_Vertical_Line(135,1,135,25,GREEN);
	LCD_Draw_Vertical_Line(186,1,186,25,GREEN);

  LCD_DisplayString_color(5,30,12,"Send to IOT:",RED,BLUE);

	LCD_DisplayString_color(50,10,12,"              ",RED,BLUE);

	LCD_DisplayString_color(190,10,12,"Wifi:  ",RED,BLUE);
	
	Device_Param_Load();  
}
/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Device_Send_Info_Display()
{

	LCD_Clear_Area(0,0,240,360,GREEN);
	
	LCD_Clear_Area(0,0,240,360,BLUE);//�������	
	LCD_Draw_Rectangle(1,1,238,318,GREEN);//��һ�����	
	LCD_Draw_Straight_Line(1,25,238,25,GREEN);//����һ������
	LCD_Draw_Vertical_Line(45,1,45,25,GREEN);
	LCD_Draw_Vertical_Line(135,1,135,25,GREEN);
	LCD_Draw_Vertical_Line(186,1,186,25,GREEN);

  LCD_DisplayString_color(5,30,12,"Send to Device:",RED,BLUE);

	LCD_DisplayString_color(50,10,12,"              ",RED,BLUE);

	LCD_DisplayString_color(190,10,12,"Wifi:  ",RED,BLUE);
	
	Device_Param_Load();  
}
/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Main_Boundary(void)
{
	Current_Display_Secrent=1;

	LCD_Clear_Area(0,0,240,360,BLUE);
	
	
	LCD_DisplayString_color(30,150,24,"Welcome To Use!",RED,BLUE);
  Delay(1000);
	
	LCD_Clear_Area(0,0,240,360,GREEN);//�������	
	LCD_Draw_Rectangle(1,1,238,318,BLUE);//��һ�����	
	LCD_Draw_Straight_Line(1,25,238,25,BLUE);//����һ������
	LCD_Draw_Vertical_Line(45,1,45,25,BLUE);
	LCD_Draw_Vertical_Line(135,1,135,25,BLUE);
	LCD_Draw_Vertical_Line(186,1,186,25,BLUE);

  LCD_Draw_Straight_Line(1,170,238,170,BLUE);
  LCD_DisplayString_color(5,30,12,"Receive from Device:",RED,GREEN);
  LCD_DisplayString_color(5,175,12,"Send to Device:",RED,GREEN);
	
	LCD_DisplayString_color(190,10,12,"Wifi:  ",RED,GREEN);

	Device_Param_Load();  
	
}
/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Main_Boundary2(void)
{

	LCD_Clear_Area(0,0,240,360,BLUE);
	
	LCD_Clear_Area(0,0,240,360,GREEN);//�������	
	LCD_Draw_Rectangle(1,1,238,318,BLUE);//��һ�����	
	LCD_Draw_Straight_Line(1,25,238,25,BLUE);//����һ������
	LCD_Draw_Vertical_Line(45,1,45,25,BLUE);
	LCD_Draw_Vertical_Line(135,1,135,25,BLUE);
	LCD_Draw_Vertical_Line(186,1,186,25,BLUE);
  LCD_Draw_Straight_Line(1,170,238,170,BLUE);
  LCD_DisplayString_color(5,30,12,"Receive from Device:",RED,GREEN);
  LCD_DisplayString_color(5,175,12,"Send:",RED,GREEN);

	LCD_DisplayString_color(190,10,12,"Wifi:  ",RED,GREEN);
	
	Device_Param_Load();  
}
u8 ASSii_TO_Decimal(u8 First,u8 Seconed)
{
	u8 Num1=0,Num2=0;
	u8 Num=0;
	
	switch(First)
	{
	  case 0x30:
			 Num1=0;
			 break;
		case 0x31:
			Num1=1;
			break;
		case 0x32:
			Num1=2;
			break;
	  case 0x33:
			Num1=3;
			break;
		case 0x34:
			Num1=4;
			break;
		case 0x35:
			Num1=5;
			break;
		case 0x36:
			Num1=6;
			break;
		case 0x37:
			Num1=7;
			break;
		case 0x38:
			Num1=8;
			break;
		case 0x39:
			Num1=9;
			break;
		case 0x41:
			Num1=10;
			break;
		case 0x42:
			Num1=11;
			break;
		case 0x43:
			Num1=12;
			break;
		case 0x44:
			Num1=13;
			break;
		case 0x45:
			Num1=14;
			break;
		case 0x46:
			Num1=15;
			break;		
		default :
				break;
							
	}
	switch(Seconed)
	{
	  case 0x30:
			Num2=0;
			break;
		case 0x31:
			Num2=1;
			break;
		case 0x32:
			Num2=2;
			break;
	  case 0x33:
			Num2=3;
			break;
		case 0x34:
			Num2=4;
			break;
		case 0x35:
			Num2=5;
			break;
		case 0x36:
			Num2=6;
			break;
		case 0x37:
			Num2=7;
			break;
		case 0x38:
			Num2=8;
			break;
		case 0x39:
			Num2=9;
			break;
		case 0x41:
			Num2=10;
			break;
		case 0x42:
			Num2=11;
			break;
		case 0x43:
			Num2=12;
		case 0x44:
			Num2=13;
		  break;
		case 0x45:
			Num2=14;
			break;
		case 0x46:
			Num2=15;	
			break;		
		default :
				break;					
	}
	Num=((Num1<<4)|Num2);
	return Num;
}

u8 Information_Filter(u8 *Buffer,u16 len)
{
	u16 i=0,Start_Location=0,Stop_Location=0;
  u8 *msg=Buffer,Len=0;
  u8 buff[300];
	
//	uart3SendChars(Buffer,len);
	while(i<len)
	{
	   if((msg[i]==0x36)&(msg[i+1]==0x38))  //�ҵ�68��ʼλ��
		 {
		    Start_Location=i;
			  Len= ASSii_TO_Decimal(msg[i+9],msg[i+10]);//�ҵ����ݳ��ȣ�
			  Stop_Location=i+8+(Len+3)*3;
			  break;
		 }
		 i++;
	}
	 memset(buff,0,300);
	 for(i=0;i<=Stop_Location-Start_Location;i++)
	 {
	   buff[i]=msg[Start_Location+i];
	 }
	
		memset(Buffer,0,len);
		memcpy(Buffer,buff,Stop_Location-Start_Location);
		len=Stop_Location-Start_Location;
	  return len;
//	uart3SendChars(Buffer,Stop_Location-Start_Location);
}

u8 Get_Pub_Massge(u8 *Buffer,u16 len)
{
	u8 *msg=Buffer;
  u8 Pub_Msg1[256];
	u8 Pub_Msg2[256];
	
  u16 i=0,Len1=0,Len2=0;
	u16 StartSave1=0;
do{
	   if(*msg=='P')
		 {
			 msg=msg+6;
			 StartSave1++;
			 if(StartSave1==1)
			 {
				 do
				 {
					 Pub_Msg1[Len1]=*msg;
					 msg++;
					 Len1++;
				 }while(*msg!=0x0D);
			 }
		   if(StartSave1==2)
			 {
				 do
				 {
					 Pub_Msg2[Len2]=*msg;
					 msg++;
					 Len2++;
				 }while(*msg!=0x0D);
			 }
		 }
	   msg++;
		 i++;
	 }while(i<len);

   uart3SendChars(Pub_Msg1,Len1);
	 uart3SendChars(Pub_Msg2,Len2);

   if(StartSave1==1)
	 {
		 
		  memset(&Pub_Data_Buffer,0,sizeof(Pub_Data_Buffer));
		  Pub_Data_Buffer.Data_Len=Len1;
		  memcpy(Pub_Data_Buffer.Data_Buffer,Pub_Msg1,Len1);
		 
			W25QXX_Write((u8*)&Pub_Data_Buffer,Current_Pub_Save_Address,sizeof(Pub_Data_Buffer));
		  Current_Pub_Save_Address=Current_Pub_Save_Address+Save_Sector_Area;
	 }
	 if(StartSave1==2)
	 {
		  memset(&Pub_Data_Buffer,0,sizeof(Pub_Data_Buffer));
	
		  Pub_Data_Buffer.Data_Len=Len1;
		  memcpy(Pub_Data_Buffer.Data_Buffer,Pub_Msg1,Len1);
		 
			W25QXX_Write((u8*)&Pub_Data_Buffer,Current_Pub_Save_Address,sizeof(Pub_Data_Buffer));
		  Current_Pub_Save_Address=Current_Pub_Save_Address+Save_Sector_Area;
	   
		  
		  memset(&Pub_Data_Buffer,0,sizeof(Pub_Data_Buffer));
		  Pub_Data_Buffer.Data_Len=Len2;
		  memcpy(Pub_Data_Buffer.Data_Buffer,Pub_Msg2,Len2);
		  
		  W25QXX_Write((u8*)&Pub_Data_Buffer,Current_Pub_Save_Address,sizeof(Pub_Data_Buffer));
		  Current_Pub_Save_Address=Current_Pub_Save_Address+Save_Sector_Area;
	 }
	 
	return 0;
	 
}

/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void  Main_Program(void)
{
	Node *node;
	u8   Buffer[USART3_REC_NUM],RSSI[2];
	u8   IP[15];
//	u8   HostName[10];
  u8   Buffer_Len=0;
	u8   Display_Buff[36];
	u8   Line=0,i=0;   
   while(UART3_Recive_List->num>0)
	 {
		 node=Get_First_Node(UART3_Recive_List);
		 memcpy(Buffer,node->Receive_Data,node->LEN);
	//	 uart3SendChars(Buffer,node->LEN);
		 if(strstr(node->Receive_Data,"ip:"))
		 {
			 memmove(IP,&Buffer[3],13);
		   LCD_DisplayString_color(50,10,12,&IP[0],RED,Back_Color);
			 memmove(SysInfo.IP,IP,sizeof(IP));
			 memset(IP,0,sizeof(IP));
			 memcpy(SysInfo.ON_OFF_Line,"ON",sizeof(SysInfo.ON_OFF_Line));
			 memcpy(SysInfo.PASSWORDD,"haoku123",sizeof(SysInfo.PASSWORDD));
			 memcpy(SysInfo.RSSI,"14",sizeof(SysInfo.RSSI));
			 memcpy(SysInfo.SSID,"haoku",sizeof(SysInfo.SSID));
			 
			 Save_flag.System_Flag=1;
			 memset(&Save_flag.Pad,0,sizeof(Save_flag.Pad));
	
			 W25QXX_Write((u8*)&Save_flag,Save_Flag_Save_Location,sizeof(Save_flag));
			 W25QXX_Write((u8*)&SysInfo,SysInfo_Save_Location,sizeof(SysInfo));
		 }
		 
		 /*
	   if(strstr(node->Receive_Data,"device info:"))
		 {
			 memmove(HostName,&Buffer[19],5);
		   memcpy(SysInfo.HosetName,HostName,sizeof(HostName));
			 W25QXX_Write((u8*)&SysInfo,SysInfo_Save_Location,sizeof(SysInfo));
		 }
		 */
		 
		 if(strstr(node->Receive_Data,"Pub:"))
		 {
		     Buffer_Len=Get_Pub_Massge(Buffer,node->LEN);
		 }
		 
	   if(strstr(node->Receive_Data,"Device R:"))
		 { 
			 Buffer_Len=Information_Filter(Buffer,node->LEN); //ȡ����������
			 
			 memset(&Receive_Data_Buffer,0,sizeof(Receive_Data_Buffer));
			 
			 Receive_Data_Buffer.Data_Len=Buffer_Len;
			 memcpy(Receive_Data_Buffer.Data_Buffer,Buffer,sizeof(Buffer));
			 
			 W25QXX_Write((u8*)&Receive_Data_Buffer,Current_Receive_Save_Address,sizeof(Receive_Data_Buffer));
			 Current_Receive_Save_Address=Current_Receive_Save_Address+Save_Sector_Area;//��һ���洢λ��
			 
			 if(Current_Receive_Save_Address>Receive_Data_Save_EndAdd)
			 {
					Current_Receive_Save_Address=Receive_Data_Save_StartAdd;
			 }
			 Line=Buffer_Len/36;
			 if(Buffer_Len%36>0)
			 {
				 Line+=1;
				 
			 }
			 
			 Line_Toal=Line_Toal+Line;
			 
			 
			 if(Line_Toal>8)  //����ܵ���ʾ��������������趨�����������ʾ�������ӵ�һ�п�ʼ��ʾ
			 {
					Line_Toal=0;
					LCD_Clear_Area2(5,50,220,170,Back_Color);
				  Display_Location_R=0;
			 }
			 
			 for(i=0;i<Line;i++)
			 {
				  memcpy(Display_Buff,&Buffer[i*36],36);//����һ�����ݵ���ʾ������
				 
					LCD_DisplayString_color(5,50+Display_Location_R,12,Display_Buff,RED,Back_Color);
				  Display_Location_R+=15;  //��ʾλ�ü�1
				  if(Display_Location_R>=120)
					  Display_Location_R=0;
			 }
		 }
		 if(strstr(node->Receive_Data,"Device S:"))
		 {  
			  Buffer_Len=Information_Filter(Buffer,node->LEN);  //ȡ����������
			 
			  memset(&Send_Data_Buffer,0,sizeof(Send_Data_Buffer));
			 
			  Send_Data_Buffer.Data_Len=Buffer_Len;
			  memcpy(Send_Data_Buffer.Data_Buffer,Buffer,sizeof(Buffer));
			 
			  W25QXX_Write((u8*)&Send_Data_Buffer,Current_Send_Save_Address,sizeof(Send_Data_Buffer));
			  Current_Send_Save_Address=Current_Send_Save_Address+Save_Sector_Area;//��һ���洢λ��
			 
			  if(Current_Send_Save_Address>=Send_Data_Save_EndAdd)  //����洢�����һ����ַ���ӵ�һ����ʼ��
			  {
					Current_Send_Save_Address=Send_Data_Save_StartAdd;  //������ʼ��ַ
			  }
			 
			  Line=Buffer_Len/36;    //������ʾ����
			  if(Buffer_Len%36>0)
			  {
				  Line+=1;
			  }
				
				Send_Displeay_Line_Toal=Send_Displeay_Line_Toal+Line;
				if(Send_Displeay_Line_Toal>8) //����ܵ���ʾ��������������趨�����������ʾ�������ӵ�һ�п�ʼ��ʾ
				{
					Send_Displeay_Line_Toal=0;
					LCD_Clear_Area2(5,190,220,315,Back_Color);
					Display_Location_S=0;	
				}
				
			  for(i=0;i<Line;i++)  
			  {
				  memcpy(Display_Buff,&Buffer[i*36],36);
					LCD_DisplayString_color(5,190+Display_Location_S,12,Display_Buff,RED,Back_Color);
					Display_Location_S+=15;
					if(Display_Location_S>=120)
						 Display_Location_S=0;	
				}
		 }
		 if(strstr(node->Receive_Data,"rssi:"))
		 {   
			   RSSI[0]=Buffer[6];
			   RSSI[1]=Buffer[7];
		     LCD_DisplayString_color(220,10,12,RSSI,RED,Back_Color);
		 }
		 free(node);
	 }
	 key_scan(0);
	 if(keyup_data==KEY1_DATA)
	 {
			 Current_Display_Secrent=2;
			 Device_Recive_Info_Display();    //������ʾ����
			 Display_Location_R=0;
			 while(1)
			 {
				 key_scan(0);
				 if(keyup_data==KEY1_DATA)//��ʾ��һ������
				 {
					 Current_Receive_Save_Address=Current_Receive_Save_Address-Save_Sector_Area;
					 if(Current_Receive_Save_Address<Receive_Data_Save_StartAdd)
					 {
								Current_Receive_Save_Address=Receive_Data_Save_EndAdd;
					 }
					 W25QXX_Read((u8*)&Receive_Data_Buffer,Current_Receive_Save_Address,sizeof(Receive_Data_Buffer));
					 
					 Line=Receive_Data_Buffer.Data_Len/36;
					 if(Receive_Data_Buffer.Data_Len%36>0)
					 {
						 Line+=1;
						 
					 } 
					 
					 Displeay_Recive_Line_SUM=Displeay_Recive_Line_SUM+Line;
					 
					 if(Displeay_Recive_Line_SUM>16)
					 {
						 Displeay_Recive_Line_SUM=0;
						 LCD_Clear_Area2(5,50,220,315,Back_Color);
						 Display_Location_R=0;
					 }
					 
					 
					 for(i=0;i<Line;i++)
					 {
						  memcpy(Display_Buff,&Receive_Data_Buffer.Data_Buffer[i*36],36);
							LCD_DisplayString_color(5,50+Display_Location_R,12,Display_Buff,RED,Back_Color);
							Display_Location_R+=15;
							if(Display_Location_R>=240)
								Display_Location_R=0;
					 } 
				 }
				 if(keyup_data==KEY2_DATA)//��ʾ��һ������
				 {
					 Current_Receive_Save_Address=Current_Receive_Save_Address+Save_Sector_Area;
					 if(Current_Receive_Save_Address>=Receive_Data_Save_EndAdd)
					 {
							Current_Receive_Save_Address=Receive_Data_Save_StartAdd;
					 }
					 
					 W25QXX_Read((u8*)&Receive_Data_Buffer,Current_Receive_Save_Address,sizeof(Receive_Data_Buffer));
		
					 Line=Receive_Data_Buffer.Data_Len/36;
					 if(Receive_Data_Buffer.Data_Len%36>0)
					 {
						 Line+=1;
					 }
					 
					 Displeay_Recive_Line_SUM=Displeay_Recive_Line_SUM+Line;
					 
					 if(Displeay_Recive_Line_SUM>16)
					 {
						 Displeay_Recive_Line_SUM=0;
						 LCD_Clear_Area2(5,50,220,315,Back_Color);
						 Display_Location_R=0;
					 }
					 
					 for(i=0;i<Line;i++)
					 {
						  memcpy(Display_Buff,&Receive_Data_Buffer.Data_Buffer[i*36],36);
							LCD_DisplayString_color(5,50+Display_Location_R,12,Display_Buff,RED,Back_Color);
							Display_Location_R+=15;
							if(Display_Location_R>=240)
								Display_Location_R=0;
					 }
				 }
				 if(keyup_data==KEY3_DATA)
				 {
					 Display_Location_R=0;
					 goto Main_Boundary2;
				 }
				 
				 if(keyup_data==KEY0_DATA)
				 {
					 Device_Pub_Info_Display();
					 while(1)
					 {
						  key_scan(0);
						  if(keyup_data==KEY1_DATA)  //��ʾǰһ����Ϣ
							{
								
								Current_Pub_Save_Address=Current_Pub_Save_Address-Save_Sector_Area;
								if(Current_Receive_Save_Address<Pub_Data_Save_StartAdd)
							  {
										Current_Pub_Save_Address=Pub_Data_Save_EndAdd;
								}
								
								W25QXX_Read((u8*)&Pub_Data_Buffer,Current_Pub_Save_Address,sizeof(Pub_Data_Buffer));
							
								Line=Pub_Data_Buffer.Data_Len/36;
								if(Pub_Data_Buffer.Data_Len%36>0)
								{
									 Line+=1;
								}
								
								Displeay_Pub_Line_SUM=Displeay_Pub_Line_SUM+Line;
								
								if(Displeay_Pub_Line_SUM>16)
								{
									 Displeay_Pub_Line_SUM=0;
									 LCD_Clear_Area2(5,50,220,315,Back_Color);
									 Display_Location_Pub=0;
								}
								
								for(i=0;i<Line;i++)
								{
									memcpy(Display_Buff,&Pub_Data_Buffer.Data_Buffer[i*36],36);
									LCD_DisplayString_color(5,50+Display_Location_Pub,12,Display_Buff,RED,Back_Color);
								  Display_Location_Pub+=15;
									if(Display_Location_Pub>=240)
										Display_Location_Pub=0;
								}
							}
							
						  if(keyup_data==KEY2_DATA)
							{
								Current_Pub_Save_Address=Current_Pub_Save_Address+Save_Sector_Area;
								if(Current_Receive_Save_Address>Pub_Data_Save_EndAdd)
							  {
										Current_Pub_Save_Address=Pub_Data_Save_StartAdd;
								}
								
								W25QXX_Read((u8*)&Pub_Data_Buffer,Current_Pub_Save_Address,sizeof(Pub_Data_Buffer));
							
								Line=Pub_Data_Buffer.Data_Len/36;
								if(Pub_Data_Buffer.Data_Len%36>0)
								{
									 Line+=1;
									 Displeay_Pub_Line_SUM=Displeay_Pub_Line_SUM+Line;
								}
								
								if(Displeay_Pub_Line_SUM>16)
								{
									 Displeay_Pub_Line_SUM=0;
									 LCD_Clear_Area2(5,50,220,315,Back_Color);
									 Display_Location_Pub=0;
								}
								for(i=0;i<Line;i++)
								{
									memcpy(Display_Buff,&Pub_Data_Buffer.Data_Buffer[i*36],36);
									LCD_DisplayString_color(5,50+Display_Location_Pub,12,Display_Buff,RED,Back_Color);
								  Display_Location_Pub+=15;
									if(Display_Location_Pub>=240)
										Display_Location_Pub=0;
								}
							}
							if(keyup_data==KEY3_DATA)
							{
								goto Main_Boundary2;
							}
					 }
				 }
			 }
		}
		if(keyup_data==KEY2_DATA)
		{
			Current_Display_Secrent=3;
			Device_Send_Info_Display();  //����������ʾ����
			Display_Location_S=0;
			while(1)
			{
				key_scan(0);
				if(keyup_data==KEY1_DATA)//��ʾ��һ������
				{
					Current_Send_Save_Address=Current_Send_Save_Address-Save_Sector_Area;
					if(Current_Send_Save_Address<Send_Data_Save_StartAdd)
					{
						Current_Send_Save_Address=Send_Data_Save_EndAdd;
					}
					W25QXX_Read((u8*)&Send_Data_Buffer,Current_Send_Save_Address,sizeof(Send_Data_Buffer));
					
					Line=Send_Data_Buffer.Data_Len/36;
				  if(Send_Data_Buffer.Data_Len%36>0)
					{
						 Line+=1;
						 Displeay_Send_Line_SUM=Displeay_Send_Line_SUM+Line;
					}
					
					if(Displeay_Send_Line_SUM>16)
					{
					  Displeay_Send_Line_SUM=0;
						LCD_Clear_Area2(5,50,220,315,Back_Color);
						Display_Location_S=0;
					}
					
					for(i=0;i<Line;i++)
					{
						memcpy(Display_Buff,&Send_Data_Buffer.Data_Buffer[i*36],36);
						LCD_DisplayString_color(5,50+Display_Location_S,12,Display_Buff,RED,Back_Color);
						Display_Location_S+=15;
						if(Display_Location_S>=240)
						  Display_Location_S=0;
					}
				}
				if(keyup_data==KEY2_DATA)//��ʾ��һ������
				{
					Current_Send_Save_Address=Current_Send_Save_Address+Save_Sector_Area;
					if(Current_Send_Save_Address>Send_Data_Save_EndAdd)
					{
						Current_Send_Save_Address=Send_Data_Save_StartAdd;
					}
					W25QXX_Read((u8*)&Send_Data_Buffer,Current_Send_Save_Address,sizeof(Send_Data_Buffer));
					
					Line=Send_Data_Buffer.Data_Len/36;
				  if(Send_Data_Buffer.Data_Len%36>0)
					{
						 Line+=1;
						Displeay_Send_Line_SUM=Displeay_Send_Line_SUM+Line;
					}
					
					if(Displeay_Send_Line_SUM>16)
					{
					  Displeay_Send_Line_SUM=0;
						LCD_Clear_Area2(5,50,220,315,Back_Color);
						Display_Location_S=0;
					}
					
					for(i=0;i<Line;i++)
					{
						memcpy(Display_Buff,&Send_Data_Buffer.Data_Buffer[i*36],36);
						LCD_DisplayString_color(5,50+Display_Location_S,12,Display_Buff,RED,Back_Color);
						Display_Location_S+=15;
						if(Display_Location_S>=240)
						  Display_Location_S=0;
					}
				}
				 if(keyup_data==KEY3_DATA)
				 {
					 Display_Location_S=0;
					 goto Main_Boundary2;
				 }
			}
		}
		if(keyup_data==KEY0_DATA)
		{
				
			W25QXX_Erase_Chip();
			LCD_DisplayString_color(5,170,12,"Erase Chip OK  !!",RED,GREEN);//��ʾ����оƬ�ɹ�
			goto Main_Boundary2;
		}
		if(keyup_data==KEY3_DATA)
		{
			Main_Boundary2:
			Current_Display_Secrent=1;
			Main_Boundary2();
		}
}

/****************************************************************************
* ��    ��: Device_Param_Load()
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
void Device_Param_Load(void)
{
	
	W25QXX_Read((u8*)&Save_flag,Save_Flag_Save_Location,sizeof(Save_flag));
	switch(Current_Display_Secrent)
	{
		case 1:
			Back_Color=	GREEN;
			break;
		case 2:
			Back_Color=	BLUE;
			break;
		case 3:
			Back_Color=	BLUE;
			break;
	}
	
	if(Save_flag.System_Flag==1)
	{
		 W25QXX_Read((u8*)&SysInfo,SysInfo_Save_Location,sizeof(SysInfo));
     LCD_DisplayString_color(50,10,12,(u8*)&SysInfo.IP,RED,Back_Color);
		 LCD_DisplayString_color(140,10,12,(u8*)&SysInfo.ON_OFF_Line,RED,Back_Color);
		 LCD_DisplayString_color(5,10,12,(u8*)&SysInfo.HosetName,RED,Back_Color);
	//	 LCD_DisplayString_color(220,10,12,(u8*)&SysInfo.RSSI,RED,GREEN);
	}
	else
	{
		memset(&SysInfo,0,sizeof(SysInfo));
		LCD_Clear_Area(0,0,240,360,BLUE);
		LCD_DisplayString_color(5,10,12,"Please Connect  Network  !!",RED,GREEN);
	}
}
/****************************************************************************
* ��    ��: 
* ��    �ܣ�
* ��ڲ�����       
* ���ز�����
* ˵    ���� 
****************************************************************************/
int main(void)
{ 
	Node *node;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2
	delay_init();		      //��ʱ��ʼ�� 
	//uart3_init(230400);	    //���ڳ�ʼ��������Ϊ230400
	Uart3_DMA_Init(230400);
	KEY_Init();
	LED_Init();		  		  //��ʼ����LED 
	BEEP_Init();          //��������ʼ��
	W25QXX_Init();			//W25QXX��ʼ�� 
	
  LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
	BRUSH_COLOR=RED;      //���û�����ɫΪ��ɫ
 
	Current_Receive_Save_Address=Receive_Data_Save_StartAdd;
	Current_Send_Save_Address=Send_Data_Save_StartAdd;
	Current_Pub_Save_Address=Pub_Data_Save_StartAdd;
	
	Main_Boundary();     //��ʾ������
	
	
  UART3_Recive_List=New_List();
  node=Create_NewNode();
	
	while(1)
	{
		Main_Program();
    if(RX_Finish==1)
		{
		   RX_Finish=0;
			 node->LEN=RX_len;
			 memcpy(node->Receive_Data,RX_buf,RX_len);
			 List_Push_Node(UART3_Recive_List,node);
			 memset(RX_buf,0,RX_len);
			// uart3SendChars("ok",2);
		}
	}
}

