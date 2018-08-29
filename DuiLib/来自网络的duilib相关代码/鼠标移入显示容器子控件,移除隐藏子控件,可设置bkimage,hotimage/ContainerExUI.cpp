#include "StdAfx.h"
#include "ContainerExUI.h"

CContainerExUI::CContainerExUI(void)
				:m_uMouseState(0)
				,m_dwDisabledBkColor(0)
				, m_dwHotBkColor(0)
				,m_bMustHotChildVisible(true)
				,m_bChildVisible(true)
{
}

CContainerExUI::~CContainerExUI(void)
{
}

LPCTSTR CContainerExUI::GetClass() const
{
	return DUI_CTR_CONTAINER_EX;
}

LPVOID CContainerExUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_CONTAINER_EX) == 0 ) return static_cast<CContainerExUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

UINT CContainerExUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CContainerExUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
			if( IsEnabled() ) {
				if( (m_uMouseState & UISTATE_HOT) == 0  ) {
					m_uMouseState |= UISTATE_HOT;
					Invalidate();
				}
			}
			m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSEENTER);
		}
	}
	else if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( !::PtInRect(&m_rcItem, event.ptMouse ) ) {
			if( IsEnabled() ) {
				if( (m_uMouseState & UISTATE_HOT) != 0  ) {
					m_uMouseState &= ~UISTATE_HOT;
					Invalidate();
				}
			}
			m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSELEAVE);
			if (m_pManager) m_pManager->RemoveMouseLeaveNeeded(this);
		}
		else {
			if (m_pManager) m_pManager->AddMouseLeaveNeeded(this);
			return;
		}
	}
	else
		CContainerUI::DoEvent(event);
}

void CContainerExUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uMouseState = 0;
	}
}

LPCTSTR CContainerExUI::GetNormalImage()
{
	return m_diNormal.sDrawString;
}
void CContainerExUI::SetNormalImage(LPCTSTR pStrImage)
{
	if( m_diNormal.sDrawString == pStrImage && m_diNormal.pImageInfo != NULL ) return;
	m_diNormal.Clear();
	m_diNormal.sDrawString = pStrImage;
	Invalidate();
}
LPCTSTR CContainerExUI::GetHotImage()
{
	return m_diHot.sDrawString;
}
void CContainerExUI::SetHotImage(LPCTSTR pStrImage)
{
	if( m_diHot.sDrawString == pStrImage && m_diHot.pImageInfo != NULL ) return;
	m_diHot.Clear();
	m_diHot.sDrawString = pStrImage;
	Invalidate();
}
LPCTSTR CContainerExUI::GetDisabledImage()
{
	return m_diDisabled.sDrawString;
}
void CContainerExUI::SetDisabledImage(LPCTSTR pStrImage)
{
	if( m_diDisabled.sDrawString == pStrImage && m_diDisabled.pImageInfo != NULL ) return;
	m_diDisabled.Clear();
	m_diDisabled.sDrawString = pStrImage;
	Invalidate();
}
void CContainerExUI::SetHotBkColor(DWORD dwColor)
{
	m_dwHotBkColor = dwColor;
}
DWORD CContainerExUI::GetHotBkColor() const
{
	return m_dwHotBkColor;
}
void CContainerExUI::SetDisabledBkColor(DWORD dwBkColor)
{
	m_dwDisabledBkColor = dwBkColor;
	Invalidate();
}
DWORD CContainerExUI::GetDisabledBkColor() const
{
	return m_dwDisabledBkColor;
}

void CContainerExUI::PaintBkColor(HDC hDC)
{
	if ( IsEnabled() )
	{
		if( m_dwBackColor != 0 ) {
			if( m_dwBackColor2 != 0 ) {
				if( m_dwBackColor3 != 0 ) {
					RECT rc = m_rcItem;
					rc.bottom = (rc.bottom + rc.top) / 2;
					CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 8);
					rc.top = rc.bottom;
					rc.bottom = m_rcItem.bottom;
					CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), true, 8);
				}
				else 
					CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 16);
			}
			else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
			else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
		}
	}
	else
	{
		if (m_dwDisabledBkColor == 0)
		{
			if( m_dwBackColor != 0 ) {
				if( m_dwBackColor2 != 0 ) {
					if( m_dwBackColor3 != 0 ) {
						RECT rc = m_rcItem;
						rc.bottom = (rc.bottom + rc.top) / 2;
						CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 8);
						rc.top = rc.bottom;
						rc.bottom = m_rcItem.bottom;
						CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), true, 8);
					}
					else 
						CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 16);
				}
				else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
				else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
			}
		}
		else
		{
			if( m_dwDisabledBkColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
			else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwDisabledBkColor));
		}
	}
}

void CContainerExUI::PaintStatusImage(HDC hDC)
{
	if( !IsEnabled() ) m_uMouseState |= UISTATE_DISABLED;
	else m_uMouseState &= ~ UISTATE_DISABLED;

	if( (m_uMouseState & UISTATE_DISABLED) != 0 ) {
		if (DrawImage(hDC, m_diDisabled)) goto Label_ForeImage;
	}
	else if( (m_uMouseState & UISTATE_HOT) != 0 ) {
		if (!DrawImage(hDC, m_diHot))
			DrawImage(hDC, m_diNormal);

		if(m_dwHotBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
			return;
		}
		else goto Label_ForeImage;
	}

	DrawImage(hDC, m_diNormal);

Label_ForeImage:
	DrawImage(hDC, m_diFore);
}

void CContainerExUI::SetMustHotChildVisible(bool bMustHot/*=true*/)
{
	if (m_bMustHotChildVisible!=bMustHot)
	{
		m_bMustHotChildVisible = bMustHot;
		Invalidate();
	}	
}
bool CContainerExUI::GetMustHotChildVisible()
{
	return m_bMustHotChildVisible;
}
void CContainerExUI::SetChildVisible(bool bVisible/*=true*/)
{
	if (m_bChildVisible!=bVisible)
	{
		m_bChildVisible = bVisible;
		Invalidate();
	}	
}
bool CContainerExUI::GetChildVisible()
{
	return m_bChildVisible;
}

bool CContainerExUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	RECT rcTemp = { 0 };
	if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;

	CRenderClip clip;
	CRenderClip::GenerateClip(hDC, rcTemp, clip);
	CControlUI::DoPaint(hDC, rcPaint, pStopControl);

	bool bContinuePaint = true;
	if (GetChildVisible())
	{
		if (GetMustHotChildVisible())
		{
			if( (m_uMouseState & UISTATE_HOT) == 0 ) 
				bContinuePaint = false;
		}
	}
	else
		bContinuePaint = false;

	if (bContinuePaint)
	{
		if( m_items.GetSize() > 0 ) {
			RECT rc = m_rcItem;
			rc.left += m_rcInset.left;
			rc.top += m_rcInset.top;
			rc.right -= m_rcInset.right;
			rc.bottom -= m_rcInset.bottom;
			if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

			if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
				for( int it = 0; it < m_items.GetSize(); it++ ) {
					CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
					if( pControl == pStopControl ) return false;
					if( !pControl->IsVisible() ) continue;
					if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
					if( pControl->IsFloat() ) {
						if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
						if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
					}
				}
			}
			else {
				CRenderClip childClip;
				CRenderClip::GenerateClip(hDC, rcTemp, childClip);
				for( int it = 0; it < m_items.GetSize(); it++ ) {
					CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
					if( pControl == pStopControl ) return false;
					if( !pControl->IsVisible() ) continue;
					if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
					if( pControl->IsFloat() ) {
						if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
						CRenderClip::UseOldClipBegin(hDC, childClip);
						if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
						CRenderClip::UseOldClipEnd(hDC, childClip);
					}
					else {
						if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
						if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
					}
				}
			}
		}

		if( m_pVerticalScrollBar != NULL ) {
			if( m_pVerticalScrollBar == pStopControl ) return false;
			if (m_pVerticalScrollBar->IsVisible()) {
				if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
					if( !m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
				}
			}
		}

		if( m_pHorizontalScrollBar != NULL ) {
			if( m_pHorizontalScrollBar == pStopControl ) return false;
			if (m_pHorizontalScrollBar->IsVisible()) {
				if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
					if( !m_pHorizontalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
				}
			}
		}
	}
	
	return true;
}

void CContainerExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
	else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetHotBkColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("disabledbkcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetDisabledBkColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("childvisible")) == 0 ) SetChildVisible(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("musthotchildvisible")) == 0 ) SetMustHotChildVisible(_tcscmp(pstrValue, _T("true")) == 0);
	else CContainerUI::SetAttribute(pstrName, pstrValue);
}