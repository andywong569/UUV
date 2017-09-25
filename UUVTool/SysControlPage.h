#pragma once

class CSysControlPage : public CNotifyPump
{
public:
	CSysControlPage();
	~CSysControlPage();

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
	void ClickSoftRestartBtn();			//	������
	void ClickManipControlBtn();		//	��е������
	void ClickManipStopBtn();			//	��е��ֹͣ
	void ClickUsblNetOpenBtn();			//	USBL������
	void ClickUsblNetCloseBtn();		//	USBL�ر�����

private:
	IUUVObject*				m_pUUV;

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pSysControlPanel;

	/*	��е�ֿ���	*/
	CSliderUI*				m_pManipOpenSlider;
	CButtonUI*				m_pManipulatorControlBtn;
	CButtonUI*				m_pManipulatorStopBtn;
	CEditUI*				m_pManipOpenEdit;

	/*	USBL����	*/
	CButtonUI*				m_pUsblNetOpenBtn;
	CButtonUI*				m_pUsblNetCloseBtn;

	/*	������	*/
	CButtonUI*				m_pSoftRestartBtn;
};

