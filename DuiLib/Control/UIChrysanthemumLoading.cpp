#include "StdAfx.h"
#include "UIChrysanthemumLoading.h"
#include <math.h>

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	CChrysanthemumLoadingUI::CChrysanthemumLoadingUI(void)	: 
		m_nTimeInterval(40), 
		m_nNumberOfSpoke(10),
		m_nSpokeThickness(4),
		m_nProgressValue(0),
		m_nOuterCircleRadius(0),
		m_nInnerCircleRadius(0),
		m_pColors(NULL),
		m_pAngles(NULL),
		m_CenterPoint(0,0),
		m_dwSpokeColor(0xFF000000),
		m_iFont(-1),
		m_TextRenderingAlias(TextRenderingHintAntiAlias),
		m_uTextStyle(DT_SINGLELINE | DT_VCENTER | DT_CENTER),
		m_dwTextColor(0xFF000000)
	{
		m_rcTextPadding.left = m_rcTextPadding.top = m_rcTextPadding.right = m_rcTextPadding.bottom = 0;
	}

	CChrysanthemumLoadingUI::~CChrysanthemumLoadingUI(void)
	{
		if (m_pManager)
			m_pManager->KillTimer( this, EVENT_TIME_ID );

		if (m_pColors)
			delete []m_pColors;

		if (m_pAngles)
			delete []m_pAngles;
	}

	LPCTSTR CChrysanthemumLoadingUI::GetClass() const
	{
		return DUI_CTR_CHRYSANTHEMUMLOADING;
	}

	LPVOID CChrysanthemumLoadingUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_CHRYSANTHEMUMLOADING) == 0 ) return static_cast<CChrysanthemumLoadingUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CChrysanthemumLoadingUI::InitChrysanthemumLoading()
	{
		//m_pAngles
		if (m_nNumberOfSpoke==0)
			return;
		if (m_pAngles)
			delete []m_pAngles;		
		m_pAngles = new double[m_nNumberOfSpoke];
		double dblAngle = 360.0 / (double)m_nNumberOfSpoke;
		for (int shtCounter = 0; shtCounter < m_nNumberOfSpoke; shtCounter++)
		{
			m_pAngles[shtCounter] = (shtCounter == 0 ? dblAngle : m_pAngles[shtCounter - 1] + dblAngle);
		}

		//m_pColors
		if (m_pColors)
			delete []m_pColors;	
		m_pColors = new Color[m_nNumberOfSpoke];
		byte bytIncrement = (byte)(255 / m_nNumberOfSpoke);
		byte PERCENTAGE_OF_DARKEN = 0;
		for (int intCursor = 0; intCursor < m_nNumberOfSpoke; intCursor++)
		{
			if (intCursor == 0)
			{
				m_pColors[intCursor] = Color(m_dwSpokeColor);
			}
			else
			{
				PERCENTAGE_OF_DARKEN += bytIncrement;
				if (PERCENTAGE_OF_DARKEN > 255)
					PERCENTAGE_OF_DARKEN = 255;
				m_pColors[intCursor] = Color(PERCENTAGE_OF_DARKEN,GetBValue(m_dwSpokeColor),GetGValue(m_dwSpokeColor),GetRValue(m_dwSpokeColor));
			}
		}
	}

	void CChrysanthemumLoadingUI::DoInit()
	{
		InitChrysanthemumLoading();

		if (m_pManager)
			m_pManager->SetTimer( this, EVENT_TIME_ID, m_nTimeInterval );
	}

	bool CChrysanthemumLoadingUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if (IsVisible())
		{
			int nWidth = m_rcItem.right-m_rcItem.left;
			int nHeight = m_rcItem.bottom-m_rcItem.top;
			m_CenterPoint.X = nWidth / 2;
			m_CenterPoint.Y = nHeight / 2;

			Bitmap btm(nWidth, nHeight);
			Graphics g(&btm);
			g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			SolidBrush backBrush(ARGB2Color(m_dwBackColor));
			g.FillRectangle(&backBrush,0,0,nWidth,nHeight);

			m_nProgressValue = ++m_nProgressValue % m_nNumberOfSpoke;
			int intPosition = m_nProgressValue;
			for (int intCounter = 0; intCounter < m_nNumberOfSpoke; intCounter++)
			{
				intPosition = intPosition % m_nNumberOfSpoke;

				{
					SolidBrush brush(m_pColors[intCounter]);
					Pen objPen(&brush, m_nSpokeThickness);				
					objPen.SetStartCap(LineCap::LineCapRound);
					objPen.SetEndCap(LineCap::LineCapRound);

					double dblAngle = 3.14 * m_pAngles[intPosition] / 180.0;
					PointF pf1(m_CenterPoint.X + m_nInnerCircleRadius * (float)cos(dblAngle), m_CenterPoint.Y + m_nInnerCircleRadius * (float)sin(dblAngle));
					PointF pf2(m_CenterPoint.X + m_nOuterCircleRadius * (float)cos(dblAngle), m_CenterPoint.Y + m_nOuterCircleRadius * (float)sin(dblAngle));
					g.DrawLine(&objPen, pf1, pf2);
				}

				intPosition++;
			}
			//绘制文字
			if (m_sText.IsEmpty() == false)
			{
				//绘制文本
#ifdef _UNICODE
				LPWSTR pWideText = (LPWSTR)m_sText.GetData();
#else 
				int iLen = _tcslen(m_sText.GetData());
				LPWSTR pWideText = new WCHAR[iLen + 1];
				::ZeroMemory(pWideText, (iLen + 1) * sizeof(WCHAR));
				::MultiByteToWideChar(CP_ACP, 0, m_sText.GetData(), -1, pWideText, iLen);
#endif
				Gdiplus::Font	nFont(hDC,m_pManager->GetFont(GetFont()));

				RECT rcText = {m_rcTextPadding.left,m_rcTextPadding.top,nWidth-m_rcTextPadding.right,nHeight-m_rcTextPadding.bottom};
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

				SolidBrush nBrush( ARGB2Color(m_dwTextColor) );
				g.DrawString(pWideText,wcslen(pWideText),&nFont,nRc,&format,&nBrush);
#ifndef _UNICODE
				delete[] pWideText;
#endif
			}

			// 获得窗口的Graphics对象
			Graphics gh(hDC);
			// 将描画好的CacheImage画到窗口上
			gh.DrawImage(&btm, m_rcItem.left, m_rcItem.top);		
		}
		return true;
	}

	void CChrysanthemumLoadingUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER )
		{
			if (event.wParam == EVENT_TIME_ID)
			{
				Invalidate();
			}
			else
			{
				m_pManager->SendNotify(this, DUI_MSGTYPE_TIMER, event.wParam, event.lParam);
				return;
			}
		}
		else
			CControlUI::DoEvent(event);
	}	

	void CChrysanthemumLoadingUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if (_tcscmp(pstrName, _T("timeinterval")) == 0)
		{
			SetTimeInterval(_ttoi(pstrValue));
		}
		else if (_tcscmp(pstrName, _T("spokenum")) == 0)
		{
			SetNumberOfSpoke(_ttoi(pstrValue));
		}
		else if (_tcscmp(pstrName, _T("thickness")) == 0)
		{
			SetSpokeThickness(_ttoi(pstrValue));
		}
		else if (_tcscmp(pstrName, _T("outradius")) == 0)
		{
			SetOuterCircleRadius(_ttoi(pstrValue));
		}
		else if (_tcscmp(pstrName, _T("innerradius")) == 0)
		{
			SetInnerCircleRadius(_ttoi(pstrValue));
		}
		else if( _tcscmp(pstrName, _T("bkcolor")) == 0) 
		{
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBkColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("spokecolor")) == 0) 
		{
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSpokeColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("align")) == 0 ) 
		{
			if( _tcscmp(pstrValue, _T("left")) == 0 ) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_uTextStyle |= DT_LEFT;
			}
			else if( _tcscmp(pstrValue, _T("center")) == 0 ) {
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
				m_uTextStyle |= DT_CENTER;
			}
			else if( _tcscmp(pstrValue, _T("right")) == 0 ) {
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
				m_uTextStyle |= DT_RIGHT;
			}
		}
		else if (_tcscmp(pstrName, _T("valign")) == 0)
		{
			if (_tcscmp(pstrValue, _T("top")) == 0) {
				m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER);
				m_uTextStyle |= DT_TOP;
			}
			else if (_tcscmp(pstrValue, _T("vcenter")) == 0) {
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM);
				m_uTextStyle |= DT_VCENTER;
			}
			else if (_tcscmp(pstrValue, _T("bottom")) == 0) {
				m_uTextStyle &= ~(DT_TOP | DT_VCENTER);
				m_uTextStyle |= DT_BOTTOM;
			}
		}
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if(_tcscmp(pstrName, _T("rhaa")) == 0 ) SetTextRenderingAlias(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("multiline")) == 0 ) SetMultiLine(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetTextPadding(rcTextPadding);
		}
		else
		{
			__super::SetAttribute(pstrName, pstrValue);
		}
	}

	DWORD CChrysanthemumLoadingUI::GetBkColor() const
	{
		return m_dwBackColor;
	}
	void CChrysanthemumLoadingUI::SetBkColor(DWORD dwBackColor)
	{
		m_dwBackColor = dwBackColor;
	}
	DWORD CChrysanthemumLoadingUI::GetSpokeColor() const
	{
		return m_dwSpokeColor;
	}
	void CChrysanthemumLoadingUI::SetSpokeColor(DWORD dwSpokeColor)
	{
		if (m_dwSpokeColor == dwSpokeColor)
			return;
		m_dwSpokeColor = dwSpokeColor;
		InitChrysanthemumLoading();
	}

	void CChrysanthemumLoadingUI::SetTimeInterval(const int& nValue)
	{
		if (m_nTimeInterval==nValue)
			return;

		m_nTimeInterval = nValue;
		if (m_pManager)
		{
			m_pManager->KillTimer(this,EVENT_TIME_ID);
			m_pManager->SetTimer(this,EVENT_TIME_ID,m_nTimeInterval);
		}
	}
	int CChrysanthemumLoadingUI::GetTimeInterval()
	{
		return m_nTimeInterval;
	}
	void CChrysanthemumLoadingUI::SetNumberOfSpoke(const int& nValue)
	{
		if (m_nNumberOfSpoke == nValue)
			return;
		m_nNumberOfSpoke = nValue;
		InitChrysanthemumLoading();
	}
	int CChrysanthemumLoadingUI::GetNumberOfSpoke()
	{
		return m_nNumberOfSpoke;
	}
	void CChrysanthemumLoadingUI::SetSpokeThickness(const int& nValue)
	{
		m_nSpokeThickness = nValue;
	}
	int CChrysanthemumLoadingUI::GetSpokeThickness()
	{
		return m_nSpokeThickness;
	}
	void CChrysanthemumLoadingUI::SetOuterCircleRadius(const int& nValue)
	{
		m_nOuterCircleRadius = nValue;
	}
	int CChrysanthemumLoadingUI::GetOuterCircleRadius()
	{
		return m_nOuterCircleRadius;
	}
	void CChrysanthemumLoadingUI::SetInnerCircleRadius(const int& nValue)
	{
		m_nInnerCircleRadius = nValue;
	}
	int CChrysanthemumLoadingUI::GetInnerCircleRadius()
	{
		return m_nInnerCircleRadius;
	}

	void CChrysanthemumLoadingUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
	}

	DWORD CChrysanthemumLoadingUI::GetTextColor() const
	{
		return m_dwTextColor;
	}

	bool CChrysanthemumLoadingUI::IsMultiLine()
	{
		return (m_uTextStyle & DT_SINGLELINE) == 0;
	}

	void CChrysanthemumLoadingUI::SetMultiLine(bool bMultiLine)
	{
		if (bMultiLine)	{
			m_uTextStyle  &= ~DT_SINGLELINE;
			m_uTextStyle |= DT_WORDBREAK;
		}
		else 
			m_uTextStyle |= DT_SINGLELINE;
	}

	void CChrysanthemumLoadingUI::SetTextRenderingAlias(int nTextRenderingAlias)
	{
		m_TextRenderingAlias = (TextRenderingHint)nTextRenderingAlias;
	}

	TextRenderingHint CChrysanthemumLoadingUI::GetTextRenderingAlias()
	{
		return m_TextRenderingAlias;
	}

	void CChrysanthemumLoadingUI::SetFont(int index)
	{
		m_iFont = index;
	}

	int CChrysanthemumLoadingUI::GetFont() const
	{
		return m_iFont;
	}

	RECT CChrysanthemumLoadingUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}

	void CChrysanthemumLoadingUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
	}
}
