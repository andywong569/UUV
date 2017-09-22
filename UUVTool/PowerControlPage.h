#pragma once

class CPowerControlPage : public CNotifyPump
{
public:
	CPowerControlPage();
	~CPowerControlPage();

public:
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void InitWindow();
	void InitUUV(IUUVObject* pUUV);

protected:
	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);

private:
	void ClickPowerOneBtn();			//	���ع������һ
	void ClickPowerTwoBtn();			//	���ع�����ƶ�
	void ClickPowerConsumModeBtn();		//	��/���Ƕ�

private:
	IUUVObject*			m_pUUV;

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pPowerControlPanel;

	/*	���ع������һ	*/
	COptionUI*				m_pCameraPowerOption;
	COptionUI*				m_pPOEPowerOption;
	CButtonUI*				m_pPowerOneBtn;

	/*	���ع�����ƶ�	*/
	COptionUI*				m_pPeripPowerOption;
	COptionUI*				m_pOtherPowerOption;
	CButtonUI*				m_pPowerTwoBtn;

	/*	����ģʽ	*/
	COptionUI*				m_pLowConsumOption;
	COptionUI*				m_pHighConsumOption;
	CButtonUI*				m_pPowerConsumModeBtn;
};

