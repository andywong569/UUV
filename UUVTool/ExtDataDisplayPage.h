#pragma once

class CExtDataDisplayPage :	public CNotifyPump
{
public:
	CExtDataDisplayPage(void);
	~CExtDataDisplayPage(void);

public:
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	void InitWindow();
	void ShowData(UUV_RESULT pResult);

protected:
	DUI_DECLARE_MESSAGE_MAP()

private:
	CPaintManagerUI*		m_pPaintManager;
	CHorizontalLayoutUI*	m_pExtDataDisplayPanel;

	CEditUI*				m_pRelativedDistEdit;			//	��Ի������
	CEditUI*				m_pRelativedDirectEdit;			//	��Ի���Ƕ�
	CEditUI*				m_pRelativedPosNorthEdit;		//	��Ի���ƫ��
	CEditUI*				m_pRelativedPosEastEdit;		//	��Ի���ƫ��
	CEditUI*				m_pLonEdit			;			//	����
	CEditUI*				m_pLatEdit;						//	γ��
};

