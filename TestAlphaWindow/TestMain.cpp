#include "StdAfx.h"
#include "MainWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CDuiString szRes = CPaintManagerUI::GetInstancePath() + _T("Res");
	CPaintManagerUI::SetResourcePath(szRes.GetData());

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	HWND  hwndParent = ::FindWindow(_T("Progman"), _T("Program Manager"));
	if(NULL != hwndParent)
	{
		hwndParent = ::FindWindowEx(hwndParent, NULL, _T("SHELLDLL_DefView"), NULL);
		if(NULL != hwndParent)
		{
			hwndParent = ::FindWindowEx(hwndParent, NULL, _T("SysListView32"), NULL);	
			//SetParent(m_hWnd,hwndParent);
		}	
	}

	CMainWnd theMainWnd;
	theMainWnd.Create(hwndParent,_T(""),UI_WNDSTYLE_DIALOG, 0L, 0, 0, 0,0,NULL);		//UI_WNDSTYLE_FRAME居然有窗口最小大小限制？
	theMainWnd.CenterWindow();
	theMainWnd.ShowModal();

	//CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}
