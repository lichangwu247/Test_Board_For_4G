/*********************************************************************************
**********************************************************************************
* 文件名称: LinkList.c                                                           *
* 文件简述：单链表操作文件                                                       *
* 创建日期：2018.07.10                                                           *
* 版    本：V1.0                                                                 *
* 作    者：                                                                     *
* 说    明：                                                                     * 
*                                                                                *
**********************************************************************************
*********************************************************************************/
#include "LinkList.h"
#include "stdlib.h"



/****************************************************************************
* 名    称: List* New_List(void)
* 功    能:  创建一个链表
* 入口参数:无
*           
* 返回参数：返回一个List链表
* 说    明：       
****************************************************************************/	
//List *UART3_Send_List;

List* New_List(void)
{  
	List *NewList;
	NewList=(List *)malloc(sizeof(List));
	if(NewList==NULL)
		return NULL;
  NewList->Head=NULL;
	NewList->num=0;
	NewList->Tail=NULL;
	return NewList;
}

/****************************************************************************
* 名    称: 
* 功    能: 插入一个节点到链表中（在链表尾部插入数据节点）
* 入口参数：*list  指向要插入的链表
*					  *node  要插入到链表的节点
*						 len   要插入数据的长度
*            *Buffer 要插入到节点中的数据
* 返回参数：返回Node：插入链表成功，返回0  插入失败
* 说    明：       
****************************************************************************/	
Node *List_Push_Node(List *list,Node *node)
{
	if(!node) return NULL;
  
	if(list->num)//如果链表节点数大于0，
	{
		node->prev=list->Tail;    //要插入节点的前驱，指向链表最后一个元素
		node->next=NULL;          //要插入节点的后继，为空
		list->Tail->next=node;    //链表最后一个元素的后继指向要插入的节点
		list->Tail=node;         // 链表的尾指针指向新插入的元素
	}
	else
	{
		list->Head=list->Tail=node;  //插入第一个数据，链表头尾指针指向第一个节点
		node->prev=node->next=NULL;  //插入第一个节点，前后都没有数据，都指向NULL
	}
	++list->num;//链表节点数加一
  return node;
}

/****************************************************************************
* 名    称: Create_NewNode
* 功    能: 创建一个新节点
* 入口参数：u16 len  数据长度
*						u8  *Buffer   指向的数据
*           
* 返回参数：返回一个节点：初始化节点成功，返回0，创建节点失败
* 说    明：       
****************************************************************************/	
Node *Create_NewNode(void)
{
	Node *NewNode;
	NewNode = (Node *)malloc(sizeof(Node));
	if(NewNode==NULL) 
	{
	  return NULL;
	}
  NewNode->LEN=0;
	NewNode->next=NULL;
	NewNode->prev=NULL;
	return NewNode;
	
}
/****************************************************************************
* 名    称: List_Destroy(List *list)
* 功    能: 释放一个链表
* 入口参数：*list要释放的链表
*           
* 返回参数：无
* 说    明：       
****************************************************************************/	
void List_Destroy(List *list)
{
  unsigned int len=list->num;
	Node *next;
	Node *curr=list->Head;
	while(len--)
	{
		next=curr->next;
		free(curr);//释放当前节点
		curr=next;
	}
	free(list); //释放链表
}
/****************************************************************************
* 名    称: Get_First_Node
* 功    能: 得到链表中第一个节点
* 入口参数：*list 要取数据的链表
*           
* 返回参数：得到的节点
* 说    明：       
****************************************************************************/	
Node *Get_First_Node(List *list)
{
	Node *node=NULL;
	if(!list->num) return NULL; //如果链表长度为0，返回NULL
	
	node=list->Head;   //指向头节点
	
	if(--list->num)//如果链表长度大于1
	{
	  (list->Head=node->next)->prev=NULL;//链表的头指针指向下一个节点，第一个节点的前驱为空
	}
	else          //链表长度小于1
	{
			list->Head=list->Tail=NULL;//链表头与尾都指向空
	}
	node->next=node->prev=NULL;  //取出的节点前后驱指向空
	return node;
}
/****************************************************************************
* 名    称: Get_Last_Node
* 功    能: 得到链表中最后一个节点
* 入口参数：*list 要取数据的链表
*           
* 返回参数：得到的节点
* 说    明：       
****************************************************************************/	
Node *Get_Last_Node(List *list)
{ 
	Node *node=NULL;
	
	if(!list->num) return NULL; //如果链表长度为0，返回NULL
	
	node=list->Tail;//指向最后一个节点
	
	if(--list->num)//如果链表长度大于1
	{
		(list->Tail=node->prev)->next=NULL;//链表的尾指针指向最后一个节点的前驱，最后一个节点的后继为空
	}
	else        //链表长度小于1
	{
		list->Tail=list->Head=NULL;  //链表头与尾都指向空
	}
	
	node->next=node->prev=NULL;
	return node;
}

