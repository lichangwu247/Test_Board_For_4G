#ifndef __LINKLIST_H
#define __LINKLIST_H
#include "common.h"
#include "usart3.h"
#include "string.h"

/*********************************************************************************
**********************************************************************************
* 文件名称: LinkList.h                                                           *
* 文件简述：单链表头文件                                                         *
* 创建日期：2018.07.10                                                           *
* 版    本：V1.0                                                                 *
* 作    者：                                                                     *
* 说    明：                                                                     * 
*                                                                                *
**********************************************************************************
*********************************************************************************/

//List Node struct
typedef struct NODE
{   //定义链表结点
	struct NODE *next;   //指向下一个数据
	struct NODE *prev;   //指向上一个数据
	u16   LEN;
  char   Receive_Data[300];    //节点中的数据
}Node;       //定义一个节点变量

//List  struct
typedef struct
{
	Node *Head;
	Node *Tail;
  u32  num;
}List;

extern List *UART3_Recive_List;//创建一个链表

extern List *New_List(void);
extern Node *List_Push_Node(List *list,Node *node);
extern Node *Create_NewNode(void);
extern void List_Destroy(List *list);
extern Node *Get_First_Node(List *list);
extern Node *Get_Last_Node(List *list);

#endif
