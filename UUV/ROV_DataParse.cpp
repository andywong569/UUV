#include "stdafx.h"
#include "ROV_DataParse.h"
#include "BaseFunc.h"

//����ROV��������ԭʼ���ݣ�����ṹ��***********************************************
bool ROV_Recv_procrec(ListQueue *listqueue,unsigned char *datapackage)
{
	bool  Receive_Success_flag=false;
	unsigned short crc_tmp;
	int State = ROV_FIND_HEADER;
	int count;
	unsigned char * p;
	unsigned char Temp_Buffer[200];
	//STAT_ROVtoRC_Last = STAT_ROVtoRC;

	while ((listqueue->GetLength()>0 && (State != ROV_FIND_HEADER)) || ((listqueue->GetLength()>=DATA_TOTAL_LEN) && (State == ROV_FIND_HEADER)))
	{
		switch (State)
		{
		case ROV_FIND_HEADER:     //Ѱ�Ұ�ͷ
			while (listqueue->GetLength()>=DATA_TOTAL_LEN)
			{
				for (count = 0; count < DATA_HEAD_LEN; count ++)//���ͷ
				{
					if (listqueue->peekQueue (count) != RCBOX_start[count])
					{
						break;
					}
				}
				if (count==DATA_HEAD_LEN)//���ͷ�ɹ�
				{
					State=ROV_FIND_TAIL;  //8���ֽ���ȫ���
					break;
				}
				else
				{
					listqueue->outQueue ();// �����ͷƥ�䲻�ɹ�����Ҫ������������
				}
			}
			break;

		case ROV_FIND_TAIL:     //�жϰ�β  //Ѱ�Ұ�ͷʱ�Ѿ�ȷ�����ݳ��Ⱥ��ʣ����ﲻ���ж�

			for (count = 0; count < DATA_TAIL_LEN; count ++)//���ͷ
			{
				if (listqueue->peekQueue (DATA_TOTAL_LEN-DATA_TAIL_LEN+count) != RCBOX_end[count])
				{
					break;
				}
			}
			if (count == DATA_TAIL_LEN)
				State = ROV_CHECK_CRC;   //8���ֽ���ȫ���
			else
			{
				State = ROV_FIND_HEADER;
				for (int temp = 0; temp < DATA_HEAD_LEN; temp++)
				{
					listqueue->outQueue ();//�����β�Ŀ�ʼ�����������Ҫ����һ����������
				}
			}
			break;

		case ROV_CHECK_CRC:     //��֤crc���������ִ��can���ͣ�������

			for(count=0;count<DATA_TOTAL_LEN;count++)
			{
				Temp_Buffer[count]= listqueue->peekQueue(count);
			}
			crc_tmp = Tcp_Crc16_Check(Temp_Buffer + 8, DATA_TOTAL_LEN - 18);


			if(crc_tmp == (listqueue->peekQueue(DATA_TOTAL_LEN-10)+256*listqueue->peekQueue(DATA_TOTAL_LEN-9)))
			{//crc��ȷ�������������ݰ�

				for (int tmp = 0; tmp <DATA_TOTAL_LEN; tmp++)
				{
					datapackage[tmp]= listqueue->outQueue();//���������
				}
				Receive_Success_flag = true;
			}
			else
			{
				for (int tmp = 0; tmp <DATA_TOTAL_LEN; tmp++)
				{
					listqueue->outQueue();//���������
				}
			}         
			State = ROV_FIND_HEADER;
			break;

		default:
			State = ROV_FIND_HEADER; //��ת��Ѱ�Ұ�ͷ��ģʽ
			break;
		}
	}
	return Receive_Success_flag;

}


static UUV_F32 ByteToFloat(UUV_U8* byteArray)  
{  
    return *((float*)byteArray);  
}  

static UUV_F64 ByteToDouble(UUV_U8* byteArray)  
{  
    return *((double*)byteArray);  
}  


/** 
*  @brief       ����USBL���ص����� 
*  @param[in]   pResult    
*  @return      ��
*  @remarks		�ûص�ֻ��������ʾ      
*  @see          
*/  
bool USBL_Recv_procrec(UUV_U8 *pdata,int datalen)
{
	bool ret = false;
	//�ж����ݵĳ��� //�ж����ݵİ�ͷ��β
	UUV_U16 temp_len = *(UUV_U16*)(pdata+2);
	UUV_U16 head = *(UUV_U16*)pdata;
	UUV_U8 tail = *(pdata+datalen-1);
	if((temp_len==datalen)&&(head==0xAAAA)&&(tail==0x0A))
	{
		//�ж����ݵ�У���
		UUV_U16 crc= *(UUV_U16*)(pdata+datalen-3);
		UUV_U16 calc_crc= Udp_CalcCRC16(0, 5, datalen -8, pdata);
		if(crc==calc_crc)
		{
			 ret = true;
		}
	}
	return ret;

	//������Ŀ������
}