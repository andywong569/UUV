#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
 using namespace std;
typedef unsigned char elemType;
struct  sNode{
    elemType data;            /* ֵ�� */
    struct sNode *next;        /* ����ָ�� */
  //  elemType lenth;   //����
};
struct queueLK{
    struct sNode *front;    /* ����ָ�� */
    struct sNode *rear;        /* ��βָ�� */
	int NodeNum;
};

class ListQueue
{
private :
	 queueLK Q;  //����һ������Q  
	 queueLK *hq;  //����һ������Q  
public :
	ListQueue();
	void InitQueue();
	void viewAllQueue();
	 void initQueue();
	 void enQueue( elemType x);
	 elemType outQueue();
	 elemType peekQueue(int index);
	 int emptyQueue();
	 void clearQueue();
	 void viewData(int x);
	int GetLength();

};