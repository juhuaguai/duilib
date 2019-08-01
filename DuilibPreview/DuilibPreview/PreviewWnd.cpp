#include "StdAfx.h"
#include "PreviewWnd.h"


DuiLib::CDuiString CPreviewWnd::GetSkinFolder()
{
	return m_skinFolder;
}

DuiLib::CDuiString CPreviewWnd::GetSkinFile()
{
	return m_skinFile;
}

LPCTSTR CPreviewWnd::GetWindowClassName( void ) const
{
	return _T("PreviewWnd");
}

void CPreviewWnd::Notify(TNotifyUI& msg)
{

}

LRESULT CPreviewWnd::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	PostQuitMessage(0);
	bHandled=TRUE;
	return 0;
}

void CPreviewWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CPreviewWnd::OnKeyDown( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled )
{
	if(wParam==VK_ESCAPE){
		Close();
	}
	else
		bHandled = FALSE;

	return 0;
}

void CPreviewWnd::SetSkinFile( CDuiString skinFile )
{
	m_skinFile=skinFile;
}

void CPreviewWnd::SetSkinFolder( CDuiString skinFolder)
{
	m_skinFolder=skinFolder;
}
