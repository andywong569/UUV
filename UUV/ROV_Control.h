#pragma once
#include "UUV_Command_Param.h"
#include <windows.h>
typedef struct       //�����������,78�ֽ�
{
	UUV_U8 data_start[8];		//���ݿ�ʼ��־
	Attitude ATT;				//��̬�������� 20�ֽ�
	UUV_U16 power;					//�����豸�������	0������1�ر� bit0:����ͷ��bit1:POE,bit2:���CAN����
	CAM camdata[1];			//����ͷ����		2��12�ֽ�
	MOTOR motor;			//	���������ת�ٿ���
	Extraman extraman;		//��е������		8�ֽ�	
	UUV_U8 reserved[3];			//����������
	UUV_U8 OSD_Set;				//OSD���ƿ���
	UUV_U16 Send_Cnt;			//���ͼ���++(����ͨѶ)
	UUV_U16 crc16;				//crcУ��ֵ��������ʼ�ͽ�β�ı�־
	UUV_U8 data_end[8];			//���ݽ�β				//���ݳ���80�ֽڣ���ͷβ
}ROV_CMD;


//====================================================================================================
typedef struct      //״̬���������壬114�ֽ� 8+20+14(2*7)+24(12*2)+4(2*2)+8+16+4+6+2+8=14
{
	UUV_U8 data_start[8];		//���ݿ�ʼ��־

	Attitude ATT;				//��̬�������� 
	UUV_U16 power;					//�����豸�������	0������1�ر� bit0:����ͷ��bit1:POE,bit2:���CAN����
	
	UUV_S16 Temperature;		//����ˮ�£������¶���Ϣͨ��can���߻�ȡ
	UUV_U16 Salinity;			//�ζȣ����Ե絼�ʴ��������ٷֱ�
	UUV_S16 depth;				//��ȣ���λcm��0-100�׷�Χ U16
	UUV_S16 Temperature_in;		//�����¶�
	UUV_U16 pressure_in;			//����ѹ�� kPa
	UUV_U16 humidity_in;				//����ʪ�ȣ��ٷֱ�

	CAM camdata[1];			//����ͷ����			2��12�ֽ�
	MOTOR motor;			//	���������ת�ٿ���
	UUV_S16 llumination[2];	//����ͷ�ն���Ϣ
	Extraman extraman;		//��е������			8�ֽ�
	POWER p_inf;			//��Դ��Ϣ				16�ֽ�
	//HI_U8 ERROR[4];			//ϵͳ����״̬
	UUV_U16 Send_Packet_Num;	//ң��������F407�İ�����
	UUV_U16 Send_Packet_Loss_Num;	//ң��������F407�Ķ�������
	UUV_U16 Receive_Cnt;		//ROV���ͼ���(����ͨѶ)
	UUV_U16 crc16;				//crcУ��ֵ��������ʼ�ͽ�β�ı�־
	UUV_U8 data_end[8];			//���ݽ�β
}ROV_STAT,*PROV_STAT;


const int Depth_Offset=100;
class ROV_Control
{
public:
	ROV_Control(void);
	~ROV_Control();
	ROV_CMD CMD_RCtoROV;      //RC��Ҫ����ROV������
	ROV_STAT STAT_ROVtoRC;      //ROV�����������ݣ�����֮��

	void ROV_CMD_Init();
		/******************************************************************************
	 * Function: RC_CMD_Assemble_New - ң�����������ݴ���
	 * Description: 
	 *     ң�����������ݴ��� 
	 * Input: 
	 * Output: 
	 * Returns: 
	 * 
	 * revision history:
	 *     version 1.0 
	******************************************************************************/
	void ROV_RC_CMD_Assemble(void);
	void Set_Reset_Flag(bool flag);
	bool Get_Reset_Flag();

	public:
	short m_temproll;//���
	short m_tempitch;//����
	short m_temyaw;//����
	short m_temspeed;//ǰ���ٶ�
	short m_temdepth;//Ŀ�����
	short m_camerapitch;//��̨����
	short m_camerarotate;//��̨��ת
	short m_camerazoom;//��ͷ�䱶
	short m_camerafocus;//��ͷ�۽�
	unsigned char m_leftlight;//��ͷ�۽�
	unsigned char m_rightlight;//��ͷ�۽�
	short m_left_forward_motor;//��ǰ���
	short m_left_backward_motor;//��ǰ���
	short m_right_forward_motor;//��ǰ���
	short m_right_backward_motor;//��ǰ���
	short m_left_H_motor;//��ǰ���
	short m_right_H_motor;//��ǰ���

private:
	 bool m_need_reset;
	HANDLE m_Mutex;
};


