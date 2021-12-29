#include "StdAfx.h"
#include "MainWnd.h"

CMainWnd::CMainWnd()
{
}
CMainWnd::~CMainWnd()
{
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	//if(_tcscmp(pstrClass, _T("ContainerEx")) == 0)
	//{
	//	return new CContainerExUI;
	//}

	return NULL;
}

void CMainWnd::InitDlg()
{
	//窗口置前
	SetForegroundWindow(m_hWnd);
	BringWindowToTop(m_hWnd);	
}

void CMainWnd::ChangeLanguage(const int& nLanguageID)
{
	wstring strFile = g_strAppPath + L"data\\";
	if (nLanguageID == 0)
		strFile += L"zh";
	else if (nLanguageID == 1)
		strFile += L"en";
	else if (nLanguageID == 2)
		strFile += L"jp";

	strFile += L".json";
	LoadMultiLanguageString(strFile);

	m_PM.GetRoot()->NeedUpdate();
}

void CMainWnd::Notify(TNotifyUI& msg) 
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		InitDlg();
	}
	else if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("close_btn"))
		{
			Close();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender->GetName() == L"languageCombo")
		{
			CComboUI* pCombo = (CComboUI*)(msg.pSender);
			ChangeLanguage(pCombo->GetCurSel());
		}
	}
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch (uMsg)
	{
	case WM_CREATE:						lRes = OnCreate(uMsg, wParam, lParam);							break;
	case WM_NCCALCSIZE:					break;
	case WM_NCHITTEST:					lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);				break;
	case WM_SIZE:						lRes = OnSize(uMsg, wParam, lParam, bHandled);					break;
	case WM_NCLBUTTONDBLCLK:			bHandled = TRUE;												break;
	default:
		{
			bHandled =FALSE;
		}
		
	}
	if( bHandled ) return lRes;
	if( m_PM.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;	//取消标题框
	styleValue &= ~WS_SIZEBOX;	//取消自动调整边框的风格,可以防止贴边自动最大化
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PM.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("main.xml"), (UINT)0, this, &m_PM);   
	if (pRoot)
	{
		m_PM.AttachDialog(pRoot);
		m_PM.AddNotifier(this); 
	}

	return 0;
}

LRESULT CMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PM.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PM.FindControl(pt));
			if( IsHTCaptionFlag(pControl) )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PM.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}