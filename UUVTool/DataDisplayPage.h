#pragma once

class CDataDisplayPage : public CNotifyPump
{
public:
	CDataDisplayPage();
	~CDataDisplayPage();

public:
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void InitWindow();
	void ShowData(UUV_RESULT pResult);

protected:
	DUI_DECLARE_MESSAGE_MAP()

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pDataDisplayPanel;

	CEditUI*				m_pRollEdit;			//	�����
	CEditUI*				m_pPitchEdit;			//	������
	CEditUI*				m_pHeadEdit;			//	�����
	CEditUI*				m_pSpeedFrontBackEdit;	//	ǰ���ٶ�
	CEditUI*				m_pSpeedTraverseEdit;	//	�����ٶ�
	CEditUI*				m_pSpeedUpDownEdit;		//	�����ٶ�
	CEditUI*				m_pWorkModeEdit;		//	����ģʽ
	CEditUI*				m_pControlModeEdit;		//	����ģʽ
	CEditUI*				m_pNaviModeEdit;		//	����ģʽ
	CEditUI*				m_pDeepCurEdit;			//	��ǰ���
	CEditUI*				m_pDeepTarEdit;			//	Ŀ�����
	CEditUI*				m_pBoardDevPowerEdit;	//	�����豸����
	CEditUI*				m_pPowerConsumEdit;		//	�����豸����
	CEditUI*				m_pTempEdit;			//	�����¶�
	CEditUI*				m_pSaliEdit;			//	�ζ�
	CEditUI*				m_pPressEdit;			//	����ѹ��
	CEditUI*				m_pHumidityEdit;		//	����ʪ��
	CEditUI*				m_pCloudPitchEdit;		//	��̨����
	CEditUI*				m_pCloudRotateEdit;		//	��̨��ת
	CEditUI*				m_pCameraFocusEdit;		//	��ͷ�۽�
	CEditUI*				m_pCameraZoomEdit;		//	��ͷ�佹
	CEditUI*				m_pLigthLeftEdit;		//	������
	CEditUI*				m_pLightRightEdit;		//	������
	CEditUI*				m_pLaserEdit;			//	����
	CEditUI*				m_pVLeftFrontEdit;		//	0x50���ת��
	CEditUI*				m_pVLeftBackEdit;		//	0x51���ת��
	CEditUI*				m_pVRightBackEdit;		//	0x52���ת��
	CEditUI*				m_pVRightFrontEdit;		//	0x53���ת��
	CEditUI*				m_pHLeftEdit;			//	0x58���ת��
	CEditUI*				m_pHRightEdit;			//	0x59���ת��
	CEditUI*				m_pManipStateEdit;		//	��е��״̬
	CEditUI*				m_pIlluTensorEdit;		//	�նȴ�����
	CEditUI*				m_pInputVolEdit;		//	�����ѹ
	CEditUI*				m_pInputElecEdit;		//	�������
	CEditUI*				m_pBatteryVolEdit;		//	��ص�ѹ
	CEditUI*				m_pLoadElecEdit;		//	���ص���
	CEditUI*				m_pSocStateEdit;		//	SOC״̬
	CEditUI*				m_pSysPowerStateEdit;	//	ϵͳ��Դ״̬
	CEditUI*				m_pTotalPowerEdit;		//	�ܹ���
};

