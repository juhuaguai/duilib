#include "StdAfx.h"
#include "MainWnd.h"

wstring g_strAppPath;	//"e:\Bin\"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
	g_strAppPath = GetAppPath();

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

	OleInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);

	wstring strRes = g_strAppPath + L"res";
	CPaintManagerUI::SetResourcePath(strRes.c_str());

	//º”‘ÿ∂‡”Ô—‘
	{
		wstring strZhJson = g_strAppPath + L"\\data\\zh.json";
		LoadMultiLanguageString(strZhJson);
	}

	wstring strTitle = CPaintManagerUI::GetMultiLanguageString(0);

	CMainWnd theMainWnd;
	theMainWnd.Create(NULL, strTitle.c_str(),UI_WNDSTYLE_DIALOG|WS_OVERLAPPEDWINDOW, 0L, 0, 0, 0,0,NULL);
	theMainWnd.CenterWindow();
	theMainWnd.ShowModal();

	OleUninitialize();
	::CoUninitialize();

	return 0;
}