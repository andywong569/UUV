#pragma once

class CMoveControlPage : public CNotifyPump
{
public:
	CMoveControlPage();
	~CMoveControlPage();

public:
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void InitWindow();
	void InitUUV(IUUVObject* pUUV);

protected:
	DUI_DECLARE_MESSAGE_MAP()
	void OnClick(TNotifyUI& msg);
	void OnValueChanged(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);
	void OnTextChanged(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);

private:
	void ClickSpeedBtn();		//	ǰ��/����
	void ClickSpeedRestBtn();	//	ǰ��/���˸�λ
	void ClickDeepBtn();		//	���
	void ClickDeepRestBtn();	//	��ȸ�λ
	void ClickPitchBtn();		//	��/���Ƕ�
	void ClickPitchRestBtn();	//	��/���Ƕȸ�λ
	void ClickRollBtn();		//	����Ƕ�
	void ClickRollRestBtn();	//	����Ƕȸ�λ
	void ClickHeadBtn();		//	����Ƕ�	
	void ClickHeadRestBtn();	//	����Ƕȸ�λ
	void ClickCtrlModeBtn();	//	����ģʽ
	void ClickNaviModeBtn();	//	����ģʽ
	void ClickMoveModeBtn();	//	�˶�ģʽ

private:
	IUUVObject*			m_pUUV;

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pMoveControlPanel;

	/*	ǰ��/����	*/
	CSliderUI*				m_pSpeedSlider;
	CButtonUI*				m_pSpeedBtn;
	CButtonUI*				m_pSpeedRestBtn;
	CEditUI*				m_pSpeedEdit;

	/*	���	*/
	CSliderUI*				m_pDeepSlider;
	CButtonUI*				m_pDeepBtn;
	CButtonUI*				m_pDeepRestBtn;
	CEditUI*				m_pDeepEdit;

	/*	��/���Ƕ�	*/
	CSliderUI*				m_pPitchAngleSlider;
	CButtonUI*				m_pPitchAngleBtn;
	CButtonUI*				m_pPitchAngleRestBtn;
	CEditUI*				m_pPitchAngleEdit;

	/*	����Ƕ�	*/
	CSliderUI*				m_pRollAngleSlider;
	CButtonUI*				m_pRollAngleBtn;
	CButtonUI*				m_pRollAngleRestBtn;
	CEditUI*				m_pRollAngleEdit;

	/*	����Ƕ�	*/
	CSliderUI*				m_pHeadAngleSlider;
	CButtonUI*				m_pHeadAngleBtn;
	CButtonUI*				m_pHeadAngleRestBtn;
	CEditUI*				m_pHeadAngleEdit;

	/*	����ģʽ	*/
	COptionUI*				m_pDiffControlOption;
	COptionUI*				m_pGesControlOption;
	CButtonUI*				m_pControlModeBtn;

	/*	����ģʽ	*/
	COptionUI*				m_pIneNaviOption;
	COptionUI*				m_pGeoNaviOption;
	CButtonUI*				m_pNaviModeBtn;

	/*	�˶�ģʽ	*/
	COptionUI*				m_pMoveModeStopOption;
	COptionUI*				m_pMoveModeDeepOption;
	COptionUI*				m_pMoveModeGestOption;
	CButtonUI*				m_pMoveModeBtn;
};

