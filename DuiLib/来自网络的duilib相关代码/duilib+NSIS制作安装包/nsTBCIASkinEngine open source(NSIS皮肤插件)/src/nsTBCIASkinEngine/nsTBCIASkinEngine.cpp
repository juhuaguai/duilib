#include "nsTBCIASkinEngine.h"
#include "SkinEngine.h"
#include <map>
#include <shlobj.h>
#include <stdio.h>
#include <atlconv.h>
#include <string>
using namespace DuiLib;

extern HINSTANCE g_hInstance;
extra_parameters* g_pluginParms;
DuiLib::CSkinEngine* g_pFrame = NULL;
BOOL g_bMSGLoopFlag = TRUE;
std::map<HWND, WNDPROC> g_windowInfoMap;
CStdString g_tempParam = _T("");
CStdString g_installPageTabName = _T("");
std::map<CStdString, CStdString> g_controlLinkInfoMap;
CStdString g_skinPath = _T("");

DuiLib::CTBCIAMessageBox* g_pMessageBox = NULL;

TCHAR g_messageBoxLayoutFileName[MAX_PATH] = {0};
TCHAR g_messageBoxTitleControlName[MAX_PATH] = {0};
TCHAR g_messageBoxTextControlName[MAX_PATH] = {0};

TCHAR g_messageBoxCloseBtnControlName[MAX_PATH] = {0}; 
TCHAR g_messageBoxYESBtnControlName[MAX_PATH] = {0}; 
TCHAR g_messageBoxNOBtnControlName[MAX_PATH] = {0}; 

static UINT_PTR PluginCallback(enum NSPIM msg)
{
	return 0;
}

void InitTBCIASkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	g_pluginParms = extra;
	EXDLL_INIT();
	extra->RegisterPluginCallback(g_hInstance, PluginCallback);
	{
		TCHAR skinPath[MAX_PATH];
		TCHAR skinLayoutFileName[MAX_PATH];
		TCHAR installPageTabName[MAX_PATH];
		ZeroMemory(skinPath, MAX_PATH);
		ZeroMemory(skinLayoutFileName, MAX_PATH);
		ZeroMemory(installPageTabName, MAX_PATH);

		popstring(skinPath);  // 皮肤路径
		popstring(skinLayoutFileName); //皮肤文件
		popstring( installPageTabName ); // 安装页面tab的名字

		DuiLib::CPaintManagerUI::SetInstance(g_hInstance);
		DuiLib::CPaintManagerUI::SetResourcePath( skinPath);
		g_installPageTabName = installPageTabName;
		g_skinPath = skinPath;

		g_pFrame = new DuiLib::CSkinEngine();
		if( g_pFrame == NULL ) return;
		g_pFrame->SetSkinXMLPath( skinLayoutFileName );
		g_pFrame->Create( NULL, _T("360Safe安装包"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 0, 0 );
		g_pFrame->CenterWindow();
		ShowWindow( g_pFrame->GetHWND(), FALSE );

		pushint( int(g_pFrame->GetHWND()));
	}
}

void FindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);

	popstring( controlName );
	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl( controlName ));
	if( pControl == NULL )
		pushint( - 1 );

	pushint( 0 );
}

void ShowLicense(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(fileName, MAX_PATH);
	popstring( controlName );
	popstring( fileName );
	CStdString finalFileName = g_skinPath + _T("\\") + fileName;	
	CRichEditUI* pRichEditControl = static_cast<CRichEditUI*>(g_pFrame->GetPaintManager().FindControl( controlName ));
	if( pRichEditControl == NULL )
		return;

	// 读许可协议文件，append到richedit中
	USES_CONVERSION;
	FILE* infile;
	char *pLicense = NULL;	
	infile = fopen( T2A(finalFileName.GetData()), "r" );
	fseek( infile, 0,  SEEK_END );
	int nSize = ftell(infile);
	fseek(infile, 0, SEEK_SET);
	pLicense = new char[nSize];	
	if (pLicense == NULL)
	{
		fclose(infile);
		return;
	}

	ZeroMemory(pLicense, sizeof(char) * nSize);
	fread_s(pLicense, nSize, sizeof(char), nSize, infile);
	pRichEditControl->AppendText( A2T(pLicense) );
	if (pLicense != NULL)
	{
		delete []pLicense;
	}
	fclose( infile );
}

void  OnControlBindNSISScript(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);

	popstring(controlName); 
	int callbackID = popint();
	g_pFrame->SaveToControlCallbackMap( controlName, callbackID );
}

void  SetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR controlData[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(controlData, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);

	popstring( controlName );
	popstring( controlData );
	popstring( dataType );

	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl( controlName ));
	if( pControl == NULL )
		return;

	if( _tcsicmp( dataType, _T("text") ) == 0 )
	{
		if( _tcsicmp( controlData, _T("error")) == 0 || _tcsicmp( controlData, _T("")) == 0 )
			pControl->SetText( pControl->GetText() );
		else
			pControl->SetText( controlData );
	}
	else if( _tcsicmp( dataType, _T("bkimage") ) == 0 )
	{
		if( _tcsicmp( controlData, _T("error")) == 0 || _tcsicmp( controlData, _T("")) == 0 )
			pControl->SetBkImage( pControl->GetBkImage());
		else
			pControl->SetBkImage( controlData );
	}
	else if( _tcsicmp( dataType, _T("link") ) == 0 )
	{
		g_controlLinkInfoMap[controlName] = controlData;
	}
	else if( _tcsicmp( dataType, _T("enable") ) == 0 )
	{
		if( _tcsicmp( controlData, _T("true")) == 0 )
			pControl->SetEnabled( true );
		else if( _tcsicmp( controlData, _T("false")) == 0 )
			pControl->SetEnabled( false );
	}
	else if( _tcsicmp( dataType, _T("visible") ) == 0 )
	{
		if( _tcsicmp( controlData, _T("true")) == 0 )
			pControl->SetVisible( true );
		else if( _tcsicmp( controlData, _T("false")) == 0 )
			pControl->SetVisible( false );
	}
}

void  GetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR ctlName[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(ctlName, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);
	popstring( ctlName );
	popstring( dataType );
	
	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl( ctlName ));
	if( pControl == NULL )
		return;

	TCHAR temp[MAX_PATH] = {0};
	_tcscpy( temp, pControl->GetText().GetData());
	if( _tcsicmp( dataType, _T("text") ) == 0 )
		pushstring( temp );
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	g_pluginParms->ExecuteCodeSegment(idEvent - 1, 0);
}

void  TBCIACreatTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	UINT callback;
	UINT interval;

	callback = popint();
	interval = popint();

	if (!callback || !interval)
		return;

	SetTimer( g_pFrame->GetHWND(), callback, interval, TimerProc );
}

void  TBCIAKillTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	UINT id;
	id = popint();
	KillTimer(g_pFrame->GetHWND(), id);
}

UINT  TBCIAMessageBox( HWND hwndParent, LPCTSTR lpTitle, LPCTSTR lpText )
{
	if( g_pMessageBox == NULL )
	{
		g_pMessageBox = new DuiLib::CTBCIAMessageBox();
		if( g_pMessageBox == NULL ) return IDNO;
		g_pMessageBox->SetSkinXMLPath( g_messageBoxLayoutFileName );
		g_pMessageBox->Create( hwndParent, _T(""), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 0, 0 );
		g_pMessageBox->CenterWindow();
	}

	CControlUI* pTitleControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl( g_messageBoxTitleControlName ));
	CControlUI* pTipTextControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl( g_messageBoxTextControlName ));
	if( pTitleControl != NULL )
		pTitleControl->SetText( lpTitle );
	if( pTipTextControl != NULL )
		pTipTextControl->SetText( lpText );

	if( g_pMessageBox->ShowModal() == -1 )
		return IDYES;

	return IDNO;
}

void  TBCIASendMessage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	HWND hwnd = (HWND)popint();
	TCHAR msgID[MAX_PATH];
	TCHAR wParam[MAX_PATH];
	TCHAR lParam[MAX_PATH];

 	ZeroMemory(msgID, MAX_PATH);
	ZeroMemory(wParam, MAX_PATH);
	ZeroMemory(lParam, MAX_PATH);

	popstring( msgID );
	popstring( wParam );
	popstring( lParam );

	if( _tcsicmp( msgID, _T("WM_TBCIAMIN")) == 0 )
		::SendMessage( hwnd, WM_TBCIAMIN, (WPARAM)wParam, (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIACLOSE")) == 0 )
		::SendMessage( hwnd, WM_TBCIACLOSE, (WPARAM)wParam, (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIABACK")) == 0 )
		::SendMessage( hwnd, WM_TBCIABACK, (WPARAM)g_installPageTabName.GetData(), (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIANEXT")) == 0 )
		::SendMessage( hwnd, WM_TBCIANEXT, (WPARAM)g_installPageTabName.GetData(), (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIACANCEL")) == 0 )
	{
		LPCTSTR lpTitle = (LPCTSTR)wParam;
		LPCTSTR lpText = (LPCTSTR)lParam;
		if( IDYES == MessageBox( hwnd, lpText, lpTitle, MB_YESNO)/*TBCIAMessageBox( hwnd, lpTitle, lpText )*/)
		{
			pushint( 0 );
			::SendMessage( hwnd, WM_TBCIACLOSE, (WPARAM)wParam, (LPARAM)lParam );
		}
		else
			pushint( -1 );
	}
	else if( _tcsicmp( msgID, _T("WM_TBCIASTARTINSTALL")) == 0 )
		::SendMessage( hwnd, WM_TBCIASTARTINSTALL, (WPARAM)g_installPageTabName.GetData(), (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIASTARTUNINSTALL")) == 0 )
		::SendMessage( hwnd, WM_TBCIASTARTUNINSTALL, (WPARAM)g_installPageTabName.GetData(), (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIAFINISHEDINSTALL")) == 0 )
		::SendMessage( hwnd, WM_TBCIAFINISHEDINSTALL, (WPARAM)wParam, (LPARAM)lParam );
	else if( _tcsicmp( msgID, _T("WM_TBCIAOPTIONSTATE")) == 0 ) // 返回option的状态
	{
		COptionUI* pOption = static_cast<COptionUI*>(g_pFrame->GetPaintManager().FindControl( wParam ));
		if( pOption == NULL )
			return;
		pushint( !pOption->IsSelected() );
	}
	else if( _tcsicmp( msgID, _T("WM_TBCIAOPENURL")) == 0 )
	{
		CStdString url = (CStdString)wParam;
		if( url.Find( _T("http://") ) == -1 )
		{
			pushstring( _T("url error") );
			return;
		}
		CStdString lpCmdLine = _T("explorer \"");
		lpCmdLine += url;
		lpCmdLine += _T("\"");
		USES_CONVERSION;
		std::string strCmdLine = T2A(lpCmdLine.GetData());		
		WinExec( strCmdLine.c_str(), SW_SHOWNORMAL);
	}
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED)
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);

	return 0;
}

void SelectFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	BROWSEINFO bi;
	TCHAR result[MAX_PATH];
	TCHAR title[MAX_PATH];
	LPITEMIDLIST resultPIDL;
	ZeroMemory(result, MAX_PATH);
	ZeroMemory(title, MAX_PATH);

	popstring( title );
	bi.hwndOwner = g_pFrame->GetHWND();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = result;
	bi.lpszTitle = title;
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = NULL;
	bi.iImage = 0;

	resultPIDL = SHBrowseForFolder(&bi);
	if (!resultPIDL)
	{
		pushint(-1);
		return;
	}

	if (SHGetPathFromIDList(resultPIDL, result))
	{
		if( result[_tcslen(result)-1] == _T('\\') )
			result[_tcslen(result)-1] = _T('');
		pushstring(result);
	}
	else
		pushint(-1);

	CoTaskMemFree(resultPIDL);
}

BOOL CALLBACK TBCIAWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL res = 0;
	std::map<HWND, WNDPROC>::iterator iter = g_windowInfoMap.find( hwnd );
	if( iter != g_windowInfoMap.end() )
	{
 		if (message == WM_PAINT)
 		{
 			ShowWindow( hwnd, SW_HIDE );
 		}
		else if( message == LVM_SETITEMTEXT ) // TODO  安装细节显示  等找到消息再写
		{
			;
		}
 		else if( message == PBM_SETPOS ) 
 		{
			CProgressUI* pProgress = static_cast<CProgressUI*>(g_pFrame->GetPaintManager().FindControl( g_tempParam ));
			pProgress->SetMaxValue( 30000 );
			if( pProgress == NULL )
				return 0;
			pProgress->SetValue( (int)wParam);

			if( pProgress->GetValue() == 30000 )
			{
				CTabLayoutUI* pTab = NULL;
				int currentIndex;
				pTab = static_cast<CTabLayoutUI*>(g_pFrame->GetPaintManager().FindControl( g_installPageTabName ));
				if( pTab == NULL )
					return -1;
				currentIndex = pTab->GetCurSel();
				pTab->SelectItem( currentIndex + 1 );
			}
 		}
 		else
 		{
			res = CallWindowProc( iter->second, hwnd, message, wParam, lParam);
		}
	}	
	return res;
}

void InstallCore( HWND hwndParent )
{
	TCHAR progressName[MAX_PATH];
	ZeroMemory(progressName, MAX_PATH);
	popstring( progressName );
	g_tempParam = progressName;
	// 接管page instfiles的消息
	g_windowInfoMap[hwndParent] = (WNDPROC) SetWindowLong(hwndParent, GWL_WNDPROC, (long) TBCIAWindowProc);
	HWND hProgressHWND = FindWindowEx( FindWindowEx( hwndParent, NULL, _T("#32770"), NULL ), NULL, _T("msctls_progress32"), NULL );
	g_windowInfoMap[hProgressHWND] = (WNDPROC) SetWindowLong(hProgressHWND, GWL_WNDPROC, (long) TBCIAWindowProc);
	HWND hInstallDetailHWND = FindWindowEx( FindWindowEx( hwndParent, NULL, _T("#32770"), NULL ), NULL, _T("SysListView32"), NULL ); 
	g_windowInfoMap[hInstallDetailHWND] = (WNDPROC) SetWindowLong(hInstallDetailHWND, GWL_WNDPROC, (long) TBCIAWindowProc);
}

void StartInstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	InstallCore( hwndParent );
}

void StartUninstall(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	InstallCore( hwndParent );
}

void ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	ShowWindow( g_pFrame->GetHWND(), TRUE );
	MSG msg = { 0 };
	while( ::GetMessage(&msg, NULL, 0, 0) && g_bMSGLoopFlag ) 
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void  ExitTBCIASkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	ExitProcess( 0 );
}

DLLEXPORT void  InitTBCIAMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	popstring( g_messageBoxLayoutFileName );

	popstring( g_messageBoxTitleControlName );
	popstring( g_messageBoxTextControlName );

	popstring( g_messageBoxCloseBtnControlName );
	popstring( g_messageBoxYESBtnControlName );
	popstring( g_messageBoxNOBtnControlName );
}