#pragma once

class CImageControlPage : public CNotifyPump
{
public:
	CImageControlPage();
	~CImageControlPage();

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

	void ClickCloudPitchBtn();		//	��̨����
	void ClickCloudPitchRestBtn();	//	��̨������λ
	void ClickCloudRotateBtn();		//	��̨��ת
	void ClickCloudRotateRestBtn();	//	��̨��ת��λ
	void ClickCameraFocusBtn();		//	��ͷ�۽�
	void ClickCameraFocusRestBtn();	//	��ͷ�۽���λ
	void ClickCameraZoomBtn();		//	��ͷ�佹
	void ClickCameraZoomRestBtn();	//	��ͷ�佹��λ
	void ClickLightLeftBtn();		//	������
	void ClickLightLeftRestBtn();	//	������λ
	void ClickLightRightBtn();		//	������
	void ClickLightRightRestBtn();	//	�����Ҹ�λ
	void ClickBatteryBtn();			//	�����ʾ
	void ClickRockerBtn();			//	ҡ����ʾ
	void ClickOsdBtn();				//	OSD��ʾ
	void ClickPeripBtn();			//	������ʾ

private:
	IUUVObject*				m_pUUV;

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pImageControlPanel;

	/*	��̨����	*/
	CSliderUI*				m_pCloudPitchSlider;
	CButtonUI*				m_pCloudPitchBtn;
	CButtonUI*				m_pCloudPitchRestBtn;
	CEditUI*				m_pCloudPitchEdit;

	/*	��̨��ת	*/
	CSliderUI*				m_pCloudRotateSlider;
	CButtonUI*				m_pCloudRotateBtn;
	CButtonUI*				m_pCloudRotateRestBtn;
	CEditUI*				m_pCloudRotateEdit;

	/*	��ͷ�۽�	*/
	CSliderUI*				m_pCameraFocusSlider;
	CButtonUI*				m_pCameraFocusBtn;
	CButtonUI*				m_pCameraFocusRestBtn;
	CEditUI*				m_pCameraFocusEdit;

	/*	��ͷ�佹	*/
	CSliderUI*				m_pCameraZoomSlider;
	CButtonUI*				m_pCameraZoomBtn;
	CButtonUI*				m_pCameraZoomRestBtn;
	CEditUI*				m_pCameraZoomEdit;

	/*	������	*/
	CSliderUI*				m_pLightLeftSlider;
	CButtonUI*				m_pLightLeftBtn;
	CButtonUI*				m_pLightLeftRestBtn;
	CEditUI*				m_pLightLeftEdit;

	/*	������	*/
	CSliderUI*				m_pLightRightSlider;
	CButtonUI*				m_pLightRightBtn;
	CButtonUI*				m_pLightRightRestBtn;
	CEditUI*				m_pLightRightEdit;

	/*	�����ʾ	*/
	COptionUI*				m_pOpenBatteryOption;
	COptionUI*				m_pCloseBatteryOption;
	CButtonUI*				m_pBatteryBtn;

	/*	ҡ����ʾ	*/
	COptionUI*				m_pOpenRockerOption;
	COptionUI*				m_pCloseRockerOption;
	CButtonUI*				m_pRockerBtn;

	/*	OSD��ʾ	*/
	COptionUI*				m_pOpenOSDOption;
	COptionUI*				m_pCloseOSDOption;
	CButtonUI*				m_pOosdBtn;

	/*	�����ʾ	*/
	COptionUI*				m_pOpenPeripOption;
	COptionUI*				m_pClosePeripOption;
	CButtonUI*				m_pPeripBtn;
};

