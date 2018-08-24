#include "stdafx.h"
#include "LoadingCircle.h"
#include <Gdiplus.h>

void DrawLine(Graphics* _objGraphics, PointF _objPointOne, PointF _objPointTwo, Color _objColor, int _intLineThickness)
{
	SolidBrush brush(_objColor);
	Pen objPen(&brush, _intLineThickness);
	{
		objPen.SetStartCap(LineCap::LineCapRound);
		objPen.SetEndCap(LineCap::LineCapRound);
		_objGraphics->DrawLine(&objPen, _objPointOne, _objPointTwo);
	}
}

PointF GetCoordinate(PointF _objCircleCenter, int _intRadius, double _dblAngle)
{
	double dblAngle = 3.14 * _dblAngle / 180;
	PointF pf(_objCircleCenter.X + _intRadius * (float)cos(dblAngle), _objCircleCenter.Y + _intRadius * (float)sin(dblAngle));
	return pf;
}

Color Darken(Color _objColor, int _intPercent)
{
	int intRed = _objColor.GetR();
	int intGreen = _objColor.GetG();
	int intBlue = _objColor.GetB();
	Color color(_intPercent, min(intRed, 255), min(intGreen, 255), min(intBlue, 255));
	return color;
}

double* GetSpokesAngles(int _intNumberSpoke)
{
	double* Angles = new double[_intNumberSpoke];
	double dblAngle = (double)360 / _intNumberSpoke;
	for (int shtCounter = 0; shtCounter < _intNumberSpoke; shtCounter++)
	{
		Angles[shtCounter] = (shtCounter == 0 ? dblAngle : Angles[shtCounter - 1] + dblAngle);
	}
	return Angles;
}

CLoadingCircle::CLoadingCircle()
	: m_pTrdAni(NULL)
	, m_bStop(false)
	, m_nTime(0)
	, m_nNumber(0)
{

}

CLoadingCircle::~CLoadingCircle()
{
	if (m_pTrdAni)
	{
		m_bStop = true;
		m_bExit = true;
		if (m_pTrdAni->joinable())
		{
			m_condQueue.notify_all();
			m_pTrdAni->join();
		}
		delete m_Angles;
		delete m_Colors;
		delete m_pTrdAni;
	}
}

LPCTSTR CLoadingCircle::GetClass() const
{
	return _T("AniUI");
}

LPVOID CLoadingCircle::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("Loading")) == 0) return static_cast<CLoadingCircle*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CLoadingCircle::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (wcscmp(pstrName, L"style") == 0)
	{
		LPTSTR pstr = NULL;
		m_nNumber = _tcstol(pstrValue, &pstr, 10);
	}
	else if (wcscmp(pstrName, L"time") == 0)
	{
		LPTSTR pstr = NULL;
		m_nTime = _tcstol(pstrValue, &pstr, 10);
	}
	else if (wcscmp(pstrName, L"spoke") == 0)
	{
		LPTSTR pstr = NULL;
		m_NumberOfSpoke = _tcstol(pstrValue, &pstr, 0);
	}
	else if (wcscmp(pstrName, L"thickness") == 0)
	{
		LPTSTR pstr = NULL;
		m_SpokeThickness = _tcstol(pstrValue, &pstr, 0);
	}
	else if (wcscmp(pstrName, L"outradius") == 0)
	{
		LPTSTR pstr = NULL;
		m_OuterCircleRadius = _tcstol(pstrValue, &pstr, 0);
	}
	else if (wcscmp(pstrName, L"innerradius") == 0)
	{
		LPTSTR pstr = NULL;
		m_InnerCircleRadius = _tcstol(pstrValue, &pstr, 0);
	}
	else if (wcscmp(pstrName, L"color") == 0)
	{
		while (*pstrValue > _T('\0') && *pstrValue <= _T(' ')) pstrValue = ::CharNext(pstrValue);
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		m_Color.SetValue(clrColor);
	}
	else
	{
		__super::SetAttribute(pstrName, pstrValue);
	}
}

void CLoadingCircle::PaintBkImage(HDC hDC)
{
	m_CenterPoint.X = this->GetWidth() / 2;
	m_CenterPoint.Y = this->GetHeight() / 2;
	if (m_NumberOfSpoke > 0)
	{
		Image* img = new Bitmap(this->GetWidth(), this->GetHeight());
		Graphics g(img);
		g.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);

		int intPosition = m_ProgressValue;
		for (int intCounter = 0; intCounter < m_NumberOfSpoke; intCounter++)
		{
			intPosition = intPosition % m_NumberOfSpoke;
			DrawLine(&g,
				GetCoordinate(m_CenterPoint, m_InnerCircleRadius, m_Angles[intPosition]),
				GetCoordinate(m_CenterPoint, m_OuterCircleRadius, m_Angles[intPosition]),
				m_Colors[intCounter], m_SpokeThickness);
			intPosition++;
		}
		// 获得窗口的Graphics对象
		Graphics gh(hDC);
		// 将描画好的CacheImage画到窗口上
		gh.DrawImage(img, m_rcItem.left, m_rcItem.top);
		delete img;
	}
}

Color* CLoadingCircle::GenerateColorsPallet(Color _objColor, bool _blnShadeColor, int _intNbSpoke)
{
	Color* objColors = new Color[m_NumberOfSpoke];
	byte bytIncrement = (byte)(255 / m_NumberOfSpoke);
	byte PERCENTAGE_OF_DARKEN = 0;
	for (int intCursor = 0; intCursor < m_NumberOfSpoke; intCursor++)
	{
		if (_blnShadeColor)
		{
			if (intCursor == 0 || intCursor < m_NumberOfSpoke - _intNbSpoke)
			{
				objColors[intCursor] = _objColor;
			}
			else
			{
				PERCENTAGE_OF_DARKEN += bytIncrement;
				if (PERCENTAGE_OF_DARKEN > 255)
				{
					PERCENTAGE_OF_DARKEN = 255;
				}
				objColors[intCursor] = Darken(_objColor, PERCENTAGE_OF_DARKEN);
			}
		}
		else
		{
			objColors[intCursor] = _objColor;
		}
	}
	return objColors;
}

void CLoadingCircle::Start()
{
	if (m_nTime > 0 && m_pTrdAni == NULL)
	{
		m_pTrdAni = new std::thread(std::bind(&CLoadingCircle::ThreadAni, this));
	}
	m_bStop = false;
	m_condQueue.notify_all();
}

void CLoadingCircle::Stop()
{
	m_bStop = true;
}

void CLoadingCircle::ThreadAni()
{
	std::unique_lock<std::mutex> lock(m_mutx);
	while (!m_bExit)
	{
		if (m_bStop)
		{
			m_condQueue.wait(lock);
		}
		if (m_bExit)
		{
			break;
		}
		m_ProgressValue = ++m_ProgressValue % m_NumberOfSpoke;
		Invalidate();
		std::this_thread::sleep_for(std::chrono::milliseconds(m_nTime));
	}
}

void CLoadingCircle::Init()
{
	m_Angles = GetSpokesAngles(m_NumberOfSpoke);
	m_Colors = GenerateColorsPallet(m_Color, !m_bStop, m_NumberOfSpoke);
}


CControlUI* CreateLoadingControl(LPCTSTR pstrType)
{
	if (wcscmp(pstrType, L"Loading") == 0)
	{
		return new CLoadingCircle();
	}

	return NULL;
}

