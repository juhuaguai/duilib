#include "StdAfx.h"
#include "UIGifButton.h"

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	CGifButtonUI::CGifButtonUI(void)
	{
		m_uButtonState = 0;
		m_sCursor = _T("hand");

		m_dwHotTextColor = 0xFF000000;

		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	CGifButtonUI::~CGifButtonUI(void)
	{
	}

	LPCTSTR CGifButtonUI::GetClass() const
	{
		return DUI_CTR_GIFBUTTON;
	}

	LPVOID CGifButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_GIFBUTTON) == 0 ) 
			return static_cast<CGifButtonUI*>(this);
		return CGifAnimUI::GetInterface(pstrName);
	}

	UINT CGifButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CGifButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) 
		{
			if( m_pParent != NULL ) 
				m_pParent->DoEvent(event);
			else 
				CGifAnimUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled() && IsEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) 
				{
					Activate();
					return;
				}
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) 
			{
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			{
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
					m_uButtonState |= UISTATE_PUSHED;
				else 
					m_uButtonState &= ~UISTATE_PUSHED;

				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			{
				if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) 
					Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);

				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() && IsEnabled()) 
			{
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse ) ) 
			{
				if( IsEnabled() ) 
				{
					if( (m_uButtonState & UISTATE_HOT) == 0  ) 
					{
						m_uButtonState |= UISTATE_HOT;
						Invalidate();
					}
				}
				if (m_pManager) 
					m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSEENTER);
			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( !::PtInRect(&m_rcItem, event.ptMouse ) ) 
			{
				if( IsEnabled() ) 
				{
					if( (m_uButtonState & UISTATE_HOT) != 0  ) 
					{
						m_uButtonState &= ~UISTATE_HOT;
						Invalidate();
					}
				}
				if (m_pManager) 
				{
					m_pManager->SendNotify(this,DUI_MSGTYPE_MOUSELEAVE);
					m_pManager->RemoveMouseLeaveNeeded(this);
				}
			}
			else 
			{
				if (m_pManager)
					m_pManager->AddMouseLeaveNeeded(this);
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

		CGifAnimUI::DoEvent(event);
	}

	bool CGifButtonUI::Activate()
	{
		if( !CGifAnimUI::Activate() ) 
			return false;
		if( m_pManager != NULL ) 
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CGifButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) 
		{
			m_uButtonState = 0;
		}
	}

	CDuiString CGifButtonUI::GetCursor()
	{
		return m_sCursor;
	}

	void CGifButtonUI::SetCursor(LPCTSTR pStrCursor)
	{
		m_sCursor = pStrCursor;
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		if(IsEnabled() && ::PtInRect(&m_rcItem,ptMouse))
		{
			if (_tcscmp(pStrCursor,_T("arrow")) == 0)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			else if (_tcscmp(pStrCursor,_T("hand")) == 0)
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		}
	}

	void CGifButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("cursor")) == 0 ) 
			SetCursor(pstrValue);
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
		else
			CGifAnimUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}
	DWORD CGifButtonUI::GetHotBkColor() const
	{
		return m_dwHotBkColor;
	}
	void CGifButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}
	DWORD CGifButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}
	void CGifButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}
	DWORD CGifButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CGifButtonUI::PaintBkColor(HDC hDC)
	{
		do 
		{
			if ( (IsEnabled()==false) && (m_dwDisabledBkColor != 0) )
			{
				if( m_dwDisabledBkColor >= 0xFF000000 ) 
					CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
				else 
					CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwDisabledBkColor));

				break;
			}

			if( (m_uButtonState & UISTATE_HOT)!=0 && (m_dwHotBkColor!=0)) 
			{
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				break;
			}

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
		} while (0);
	}
	void CGifButtonUI::PaintText(HDC hDC)
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
		//else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
		//	clrColor = GetFocusedTextColor();

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

			CDuiString sText1 = m_sText;
			CPaintManagerUI::ProcessMultiLanguageTokens(sText1);
#ifdef _UNICODE			
			LPCWSTR pstrText = sText1.GetData();
#else
			int iLen = _tcslen(sText1.GetData());
			LPWSTR pWideText = new WCHAR[iLen + 1];
			::ZeroMemory(pWideText, (iLen + 1) * sizeof(WCHAR));
			::MultiByteToWideChar(CP_ACP, 0, sText1.GetData(), -1, pWideText, iLen);
			LPCWSTR pstrText = pWideText;			
#endif
			nGraphics.DrawString(pstrText,wcslen(pstrText),&nFont,RectF((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top),&format,&nSolidBrush);
#ifndef _UNICODE
			delete[] pWideText;
#endif	
		}
	}
}