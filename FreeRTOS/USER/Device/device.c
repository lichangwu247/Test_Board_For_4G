#include "device.h"
#include "usart3.h"
#include "common.h"
#include "LinkList.h"
#include "string.h"


u8 Pub_Global_buf[200]={0};
u8 ip_Global_buf[30]={0};
u8 Device_S_Global_buf[100]={0};
u8 Device_R_Global_buf[100]={0};
u8 RSSI_Global_buf[15]={0};
u16 Pub_data_Global_len=0,ip_data_Global_len=0,Device_S_data_Global_len=0,Device_R_Global_data_len=0,RSSI_data_Global_len=0;

Node *node=NULL;
int device_data_filter(u8 * msg,u16 len)
{
//	u8 data_buf[300]={0};
	u8 Pub_buf[200]={0};
	u8 ip_buf[30]={0};
	u8 Device_S_buf[100]={0};
	u8 Device_R_buf[100]={0};
	u8 RSSI_buf[15]={0};
	
	char *head,*msg_buf;
	u16 data_line=0;
	
	u16 Pub_data_len=0,ip_data_len=0,Device_S_data_len=0,Device_R_data_len=0,RSSI_data_len=0;
	
	msg_buf=(char *)msg;
	
	if((head=strstr((char *)msg_buf,"Pub:"))!=NULL) //提取pub数据
	{
		memset(Pub_buf,0,sizeof(Pub_buf));
		do
		{
			Pub_buf[Pub_data_len]=*head;
			head++;
			Pub_data_len++;
			if(*head=='\n')
			{
				data_line++;
			}
		}while(data_line<2);
		data_line=0;
		
		memset(Pub_Global_buf,0,sizeof(Pub_Global_buf));
		memcpy(Pub_Global_buf,Pub_buf,Pub_data_len-1);
		Pub_data_Global_len=Pub_data_len-1;
		
	//	USART3_SendData(Pub_Global_buf,Pub_data_Global_len);
	}
	
	if((head=strstr((char *)msg_buf,"ip:"))!=NULL)//提取ip数据
	{
		memset(ip_buf,0,sizeof(ip_buf));
		do
		{
			ip_buf[ip_data_len]=*head;
			head++;
			ip_data_len++;
			if(*head=='\n')
			{
				data_line++;
			}
		}while(data_line<1);
		data_line=0;
		
		memset(ip_Global_buf,0,sizeof(ip_Global_buf));
		memcpy(ip_Global_buf,ip_buf,ip_data_len-1);
		ip_data_Global_len=ip_data_len-1;
		
	//	USART3_SendData(ip_Global_buf,ip_data_Global_len);
	}
	
	if((head=strstr((char *)msg_buf,"Device S:"))!=NULL)//提取Device S:数据
	{
		memset(Device_S_buf,0,sizeof(Device_S_buf));
		do
		{
			Device_S_buf[Device_S_data_len]=*head;
			head++;
			Device_S_data_len++;
			if(*head=='\n')
			{
				data_line++;
			}
		}while(data_line<2);
		data_line=0;
		
		memset(Device_S_Global_buf,0,sizeof(Device_S_Global_buf));
		memcpy(Device_S_Global_buf,Device_S_buf,Device_S_data_len-1);
		Device_S_data_Global_len=Device_S_data_len-1;
		
	//	USART3_SendData(Device_S_Global_buf,Device_S_data_Global_len);
	}
	
  if((head=strstr((char *)msg_buf,"Device R:"))!=NULL)
	{
		memset(Device_R_buf,0,sizeof(Device_R_buf));
		do
		{
			Device_R_buf[Device_R_data_len]=*head;
			head++;
			Device_R_data_len++;
			if(*head=='\n')
			{
				data_line++;
			}
		}while(data_line<2);
		data_line=0;

		memset(Device_R_Global_buf,0,sizeof(Device_R_Global_buf));
		memcpy(Device_R_Global_buf,Device_R_buf,Device_R_data_len-1);
		Device_R_Global_data_len=Device_R_data_len-1;
//		USART3_SendData(Device_R_Global_buf,Device_R_Global_data_len);
	}
	
	if((head=strstr((char *)msg_buf,"rssi:"))!=NULL)
	{
		memset(RSSI_buf,0,sizeof(RSSI_buf));
		do
		{
			RSSI_buf[RSSI_data_len]=*head;
			head++;
			RSSI_data_len++;
			if(*head=='\n')
			{
				data_line++;
			}
		}while(data_line<1);
		data_line=0;
	
		memset(RSSI_Global_buf,0,sizeof(RSSI_Global_buf));
		memcpy(RSSI_Global_buf,RSSI_buf,RSSI_data_len-1);
		RSSI_data_Global_len=RSSI_data_len-1;
//		USART3_SendData(RSSI_Global_buf,RSSI_data_Global_len);
	}

	memset(msg_buf,0,sizeof(&msg_buf));
/*	for(i=0;i<data_len-1;i++)
	{
		msg[i]=data_buf[i];
	}
//	USART3_SendData(data_buf,data_len-1);
*/
	return len;
}

