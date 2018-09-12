#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "common.h"


extern u8 Pub_Global_buf[200];
extern u8 ip_Global_buf[30];
extern u8 Device_S_Global_buf[100];
extern u8 Device_R_Global_buf[100];
extern u8 RSSI_Global_buf[15];
extern u16 Pub_data_Global_len,ip_data_Global_len,Device_S_data_Global_len,Device_R_Global_data_len,RSSI_data_Global_len;


int device_data_filter(u8 * msg,u16 len);


#endif




