#include "stdafx.h"
#include "Resource.h"
#include "MainWnd.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include "core/core.hpp"
#include "objdetect/objdetect.hpp"

CMainWndDlg* g_pMainWndDlg = nullptr;

//////////////////////////////////////////////////////////////////////////
///
DUI_BEGIN_MESSAGE_MAP(CMainWndDlg, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWINIT, OnWindowInit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CMainWndDlg::CMainWndDlg(void)
{
	g_pMainWndDlg = this;

	m_pCloseBtn = nullptr;
	m_pMaxBtn = nullptr;
	m_pRestoreBtn = nullptr;
	m_pMinBtn = nullptr;

	m_pNetOpenBtn = nullptr;
	m_pNetCloseBtn = nullptr;
	m_pNetStateBtn = nullptr;

	m_pMediaCtrl = nullptr;

	m_pVideoSnopBtn = nullptr;
	m_pVideoOpenFaceDetectBtn = nullptr;
	m_pVideoCloseFaceDetectBtn = nullptr;

	m_pLogRichEdit = nullptr;

	m_pNetStateLabel = nullptr;

	m_timerNetState = 0;

	//////////////////////////////////////////////////////////////////////////

	m_pMediaWnd = nullptr;
	m_pUUV = nullptr;
	m_bOpenFD = false;
	m_bOpenSnop = false;

	m_moveControlPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("moveControlPage"), &m_moveControlPage);

	m_iamgeControlPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("imageControlPage"), &m_iamgeControlPage);
	m_powerControlPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("powerControlPage"), &m_powerControlPage);

	m_motoControlPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("motoControlPage"), &m_motoControlPage);

	m_sysControlPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("sysControlPage"), &m_sysControlPage);

	m_dataDisplayPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("dataDisplayPage"), &m_dataDisplayPage);

	m_extDataDisplayPage.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("extDataDisplayPage"), &m_extDataDisplayPage);
}

CMainWndDlg::~CMainWndDlg(void)
{
	RemoveVirtualWnd(_T("moveControlPage"));
	RemoveVirtualWnd(_T("imageControlPage"));
	RemoveVirtualWnd(_T("powerControlPage"));
	RemoveVirtualWnd(_T("motoControlPage"));
	RemoveVirtualWnd(_T("sysControlPage"));
	RemoveVirtualWnd(_T("dataDisplayPage"));
	RemoveVirtualWnd(_T("extDataDisplayPage"));

	ReleaseUUV(m_pUUV);

	if (m_timerNetState != 0)
	{
		::KillTimer(this->GetHWND(), m_timerNetState);
	}
}

LPCTSTR CMainWndDlg::GetWindowClassName() const
{
	return _T("UUVTool");
}

CDuiString CMainWndDlg::GetSkinFile()
{
	return _T("main_wnd.xml");
}

CDuiString CMainWndDlg::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath() + _T("Skin\\UUVTool"));
}

void CMainWndDlg::OnFinalMessage(HWND hWnd)
{
	if (m_pMediaWnd)
	{
		DestroyWindow(m_pMediaWnd->GetHWND());
		m_pMediaWnd = nullptr;
	}

	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CMainWndDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ����˵����ر���Ϣ
	if (wParam == SC_CLOSE)
	{
		Close();
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}

	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

void CMainWndDlg::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closeBtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxBtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restoreBtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minBtn")));

	m_pNetOpenBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("netOpenBtn")));
	m_pNetCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("netCloseBtn")));
	m_pNetStateBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("netStateBtn")));

	m_pMediaCtrl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("mediaCtrl")));

	m_pVideoSnopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("videoSnopBtn")));
	m_pVideoOpenFaceDetectBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("videoOpenFaceDetectBtn")));
	m_pVideoCloseFaceDetectBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("videoCloseFaceDetectBtn")));

	m_pLogRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("logRichEdit")));

	m_pNetStateLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("netStateLabel")));

	m_moveControlPage.InitWindow();
	m_iamgeControlPage.InitWindow();
	m_powerControlPage.InitWindow();
	m_motoControlPage.InitWindow();
	m_sysControlPage.InitWindow();
	m_dataDisplayPage.InitWindow();
	m_extDataDisplayPage.InitWindow();
	WindowImplBase::InitWindow();
}

CControlUI* CMainWndDlg::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcsicmp(pstrClassName, _T("WndMediaDisplay")) == 0)
	{
		CWndUI *pUI = new CWndUI;
		HWND   hWnd = CreateWindow(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
		pUI->Attach(hWnd);
		return pUI;
	}

	return nullptr;
}

LRESULT CMainWndDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE; // ��Ϣ���ش���������ϵͳ����
	switch (uMsg)
	{
	case WM_SIZE:
		switch (wParam)
		{
		case SIZE_RESTORED: // ���񵽴��ڻ�ԭ��Ϣ
			{
				if (m_PaintManager.GetRoot() != NULL)
				{
					if (m_pMaxBtn != NULL) m_pMaxBtn->SetVisible(true);
					if (m_pRestoreBtn != NULL) m_pRestoreBtn->SetVisible(false);
				}
			}
			break;
		case SIZE_MAXIMIZED: // ���񵽴��������Ϣ
			{
				if (m_PaintManager.GetRoot() != NULL)
				{
					if (m_pMaxBtn != NULL) m_pMaxBtn->SetVisible(false);
					if (m_pRestoreBtn != NULL) m_pRestoreBtn->SetVisible(true);
				}
			}
			break;
		default:
			{
				
			}
			break;
		}
		bHandled = FALSE;
		break;
	case WM_PAINT:
		{
			if (m_pMediaWnd != nullptr && m_pMediaCtrl)
			{
				m_pMediaWnd->SetPos(m_pMediaCtrl->GetPos());
			}
		}
		bHandled = FALSE;
		break;
	case WM_LOGOUTPUT:
		{
			this->OnLogOutput(wParam, lParam);
		}
		break;
	default:
		bHandled = FALSE; // ����ϵͳ����
	}

	return 0;
}

void CMainWndDlg::Notify(TNotifyUI& msg)
{
	return WindowImplBase::Notify(msg);
}

void CMainWndDlg::LogMsgShow( WT_EVENTLOG_TYPE emType, SYSTEMTIME stTime, LPCTSTR lpszMsg )
{
	if (g_pMainWndDlg != NULL)
	{
		LOG_ITEM_DATA* pLogItemData = new LOG_ITEM_DATA();
		pLogItemData->emType = emType;
		pLogItemData->stTime = stTime;
		pLogItemData->strMsg = lpszMsg;

		if (!g_pMainWndDlg->PostMessage(WM_LOGOUTPUT, (WPARAM)pLogItemData, NULL)) 
		{ 
			// ������Ϣ�������ᵼ�·���ʧ��
			delete pLogItemData;
			pLogItemData = NULL;
		}
	}
}

LRESULT CMainWndDlg::OnLogOutput( WPARAM wParam, LPARAM lParam )
{
	LOG_ITEM_DATA* pLogItemData = (LOG_ITEM_DATA*)wParam;
	if (pLogItemData != NULL)
	{
		tstring strEventType;
		switch (pLogItemData->emType)
		{
		case WT_EVENTLOG_SUCCESS_TYPE:
			strEventType = _T("�ɹ�");
			break;
		case WT_EVENTLOG_ERROR_TYPE:
			strEventType = _T("����");
			break;
		case WT_EVENTLOG_WARNING_TYPE:
			strEventType = _T("����");
			break;
		case WT_EVENTLOG_INFORMATION_TYPE:
			strEventType = _T("��Ϣ");
			break;
		default:
			break;
		}

		tstring strMsgHeader, strMsgContent;

		strMsgHeader = CStringUtil::Format(_T("%04d-%02d-%02d %02d:%02d:%02d [%s]\r\n"), pLogItemData->stTime.wYear, 
			pLogItemData->stTime.wMonth, pLogItemData->stTime.wDay, pLogItemData->stTime.wHour, 
			pLogItemData->stTime.wMinute, pLogItemData->stTime.wSecond, strEventType.c_str());

		CHARFORMAT2 cfHeader;
		ZeroMemory(&cfHeader, sizeof(CHARFORMAT2));
		cfHeader.cbSize = sizeof(CHARFORMAT2);
		cfHeader.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		_tccpy(cfHeader.szFaceName, _T("����")); // ��������
		cfHeader.dwEffects = 0;
		cfHeader.yHeight = 210; // ���ָ߶�
		cfHeader.crTextColor = RGB(0, 110, 254); // ������ɫ

		CHARFORMAT2 cfContent;
		ZeroMemory(&cfContent, sizeof(CHARFORMAT2));
		cfContent.cbSize = sizeof(CHARFORMAT2);
		cfContent.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		_tccpy(cfContent.szFaceName, _T("����")); // ��������
		cfContent.dwEffects = 0;
		cfContent.yHeight = 180; // ���ָ߶�
		cfContent.crTextColor = RGB(0, 0, 0); // ������ɫ

		PARAFORMAT2 pfHeader;
		ZeroMemory(&pfHeader, sizeof(PARAFORMAT2));
		pfHeader.cbSize = sizeof(PARAFORMAT2);
		pfHeader.dwMask =  PFM_STARTINDENT | PFM_OFFSET;
		pfHeader.dxStartIndent = 0;
		pfHeader.dxOffset = 0;

		PARAFORMAT2 pfContent;
		ZeroMemory(&pfContent, sizeof(PARAFORMAT2));
		pfContent.cbSize = sizeof(PARAFORMAT2);
		pfContent.dwMask =  PFM_STARTINDENT | PFM_OFFSET;
		pfContent.dxStartIndent = 200;
		pfContent.dxOffset = 0;

		if (pLogItemData->strMsg.length() > 0 && pLogItemData->strMsg[pLogItemData->strMsg.length()-1]  == _T('\n'))
		{
			strMsgContent = CStringUtil::Format(_T("%s"), pLogItemData->strMsg.c_str());
		}
		else
		{
			strMsgContent = CStringUtil::Format(_T("%s\r\n"), pLogItemData->strMsg.c_str());
		}


		m_pLogRichEdit->SetSel(-1, -1);
		m_pLogRichEdit->SetSelectionCharFormat(cfHeader);
		m_pLogRichEdit->SetParaFormat(pfHeader);
		m_pLogRichEdit->ReplaceSel(strMsgHeader.c_str(), false);

		switch (pLogItemData->emType)
		{
		case WT_EVENTLOG_SUCCESS_TYPE:
			cfContent.crTextColor = RGB(60, 180, 117); // ��ɫ
			break;
		case WT_EVENTLOG_ERROR_TYPE:
			cfContent.crTextColor = RGB(254, 0, 2); // ��ɫ
			break;
		case WT_EVENTLOG_WARNING_TYPE:
			cfContent.crTextColor = RGB(218, 69, 39); // ��ɫ
			break;
		case WT_EVENTLOG_INFORMATION_TYPE:
			break;
		default:
			break;
		}
		m_pLogRichEdit->SetSel(-1, -1);
		m_pLogRichEdit->SetSelectionCharFormat(cfContent);
		m_pLogRichEdit->SetParaFormat(pfContent);
		m_pLogRichEdit->ReplaceSel(strMsgContent.c_str(), false);

		/*if (m_pLogAutoClearOption->IsSelected())
		{
			if (m_pLogRichEdit->GetLineCount() >= 3000)
			{
				int nSelEnd = m_pLogRichEdit->LineIndex(1000); 
				m_pLogRichEdit->SetSel(0, nSelEnd);
				m_pLogRichEdit->ReplaceSel(_T(""), false);
			}
		}*/

		//if (m_pLogAutoScrollOption->IsSelected())
		{
			m_pLogRichEdit->EndDown();
		}

		delete pLogItemData;
	}

	return 0;
}

void CALLBACK CMainWndDlg::UUVResultShow( UUV_RESULT pResult )
{
	g_pMainWndDlg->m_dataDisplayPage.ShowData(pResult);
}

void CALLBACK CMainWndDlg::UUVUSBLDataCallBack( UUV_RESULT pResult )
{
	g_pMainWndDlg->m_extDataDisplayPage.ShowData(pResult);
}

void CALLBACK CMainWndDlg::UUVVideoFrame( UUV_RESULT pResult )
{
	g_pMainWndDlg->ProcessVideoFrame(pResult);
}

void CMainWndDlg::OnWindowInit( TNotifyUI& msg )
{
	g_logCallBack = CMainWndDlg::LogMsgShow;

	this->InitConfig();

	//this->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	::ShowWindow(m_hWnd, SW_MAXIMIZE);

	m_timerNetState = ::SetTimer(this->GetHWND(), 1, 1000, TimerProc);
}

void CMainWndDlg::OnClick( TNotifyUI& msg )
{
	if (msg.pSender == m_pCloseBtn)
	{
		Close();
		::PostQuitMessage(0L);
		return;
	}
	else if (msg.pSender == m_pMinBtn)
	{
		this->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else if (msg.pSender == m_pMaxBtn)
	{
		this->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else if (msg.pSender == m_pRestoreBtn)
	{
		this->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	else if (msg.pSender == m_pNetOpenBtn)
	{
		this->ClickNetOpenBtn();
	}
	else if (msg.pSender == m_pNetCloseBtn)
	{
		this->ClickNetCloseBtn();
	}
	else if (msg.pSender == m_pNetStateBtn)
	{
		this->ClickNetStateBtn();
	}
	else if (msg.pSender == m_pVideoSnopBtn)
	{
		this->ClickVideoSnopBtn();
	}
	else if (msg.pSender == m_pVideoOpenFaceDetectBtn)
	{
		this->ClickVideoStreamFDOpenBtn();
	}
	else if (msg.pSender == m_pVideoCloseFaceDetectBtn)
	{
		this->ClickVideoStreamFDColseBtn();
	}
}

void CMainWndDlg::InitConfig()
{
	this->InitMediaWnd();
	this->InitUUV();
}

void CMainWndDlg::ClickNetOpenBtn()
{
	int nErrno = ERR_SUCCESS;

	if (m_pUUV)
	{
		UUVNETOPEN netOpen;
	#if defined(DEBUG) || defined(_DEBUG)
		netOpen.strIP = _T("192.168.99.10");
	#else
		netOpen.strIP = _T("192.168.99.10");
	#endif
		netOpen.nPort = 5001;
		netOpen.hWnd = m_pMediaWnd->GetHWND();
		if(!m_pUUV->UUV_Set(UUV_NET_OPEN, &netOpen))
		{
			nErrno = m_pUUV->UUV_GetErrno();
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("������ʧ�ܣ������� %d"), nErrno);
		}
	}
	else
	{
		nErrno = m_pUUV->UUV_GetErrno();
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����UUVʵ��ʧ�ܣ������� %d"), nErrno);
	}
}

void CMainWndDlg::ClickNetCloseBtn()
{
	int nErrno = ERR_SUCCESS;

	if (m_pUUV)
	{
		if(!m_pUUV->UUV_Set(UUV_NET_CLOSE, nullptr))
		{
			nErrno = m_pUUV->UUV_GetErrno();
			LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("�ر�����ʧ�ܣ������� %d"), nErrno);
		}
	}
}


void CMainWndDlg::InitMediaWnd()
{
	m_pMediaWnd = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("mediaCtrl")));
	ASSERT(m_pMediaWnd != nullptr);

	m_pMediaWnd->SetPos(m_pMediaCtrl->GetPos());
}

void CMainWndDlg::InitUUV()
{
	m_pUUV = CreateUUV(E_ROV_SHARK_MAX);
	if (m_pUUV == nullptr)
	{
		LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����UUVʵ��ʧ��"));
	}
	else
	{
		if (!m_pUUV->UUV_RegHandler(CMainWndDlg::UUVResultShow))
		{
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("ע��UUV�������ݻص�ʧ�ܣ�������%d"), m_pUUV->UUV_GetErrno());
		}
		if (!m_pUUV->UUV_RegUSBLHandler(CMainWndDlg::UUVUSBLDataCallBack))
		{
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("ע��UUV USBL���ݻص�ʧ�ܣ�������%d"), m_pUUV->UUV_GetErrno());
		}
		if (!m_pUUV->UUV_RegVideoHandler(CMainWndDlg::UUVVideoFrame))
		{
			LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("ע��UUV��Ƶ���ݻص�ʧ�ܣ�������%d"), m_pUUV->UUV_GetErrno());
		}
		m_moveControlPage.InitUUV(m_pUUV);
		m_iamgeControlPage.InitUUV(m_pUUV);
		m_powerControlPage.InitUUV(m_pUUV);
		m_motoControlPage.InitUUV(m_pUUV);
		m_sysControlPage.InitUUV(m_pUUV);
	}
}

void CMainWndDlg::ClickNetStateBtn()
{
	int nErrno = ERR_SUCCESS;

	if (m_pUUV)
	{
		UUV_RESULT pResult = new UUV_RESULT;
		if(!m_pUUV->UUV_Get(UUV_NET_STATE, pResult))
		{
			nErrno = m_pUUV->UUV_GetErrno();
			LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("��ȡ��������״̬ʧ�ܣ������� %d"), nErrno);
		}
		else
		{
			E_NET_STATE* netState = (E_NET_STATE*)(pResult);
			if (!netState)
			{
				LogMsg(WT_EVENTLOG_ERROR_TYPE, _T("����ת��ʧ��"));
			}
			else
			{
				tstring strState = _T("����״̬δ֪");
				switch (*netState)
				{
				case E_NET_CONNING:
					strState = _T("�ͻ�������:��������");
					break;
				case E_NET_CONNED:
					strState = _T("�ͻ�������:������");
					break;
				case E_NET_DISCONNING:
					strState = _T("�ͻ�������:���ڶϿ�");
					break;
				case E_NET_DISCONNED:
					strState = _T("�ͻ�������:�ѶϿ�");
					break;
				default:
					break;
				}
				m_pNetStateLabel->SetText(strState.c_str());
			}
		}

		if (!pResult)
		{
			delete pResult;
			pResult = nullptr;
		}
	}
}

void CMainWndDlg::ClickVideoStreamFDOpenBtn()
{
	m_bOpenFD = true;
}

void CMainWndDlg::ClickVideoStreamFDColseBtn()
{
	m_bOpenFD = false;
}

void DetectAndDraw( cv::Mat& img, cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade, double scale, bool tryflip )
{
	using namespace cv;

	//����һЩ��ɫ��������ʾ��ͬ������
	const static Scalar colors[] =  {
		CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255)} ;
	
	//������С��ͼƬ���ӿ����ٶ�
	//nt cvRound (double value) ��һ��double�͵��������������룬������һ����������
	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
	
	//ת�ɻҶ�ͼ��Harr�������ڻҶ�ͼ
	cvtColor( img, gray, CV_BGR2GRAY );
	//�ı�ͼ���С��ʹ��˫���Բ�ֵ
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
	//�任���ͼ�����ֱ��ͼ��ֵ������
	equalizeHist( smallImg, smallImg );
	//����ʼ�ͽ�������˺�����ȡʱ�䣬������������㷨ִ��ʱ��
	double t = (double)cvGetTickCount();

	//�������ڴ����������������
	vector<cv::Rect> faces, faces2;
	
	//�������
	//detectMultiScale������smallImg��ʾ����Ҫ��������ͼ��ΪsmallImg
	//faces��ʾ��⵽������Ŀ������
	//1.1��ʾÿ��ͼ��ߴ��С�ı���Ϊ1.1
	//2��ʾÿһ��Ŀ������Ҫ����⵽3�β��������Ŀ��(��Ϊ��Χ�����غͲ�ͬ�Ĵ��ڴ�С�����Լ�⵽����)
	//CV_HAAR_SCALE_IMAGE��ʾ�������ŷ���������⣬��������ͼ��
	//Size(30, 30)ΪĿ�����С���ߴ�
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		|CV_HAAR_SCALE_IMAGE
		,Size(30, 30));
	//���ʹ�ܣ���תͼ��������
	if( tryflip )
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale( smallImg, faces2,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|CV_HAAR_SCALE_IMAGE
			,Size(30, 30) );
		for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)cvGetTickCount() - t;
	LogMsg( WT_EVENTLOG_INFORMATION_TYPE, _T("detection time = %g ms"), t/((double)cvGetTickFrequency()*1000.) );
	int i = 0;
	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); ++r, ++i )
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i%8];
		int radius;

		double aspect_ratio = (double)r->width/r->height;
		if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
		{
			//��ʾ����ʱ����С֮ǰ��ͼ���ϱ�ʾ����������������ű��������ȥ
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
		else
		{
			rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
			cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
			color, 3, 8, 0);
		}
		if( nestedCascade.empty() )
			continue;
		smallImgROI = smallImg(*r);
		//ͬ�������������
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|CV_HAAR_SCALE_IMAGE
			,Size(30, 30) );
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); ++nr )
		{
			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle( img, center, radius, color, 2, 8, 0 );
		}
	}

	if (!faces.empty())
	{
		//Mat rgbImg;
		//cvtColor(img, rgbImg, CV_YUV2BGR_I420);
		imshow( "ʶ����", img );
		waitKey(10000);
		cvDestroyWindow("ʶ����");
	}
}

void SaveImage( cv::Mat& img )
{
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

	tstring strDirPath = CStringUtil::Format(_T("%simage\\"), CPath::GetModulePath().c_str());
	tstring strImageFilePath = CStringUtil::Format(_T("%s%04d%02d%02d%02d%02d%02d.png"), strDirPath.c_str(), 
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	CPath::CreateDirectoryW(strImageFilePath.c_str(), NULL);

	BOOL bFileCreated = !CPath::IsFileExist(strImageFilePath.c_str());

	if (bFileCreated)
	{
		using namespace cv;

		Mat rgbImg;
		cvtColor(img, rgbImg, CV_YUV2BGR_I420);
		imwrite(CStringUtil::TStrToAnsi(strImageFilePath), rgbImg);
		waitKey(0);

		LogMsg(WT_EVENTLOG_INFORMATION_TYPE, _T("ͼƬ������ %s"),  strImageFilePath.c_str());
	}
}

void CMainWndDlg::ProcessVideoFrame( UUV_RESULT pResult )
{
	AVFrame* pFrameYUV = static_cast<AVFrame*>(pResult);
	if (pFrameYUV == nullptr)
	{
		return;
	}

	if (g_pMainWndDlg->m_bOpenSnop)
	{
		g_pMainWndDlg->m_bOpenSnop = false;

		using namespace cv;

		Mat yuvImg;
		yuvImg.create(pFrameYUV->height*3/2, pFrameYUV->width, CV_8UC1);
		memcpy(yuvImg.data, pFrameYUV->data[0], (pFrameYUV->width)*(pFrameYUV->height));
		memcpy(yuvImg.data + (pFrameYUV->width)*(pFrameYUV->height), pFrameYUV->data[1], (pFrameYUV->width)*(pFrameYUV->height)/4);
		memcpy(yuvImg.data +(pFrameYUV->width)*(pFrameYUV->height) + (pFrameYUV->width)*(pFrameYUV->height)/4, pFrameYUV->data[2], (pFrameYUV->width)*(pFrameYUV->height)/4);

		SaveImage(yuvImg);
	}

	if (g_pMainWndDlg->m_bOpenFD)
	{
		g_pMainWndDlg->m_bOpenFD = false;

		using namespace cv;

		CascadeClassifier cascade, nestedCascade;
		cascade.load("data\\haarcascades\\haarcascade_frontalface_alt.xml");
		nestedCascade.load("data\\haarcascades\\haarcascade_eye.xml");

		Mat yuvImg;
		yuvImg.create(pFrameYUV->height*3/2, pFrameYUV->width, CV_8UC1);
		memcpy(yuvImg.data, pFrameYUV->data[0], (pFrameYUV->width)*(pFrameYUV->height));
		memcpy(yuvImg.data + (pFrameYUV->width)*(pFrameYUV->height), pFrameYUV->data[1], (pFrameYUV->width)*(pFrameYUV->height)/4);
		memcpy(yuvImg.data +(pFrameYUV->width)*(pFrameYUV->height) + (pFrameYUV->width)*(pFrameYUV->height)/4, pFrameYUV->data[2], (pFrameYUV->width)*(pFrameYUV->height)/4);

		Mat smallImg;
		resize(yuvImg, smallImg, Size(640, 480));
		Mat rgbImg;
		cvtColor(smallImg, rgbImg, CV_YUV2BGR_I420);

		DetectAndDraw(rgbImg, cascade, nestedCascade, 2, 0);
	}
}

void CALLBACK CMainWndDlg::TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	switch (idEvent)
	{
	case 1:	/*	����״̬	*/
		{
			g_pMainWndDlg->ClickNetStateBtn();
		}
		break;
	default:
		break;
	}
}

void CMainWndDlg::ClickVideoSnopBtn()
{
	m_bOpenSnop = true;
}
