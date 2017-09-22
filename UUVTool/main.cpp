// main.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MainWnd.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance); // ʵ���������Ⱦ�����
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\UUVTool")); // ָ����Դ·�� 

	HRESULT Hr = ::CoInitialize(NULL); // ��ʼ��COM��, Ϊ����COM���ṩ֧��
	if (FAILED(Hr)) return 0;


	CMainWndDlg* pMainDlg = new CMainWndDlg(); // ����������
	pMainDlg->Create(NULL, _T("UUVTool"), UI_WNDSTYLE_FRAME/* | WS_MAXIMIZE*/, WS_EX_WINDOWEDGE); // ע�ᴰ�����봴������ 
	pMainDlg->CenterWindow(); // ���ھ�����ʾ
	pMainDlg->ShowModal();

	CPaintManagerUI::MessageLoop();	// ������Ϣѭ��

	::CoUninitialize(); // �˳������ͷ�COM��

	return 0;
}