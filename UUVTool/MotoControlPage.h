#pragma once

class CMotoControlPage : public CNotifyPump
{
public:
	CMotoControlPage();
	~CMotoControlPage();

public:
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void InitWindow();
	void InitUUV(IUUVObject* pUUV);

protected:
	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnValueChanged(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);

private:
	void SetSliderTip(CSliderUI* pSlider);

	void ClickVLeftFrontBtn();		//	��ֱ��ǰ
	void ClickVLeftFrontRestBtn();	//	��ֱ��ǰ��λ
	void ClickVRightFrontBtn();		//	��ֱ��ǰ
	void ClickVRightFrontRestBtn();	//	��ֱ��ǰ��λ
	void ClickVLeftBackBtn();		//	��ֱ���
	void ClickVLeftBackRestBtn();	//	��ֱ���λ
	void ClickVRightBackBtn();		//	��ֱ�Һ�
	void ClickVRightBackRestBtn();	//	��ֱ�Һ�λ
	void ClickHLeftBtn();			//	ˮƽ��
	void ClickHLeftRestBtn();		//	ˮƽ��λ
	void ClickHRightBtn();			//	ˮƽ��
	void ClickHRightRestBtn();		//	ˮƽ�Ҹ�λ

	//added by houmb
	void Send_Motor_Speed(UUVMOTOR_T motor);
private:
	IUUVObject*				m_pUUV;

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pMotoControlPanel;

	/*	��ֱ��ǰ	*/
	CSliderUI*				m_pVLeftFrontSlider;
	CButtonUI*				m_pVLeftFrontBtn;
	CButtonUI*				m_pVLeftFrontRestBtn;
	CEditUI*				m_pVLeftFrontEdit;

	/*	��ֱ��ǰ	*/
	CSliderUI*				m_pVRightFrontSlider;
	CButtonUI*				m_pVRightFrontBtn;
	CButtonUI*				m_pVRightFrontRestBtn;
	CEditUI*				m_pVRightFrontEdit;

	/*	��ֱ���	*/
	CSliderUI*				m_pVLeftBackSlider;
	CButtonUI*				m_pVLeftBackBtn;
	CButtonUI*				m_pVLeftBackRestBtn;
	CEditUI*				m_pVLeftBackEdit;

	/*	��ֱ�Һ�	*/
	CSliderUI*				m_pVRightBackSlider;
	CButtonUI*				m_pVRightBackBtn;
	CButtonUI*				m_pVRightBackRestBtn;
	CEditUI*				m_pVRightBackEdit;

	/*	ˮƽ��	*/
	CSliderUI*				m_pHLeftSlider;
	CButtonUI*				m_pHLeftBtn;
	CButtonUI*				m_pHLeftRestBtn;
	CEditUI*				m_pHLeftEdit;

	/*	ˮƽ��	*/
	CSliderUI*				m_pHRightSlider;
	CButtonUI*				m_pHRightBtn;
	CButtonUI*				m_pHRightRestBtn;
	CEditUI*				m_pHRightEdit;
};

