#include "StdAfx.h"
#include "UIGifButton.h"

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	CGifButtonUI::CGifButtonUI(void)
	{
		m_uButtonState = 0;
		m_sCursor = _T("hand");
		m_rcBkImageDest.left = 0;
		m_rcBkImageDest.top = 0;
		m_rcBkImageDest.right = 0;
		m_rcBkImageDest.bottom = 0;

		m_dwHotTextColor = 0xFF000000;
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
		else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else
			CGifAnimUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}
	DWORD CGifButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CGifButtonUI::SetBkImageDest(const RECT& rcDest)
	{
		if (rcDest.left==0 && rcDest.top==0 && rcDest.right==0 && rcDest.bottom==0)
		{
			m_rcBkImageDest.left = 0;
			m_rcBkImageDest.top = 0;
			m_rcBkImageDest.right = m_rcItem.right-m_rcItem.left;
			m_rcBkImageDest.bottom = m_rcItem.bottom-m_rcItem.top;
		}
		else
			m_rcBkImageDest = rcDest;
	}

	bool CGifButtonUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if ( NULL == m_pGifImage )
		{		
			InitGifImage();
		}
		DrawFrame( hDC );
		return true;
	}

	void CGifButtonUI::DrawFrame( HDC hDC )
	{
		if ( NULL == hDC || NULL == m_pGifImage ) 
			return;
		GUID pageGuid = Gdiplus::FrameDimensionTime;
		Gdiplus::Graphics graphics( hDC );
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
		graphics.SetTextRenderingHint(m_TextRenderingAlias);
		RECT rcTemp = {0,0,0,0};
		if (m_rcBkImageDest.left==0 && m_rcBkImageDest.top==0 && m_rcBkImageDest.right==0 && m_rcBkImageDest.bottom==0)
		{
			rcTemp.left = m_rcItem.left;
			rcTemp.top = m_rcItem.top;
			rcTemp.right = m_rcItem.right;
			rcTemp.bottom = m_rcItem.bottom;		
		}
		else
		{
			rcTemp.left = m_rcItem.left + m_rcBkImageDest.left;
			rcTemp.top = m_rcItem.top + m_rcBkImageDest.top;
			rcTemp.right = rcTemp.left + m_rcBkImageDest.right - m_rcBkImageDest.left;
			rcTemp.bottom = rcTemp.top + m_rcBkImageDest.bottom - m_rcBkImageDest.top;
		}
		graphics.DrawImage( m_pGifImage, rcTemp.left, rcTemp.top, rcTemp.right-rcTemp.left, rcTemp.bottom-rcTemp.top );

		if (m_sText.IsEmpty() == false)
		{
			//»æÖÆÎÄ±¾
			Gdiplus::Font	nFont(hDC,m_pManager->GetFont(GetFont()));

			RECT rcText = {m_rcItem.left+m_rcTextPadding.left,m_rcItem.top+m_rcTextPadding.top,m_rcItem.right-m_rcTextPadding.right,m_rcItem.bottom-m_rcTextPadding.bottom};
			RectF nRc((float)rcText.left,(float)rcText.top,(float)rcText.right-rcText.left,(float)rcText.bottom-rcText.top);

			StringFormat format;
			StringAlignment sa = StringAlignmentNear;
			if ((m_uTextStyle & DT_VCENTER) != 0) 
				sa = StringAlignmentCenter;
			else if( (m_uTextStyle & DT_BOTTOM) != 0) 
				sa = StringAlignmentFar;
			format.SetLineAlignment((StringAlignment)sa);
			sa = StringAlignmentNear;
			if ((m_uTextStyle & DT_CENTER) != 0) 
				sa = StringAlignmentCenter;
			else if( (m_uTextStyle & DT_RIGHT) != 0) 
				sa = StringAlignmentFar;
			format.SetAlignment((StringAlignment)sa);
			if ((m_uTextStyle & DT_SINGLELINE) != 0) 
				format.SetFormatFlags(StringFormatFlagsNoWrap);

			DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;
			if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
				clrColor = GetHotTextColor();

			SolidBrush nBrush( ARGB2Color(clrColor) );

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
			graphics.DrawString(pstrText,wcslen(pstrText),&nFont,nRc,&format,&nBrush);
#ifndef _UNICODE
			delete[] pWideText;
#endif
		}

		m_pGifImage->SelectActiveFrame( &pageGuid, m_nFramePosition );
	}
}