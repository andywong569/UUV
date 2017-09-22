#pragma once
#include "Queue.h"

const unsigned char RCBOX_start[8]=
    {
    'S','T','R','C',0x01,0x00,0x00,0x00
    };  //���ݰ���ʼ��־
const unsigned char RCBOX_end[8]=
    {
    'E','D','R','C',0x01,0x00,0x00,0x00
    };  //���ݰ�������־
const unsigned char DISP_start[8]=
    {
    'S','T','R','C',0x01,0x00,0x00,0x00
    };  //���ݰ���ʼ��־
const unsigned char DISP_end[8]=
    {
    'E','D','R','C',0x01,0x00,0x00,0x00
    };  //���ݰ�������־

enum PARSE_NODE
{
   ROV_FIND_HEADER=0,
   ROV_FIND_TAIL,
   ROV_CHECK_CRC,

};

const unsigned char DATA_TOTAL_LEN=114;
const unsigned char DATA_HEAD_LEN=8;
const unsigned char DATA_TAIL_LEN=8;
//����ROV��������ԭʼ���ݣ�����ṹ��***********************************************
extern bool ROV_Recv_procrec(ListQueue *listqueue,unsigned char *datapackage);