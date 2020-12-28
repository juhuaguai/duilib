#include "stdafx.h"
#include "UIOption.h"

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	COptionUI::COptionUI() : m_bSelected(false), m_dwSelectedBkColor(0), m_dwSelectedTextColor(0)
	{
	}

	COptionUI::~COptionUI()
	{
		if( !m_sGroupName.IsEmpty() && m_pManager ) m_pManager->RemoveOptionGroup(m_sGroupName.GetData(), this);
	}

	LPCTSTR COptionUI::GetClass() const
	{
		return DUI_CTR_OPTION;
	}

	LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_OPTION) == 0 ) return static_cast<COptionUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void COptionUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
	{
		CControlUI::SetManager(pManager, pParent, bInit);
		if( bInit && !m_sGroupName.IsEmpty() ) {
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName.GetData(), this);
		}
	}

	CDuiString COptionUI::GetGroup() const
	{
		return m_sGroupName;
	}

	void COptionUI::SetGroup(LPCTSTR pStrGroupName)
	{
		if( pStrGroupName == NULL ) {
			if( m_sGroupName.IsEmpty() ) return;
			m_sGroupName.Empty();
		}
		else {
			if( m_sGroupName == pStrGroupName ) return;
			if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName.GetData(), this);
			m_sGroupName = pStrGroupName;
		}

		if( !m_sGroupName.IsEmpty() ) {
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName.GetData(), this);
		}
		else {
			if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName.GetData(), this);
		}

		Selected(m_bSelected);
	}

	bool COptionUI::IsSelected() const
	{
		return m_bSelected;
	}

	void COptionUI::Selected(bool bSelected, bool bTriggerEvent)
	{
		if( m_bSelected == bSelected ) return;
		m_bSelected = bSelected;
		if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
		else m_uButtonState &= ~UISTATE_SELECTED;

		if( m_pManager != NULL ) {
			if( !m_sGroupName.IsEmpty() ) {
				if( m_bSelected ) {
					CDuiPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName.GetData());
					for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
						COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
						if( pControl != this ) {
							pControl->Selected(false, bTriggerEvent);
						}
					}
					if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
				}
			}
			else {
				if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			}
		}

		Invalidate();
	}

	bool COptionUI::Activate()
	{
		if( !CButtonUI::Activate() ) return false;
		if( !m_sGroupName.IsEmpty() ) Selected(true);
		else Selected(!m_bSelected);

		return true;
	}

	void COptionUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			if( m_bSelected ) m_uButtonState = UISTATE_SELECTED;
			else m_uButtonState = 0;
		}
	}

	CDuiString COptionUI::GetSelectedImage()
	{
		return m_diSelected.sDrawString;
	}

	void COptionUI::SetSelectedImage(const CDuiString& strImage)
	{
		if( m_diSelected.sDrawString == strImage && m_diSelected.pImageInfo != NULL ) return;
		m_diSelected.Clear();
		m_diSelected.sDrawString = strImage;
		Invalidate();
	}

	CDuiString COptionUI::GetSelectedHotImage()
	{
		return m_diSelectedHot.sDrawString;
	}

	void COptionUI::SetSelectedHotImage( const CDuiString& strImage )
	{
		if( m_diSelectedHot.sDrawString == strImage && m_diSelectedHot.pImageInfo != NULL ) return;
		m_diSelectedHot.Clear();
		m_diSelectedHot.sDrawString = strImage;
		Invalidate();
	}

	void COptionUI::SetSelectedTextColor(DWORD dwTextColor)
	{
		m_dwSelectedTextColor = dwTextColor;
	}

	DWORD COptionUI::GetSelectedTextColor()
	{
		if (m_dwSelectedTextColor == 0) m_dwSelectedTextColor = m_pManager->GetDefaultFontColor();
		return m_dwSelectedTextColor;
	}

	void COptionUI::SetSelectedBkColor( DWORD dwBkColor )
	{
		m_dwSelectedBkColor = dwBkColor;
	}

	DWORD COptionUI::GetSelectBkColor()
	{
		return m_dwSelectedBkColor;
	}

	CDuiString COptionUI::GetForeImage()
	{
		return m_diFore.sDrawString;
	}

	void COptionUI::SetForeImage(const CDuiString& strImage)
	{
		if( m_diFore.sDrawString == strImage && m_diFore.pImageInfo != NULL ) return;
		m_diFore.Clear();
		m_diFore.sDrawString = strImage;
		Invalidate();
	}

	SIZE COptionUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
		return CControlUI::EstimateSize(szAvailable);
	}

	void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("group")) == 0 ) SetGroup(pstrValue);
		else if( _tcscmp(pstrName, _T("selected")) == 0 ) Selected(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("selectedimage")) == 0 ) SetSelectedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("selectedhotimage")) == 0 ) SetSelectedHotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("selectedbkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedBkColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("selectedtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedTextColor(clrColor);
		}
		else CButtonUI::SetAttribute(pstrName, pstrValue);
	}

	void COptionUI::PaintStatusImage(HDC hDC)
	{
		if(IsEnabled() && (m_uButtonState & UISTATE_SELECTED) != 0 ) 
		{
			if ((m_uButtonState & UISTATE_HOT) != 0)
			{
				if (DrawImage(hDC, m_diSelectedHot)) 
					goto Label_ForeImage;
			}

			if( DrawImage(hDC, m_diSelected) ) 
				goto Label_ForeImage;				
		}

		UINT uSavedState = m_uButtonState;
		m_uButtonState &= ~UISTATE_PUSHED;
		CButtonUI::PaintStatusImage(hDC);
		m_uButtonState = uSavedState;

Label_ForeImage:
		DrawImage(hDC, m_diFore);
	}
	void COptionUI::PaintBkColor(HDC hDC)
	{
		do 
		{
			if ( IsEnabled() )
			{
				if ( IsFocused() && m_dwFocusedBkColor)
				{
					if( m_dwFocusedBkColor >= 0xFF000000 ) 
						CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwFocusedBkColor));
					else 
						CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwFocusedBkColor));

					break;
				}
				if( ((m_uButtonState & UISTATE_HOT) != 0) && m_dwHotBkColor) 
				{
					if( m_dwHotBkColor >= 0xFF000000 ) 
						CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
					else 
						CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwHotBkColor));

					break;
				}

				if(((m_uButtonState & UISTATE_SELECTED) != 0) && m_dwSelectedBkColor) 
				{

					if( m_dwSelectedBkColor >= 0xFF000000 ) 
						CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwSelectedBkColor));
					else 
						CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwSelectedBkColor));

					break;
				}
			}
			else
			{
				if (m_dwDisabledBkColor)
				{
					if( (m_dwDisabledBkColor >= 0xFF000000)) 
						CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
					else 
						CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwDisabledBkColor));

					break;
				}				
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

	void COptionUI::PaintText(HDC hDC)
	{
		if(IsEnabled() && (m_uButtonState & UISTATE_SELECTED) != 0 )
		{
			DWORD oldTextColor = m_dwTextColor;
			if( m_dwSelectedTextColor != 0 ) m_dwTextColor = m_dwSelectedTextColor;

			if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
			if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

			if( m_sText.IsEmpty() ) return;
			int nLinks = 0;
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding.left;
			rc.right -= m_rcTextPadding.right;
			rc.top += m_rcTextPadding.top;
			rc.bottom -= m_rcTextPadding.bottom;

			if(!GetEnabledEffect())
			{
				if( m_bShowHtml )
					CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText.GetData(), IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
					NULL, NULL, nLinks, m_iFont, m_uTextStyle);
				else
					CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText.GetData(), IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
					m_iFont, m_uTextStyle);
			}
			else
			{
				Font	nFont(hDC,m_pManager->GetFont(m_iFont));
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

				SolidBrush nSolidBrush(ARGB2Color(IsEnabled()?m_dwTextColor:m_dwDisabledTextColor));

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
#endif	//_UNICODE
				nGraphics.DrawString(pstrText,wcslen(pstrText),&nFont,RectF((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top),&format,&nSolidBrush);
#ifndef _UNICODE
				delete[] pWideText;
#endif
			}

			m_dwTextColor = oldTextColor;
		}
		else
		{
			UINT uSavedState = m_uButtonState;
			m_uButtonState &= ~UISTATE_PUSHED;
			CButtonUI::PaintText(hDC);
			m_uButtonState = uSavedState;
		}
	}
}