#include "stdafx.h"
#include "UIProgress.h"

namespace DuiLib
{
	CProgressUI::CProgressUI() : m_bHorizontal(true), m_nMin(0), m_nMax(100), m_nValue(0), m_dwForeColor(0)
	{
		m_uTextStyle = DT_SINGLELINE | DT_CENTER;
		SetFixedHeight(12);
		m_uTextStyle = DT_VCENTER|DT_SINGLELINE;
	}

	LPCTSTR CProgressUI::GetClass() const
	{
		return DUI_CTR_PROGRESS;
	}

	LPVOID CProgressUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_PROGRESS) == 0 ) return static_cast<CProgressUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	bool CProgressUI::IsHorizontal()
	{
		return m_bHorizontal;
	}

	void CProgressUI::SetHorizontal(bool bHorizontal)
	{
		if( m_bHorizontal == bHorizontal ) return;

		m_bHorizontal = bHorizontal;
		Invalidate();
	}

	int CProgressUI::GetMinValue() const
	{
		return m_nMin;
	}

	void CProgressUI::SetMinValue(int nMin)
	{
		m_nMin = nMin;
		Invalidate();
	}

	int CProgressUI::GetMaxValue() const
	{
		return m_nMax;
	}

	void CProgressUI::SetMaxValue(int nMax)
	{
		m_nMax = nMax;
		Invalidate();
	}

	int CProgressUI::GetValue() const
	{
		return m_nValue;
	}

	void CProgressUI::SetValue(int nValue)
	{
		m_nValue = nValue;
		if (m_nValue > m_nMax) m_nValue = m_nMax;
		if (m_nValue < m_nMin) m_nValue = m_nMin;
		Invalidate();
	}

	CDuiString CProgressUI::GetForeImage() const
	{
		return m_diFore.sDrawString;
	}

	void CProgressUI::SetForeImage(LPCTSTR pStrImage)
	{
		if( m_diFore.sDrawString == pStrImage && m_diFore.pImageInfo != NULL ) return;
		m_diFore.Clear();
		m_diFore.sDrawString = pStrImage;
		Invalidate();
	}
	DWORD CProgressUI::GetForeBkColor() const
	{
		return m_dwForeColor;
	}
	void CProgressUI::SetForeBkColor(DWORD dwForeColor)
	{
		m_dwForeColor = dwForeColor;
	}

	void CProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcscmp(pstrName, _T("hor")) == 0 ) SetHorizontal(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("min")) == 0 ) SetMinValue(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("max")) == 0 ) SetMaxValue(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("value")) == 0 ) SetValue(_ttoi(pstrValue));
		else if (_tcsicmp(pstrName, _T("forebkcolor")) == 0) {
			while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetForeBkColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	//void CProgressUI::PaintStatusImage(HDC hDC)
	//{
	//	if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
	//	if( m_nValue > m_nMax ) m_nValue = m_nMax;
	//	if( m_nValue < m_nMin ) m_nValue = m_nMin;

	//	RECT rc = {0};
	//	if( m_bHorizontal ) {
	//		rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
	//		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	//	}
	//	else {
	//		rc.top = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
	//		rc.right = m_rcItem.right - m_rcItem.left;
	//		rc.bottom = m_rcItem.bottom - m_rcItem.top;
	//	}
	//	m_diFore.rcDestOffset = rc;
	//	if( DrawImage(hDC, m_diFore) ) return;
	//}
	void CProgressUI::PaintStatusImage(HDC hDC)
	{
		if (m_nMax <= m_nMin) m_nMax = m_nMin + 1;
		if (m_nValue > m_nMax) m_nValue = m_nMax;
		if (m_nValue < m_nMin) m_nValue = m_nMin;

		RECT rc = { 0 };
		if (m_bHorizontal) {
			rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left) / (m_nMax - m_nMin);
			rc.bottom = m_rcItem.bottom - m_rcItem.top;
		}
		else {
			rc.top = (m_rcItem.bottom - m_rcItem.top) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
			rc.right = m_rcItem.right - m_rcItem.left;
			rc.bottom = m_rcItem.bottom - m_rcItem.top;
		}

		if (m_dwForeColor > 0)
		{
			int nW = rc.right - rc.left;
			int nH = rc.bottom - rc.top;
			RECT rcForeColor = { m_rcItem.left + rc.left,m_rcItem.top + rc.top,0,0 };
			rcForeColor.right = rcForeColor.left + nW;
			rcForeColor.bottom = rcForeColor.top + nH;
			CRenderEngine::DrawColor(hDC, rcForeColor, m_dwForeColor);
		}

		if (m_diFore.sDrawString.IsEmpty())
		{
			return;
		}

		CDuiString strFore;
		CDuiString strImg = m_diFore.sDrawString;
		int nPos = strImg.Find(L"file='");
		if (nPos != -1)
		{
			strImg = strImg.Mid(nPos + 6);
			nPos = strImg.Find(L"'");
			strImg = strImg.Left(nPos);
		}
		strFore.Format(L"file='%s' source='%d,%d,%d,%d' dest='%d,%d,%d,%d'", strImg.GetData(), rc.left, rc.top, rc.right, rc.bottom, rc.left, rc.top, rc.right, rc.bottom);

		CDuiString strCorner = m_diFore.sDrawString;
		nPos = strCorner.Find(L"corner='");
		if (nPos != -1)
		{
			strCorner = strCorner.Mid(nPos + 8);
			nPos = strCorner.Find(L"'");
			strCorner = strCorner.Left(nPos);

			strFore += L" corner='";
			strFore += strCorner;
			strFore += L"'";
		}	
		
		TDrawInfo diFore;
		diFore.sDrawString = strFore;

		if (DrawImage(hDC, diFore)) return;
	}
}
