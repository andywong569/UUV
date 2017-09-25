#pragma once

#include "MoveControlPage.h"
#include "ImageControlPage.h"
#include "PowerControlPage.h"
#include "MotoControlPage.h"
#include "SysControlPage.h"
#include "DataDisplayPage.h"
#include "ExtDataDisplayPage.h"

#define WM_LOGOUTPUT		(WM_USER+1001)

struct LOG_ITEM_DATA
{
	WT_EVENTLOG_TYPE emType;
	SYSTEMTIME stTime;
	tstring strMsg;
};

class CMainWndDlg : public WindowImplBase
{
public:
	CMainWndDlg(void);
	~CMainWndDlg(void);

public:
	LPCTSTR		GetWindowClassName() const;
	CDuiString	GetSkinFile();
	CDuiString	GetSkinFolder();
	void		OnFinalMessage(HWND hWnd);
	LRESULT		OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void		InitWindow();
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	LRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void		Notify(TNotifyUI& msg);

public:
	static void CALLBACK LogMsgShow(WT_EVENTLOG_TYPE emType, SYSTEMTIME stTime, LPCTSTR lpszMsg);

protected:
	LRESULT OnLogOutput(WPARAM wParam, LPARAM lParam);

public:
	static void CALLBACK UUVResultShow(UUV_RESULT pResult);
	static void CALLBACK UUVUSBLDataCallBack(UUV_RESULT pResult);
	static void CALLBACK UUVVideoFrame(UUV_RESULT pResult);

	static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


protected:
	DUI_DECLARE_MESSAGE_MAP()
	void OnWindowInit(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);

private:
	void InitConfig();
	void InitMediaWnd();
	void InitUUV();

private:
	/*	������	*/
	void ClickNetOpenBtn();
	void ClickNetCloseBtn();
	void ClickNetStateBtn();

	/*	��Ƶ������	*/
	void ClickVideoSnopBtn();
	void ClickVideoStreamFDOpenBtn();
	void ClickVideoStreamFDColseBtn();

private:
	void ProcessVideoFrame(UUV_RESULT pResult);

private:
	CWndUI*			m_pMediaWnd;
	IUUVObject*		m_pUUV;
	bool			m_bOpenFD;
	bool			m_bOpenSnop;

private:
	CMoveControlPage	m_moveControlPage;
	CImageControlPage	m_iamgeControlPage;
	CPowerControlPage	m_powerControlPage;
	CMotoControlPage	m_motoControlPage;
	CSysControlPage		m_sysControlPage;
	CDataDisplayPage	m_dataDisplayPage;
	CExtDataDisplayPage m_extDataDisplayPage;

private:
	/*	������	*/
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	/*	���������	*/
	CButtonUI*	m_pNetOpenBtn;
	CButtonUI*	m_pNetCloseBtn;
	CButtonUI*	m_pNetStateBtn;

	/*	��������	*/
	CControlUI* m_pMediaCtrl;
	
	/*	��Ƶ��	*/
	CButtonUI*	m_pVideoSnopBtn;
	CButtonUI*	m_pVideoOpenFaceDetectBtn;
	CButtonUI*	m_pVideoCloseFaceDetectBtn;

	/*	��־��	*/
	CRichEditUI*	m_pLogRichEdit;

	/*	״̬��	*/
	CLabelUI*		m_pNetStateLabel;

private:
	UINT_PTR		m_timerNetState;
};

extern CMainWndDlg* g_pMainWndDlg;

