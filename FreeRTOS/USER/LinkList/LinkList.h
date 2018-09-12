#ifndef __LINKLIST_H
#define __LINKLIST_H
#include "common.h"
#include "usart3.h"
#include "string.h"

/*********************************************************************************
**********************************************************************************
* �ļ�����: LinkList.h                                                           *
* �ļ�������������ͷ�ļ�                                                         *
* �������ڣ�2018.07.10                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�                                                                     *
* ˵    ����                                                                     * 
*                                                                                *
**********************************************************************************
*********************************************************************************/

//List Node struct
typedef struct NODE
{   //����������
	struct NODE *next;   //ָ����һ������
	struct NODE *prev;   //ָ����һ������
	u16   LEN;
  char   Receive_Data[300];    //�ڵ��е�����
}Node;       //����һ���ڵ����

//List  struct
typedef struct
{
	Node *Head;
	Node *Tail;
  u32  num;
}List;

extern List *UART3_Recive_List;//����һ������

extern List *New_List(void);
extern Node *List_Push_Node(List *list,Node *node);
extern Node *Create_NewNode(void);
extern void List_Destroy(List *list);
extern Node *Get_First_Node(List *list);
extern Node *Get_Last_Node(List *list);

#endif
