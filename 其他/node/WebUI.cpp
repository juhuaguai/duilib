#include "WebUI.h"

bool CWebUI::m_static_bInit_webui=false;
CWebUI::CWebUI()
{
	m_pCreateViewCallback = NULL;
	m_pLoadFinishCallback = NULL;
	m_pNavigationCallback = NULL;
	m_pLoadUrlBeginCallback = NULL;

	m_WebView = wkeCreateWebView();
}

CWebUI::~CWebUI()
{
	if (m_WebView)
	{
		wkeDestroyWebView(m_WebView);
		m_WebView = NULL;
	}
}

void CWebUI::InitGlobal()
{
	if (m_static_bInit_webui)
		return;

	wkeInit();
	m_static_bInit_webui = true;
}
void CWebUI::UnInitGlobal()
{
	if (m_static_bInit_webui)
	{
		wkeShutdown();
		m_static_bInit_webui = false;
	}
}

LPCTSTR	CWebUI::GetClass() const
{
	return DUI_CTR_WEBUI;
}
LPVOID CWebUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_WEBUI) == 0 ) return static_cast<CWebUI*>(this);
	return CControlUI::GetInterface(pstrName);
}
void CWebUI::DoInit()
{
	CControlUI::DoInit();

	HWND hWnd = m_pManager->GetPaintWindow();
	wkeSetHandle(m_WebView,hWnd);
	wkeOnLoadingFinish(m_WebView, OnLoadFinish,this);
	wkeOnCreateView(m_WebView,OnCreateViewCallback,this);
	wkeOnNavigation(m_WebView,OnNavigationCallback,this);
	wkeOnPaintUpdated(m_WebView,OnPaintUpdatedCallback,this);
	wkeOnLoadUrlBegin(m_WebView, OnLoadUrlBeginCallback, this);
	wkeOnDidCreateScriptContext(m_WebView, OnCreateScriptContextCallback,this);

	wkeShowWindow(m_WebView, true);

	if (m_pManager->IsLayered())
		wkeSetTransparent( m_WebView, true);
	else
		wkeSetTransparent( m_WebView, false);

}

void CWebUI::SetPos(RECT rc, bool bNeedInvalidate/* = true*/)
{
	CControlUI::SetPos(rc,bNeedInvalidate);

	if (m_pManager && m_pManager->IsLayered())
	{
		if (m_pManager->IsHaveNoLayeredPaddingRect())
		{
			RECT rcWnd = {0};
			GetWindowRect(m_pManager->GetPaintWindow(),&rcWnd);
			RECT rcNolayeredPadding = m_pManager->GetNoLayeredPaddingRect();
			RECT rcNolayered = {rcNolayeredPadding.left,rcNolayeredPadding.top,rcWnd.right-rcWnd.left-rcNolayeredPadding.right,rcWnd.bottom-rcWnd.top-rcNolayeredPadding.bottom};
			if (rc.left>=rcNolayered.left && rc.top>=rcNolayered.top && rc.right<=rcNolayered.right &&rc.bottom<=rcNolayered.bottom)
			{
				wkeSetTransparent( m_WebView, false);
			}
			else
			{
				wkeSetTransparent( m_WebView, true);
			}
		}
		else
		{
			if (m_pManager->IsLayered())
				wkeSetTransparent( m_WebView, true);
			else
				wkeSetTransparent( m_WebView, false);
		}
	}

	wkeResize(m_WebView, rc.right - rc.left, rc.bottom - rc.top);	
}

void CWebUI::DoEvent(DuiLib::TEventUI& event)
{
	RECT rc = GetPos();
	POINT pt = {event.ptMouse.x - rc.left, event.ptMouse.y - rc.top};

	switch (event.Type)
	{
		/***************************************/
		//给webkit提供用户事件
	case DuiLib::UIEVENT_MOUSEENTER:
	case DuiLib::UIEVENT_MOUSELEAVE:
	case DuiLib::UIEVENT_MOUSEMOVE:
		{
			unsigned int flags = 0;
			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;

			wkeFireMouseEvent(m_WebView, WKE_MSG_MOUSEMOVE, pt.x, pt.y, flags);		
		}		
		break;
	case DuiLib::UIEVENT_BUTTONDOWN:
		{
			unsigned int flags = 0;
			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;
			wkeFireMouseEvent(m_WebView, WKE_MSG_LBUTTONDOWN, pt.x, pt.y, flags);
		}						
		break;
	case DuiLib::UIEVENT_BUTTONUP:
		{
			unsigned int flags = 0;
			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;
			wkeFireMouseEvent(m_WebView, WKE_MSG_LBUTTONUP, pt.x, pt.y, flags);
		}
						
		break;
	case DuiLib::UIEVENT_RBUTTONDOWN:
		{
			unsigned int flags = 0;
			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;
			wkeFireMouseEvent(m_WebView, WKE_MSG_RBUTTONDOWN, pt.x, pt.y, flags);	
		}					
		break;
	case DuiLib::UIEVENT_DBLCLICK:
		{
			unsigned int flags = 0;
			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;
			wkeFireMouseEvent(m_WebView, WKE_MSG_LBUTTONDBLCLK, pt.x, pt.y, flags);
		}						
		break;
	case DuiLib::UIEVENT_SCROLLWHEEL:
		{
			if (event.wKeyState & MK_CONTROL)
			{
				if (event.wParam == SB_LINEUP)
				{
					float fFactor = wkeGetZoomFactor(m_WebView);
					fFactor += (float)0.05;
					wkeSetZoomFactor(m_WebView,fFactor);
				}
				else
				{
					float fFactor = wkeGetZoomFactor(m_WebView);
					fFactor -= (float)0.05;
					wkeSetZoomFactor(m_WebView,fFactor);
				}
			}
			else
			{
				unsigned int flags = 0;
				//if (event.wKeyState & MK_CONTROL)
				//	flags |= WKE_CONTROL;
				if (event.wKeyState & MK_SHIFT)
					flags |= WKE_SHIFT;

				if (event.wKeyState & MK_LBUTTON)
					flags |= WKE_LBUTTON;
				if (event.wKeyState & MK_MBUTTON)
					flags |= WKE_MBUTTON;
				if (event.wKeyState & MK_RBUTTON)
					flags |= WKE_RBUTTON;
				wkeFireMouseWheelEvent(m_WebView, pt.x, pt.y,event.wParam == SB_LINEUP ? 120 : -120, flags);
			}			
		}				
		break;
	case DuiLib::UIEVENT_KEYDOWN:
		{
			unsigned int vkCode = event.wParam;
			if (vkCode == VK_F5)
			{
				wkeReload(m_WebView);
			}
			else
			{
				unsigned int flags = 0;
				if (HIWORD(event.lParam) & KF_REPEAT)
					flags |= WKE_REPEAT;
				if (HIWORD(event.lParam) & KF_EXTENDED)
					flags |= WKE_EXTENDED;
				wkeFireKeyDownEvent(m_WebView, vkCode, flags, false);
			}			
		}				
		break;
	case DuiLib::UIEVENT_KEYUP:
		{
			unsigned int vkCode = event.wParam;
			unsigned int flags = 0;
			if (HIWORD(event.lParam) & KF_REPEAT)
				flags |= WKE_REPEAT;
			if (HIWORD(event.lParam) & KF_EXTENDED)
				flags |= WKE_EXTENDED;
			wkeFireKeyUpEvent(m_WebView, vkCode, flags, false);
		}				
		break;	
	case DuiLib::UIEVENT_CHAR:
		{
			unsigned int charCode = event.wParam;
			unsigned int flags = 0;
			if (HIWORD(event.lParam) & KF_REPEAT)
				flags |= WKE_REPEAT;
			if (HIWORD(event.lParam) & KF_EXTENDED)
				flags |= WKE_EXTENDED;
			wkeFireKeyPressEvent(m_WebView, charCode, flags, false);	
		}					
		break;
	case DuiLib::UIEVENT_SETFOCUS:
		wkeSetFocus(m_WebView);		
		break;
	case DuiLib::UIEVENT_KILLFOCUS:
		wkeKillFocus(m_WebView);		
		break;	
	case DuiLib::UIEVENT_SETCURSOR:
		{
			int cursorInfoType = wkeGetCursorInfoType(m_WebView);
			HCURSOR hCur = NULL;
			switch (cursorInfoType) 
			{
			case WkeCursorInfoPointer:
				hCur = ::LoadCursor(NULL, IDC_ARROW);
				break;
			case WkeCursorInfoIBeam:
				hCur = ::LoadCursor(NULL, IDC_IBEAM);
				break;
			case WkeCursorInfoHand:
				hCur = ::LoadCursor(NULL, IDC_HAND);
				break;
			case WkeCursorInfoWait:
				hCur = ::LoadCursor(NULL, IDC_WAIT);
				break;
			case WkeCursorInfoHelp:
				hCur = ::LoadCursor(NULL, IDC_HELP);
				break;
			case WkeCursorInfoEastResize:
				hCur = ::LoadCursor(NULL, IDC_SIZEWE);
				break;
			case WkeCursorInfoNorthResize:
				hCur = ::LoadCursor(NULL, IDC_SIZENS);
				break;
			case WkeCursorInfoSouthWestResize:
			case WkeCursorInfoNorthEastResize:
				hCur = ::LoadCursor(NULL, IDC_SIZENESW);
				break;
			case WkeCursorInfoSouthResize:
			case WkeCursorInfoNorthSouthResize:
				hCur = ::LoadCursor(NULL, IDC_SIZENS);
				break;
			case WkeCursorInfoNorthWestResize:
			case WkeCursorInfoSouthEastResize:
				hCur = ::LoadCursor(NULL, IDC_SIZENWSE);
				break;
			case WkeCursorInfoWestResize:
			case WkeCursorInfoEastWestResize:
				hCur = ::LoadCursor(NULL, IDC_SIZEWE);
				break;
			case WkeCursorInfoNorthEastSouthWestResize:
			case WkeCursorInfoNorthWestSouthEastResize:
				hCur = ::LoadCursor(NULL, IDC_SIZEALL);
				break;
			default:
				hCur = ::LoadCursor(NULL, IDC_ARROW);
				break;
			}

			if (hCur) 
			{
				::SetCursor(hCur);
				return;
			}	
		}
		break;
	case DuiLib::UIEVENT_CONTEXTMENU:
		{
			unsigned int flags = 0;

			if (event.wKeyState & MK_CONTROL)
				flags |= WKE_CONTROL;
			if (event.wKeyState & MK_SHIFT)
				flags |= WKE_SHIFT;

			if (event.wKeyState & MK_LBUTTON)
				flags |= WKE_LBUTTON;
			if (event.wKeyState & MK_MBUTTON)
				flags |= WKE_MBUTTON;
			if (event.wKeyState & MK_RBUTTON)
				flags |= WKE_RBUTTON;
			wkeFireContextMenuEvent(m_WebView, pt.x, pt.y, flags);
		}				
		break;	
	default:
		break;
	}
	CControlUI::DoEvent(event);
}

bool CWebUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{	
	CControlUI::DoPaint(hDC,rcPaint,pStopControl);

	HDC hWebView = wkeGetViewDC(m_WebView);

	int nWidth = m_rcItem.right-m_rcItem.left;
	int nHeight = m_rcItem.bottom-m_rcItem.top;

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(hDC, m_rcItem.left, m_rcItem.top,nWidth, nHeight, hWebView,0, 0,nWidth,nHeight, bf);
	return true;
}

void CWebUI::JSBindFunction(const char* pszJSFuncName,PJsCallback pFunc,void* param, const int& nParamCount)
{
	wkeJsBindFunction(pszJSFuncName,pFunc,param,nParamCount);
}

void CWebUI::SetCreateViewCallback(PCreateViewCallback pCallback)
{
	m_pCreateViewCallback = pCallback;
}
void CWebUI::SetLoadFinishCallback(PLoadFinishCallback pCallback)
{
	m_pLoadFinishCallback = pCallback;
}
void CWebUI::SetNavigationCallback(PNavigationCallback pCallback)
{
	m_pNavigationCallback = pCallback;
}
void CWebUI::SetLoadUrlBeginCallback(PLoadUrlBeginCallback pCallback)
{
	m_pLoadUrlBeginCallback = pCallback;
}
void CWebUI::SetExternalJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc)
{
	CKAutoLock theLock(&m_lockExternalJs);
	m_mapExternalJs[strJSFuncName] = pFunc;
}
void CWebUI::SetWindowJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc)
{
	CKAutoLock theLock(&m_lockWindowJs);
	m_mapWindowJs[strJSFuncName] = pFunc;
}

void CWebUI::SetURL(const wstring& strUrl)
{
	Stop();

	wkeLoadURLW(m_WebView, strUrl.c_str());	
}

void CWebUI::LoadHtml(const wstring& strHtml)
{
	wkeLoadHTMLW(m_WebView, strHtml.c_str());		
}

//页面操作：后退、前进、停止、刷新
void CWebUI::Back()
{
	wkeGoBack(m_WebView);	
}
void CWebUI::Forward()
{
	wkeGoForward(m_WebView);
}
void CWebUI::Stop()
{
	wkeStopLoading(m_WebView);	
}
void CWebUI::Reload()
{
	wkeReload(m_WebView);
}

jsValue CWebUI::RunJS(const wstring& strValue)
{
	return wkeRunJSW(m_WebView, strValue.c_str());
}

void CWebUI::SetDynamicHeader(void* jobPtr,wchar_t* pszKey,wchar_t* value)
{
	wkeNetSetHTTPHeaderField(jobPtr,pszKey,value,false);
}

wkeWebView CWebUI::OnCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures)
{
	CWebUI *pWkeUI = (CWebUI*)param;
	const wchar_t* pszUrl = wkeGetStringW(url);

	if (pWkeUI && pWkeUI->m_pCreateViewCallback)
		return pWkeUI->m_pCreateViewCallback(webView,pWkeUI,navigationType,pszUrl,windowFeatures);

	return webView;
}
void CWebUI::OnLoadFinish(wkeWebView webView,void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason)
{
	CWebUI *pWkeUI = (CWebUI*)param;
	const wchar_t* pszUrl = wkeGetStringW(url);
	const wchar_t* pszFailedReason = wkeGetStringW(failedReason);

	if (pWkeUI && pWkeUI->m_pLoadFinishCallback)
		pWkeUI->m_pLoadFinishCallback(pWkeUI,pszUrl,result,pszFailedReason);
}

bool CWebUI::OnNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navType, const wkeString urlStr)
{
	CWebUI *pWkeUI = (CWebUI*)param;
	const wchar_t* pszUrl = wkeGetStringW(urlStr);

	if (pWkeUI && pWkeUI->m_pNavigationCallback)
		return pWkeUI->m_pNavigationCallback(webView,pWkeUI,navType,pszUrl);

	return true;
}

void CWebUI::OnPaintUpdatedCallback(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy)
{
	CWebUI *pWkeUI = (CWebUI*)param;
	if (pWkeUI)
		pWkeUI->Invalidate();
}

bool CWebUI::OnLoadUrlBeginCallback(wkeWebView webView, void* param, const char* url, void *job)
{
	CWebUI *pWkeUI = (CWebUI*)param;

	if (pWkeUI && pWkeUI->m_pLoadUrlBeginCallback)
		return pWkeUI->m_pLoadUrlBeginCallback(webView,pWkeUI,url,job);

	return false;
}

void CWebUI::OnCreateScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId)
{
	CWebUI *pWkeUI = (CWebUI*)param;
	if (pWkeUI)
	{
		{
			CKAutoLock theLock(&(pWkeUI->m_lockExternalJs));
			for (map<string,PJsExternalCallback>::iterator itr=pWkeUI->m_mapExternalJs.begin();itr!=pWkeUI->m_mapExternalJs.end();itr++)
			{
				jsExecState es = wkeGlobalExec(webView);
				jsValue jsExternal = jsGetGlobal(es, "external");

				jsData* data = new jsData();
				memset(data, 0, sizeof(jsData));
				strcpy(data->typeName, "Function");
				data->callAsFunction = (jsCallAsFunctionCallback)(itr->second);
				data->finalize = pWkeUI->ReleaseExternalFunction;
				jsValue func = jsFunction(es, data);
				jsSet(es, jsExternal, itr->first.c_str(), func);
				jsSetGlobal(es, "external", jsExternal);
			}
		}


		{
			CKAutoLock theLock(&(pWkeUI->m_lockWindowJs));
			for (map<string,PJsExternalCallback>::iterator itr=pWkeUI->m_mapWindowJs.begin();itr!=pWkeUI->m_mapWindowJs.end();itr++)
			{
				jsExecState es = wkeGlobalExec(webView);

				jsData* data = new jsData();
				memset(data, 0, sizeof(jsData));
				strcpy(data->typeName, "Function");
				data->callAsFunction = (jsCallAsFunctionCallback)(itr->second);
				data->finalize = pWkeUI->ReleaseExternalFunction;
				jsValue func = jsFunction(es, data);
				jsSetGlobal(es, itr->first.c_str(), func);
			}
		}
	
	}
}
void CWebUI::ReleaseExternalFunction(jsData* data)
{
	if (data)
		delete data;
}