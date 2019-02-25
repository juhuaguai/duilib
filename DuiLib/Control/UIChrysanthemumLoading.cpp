#include "StdAfx.h"
#include "UIChrysanthemumLoading.h"
#include <math.h>

namespace DuiLib
{
	CChrysanthemumLoadingUI::CChrysanthemumLoadingUI(void)	: 
		m_nTimeInterval(40), 
		m_nNumberOfSpoke(10),
		m_nSpokeThickness(4),
		m_nProgressValue(0),
		m_nOuterCircleRadius(0),
		m_nInnerCircleRadius(0),
		m_pColors(NULL),
		m_pAngles(NULL),
		m_CenterPoint(0,0)
	{
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
				m_pColors[intCursor] = Color(m_dwBackColor);
			}
			else
			{
				PERCENTAGE_OF_DARKEN += bytIncrement;
				if (PERCENTAGE_OF_DARKEN > 255)
					PERCENTAGE_OF_DARKEN = 255;
				m_pColors[intCursor] = Color(PERCENTAGE_OF_DARKEN,GetRValue(m_dwBackColor),GetGValue(m_dwBackColor),GetBValue(m_dwBackColor));
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
		else
		{
			__super::SetAttribute(pstrName, pstrValue);
		}
	}

	void CChrysanthemumLoadingUI::SetBkColor(DWORD dwBackColor)
	{
		if (m_dwBackColor == dwBackColor)
			return;
		m_dwBackColor = dwBackColor;
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
		if (m_nSpokeThickness == nValue)
			return;
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
}
