#include "stdafx.h"
#include "Queue.h"

ListQueue::ListQueue()
{
	hq=&Q;
	this->InitQueue();
}

/*��ʾ�����е����е�Ԫ��*/
void ListQueue::viewAllQueue()
{
	struct sNode *newP;
	//��սṹ
	newP = (sNode *)malloc(sizeof(struct sNode));
	if(newP == NULL)
	{
		printf("�ڴ�ռ����ʧ�ܣ� ");
		exit(1);
	}
	//���׵�ַ������ʱ�ṹ
	newP = hq->front;    
	//�����ʱ�ṹû��ָ�����
	while(newP != NULL)
	{
		//��ӡ������
		printf(" %d\r\n",newP->data);
		//����ʱ�ṹ��ָ��ָ����һ���ڵ��ַ
		newP = newP->next;
	}
	free(newP);
	return;
}

/* ��ʼ������ */
void ListQueue::InitQueue()
{
	hq->front = hq->rear = NULL;        /* �Ѷ��׺Ͷ�βָ���ÿ� */
	hq->NodeNum =0;
	return;
}

/* �������в���һ��Ԫ��x ��������Ԫ��*/
void ListQueue::enQueue( elemType x)
{
	/* �õ�һ����newPָ����ָ����½�� */
	struct sNode *newP;
	newP = (sNode *)malloc(sizeof(struct sNode));
	if(newP == NULL){
		printf("�ڴ�ռ����ʧ�ܣ� ");
		exit(1);
	}
	/* ��x��ֵ�����½���ֵ�򣬰��½���ָ�����ÿ� */
	newP->data = x;
	newP->next = NULL;
	/* ������Ϊ�գ����½�㼴�Ƕ��׽�����Ƕ�β��� */
	if(hq->rear == NULL)//�п��õ�βָ��
	{
		hq->front = hq->rear = newP;
		hq->NodeNum = 1;
		//printf("%d",hq->rear->lenth);
	}
	else
	{    /* �����ӷǿգ��������޸Ķ�β����ָ����Ͷ�βָ�룬ʹָ֮���µĶ�β��� */

		hq->rear = hq->rear->next = newP;        /* ע�⸳ֵ˳��Ŷ */
		hq->NodeNum ++;
	}
	//free(newP);
	return;
}

/* �Ӷ�����ɾ��һ��Ԫ�� ����ǰ��ɾ��*/
elemType ListQueue::outQueue()
{
	struct sNode *p;
	elemType temp;
	/* ������Ϊ����ֹͣ���� */
	if(hq->front == NULL){
		printf("����Ϊ�գ��޷�ɾ���� ");
		exit(1);
	}
	temp = hq->front->data;        /* �ݴ��βԪ���Ա㷵�� */
	p = hq->front;                /* �ݴ��βָ���Ա���ն�β��� */
	hq->front = p->next;        /* ʹ����ָ��ָ����һ����� */
	hq->NodeNum--;//��һ��Ԫ�� ��������һ
	/* ��ɾ��������Ϊ�գ�����ͬʱʹ��βָ��Ϊ�� */
	if(hq->front == NULL){
		hq->rear = NULL;
		hq->NodeNum =0;
	}
	free(p);        /* ����ԭ���׽�� */
	return temp;    /* ���ر�ɾ���Ķ���Ԫ��ֵ */
}

int ListQueue::GetLength()
{
	return hq->NodeNum;
}
///* ��ȡ����Ԫ�� */
//int ListQueue::peekQueue()
//{
//    /* ������Ϊ����ֹͣ���� */
//    if(hq->front == NULL){
//        printf("����Ϊ�գ��޷�ɾ���� ");
//        exit(1);
//    }
//    return hq->front->data;        /* ���ض���Ԫ�� */
//}

/* ��������Ƿ�Ϊ�գ���Ϊ���򷵻�1, ���򷵻�0 */
int ListQueue::emptyQueue()
{
	/* �ж϶��׻��β��һ��ָ���Ƿ�Ϊ�ռ��� */
	if(hq->front == NULL){
		return 1;
	}else{
		return 0;
	}
}

/* ��������е�����Ԫ�� */
void ListQueue::clearQueue()
{
	struct sNode *p = hq->front;        /* ����ָ�븳��p */
	/* ����ɾ�������е�ÿһ����㣬���ʹ����ָ��Ϊ�� */
	while(p != NULL){
		hq->front = hq->front->next;
		free(p);
		p = hq->front;
	}    /* ѭ�����������ָ���Ѿ�Ϊ�� */
	hq->rear = NULL;        /* �ö�βָ��Ϊ�� */
	free(p);
	return;

}

elemType ListQueue::peekQueue(int index)
{
	//������
	int count;
	int get_lenth;
	get_lenth = 0;
	count = 0;
	elemType temp;
	struct sNode *newP;
	get_lenth = hq->NodeNum ;
	//��սṹ
	/*  newP = (sNode *)malloc(sizeof(struct sNode));
	if(newP == NULL)
	{
	printf("�ڴ�ռ����ʧ�ܣ� ");
	exit(1);
	}*/
	//���׵�ַ������ʱ�ṹ
	newP = hq->front;    
	if (index>get_lenth || index<0)
	{
		printf("error input data\n");
		return 0;
	}
	//�����ʱ�ṹû��ָ�����
	while(newP != NULL)
	{
		if (count ==index)
		{
			//��ӡ������
			temp=newP->data;
		}
		//����ʱ�ṹ��ָ��ָ����һ���ڵ��ַ
		newP = newP->next;
		count++;
	}
	/*free(newP);*/
	return temp;
}
