#include "stdafx.h"
#include "App.h"
#include "PreviewWnd.h"
#include "StringTools.h"

//  程序入口及Duilib初始化部分 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow)
{ 
#ifdef UNICODE
	CDuiString* filepath = new CDuiString(_W(lpCmdLine));
#else
	CDuiString* filepath = new CDuiString(lpCmdLine);
#endif
	//CDuiString* filepath = new CDuiString(_T("C:\\Users\\rwh\\Desktop\\新建文件夹\\test1.xml"));

	if(filepath->IsEmpty()) return ShowHelpInfo();
	int n = filepath->ReverseFind('\\')+1;
	if(filepath->Right(4)!= _T(".xml")) return ShowHelpInfo();

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	 CPreviewWnd* pPreviewForm = new CPreviewWnd();
     if( pPreviewForm == NULL ) return 0; 
	 pPreviewForm->SetSkinFile(filepath->Right(filepath->GetLength() - n).GetData());
	 pPreviewForm->SetSkinFolder(filepath->Left(n).GetData());
	 SetCurrentDirectory(pPreviewForm->GetSkinFolder().GetData());
     pPreviewForm->Create(NULL, _T("预览窗口"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE); 
	 pPreviewForm->CenterWindow();
     pPreviewForm->ShowWindow(true); 

     CPaintManagerUI::MessageLoop(); 
 
	::CoUninitialize();
     return 0; 
} 