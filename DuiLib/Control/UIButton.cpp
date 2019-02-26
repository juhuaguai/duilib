#include "stdafx.h"
#include "UIButton.h"

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	CButtonUI::CButtonUI()
		: m_uButtonState(0)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		, m_dwFocusedBkColor(0)
		, m_dwHotBkColor(0)
		, m_uFadeAlphaDelta(0)
		, m_uFadeAlpha(255)
		,m_sCursor(_T("hand"))
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	LPCTSTR CButtonUI::GetClass() const
	{
		return DUI_CTR_BUTTON;
	}

	LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_BUTTON) == 0 ) return static_cast<CButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled() && IsEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() && IsEnabled()) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
            if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
                if( IsEnabled() ) {
                    if( (m_uButtonState & UISTATE_HOT) == 0  ) {
                        m_uButtonState |= UISTATE_HOT;
                        Invalidate();
                    }
                }
				m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSEENTER);
            }
			if ( GetFadeAlphaDelta() > 0 ) {
				m_pManager->SetTimer(this, FADE_TIMERID, FADE_ELLAPSE);
			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
            if( !::PtInRect(&m_rcItem, event.ptMouse ) ) {
                if( IsEnabled() ) {
                    if( (m_uButtonState & UISTATE_HOT) != 0  ) {
                        m_uButtonState &= ~UISTATE_HOT;
                        Invalidate();
                    }
                }
				m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSELEAVE);
                if (m_pManager) m_pManager->RemoveMouseLeaveNeeded(this);
                if ( GetFadeAlphaDelta() > 0 ) {
                    m_pManager->SetTimer(this, FADE_TIMERID, FADE_ELLAPSE);
                }
            }
            else {
                if (m_pManager) m_pManager->AddMouseLeaveNeeded(this);
                return;
            }
		}
		if( event.Type == UIEVENT_SETCURSOR )
		{
			if (m_sCursor == _T("arrow"))
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			else if (m_sCursor == _T("hand"))
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			else
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		if( event.Type == UIEVENT_TIMER  && event.wParam == FADE_TIMERID ) 
		{
			if( (m_uButtonState & UISTATE_HOT) != 0 ) {
				if( m_uFadeAlpha > m_uFadeAlphaDelta ) m_uFadeAlpha -= m_uFadeAlphaDelta;
				else {
					m_uFadeAlpha = 0;
					m_pManager->KillTimer(this, FADE_TIMERID);
				}
			}
			else {
				if( m_uFadeAlpha < 255-m_uFadeAlphaDelta ) m_uFadeAlpha += m_uFadeAlphaDelta;
				else {
					m_uFadeAlpha = 255;
					m_pManager->KillTimer(this, FADE_TIMERID);
				}
			}
			Invalidate();
			return;
		}
		CLabelUI::DoEvent(event);
	}

	bool CButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	void CButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}

	DWORD CButtonUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}

	void CButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CButtonUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CButtonUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	void CButtonUI::SetFocusedBkColor(DWORD dwColor)
	{
		m_dwFocusedBkColor = dwColor;
	}

	DWORD CButtonUI::GetFocusedBkColor() const
	{
		return m_dwFocusedBkColor;
	}

	CDuiString CButtonUI::GetNormalImage()
	{
		return m_diNormal.sDrawString;
	}

	void CButtonUI::SetNormalImage(const CDuiString& strImage)
	{
		if( m_diNormal.sDrawString == strImage && m_diNormal.pImageInfo != NULL ) return;
		m_diNormal.Clear();
		m_diNormal.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetHotImage()
	{
		return m_diHot.sDrawString;
	}

	void CButtonUI::SetHotImage(const CDuiString& strImage)
	{
		if( m_diHot.sDrawString == strImage && m_diHot.pImageInfo != NULL ) return;
		m_diHot.Clear();
		m_diHot.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetPushedImage()
	{
		return m_diPushed.sDrawString;
	}

	void CButtonUI::SetPushedImage(const CDuiString& strImage)
	{
		if( m_diPushed.sDrawString == strImage && m_diPushed.pImageInfo != NULL ) return;
		m_diPushed.Clear();
		m_diPushed.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetFocusedImage()
	{
		return m_diFocused.sDrawString;
	}

	void CButtonUI::SetFocusedImage(const CDuiString& strImage)
	{
		if( m_diFocused.sDrawString == strImage && m_diFocused.pImageInfo != NULL ) return;
		m_diFocused.Clear();
		m_diFocused.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetDisabledImage()
	{
		return m_diDisabled.sDrawString;
	}

	void CButtonUI::SetDisabledImage(const CDuiString& strImage)
	{
		if( m_diDisabled.sDrawString == strImage && m_diDisabled.pImageInfo != NULL ) return;
		m_diDisabled.Clear();
		m_diDisabled.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetForeImage()
	{
		return m_diFore.sDrawString;
	}

	void CButtonUI::SetForeImage(const CDuiString& strImage)
	{
		if( m_diFore.sDrawString == strImage && m_diFore.pImageInfo != NULL ) return;
		m_diFore.Clear();
		m_diFore.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetHotForeImage()
	{
		return m_diHotFore.sDrawString;
	}

	void CButtonUI::SetHotForeImage(const CDuiString& strImage)
	{
		if( m_diHotFore.sDrawString == strImage && m_diHotFore.pImageInfo != NULL ) return;
		m_diHotFore.Clear();
		m_diHotFore.sDrawString = strImage;
		Invalidate();
	}

	CDuiString CButtonUI::GetCursor()
	{
		return m_sCursor;
	}

	void CButtonUI::SetCursor(const CDuiString& strCursor)
	{
		m_sCursor = strCursor;
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		if(IsEnabled() && ::PtInRect(&m_rcItem,ptMouse))
		{
			if (_tcscmp(m_sCursor.GetData(),_T("arrow")) == 0)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			else if (_tcscmp(m_sCursor.GetData(),_T("hand")) == 0)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		}
	}

	void CButtonUI::SetFiveStatusImage(const CDuiString& strImage)
	{
		m_diNormal.Clear();
		m_diNormal.sDrawString = strImage;
		DrawImage(NULL, m_diNormal);
		if (m_diNormal.pImageInfo) {
			LONG width = m_diNormal.pImageInfo->nX / 5;
			LONG height = m_diNormal.pImageInfo->nY;
			m_diNormal.rcBmpPart = CDuiRect(0, 0, width, height);
			if( m_bFloat && m_cxyFixed.cx == 0 && m_cxyFixed.cy == 0 ) {
				m_cxyFixed.cx = width;
				m_cxyFixed.cy = height;
			}
		}

		m_diPushed.Clear();
		m_diPushed.sDrawString = strImage;
		DrawImage(NULL, m_diPushed);
		if (m_diPushed.pImageInfo) {
			LONG width = m_diPushed.pImageInfo->nX / 5;
			LONG height = m_diPushed.pImageInfo->nY;
			m_diPushed.rcBmpPart = CDuiRect(width, 0, width*2, height);
		}

		m_diHot.Clear();
		m_diHot.sDrawString = strImage;
		DrawImage(NULL, m_diHot);
		if (m_diHot.pImageInfo) {
			LONG width = m_diHot.pImageInfo->nX / 5;
			LONG height = m_diHot.pImageInfo->nY;
			m_diHot.rcBmpPart = CDuiRect(width*2, 0, width*3, height);
		}

		m_diFocused.Clear();
		m_diFocused.sDrawString = strImage;
		DrawImage(NULL, m_diFocused);
		if (m_diFocused.pImageInfo) {
			LONG width = m_diFocused.pImageInfo->nX / 5;
			LONG height = m_diFocused.pImageInfo->nY;
			m_diFocused.rcBmpPart = CDuiRect(width*3, 0, width*4, height);
		}

		m_diDisabled.Clear();
		m_diDisabled.sDrawString = strImage;
		DrawImage(NULL, m_diDisabled);
		if (m_diDisabled.pImageInfo) {
			LONG width = m_diDisabled.pImageInfo->nX / 5;
			LONG height = m_diDisabled.pImageInfo->nY;
			m_diDisabled.rcBmpPart = CDuiRect(width*4, 0, width*5, height);
		}

		Invalidate();
	}

	void CButtonUI::SetFadeAlphaDelta(BYTE uDelta)
	{
		m_uFadeAlphaDelta = uDelta;
	}

	BYTE CButtonUI::GetFadeAlphaDelta()
	{
		return m_uFadeAlphaDelta;
	}

	SIZE CButtonUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("fivestatusimage")) == 0 ) SetFiveStatusImage(pstrValue);
		else if( _tcscmp(pstrName, _T("fadedelta")) == 0 ) SetFadeAlphaDelta((BYTE)_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("focusedbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedBkColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("cursor")) == 0)	SetCursor(pstrValue);
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CButtonUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
			clrColor = GetHotTextColor();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		//if( m_bShowHtml )
		//	CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
		//	NULL, NULL, nLinks, m_iFont, m_uTextStyle);
		//else
		//	CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, \
		//	m_iFont, m_uTextStyle);

		if(!GetEnabledEffect())
		{
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText.GetData(), clrColor, \
				NULL, NULL, nLinks, m_iFont, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText.GetData(), clrColor, \
				m_iFont, m_uTextStyle);
		}
		else
		{
			Font	nFont(hDC,m_pManager->GetFont(GetFont()));
			Graphics nGraphics(hDC);
			nGraphics.SetTextRenderingHint(GetTextRenderingAlias());

			StringFormat format;
			StringAlignment sa = StringAlignment::StringAlignmentNear;
			if ((m_uTextStyle & DT_VCENTER) != 0) sa = StringAlignment::StringAlignmentCenter;
			else if( (m_uTextStyle & DT_BOTTOM) != 0) sa = StringAlignment::StringAlignmentFar;
			format.SetLineAlignment((StringAlignment)sa);
			sa = StringAlignment::StringAlignmentNear;
			if ((m_uTextStyle & DT_CENTER) != 0) sa = StringAlignment::StringAlignmentCenter;
			else if( (m_uTextStyle & DT_RIGHT) != 0) sa = StringAlignment::StringAlignmentFar;
			format.SetAlignment((StringAlignment)sa);
			if ((m_uTextStyle & DT_SINGLELINE) != 0) format.SetFormatFlags(StringFormatFlagsNoWrap);
			
			SolidBrush nSolidBrush(ARGB2Color(clrColor));

#ifdef _UNICODE
			nGraphics.DrawString(m_sText.GetData(),m_sText.GetLength(),&nFont,RectF((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top),&format,&nSolidBrush);
#else
			int iLen = wcslen(m_pWideText);
			nGraphics.DrawString(m_pWideText,iLen,&nFont,RectF((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top),&format,&nSolidBrush);
#endif	//_UNICODE
		}
	}

	void CButtonUI::PaintBkColor(HDC hDC)
	{
		if ( IsEnabled() )
		{
			if ( IsFocused() )
			{
				if (m_dwFocusedBkColor == 0)
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
					if( m_dwFocusedBkColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwFocusedBkColor));
					else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwFocusedBkColor));
				}
			}
			else
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

	void CButtonUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if (DrawImage(hDC, m_diDisabled)) goto Label_ForeImage;
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
			if (!DrawImage(hDC, m_diPushed))
				DrawImage(hDC, m_diNormal);
			if (DrawImage(hDC, m_diPushedFore)) return;
			else goto Label_ForeImage;
		}
		else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( GetFadeAlphaDelta() > 0 ) {
				if( m_uFadeAlpha == 0 ) {
					m_diHot.uFade = 255;
					DrawImage(hDC, m_diHot);
				}
				else {
					m_diNormal.uFade = m_uFadeAlpha;
					DrawImage(hDC, m_diNormal);
					m_diHot.uFade = 255 - m_uFadeAlpha;
					DrawImage(hDC, m_diHot);
				}
			}
			else {
				if (!DrawImage(hDC, m_diHot))
					DrawImage(hDC, m_diNormal);
			}

			if (DrawImage(hDC, m_diHotFore)) return;
			else if(m_dwHotBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				return;
			}
			else goto Label_ForeImage;
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if (DrawImage(hDC, m_diFocused)) goto Label_ForeImage;;
		}

		if ( GetFadeAlphaDelta() > 0 ) {
			if( m_uFadeAlpha == 255 ) {
				m_diNormal.uFade = 255;
				DrawImage(hDC, m_diNormal);
			}
			else {
				m_diHot.uFade = 255 - m_uFadeAlpha;
				DrawImage(hDC, m_diHot);
				m_diNormal.uFade = m_uFadeAlpha;
				DrawImage(hDC, m_diNormal);
			}
		}
		else {
			DrawImage(hDC, m_diNormal);
		}

Label_ForeImage:
		DrawImage(hDC, m_diFore);
	}
}