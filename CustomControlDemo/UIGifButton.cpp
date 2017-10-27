#include "StdAfx.h"
#include "UIGifButton.h"

CGifButtonUI::CGifButtonUI(void)
{
	m_uButtonState = 0;
	m_sCursor = _T("hand");
}

CGifButtonUI::~CGifButtonUI(void)
{
}

LPCTSTR CGifButtonUI::GetClass() const
{
	return DUI_CTR_GIF_BUTTON;
}

LPVOID CGifButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_GIF_BUTTON) == 0 ) 
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
				m_pManager->RemoveMouseLeaveNeeded(this);
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
	else
		CGifAnimUI::SetAttribute(pstrName, pstrValue);
}