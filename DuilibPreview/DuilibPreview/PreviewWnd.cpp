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

	return 0;
}

LRESULT CPreviewWnd::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	//处理窗口Resize
	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	//处理标题栏移动
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 && 
				_tcsicmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_EDIT) != 0 &&
				_tcsicmp(pControl->GetClass(), DUI_CTR_RICHEDIT) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

void CPreviewWnd::SetSkinFile( CDuiString skinFile )
{
	m_skinFile=skinFile;
}

void CPreviewWnd::SetSkinFolder( CDuiString skinFolder)
{
	m_skinFolder=skinFolder;
}
