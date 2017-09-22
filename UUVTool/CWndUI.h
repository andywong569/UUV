#pragma once

// ��HWND��ʾ��CControlUI����
class CWndUI : public CControlUI
{
public:
	CWndUI();

	virtual void SetVisible(bool bVisible = true);

	virtual void SetInternVisible(bool bVisible = true);

	virtual void SetPos(RECT rc);

	BOOL Attach(HWND hWndNew);

	HWND Detach();

	HWND GetHWND();

protected:
	HWND m_hWnd;

};

