/** 
*  Copyright (c) 2017, �����֮�������豸�Ƽ����޹�˾(http://www.deepinfar.com/) 
*  All rights reserved. 
* 
*  @file        Sample.cpp 
*  @author      wangds
*  @email		dengsheng.wang@deepinfar.com
*  @date        2017/05/16 16:17 
* 
*  @brief       ʹ��Sublue UUV SDK�򵥵�demo 
*  @note        �ַ�����ʹ�� Unicode �ַ��� 
* 
*  @version 
*    - v1.0   
*/ 

#include "stdafx.h"
#include "PitchCtrl.h"

/** ����UUV���󣬴���ʱ����CreateUUV����������ʱ����ReleaseUUV */ 
IUUVObject* m_pUUV = nullptr;


/** ����ǰ�������ٶ� */ 
short nSpeed = 0;


/** ���帩���Ƕ� */ 
short nPitch = 0;


/** �������Ƕ� */ 
short nRoll = 0;


/** ����UUV״̬���� */ 
PUUV_STAT puuv_status = nullptr;


/** ����USBL���ݶ���ָ�� */ 
PUUV_USBL_INFO pusbl_data = nullptr;


/** �����Ƿ�������ʶ�� */ 
bool open_face_detect = false;


/** 
*  @brief		UUV�������ݻص����� 
*  @param[out]  pResult��UUV���巵�ص�����  
*  @return		��            
*/  
static void CALLBACK UUVResultShow(UUV_RESULT pResult);


/** 
*  @brief		��ӡUUV�������� 
*  @param[out]  ��
*  @return		��            
*/  
void ShowUUVData(void);


/** 
*  @brief       ����ͷ���ݻص����� 
*  @param[out]  pResult������ͷ���ص�yuv420����  
*  @return      ��          
*/  
static void CALLBACK UUVVideoFrameCallBack(UUV_RESULT pResult);


/** 
*  @brief		��ӡUSBL���� 
*  @param[out]  ��
*  @return		��            
*/  
void ShowUSBLData(void);


/** 
*  @brief       USBL���ݻص����� 
*  @param[out]  pResult��USBL���ص�����  
*  @return      ��          
*/  
static void CALLBACK UUVUSBLDataCallBack(UUV_RESULT pResult);


/** 
*  @brief       ��ʼ��UUV���� 
*  @param[in]   �� 
*  @return      
*/  
bool InitUUV()
{
	m_pUUV = CreateUUV(E_ROV_SHARK_MAX);
	if (m_pUUV == nullptr)
	{
		printf("����UUVʵ��ʧ��\n");
		exit(1);
	}
	else
	{
		if (!m_pUUV->UUV_RegHandler(UUVResultShow))
		{
			printf("ע��UUV�������ݻص�ʧ�ܣ�������%d\n", m_pUUV->UUV_GetErrno());
		}
		if (!m_pUUV->UUV_RegUSBLHandler(UUVUSBLDataCallBack))
		{
			printf("ע��UUV USBL���ݻص�ʧ�ܣ�������%d\n", m_pUUV->UUV_GetErrno());
		}
		if (!m_pUUV->UUV_RegVideoHandler(UUVVideoFrameCallBack))
		{
			printf("ע��UUV��Ƶ���ݻص�ʧ�ܣ�������%d\n", m_pUUV->UUV_GetErrno());
		}
	}
}


/** 
*  @brief       ������ 
*  @param[in]   ��   
*  @return      
*  @remarks     �ÿ�����Ѿ������������� 
*/  
void NetOpen()
{
	UUVNETOPEN netOpen;
	netOpen.strIP = _T("192.168.99.10");
	netOpen.nPort = 5001;
	netOpen.hWnd = NULL;
	if(!m_pUUV->UUV_Set(UUV_NET_OPEN, &netOpen))
	{
		printf("������ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}

	if (!m_pUUV->UUV_Set(UUV_USBL_NET_OPEN, NULL))
	{
		printf("��USBL����ʧ�ܣ�������%d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       �ر����� 
*  @param[in]   ��  
*  @return                  
*/  
void NetClose()
{
	if(!m_pUUV->UUV_Set(UUV_NET_CLOSE, nullptr))
	{
		printf("�ر�����ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}

	if(!m_pUUV->UUV_Set(UUV_USBL_NET_CLOSE, nullptr))
	{
		printf("�ر�USBL����ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ��ȡ����״̬ 
*  @param[in]   ��   
*  @return      
*/  
void NetState()
{
	UUV_RESULT pResult = new UUV_RESULT;
	if(!m_pUUV->UUV_Get(UUV_NET_STATE, pResult))
	{
		printf("��ȡ��������״̬ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
	else
	{
		E_NET_STATE* netState = (E_NET_STATE*)(pResult);
		if (!netState)
		{
			printf("����ת��ʧ��\n");
		}
		else
		{
			tstring strState = _T("����״̬δ֪");
			switch (*netState)
			{
			case E_NET_CONNING:
				printf("����״̬����������\n");
				break;
			case E_NET_CONNED:
				printf("����״̬��������\n");
				break;
			case E_NET_DISCONNING:
				printf("����״̬�����ڶϿ�\n");
				break;
			case E_NET_DISCONNED:
				printf("����״̬���ѶϿ�\n");
				break;
			default:
				break;
			}
		}
	}

	if (!pResult)
	{
		delete pResult;
		pResult = nullptr;
	}
}


/** 
*  @brief       ǰ��
*  @param[in]   ��  
*  @return          
*  @remarks     �����ٶ�ÿ��+1���ٶ����Ϊ100
*/ 
void Forward(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nSpeed++ > 100)
	{
		nSpeed = 100;
		printf("ǰ���ٶ��Ѵ����ֵ\n");
	}

	if(!m_pUUV->UUV_Set(UUV_FORWARD_BACKWARD_SPEED_SET, &nSpeed))
	{
		printf("����ǰ���ٶ�ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ����
*  @param[in]   ��  
*  @return          
*  @remarks     �����ٶ�ÿ��-1���ٶ���СΪ-100
*/ 
void Back(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nSpeed-- < -100)
	{
		nSpeed = -100;
		printf("�����ٶ��Ѵ����ֵ\n");
	}

	if(!m_pUUV->UUV_Set(UUV_FORWARD_BACKWARD_SPEED_SET, &nSpeed))
	{
		printf("���ú����ٶ�ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ����
*  @param[in]   ��  
*  @return          
*  @remarks     �����Ƕ�ÿ��1���Ƕ���СΪ450����45�㣩
*/ 
void Up(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nPitch++ > 450)
	{
		nPitch = 450;
		printf("���ӽ��Ѵ����ֵ\n");
	}
	
	//short nSpeed = puuv_status->ATT.pitch - 1;
	if(!m_pUUV->UUV_Set(UUV_ATT_PITCH_SET, &nPitch))
	{
		printf("��������ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ����
*  @param[in]   ��  
*  @return          
*  @remarks     �����Ƕ�ÿ��-1���Ƕ����Ϊ-450����-45�㣩
*/ 
void Down(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nPitch-- < -450)
	{
		nPitch = -450;
		printf("���ӽ��Ѵ����ֵ\n");
	}
	
	//short nSpeed = puuv_status->ATT.pitch + 1;
	if(!m_pUUV->UUV_Set(UUV_ATT_PITCH_SET, &nPitch))
	{
		printf("���ø���ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ����
*  @param[in]   ��  
*  @return          
*  @remarks     �����Ƕ�ÿ��-1���Ƕ���СΪ-450����-45�㣩
*/ 
void Left(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nRoll-- < -450)
	{
		nRoll = -450;
		printf("�������Ѵ����ֵ\n");
	}
	
	//short nSpeed = puuv_status->ATT.roll - 1;
	if(!m_pUUV->UUV_Set(UUV_ATT_ROLL_SET, &nRoll))
	{
		printf("��������ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       �Һ��
*  @param[in]   ��  
*  @return          
*  @remarks     �����Ƕ�ÿ��+1���Ƕ����Ϊ450����45�㣩
*/ 
void Right(void)
{
	unsigned char move_pattrn = 2;
	if(!m_pUUV->UUV_Set(UUV_WORK_PATTERN_SET, &move_pattrn))
	{
		printf("���ö���ģʽʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
		return;
	}

	if (nRoll++ > 450)
	{
		nRoll = 450;
		printf("�Һ�����Ѵ����ֵ\n");
	}
	
	//short nSpeed = puuv_status->ATT.roll + 1;
	if(!m_pUUV->UUV_Set(UUV_ATT_ROLL_SET, &nRoll))
	{
		printf("�����Һ��ʧ�ܣ������� %d\n", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       �ƹ����ã��������
*  @return      ��
*  @remarks     ����ֵÿ��+1�����Ϊ100���ҵ���СΪ-100��       
*/  
void LeftLight(void)
{
	UUVLIGHT_T light;
	light.lightindex = 0;	//	0-������ƣ�1-�����ҵ�
	light.lum = 100;
	if(!m_pUUV->UUV_Set(UUV_LIGHT_VALNE_SET, &light))
	{
		printf("�����������ʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       OSD����
*  @return      ��
*  @remarks     0-����ʾ��1-��ʾ
*/  
void OSD(void)
{
	UUVOSD_T osd;
	osd.osd_on = 1;	
	osd.battery_on = 1;
	if(!m_pUUV->UUV_Set(UUV_CAMERA_OSD_SET, &osd))
	{
		printf("����ͼ��OSD��ʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       ��е������
*  @return      ��
*  @remarks     0��ֹͣ״̬��-100��������ٶȱպϣ�100��������ٶ��ſ�
*/  
void ManipControl(void)
{
	int tmp = 100;
	if(!m_pUUV->UUV_Set(UUV_MANIP_OPERATION_SET, &tmp))
	{
		printf("���û�е�ֲ���ʧ�ܣ������� %d", m_pUUV->UUV_GetErrno());
	}
}


/** 
*  @brief       �û�������ݼ�
*  @param[in]   ��  
*  @return          
*  @remarks     �����б��е�û�������û������Զ������
*/ 
void Help(void)
{
	char* info = "\n����һ����ʹ��UUV������\n"
		"\t Hot keys: \n"
		"\t	h	- help \n"
		"\t	q	- �˳�����\n"
		"\t	o	- ������\n"
		"\t	c	- �ر�����\n"
		"\t	s	- ��ȡ����״̬\n"
		"\t	f	- ǰ�����������Ӻ�ſɽ��д˲�����\n"
		"\t	b	- ���ˣ��������Ӻ�ſɽ��д˲�����\n"
		"\t	u	- ���ӣ��������Ӻ�ſɽ��д˲�����\n"
		"\t	d	- ���ӣ��������Ӻ�ſɽ��д˲�����\n"
		"\t	l	- �������������Ӻ�ſɽ��д˲�����\n"
		"\t	r	- �Һ�����������Ӻ�ſɽ��д˲�����\n"
		"\t	p	- ��ӡ��������\n"
		"\t a   - ��ӡUSBL����\n"
		"\t	t	- Ŀ��ʶ��\n"
		"\t	*	- ���������ʼĿ������\n";

	printf("%s", info);
}


/** 
*  @brief       ������ڴ� 
*  @param[in]    
*  @param[in]   
*  @return      exit code
*/  
int main(int argc, char* argv[])
{
	Help();
	if (InitUUV())
	{
		while (true)
		{
			char c = (char)getch();
			switch (c)
			{
			case 'h':
				{
					Help();
				}
				break;
			case 'o':
				{
					NetOpen();
				}
				break;
			case 'c':
				{
					NetClose();
				}
				break;
			case 's':
				{
					NetState();
				}
				break;
			case 'f':
				{
					Forward();
				}
				break;
			case 'b':
				{
					Back();
				}
				break;
			case 'u':
				{
					Up();
				}
				break;
			case 'd':
				{
					Down();
				}
				break;
			case 'l':
				{
					Left();
				}
				break;
			case 'r':
				{
					Right();
				}
				break;
			case 't':
				{
					open_face_detect = true;
				}
				break;
			case 'p':
				{
					ShowUUVData();
				}
				break;
			case 'a':
				{
					ShowUSBLData();
				}
				break;
			case 'q':
				{
					NetClose();
					ReleaseUUV(m_pUUV);
				}
				exit(0);
			default:
				{
					PitchCtrlTest();
				}
				break;
			}
		}
	}

	return 0;
}


/** 
*  @brief       UUV���巵�ص����� 
*  @param[in]   pResult    
*  @return      ��
*  @remarks		�ûص�ֻ��������ʾ      
*  @see          
*/  
void CALLBACK UUVResultShow( UUV_RESULT pResult )
{
	if (puuv_status == nullptr)
	{
		puuv_status = new UUV_STAT;
	}
	puuv_status = (PUUV_STAT)pResult;
}




/** 
*  @brief       ��ʾUUV���巵�ص�����
*  @param[in]   ��    
*  @return      ��
*  @remarks		      
*  @see          
*/  
void ShowUUVData(void)
{
	if (puuv_status == nullptr)
	{
		printf("δ���յ�UUV���巵�ص����ݣ����������Ƿ�������\n");
		return;
	}
	// Add your code about info of UUV
	printf("����� = %0.1f\n", puuv_status->ATT.roll*0.1);
	printf("������ = %0.1f\n", puuv_status->ATT.pitch*0.1);
	printf("����� = %0.1f\n", puuv_status->ATT.yaw*0.1);
	printf("ǰ���ٶ� = %d\n", puuv_status->ATT.forwar_backwar);
	printf("����ģʽ = %d\n", puuv_status->ATT.operat_mode);
	printf("����ģʽ = %d\n", puuv_status->ATT.yewmode&0x01);
	printf("����ģʽ = %d\n", puuv_status->ATT.yewmode&0x02);
	printf("��ǰ��� = %0.2f\n", puuv_status->depth*0.01);
	printf("��ֱ��ǰ���ת�� = %d\n", puuv_status->motor.l_f_v_motor);
	printf("��ֱ�����ת�� = %d\n", puuv_status->motor.l_b_v_motor);
	printf("��ֱ�Һ���ת�� = %d\n", puuv_status->motor.r_b_v_motor);
	printf("��ֱ��ǰ���ת�� = %d\n", puuv_status->motor.r_f_v_motor);
	printf("ˮƽ����ת�� = %d\n", puuv_status->motor.l_h_motor);
	printf("ˮƽ�ҵ��ת�� = %d\n", puuv_status->motor.r_h_motor);
	//	δ�꣬��Ҫ��ӡ�����Ϣ���ڴ����
}


/** 
*  @brief       USBL���ص����� 
*  @param[in]   pResult    
*  @return      ��
*  @remarks		�ûص�ֻ��������ʾ      
*  @see          
*/  
void CALLBACK UUVUSBLDataCallBack(UUV_RESULT pResult)
{
	if (pusbl_data == nullptr)
	{
		pusbl_data = new UUV_USBL_INFO;
	}
	pusbl_data = (PUUV_USBL_INFO)pResult;
}

/** 
*  @brief       ��ʾUUV���巵�ص�����
*  @param[in]   ��    
*  @return      ��
*  @remarks		      
*  @see          
*/  
void ShowUSBLData(void)
{
	if (pusbl_data == nullptr)
	{
		printf("δ���յ�USBL���ص����ݣ����������Ƿ�������\n");
		return;
	}
	// Add your code about info of UUV
	printf("UUVλ�ڻ���ľ��� = %f��\n",pusbl_data->uuv_ralatived_distannce);
	printf("UUVλ�ڻ���ĽǶ� = %f��\n", pusbl_data->uuv_ralatived_direction);
	printf("UUV��Ի���ƫ�� = %f��\n", pusbl_data->uuv_ralatived_pos_north);
	printf("UUV��Ի���ƫ�� = %f��\n", pusbl_data->uuv_ralatived_pos_east);
	printf("UUV�ľ��� = %f��\n", pusbl_data->uuv_longitude);
	printf("UUV��γ�� = %f��\n", pusbl_data->uuv_latitude);

	//	δ�꣬��Ҫ��ӡ�����Ϣ���ڴ����
}



/** 
*  @brief       ���������Ȧ�� 
*  @param[in]   img��Ҫ����ͼ�� 
*  @param[in]	cascade������ͼ��ѵ����
*  @param[in]	nestedCascade������ͼ��ѵ����
*  @param[in]	scale��ͼ�����ű������ӿ����ٶ�
*  @param[in]	tryflip���Ƿ�ʹ�ܣ�ʹ�ܼ���ʸߣ����ٶ���
*  @return      ��         
*/  
void DetectAndDraw( cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale, bool tryflip )
{
	using namespace cv;

	//����һЩ��ɫ��������ʾ��ͬ������
	const static Scalar colors[] =  {
		CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255)} ;

	//������С��ͼƬ���ӿ����ٶ�
	//nt cvRound (double value) ��һ��double�͵��������������룬������һ����������
	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

	//ת�ɻҶ�ͼ��Harr�������ڻҶ�ͼ
	cvtColor( img, gray, CV_BGR2GRAY );
	//�ı�ͼ���С��ʹ��˫���Բ�ֵ
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
	//�任���ͼ�����ֱ��ͼ��ֵ������
	equalizeHist( smallImg, smallImg );
	//����ʼ�ͽ�������˺�����ȡʱ�䣬������������㷨ִ��ʱ��
	double t = (double)cvGetTickCount();

	//�������ڴ����������������
	vector<cv::Rect> faces, faces2;

	//�������
	//detectMultiScale������smallImg��ʾ����Ҫ��������ͼ��ΪsmallImg
	//faces��ʾ��⵽������Ŀ������
	//1.1��ʾÿ��ͼ��ߴ��С�ı���Ϊ1.1
	//2��ʾÿһ��Ŀ������Ҫ����⵽3�β��������Ŀ��(��Ϊ��Χ�����غͲ�ͬ�Ĵ��ڴ�С�����Լ�⵽����)
	//CV_HAAR_SCALE_IMAGE��ʾ�������ŷ���������⣬��������ͼ��
	//Size(30, 30)ΪĿ�����С���ߴ�
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		|CV_HAAR_SCALE_IMAGE
		,Size(30, 30));
	//���ʹ�ܣ���תͼ��������
	if( tryflip )
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale( smallImg, faces2,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|CV_HAAR_SCALE_IMAGE
			,Size(30, 30) );
		for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)cvGetTickCount() - t;
	printf("detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.));
	int i = 0;
	vector<Rect>::const_iterator r = faces.begin();
	for( ; r != faces.end(); ++r, ++i )
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i%8];
		int radius;

		double aspect_ratio = (double)r->width/r->height;
		if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
		{
			//��ʾ����ʱ����С֮ǰ��ͼ���ϱ�ʾ����������������ű��������ȥ
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
		else
		{
			rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
				cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
				color, 3, 8, 0);
		}
		if( nestedCascade.empty() )
			continue;
		smallImgROI = smallImg(*r);
		//ͬ�������������
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|CV_HAAR_SCALE_IMAGE
			,Size(30, 30) );
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); ++nr )
		{
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle( img, center, radius, color, 2, 8, 0 );
		}
	}

	/*	Ŀ�������ڣ��Ե�һĿ����һ���򵥵ĸ��٣�ֻ��������ʾ	*/
	if (r != faces.end())
	{
		Point faceCenter(cvRound(r->x + r->width*0.5), cvRound(r->y + r->height*0.5));	/*	Ŀ�����ĵ�	*/
		Point smallCenter(smallImg.cols*0.5, smallImg.rows*0.5);	/*	ԭʼͼ�����ĵ�	*/
		if (faceCenter.x < smallCenter.x && faceCenter.y < smallCenter.y)	/*	Ŀ����ԭʼͼ�����Ϸ�	*/
		{
			Left();
			Up();
			Forward();
		}
		else if (faceCenter.x < smallCenter.x && faceCenter.y > smallCenter.y)	/*	Ŀ����ԭʼͼ�����·�	*/
		{
			Left();
			Down();
			Forward();
		}
		else if (faceCenter.x > smallCenter.x && faceCenter.y < smallCenter.y)	/*	Ŀ����ԭʼͼ�����Ϸ�	*/
		{
			Right();
			Up();
			Forward();
		}
		else if (faceCenter.x > smallCenter.x && faceCenter.y > smallCenter.y)	/*	Ŀ����ԭʼͼ�����·�	*/
		{
			Right();
			Down();
			Forward();
		}
		else    /*	Ŀ����ԭʼͼ������λ�ã���������	*/
		{

		}
	}

	if (!faces.empty())
	{
		imshow( "ʶ����", img );
		waitKey(10000);
	}
}


/** 
*  @brief       ����ͷ���ݻص����� 
*  @param[in]   pResult��yuv420����֡    
*  @return      ��
*  @remarks		�ó���ֻ��������ʾ��������Ч�ʵȣ������ڻص�������ʱ������
*/  
void CALLBACK UUVVideoFrameCallBack( UUV_RESULT pResult )
{
	AVFrame* pFrameYUV = static_cast<AVFrame*>(pResult);
	if (pFrameYUV == nullptr)
	{
		return;
	}

	if (open_face_detect)
	{
		open_face_detect = false;	/*	Ϊ�˲���ͼ��֡������ֻʶ��һ�Σ���Ҫ����ʶ��Ļ���һֱ����ݼ�	*/

		using namespace cv;

		CascadeClassifier cascade, nestedCascade;
		cascade.load("data\\haarcascades\\haarcascade_frontalface_alt.xml");
		nestedCascade.load("data\\haarcascades\\haarcascade_eye.xml");

		Mat yuvImg;
		yuvImg.create(pFrameYUV->height*3/2, pFrameYUV->width, CV_8UC1);
		memcpy(yuvImg.data, pFrameYUV->data[0], (pFrameYUV->width)*(pFrameYUV->height));
		memcpy(yuvImg.data + (pFrameYUV->width)*(pFrameYUV->height), pFrameYUV->data[1], (pFrameYUV->width)*(pFrameYUV->height)/4);
		memcpy(yuvImg.data +(pFrameYUV->width)*(pFrameYUV->height) + (pFrameYUV->width)*(pFrameYUV->height)/4, pFrameYUV->data[2], (pFrameYUV->width)*(pFrameYUV->height)/4);

		Mat smallImg;
		resize(yuvImg, smallImg, Size(640, 480));
		Mat rgbImg;
		cvtColor(smallImg, rgbImg, CV_YUV2BGR_I420);

		DetectAndDraw(rgbImg, cascade, nestedCascade, 2, 0);
	}
}

