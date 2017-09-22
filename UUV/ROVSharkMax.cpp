#include "stdafx.h"
#include "ROVSharkMax.h"
#include "PacketizedTcpImpl.h"

#include "ToolTask.h"

const char* DEFAULT_ADDR = "127.0.0.1";
const unsigned short DEFAULT_PORT = 12345;

UUVHandler CROVSharkMax::m_pHandler = nullptr;
UUVVideoFrame CROVSharkMax::m_pVideoHandler = nullptr;

CDirectUtility CROVSharkMax::m_dxUtility;
HWND CROVSharkMax::m_hWnd = NULL;


CROVSharkMax* g_pRovSharkMax = nullptr;

CROVSharkMax::CROVSharkMax(void) : 
	m_spTcpClient(CreatePacketizedTcpClient()), 
	m_spUdpClient(CreatePacketizedUdpClient())
{
	g_pRovSharkMax = this;

	m_unCount = 0;
}

CROVSharkMax::~CROVSharkMax(void)
{
	m_taskManager.cancelAll();

	if (m_spTcpClient)
	{
		m_spTcpClient->Disconnect();
	}

	if (m_spUdpClient)
	{
		m_spUdpClient->Disconnect();
	}
}

bool CROVSharkMax::UUV_RegHandler( UUVHandler pHandler )
{
	if (pHandler == nullptr)
	{
		m_nErrno = ERR_INVALIDPARAM;
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("UUV����ص�����ע��ʧ�ܣ�������%d"), m_nErrno);
		return false;
	}

	m_pHandler = pHandler;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("UUV����ص�����ע��ɹ�"));
	return true;
}

bool CROVSharkMax::UUV_RegVideoHandler( UUVVideoFrame pVideoHandler )
{
	if (pVideoHandler == nullptr)
	{
		m_nErrno = ERR_INVALIDPARAM;
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����ͷ�ص�����ע��ʧ�ܣ�������%d"), m_nErrno);
		return false;
	}

	m_pVideoHandler = pVideoHandler;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ͷ�ص�����ע��ɹ�"));
	return true;
}

bool CROVSharkMax::UUV_Get( UUV_COMMAND strCommand, UUV_RESULT& pResult )
{
	switch (strCommand)
	{
	case UUV_NET_STATE:
		{
			return Net_State(pResult);
		}
		break;
	default:
		{
			m_nErrno = ERR_NOTSUPPORTED;
		}
		break;
	}

	return false;
}

bool CROVSharkMax::UUV_Set( UUV_COMMAND strCommand, UUV_PARAM param )
{
	bool bRet = false;

	switch (strCommand)
	{
	case UUV_NET_OPEN:
		{
			bRet = Net_Open(param);
		}
		break;
	case UUV_NET_CLOSE:
		{
			bRet = Net_Close();
		}
		break;
	case UUV_ATT_ROLL_SET://���ú����
		{
			m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .roll = 0x20;
			bRet = Attitude_Roll_Set(param);
		}
		break;
	case UUV_ATT_PITCH_SET://���ø�����
		{
			bRet = Attitude_Pitch_Set(param);
		}
		break;
	case UUV_ATT_YAW_SET://���ú����
		{
			bRet = Attitude_Yaw_Set(param);
		}
		break;
	case UUV_FORWARD_BACKWARD_SPEED_SET://����ǰ���ٶ�
		{
			bRet = Forward_Backward_Speed_Set(param);
		}
		break;
	case UUV_YAW_CONTROL_SET://���ú���ǿ��Ʒ�ʽ
		{
			bRet = Yaw_Control_Pattern_Set(param);
		}
		break;
	case UUV_YAW_NAVIGATES_SET://���õ������Ʒ�ʽ
		{
			bRet = Navigation_Pattern_Set(param);
		}
		break;
	case UUV_TARGET_DEPTH_SET://���õ������Ʒ�ʽ
		{
			bRet = Target_Depth_Set(param);
		}
		break;
	case UUV_CAMERA_PITCH_SET://������̨����
		{
			bRet = Camera_Pitch_Set(param);
		}
		break;
	case UUV_CAMERA_ROTATE_SET://������̨��ת
		{
			bRet = Camera_Rotate_Set(param);
		}
		break;
	case UUV_CAMERA_ZOOM_SET://���þ�ͷ�佹
		{
			bRet = Camera_Zoom_Set(param);
		}
		break;
	case UUV_CAMERA_FOUCUS_SET://���þ�ͷ�۽�
		{
			bRet = Camera_Focus_Set(param);
		}
		break;
	case UUV_LIGHT_VALNE_SET://���þ�ͷ�۽�
		{
			bRet = Light_Value_Set(param);
		}
		break;
	case UUV_MOTOR_SPEED_SET://���õ��ת��
		{
			bRet = Motor_Speed_Set(param);
		}
		break;
	case UUV_WORK_PATTERN_SET://���õ��ת��
		{
			bRet = Work_Pattern_Set(param);
		}
		break;
	case UUV_POE_CAN_POWER_SET://��������豸����
		{
			bRet = External_Power_Set(param);
		}
		break;
	case UUV_SYSTEM_POWER_SET://����max������
		{
			bRet = System_Power_Set(param);
		}
		break;
	case UUV_CAMERA_OSD_SET://����osd����
		{
			bRet = Camera_Osd_Set(param);
		}
		break;
	case UUV_CONSUMPTION_SET://���ù���ģʽ
		{
			bRet = Power_ConsumMode_Set(param);
		}
		break;
	case UUV_MANIP_OPERATION_SET:
		{
			bRet = Manipulator_Operation_Set(param);
		}
		break;
	default:
		{
			m_nErrno = ERR_NOTSUPPORTED;
		}
		break;
	}

	return bRet;
}

template<typename T> bool ParaFilter(T *a, T refer)
{
	if(2==sizeof(T))
	{
		//*a=*a>refer?refer:(*a<((T)0-refer)?((T)0-refer):*a);
		if((*a>refer)||(*a<((T)0-refer)))
			return false;
	}
	else if(1==sizeof(T))
	{
		//*a=*a>refer?refer:(*a<(T)0?(T)0:*a);
		if((*a>refer)||(*a<(T)0))
			return false;
	}

	return true;
}

bool CROVSharkMax::Attitude_Roll_Set( UUV_PARAM param )
{
	short *ptemproll= static_cast<short*>(param);
	if (!ptemproll||!ParaFilter(ptemproll,(short)450))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//temproll=temproll>450?450:(temproll<-450?-450:temproll);
	m_tcpClientNetEventHandler.rov_control.m_temproll =*ptemproll;
	//m_clientNetEventHandler.rov_control .CMD_RCtoROV .ATT .roll =*ptemproll;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��������ã����� = %d"), *ptemproll);
	return true;
}

bool CROVSharkMax::Manipulator_Operation_Set( UUV_PARAM param )
{
	short *opera_speed= static_cast<short*>(param);
	if (!opera_speed||!ParaFilter(opera_speed,(short)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .extraman.switching=(signed char)(*opera_speed);
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��е�����ã����� = %d"), *opera_speed);
	return true;
}
bool CROVSharkMax::Attitude_Pitch_Set( UUV_PARAM param )
{
	short *ptempitch= static_cast<short*>(param);

	if (!ptempitch||!ParaFilter(ptempitch,(short)450))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//tempitch=tempitch>450?450:(tempitch<-450?-450:tempitch);
	m_tcpClientNetEventHandler.rov_control.m_tempitch =*ptempitch;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("���������ã����� = %d"), *ptempitch);
	return true;
}
bool CROVSharkMax::Attitude_Yaw_Set( UUV_PARAM param )
{
	short *ptemyaw=static_cast<short*>(param);
	short fixvalue=0;
	if(0==m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV.ATT.yewmode&0x01)//�жϵ�ǰ�Ŀ���ģʽ 0Ϊ����ģʽ 1Ϊ��̬����ģʽ
	{
		fixvalue=100;//����ģʽ�ķ�ΧΪ-100~100
	}
	else
	{
		fixvalue=1800;//��̬ģʽ�ķ�ΧΪ-1800~1800
	}
	if (!ptemyaw||!ParaFilter(ptemyaw,fixvalue))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//temyaw=temyaw>fixvalue?fixvalue:(temyaw<(0-fixvalue)?(0-fixvalue):temyaw);
	m_tcpClientNetEventHandler.rov_control.m_temyaw =*ptemyaw;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��������ã����� = %d"), *ptemyaw);
	return true;
}
bool CROVSharkMax::Forward_Backward_Speed_Set( UUV_PARAM param )
{
	short *ptemspeed= static_cast<short*>(param);
	if (!ptemspeed||!ParaFilter(ptemspeed,(short)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	m_tcpClientNetEventHandler.rov_control.m_temspeed =*ptemspeed;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("ǰ���ٶ����ã����� = %d"), *ptemspeed);
	return true;
}

bool CROVSharkMax::Target_Depth_Set( UUV_PARAM param )
{
	short *ptemdepth= static_cast<short*>(param);

	if (!ptemdepth||*ptemdepth>10000||*ptemdepth<0)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//*ptemdepth=*ptemdepth>9999?9999:(*ptemdepth<0?0:*ptemdepth);
	m_tcpClientNetEventHandler.rov_control.m_temdepth =*ptemdepth;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("������ã����� = %d"), *ptemdepth);
	return true;
}


bool CROVSharkMax::Yaw_Control_Pattern_Set( UUV_PARAM param )
{
	unsigned char *yaw_pattern= static_cast<unsigned char*>(param);
	if((NULL==yaw_pattern)||((0!=*yaw_pattern)&&(1!=*yaw_pattern)))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(0==*yaw_pattern)//����ģʽ
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode  &=0xfe;//bit0 ��0
	}
	else if(1==*yaw_pattern)//��̬ģʽ
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode  |=0x01;//bit0 ��1
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�������ģʽ���ã����� = %d"), m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode&0x01);

	return true;
}

bool CROVSharkMax::Navigation_Pattern_Set( UUV_PARAM param )
{
	unsigned char *navi_pattern= static_cast<unsigned char*>(param);
	if((NULL==navi_pattern)||((0!=*navi_pattern)&&(1!=*navi_pattern)))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(0==*navi_pattern)//�ߵ�ģʽ
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode &=~(1<<1);//bit1 ��0
	}
	else if(1==*navi_pattern)//�ŵ�ģʽ
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode |=(1<<1);//bit1 ��1
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ģʽ���ã����� = %d"), m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT .yewmode&(1<<1));

	return true;
}

bool CROVSharkMax::Camera_Pitch_Set( UUV_PARAM param )
{
	short *pcamerapitch= static_cast<short*>(param);
	if (!pcamerapitch||!ParaFilter(pcamerapitch,(short)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//ParaFilter(pcamerapitch,(short)100);
	m_tcpClientNetEventHandler.rov_control.m_camerapitch =*pcamerapitch;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��̨�������ã����� = %d"), *pcamerapitch);

	return true;
}

bool CROVSharkMax::Camera_Rotate_Set( UUV_PARAM param )
{
	short *pcamerarotate= static_cast<short*>(param);
	if (!pcamerarotate||!ParaFilter(pcamerarotate,(short)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//ParaFilter(pcamerarotate,(short)100);
	m_tcpClientNetEventHandler.rov_control.m_camerarotate =*pcamerarotate;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��̨��ת���ã����� = %d"), *pcamerarotate);

	return true;
}
bool CROVSharkMax::Camera_Zoom_Set( UUV_PARAM param )
{
	unsigned char *pcamerazoom= static_cast<unsigned char*>(param);
	if (!pcamerazoom||!ParaFilter(pcamerazoom,(unsigned char)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//ParaFilter(pcamerazoom,(short)100);
	m_tcpClientNetEventHandler.rov_control.m_camerazoom =*pcamerazoom;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("������佹���ã����� = %d"), *pcamerazoom);

	return true;
}
bool CROVSharkMax::Camera_Focus_Set( UUV_PARAM param )
{
	unsigned char *pcamerafocus= static_cast<unsigned char*>(param);
	if (!pcamerafocus||!ParaFilter(pcamerafocus,(unsigned char)100))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//ParaFilter(pcamerafocus,(short)100);
	m_tcpClientNetEventHandler.rov_control.m_camerafocus =*pcamerafocus;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("������۽����ã����� = %d"), *pcamerafocus);

	return true;
}
bool CROVSharkMax::Light_Value_Set( UUV_PARAM param )
{
	PUUVLIGHT_T pligt= static_cast<PUUVLIGHT_T>(param);
	if (!pligt||!ParaFilter(&pligt->lum,(unsigned char)100)||((0!=pligt->lightindex)&&(1!=pligt->lightindex)))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	//ParaFilter(&pligt->lum ,(unsigned char)100);

	if(0==pligt->lightindex )
	{
		m_tcpClientNetEventHandler.rov_control.m_leftlight =pligt->lum;
	}
	else if(1==pligt->lightindex)
	{
		m_tcpClientNetEventHandler.rov_control.m_rightlight =pligt->lum;
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��%d�������ã����� = %d"),pligt->lightindex, pligt->lum);
	return true;
}

bool CROVSharkMax::Motor_Speed_Set(UUV_PARAM param )
{
	PUUVMOTOR_T pmotor= static_cast<PUUVMOTOR_T>(param);
	short refervalue=0;
	if (!pmotor)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	switch(pmotor->motorindex )
	{
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
		{
			refervalue=2800;//�߹���2800ת
		}
		break;
	case 0x58:
	case 0x59:
		{
			refervalue=1600;//�߹���1600ת
		}
		break;
	default :
		{
			m_nErrno = ERR_INVALIDPARAM;//��������
			return false;
		}
		break;

	}
	if(!ParaFilter(&pmotor->motorspeed  ,refervalue))
	{
		m_nErrno = ERR_INVALIDPARAM;//ֵ����
		return false;
	}

	switch(pmotor->motorindex )
	{
	case 0x50:
		{
			m_tcpClientNetEventHandler.rov_control.m_left_forward_motor  =pmotor->motorspeed;
		}
		break;
	case 0x51:
		{
			m_tcpClientNetEventHandler.rov_control.m_left_backward_motor  =pmotor->motorspeed;
		}
		break;
	case 0x52:
		{
			m_tcpClientNetEventHandler.rov_control.m_right_forward_motor  =pmotor->motorspeed;
		}
		break;
	case 0x53:
		{
			m_tcpClientNetEventHandler.rov_control.m_right_backward_motor  =pmotor->motorspeed;
		}
		break;
	case 0x58:
		{
			m_tcpClientNetEventHandler.rov_control.m_left_H_motor  =pmotor->motorspeed;
		}
		break;
	case 0x59:
		{
			m_tcpClientNetEventHandler.rov_control.m_right_H_motor  =pmotor->motorspeed;
		}
		break;
	default :
		break;
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�ƽ���%dת�����ã����� = %d"),pmotor->motorindex, pmotor->motorspeed);

	return true;
}


bool CROVSharkMax::External_Power_Set( UUV_PARAM param )
{
	PUUVEXTERNALPOWER_T power_pattern= static_cast<PUUVEXTERNALPOWER_T>(param);
	if (!power_pattern)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(0==power_pattern->POEpoweron)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power &=~(1<<1);
	}
	else if (1==power_pattern->POEpoweron)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power |=(1<<1);
	}

	if(0==power_pattern->CANpoweron)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power &=~(1<<2);
	}
	else if (1==power_pattern->CANpoweron)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power |=(1<<2);
	}

	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����״̬���ã����� = %d"),m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power);
	return true;
}

//����ģʽ
bool CROVSharkMax::Power_ConsumMode_Set( UUV_PARAM param )
{
	unsigned char *power_pattern= static_cast<unsigned char*>(param);
	if (!power_pattern||((0!=*power_pattern)&&(1!=*power_pattern)))
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(0==*power_pattern)//�߹���
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set&=~(1<<6);
	}
	else if(1==*power_pattern)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set |=(1<<6);

	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ģʽ���ã����� = %d"),*power_pattern);
	return true;
}
bool CROVSharkMax::Work_Pattern_Set( UUV_PARAM param )
{
	unsigned char*work_pattern= static_cast<unsigned char*>(param);
	if (!work_pattern)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .ATT.operat_mode =*work_pattern;
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ģʽ���ã����� = %d"),*work_pattern);
	return true;
}

bool CROVSharkMax::System_Power_Set( UUV_PARAM param )
{
	unsigned char*work_pattern= static_cast<unsigned char*>(param);
	if (!work_pattern)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(1==*work_pattern)
	{
		//m_clientNetEventHandler.rov_control .CMD_RCtoROV .power|=(1<<15);
		m_tcpClientNetEventHandler.rov_control.Set_Reset_Flag(true);
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("ϵͳ���������ã����� = %d"),m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .power);
	return true;
}


bool CROVSharkMax::Camera_Osd_Set( UUV_PARAM param )
{
	PUUVOSD_T osd_pattern= static_cast<PUUVOSD_T>(param);
	if (!osd_pattern)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}
	if(0==osd_pattern->osd_on)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set &=~(1<<0);
	}
	else if (1==osd_pattern->osd_on)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set |=(1<<0);
	}

	if(0==osd_pattern->battery_on)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set &=~(1<<1);
	}
	else if (1==osd_pattern->battery_on)
	{
		m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV .OSD_Set |=(1<<1);
	}
	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("OSD���ã����� = %d"),m_tcpClientNetEventHandler.rov_control .CMD_RCtoROV.OSD_Set);
	return true;
}


bool CROVSharkMax::Net_Open( UUV_PARAM param )
{
	PUUVNETOPEN pNetOpen = static_cast<PUUVNETOPEN>(param);
	if (pNetOpen == nullptr)
	{
		m_nErrno = ERR_INVALIDPARAM;
		return false;
	}

#if !defined(DEBUG) || !defined(_DEBUG)
	if (!m_spTcpClient->Connect(CStringUtil::TStrToAnsi(pNetOpen->strIP).c_str(), pNetOpen->nPort))
	{
		m_nErrno = ERR_TIMEOUT;
		return false;
	}

	m_spTcpClient->SetClientListener(&m_tcpClientNetEventHandler);

	m_hWnd = pNetOpen->hWnd;
	m_unCount = 0;
	if (!this->VideoStream_Open())
	{
		m_spTcpClient->Disconnect();
		m_spTcpClient->SetClientListener(nullptr);
		m_nErrno = ERR_INNEREXCEPTION;
		return false;
	}
#endif

	if (!m_spUdpClient->Connect(DEFAULT_ADDR, DEFAULT_PORT))
	{
		m_nErrno = ERR_TIMEOUT;
		return false;
	}
	m_spUdpClient->SetClientListener(&m_udpClientNetEventHandler);

	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����������"));
	return true;
}

bool CROVSharkMax::Net_Close( void )
{
	this->VideoStream_Close();

	if (m_spTcpClient)
	{
		CPacketizedTcpClient* spClient = dynamic_cast<CPacketizedTcpClient*>(m_spTcpClient.get());
		if (spClient == nullptr)
		{
			m_nErrno = ERR_INNEREXCEPTION;
			return false;
		}

		FastMutex::ScopedLock lock(spClient->m_mciMutex);

		m_spTcpClient->Disconnect();
		m_spTcpClient->SetClientListener(nullptr);

		spClient->m_bManualDisconn = true;
	}

	if (m_spUdpClient)
	{
		m_spUdpClient->Disconnect();
		m_spUdpClient->SetClientListener(nullptr);
	}

	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�����ѶϿ�"));
	return true;
}

bool CROVSharkMax::Net_State( UUV_RESULT& pResult )
{
	if (pResult == nullptr)
	{
		m_nErrno = ERR_INVALIDPARAM;
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("��ȡ����״̬ʧ�ܣ�������%d"), m_nErrno);
		return false;
	}

	static E_NET_STATE netState;
	netState = m_spTcpClient->State();
	pResult = (UUV_RESULT)&netState;

	return true;
}

bool CROVSharkMax::VideoStream_Open( )
{
	++m_unCount;
	m_taskManager.cancelAll();
	m_taskManager.start(new CParseTask);

	if (::IsWindow(m_hWnd))
	{
		::ShowWindow(m_hWnd, SW_NORMAL);
	}

	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ͷ�Ѵ�"));
	return true;
}

bool CROVSharkMax::VideoStream_Close()
{
	m_unCount = 0;
	m_taskManager.cancelAll();

	if (::IsWindow(m_hWnd))
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}

	LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("����ͷ�ѹر�"));
	return true;
}
