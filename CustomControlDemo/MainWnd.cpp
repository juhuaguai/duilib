#include "StdAfx.h"
#include "MainWnd.h"
#include "CustomLayout.h"

CMainWnd::CMainWnd()
{
}

CMainWnd::~CMainWnd()
{
}

CControlUI* CMainWnd::CreateControl(LPCTSTR pstrClass)
{
	if(_tcscmp(pstrClass, _T("CustomLayout")) == 0)
	{
		return new CCustomLayoutUI;
	}

	return NULL;
}

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;	//取消标题框
	styleValue &= ~WS_SIZEBOX;	//RWH 取消自动调整边框的风格,可以放置贴边自动最大化
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PM.Init(m_hWnd);
	CDialogBuilder builder; 
	CControlUI* pRoot = builder.Create(_T("TestJsC++.xml"), (UINT)0,  this, &m_PM);		//直接修改Res目录下面的MainWnd - 副本.xml为这个文件名,即可看到另外一个效果了
	ASSERT(pRoot && "Failed to parse XML");
	m_PM.AttachDialog(pRoot);
	m_PM.AddNotifier(this);

	return 0;
}

void JSCallTest(CWebBrowserUI* pBrowser, DISPPARAMS *pDispParams,/* [out] */ VARIANT *pVarResult,/* [out] */ EXCEPINFO *pExcepInfo,/* [out] */ UINT *puArgErr)
{
	CDuiString strMsg;
	strMsg.Format(L"%d,%d,%s",pDispParams->rgvarg[0].boolVal,pDispParams->rgvarg[1].intVal,pDispParams->rgvarg[2].bstrVal);
	MessageBox(NULL,strMsg,L"测试",MB_OK);
	if (pVarResult)
	{
		BSTR bstrRet = SysAllocString(L"c++传递的返回值");
		pVarResult->bstrVal = bstrRet;
		pVarResult->vt = VT_BSTR;
	}

}

void CMainWnd::InitDlg()
{
	CWebBrowserUI* pWeb = static_cast<CWebBrowserUI*>(m_PM.FindControl(_T("web")));
	pWeb->BindJSWindowExternalFunc(L"testjscallcplusplus",JSCallTest);
	pWeb->Navigate2(L"about:blank");
	pWeb->Navigate2(L"file:///E:\\0rwh\\duilib\\bin\\testjs.html");
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg ) 
	{
	case WM_CREATE:				lRes = OnCreate(uMsg, wParam, lParam, bHandled);		break;
	case WM_NCHITTEST:			lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);		break;
	case WM_SYSCOMMAND:			lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);	break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				Close();
			}
			else
				bHandled = FALSE;
		}
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PM.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		InitDlg();
	}
	if ( msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName()==_T("test_btn"))
		{
			CTabLayoutUI* pLay = (CTabLayoutUI*)m_PM.FindControl(_T("test_tab"));
			if (pLay->GetCurSel() == 0)
			{
				pLay->SelectItem(1);
			}
			else
				pLay->SelectItem(0);
			
		}
	}
	else if (msg.sType == DUI_MSGTYPE_TIMER)
	{
	}
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
			if( pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
				_tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 && 
				_tcscmp(pControl->GetClass(), _T("CButtonUIEx")) != 0)
				return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT CMainWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == SC_CLOSE ) 
	{
		Close(0);

		bHandled = TRUE;
		return 0;
	}
	else if (wParam == SC_KEYMENU)	//RWH 处理打开系统菜单的响应,屏蔽掉ALT+SPACE调出系统菜单的操作
		return 0;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);//必须保留，否则消息分发就出问题了。
}