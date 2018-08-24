#include "StdAfx.h"
#include "CircleProgressUI.h"
 
namespace DuiLib
{
	extern int GetEncoderClsid(const WCHAR* format, CLSID *pClsid);
 
	Color ARGB2Color(DWORD dwColor)
	{
		return Color(HIBYTE((dwColor)>>16), GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
	}
 
	CCircleProgressUI::CCircleProgressUI(void)
	: m_bCircular(FALSE)
	, m_bClockwise(TRUE)
	, m_dwCircleWidth(10.0)
	, m_dwFgColor()
	, m_dwBgColor()
	, m_bEnableCircleEffect(FALSE)
	, m_dwGradientColor1()
	, m_dwGradientColor2()
	, m_sIndicatorImage(L"")
	, m_pIndicator(NULL)
	{
 
	}
 
	CCircleProgressUI::~CCircleProgressUI(void)
	{
		if (m_pIndicator)
		{
			delete m_pIndicator;
		}
	}
 
	void CCircleProgressUI::SetCircular(BOOL bCircular /* = TRUE */)
	{
		m_bCircular = bCircular;
		Invalidate();
	}
 
	void CCircleProgressUI::SetClockwiseRotation(BOOL bClockwise)
	{
		if (bClockwise != m_bClockwise)
		{
			m_bClockwise = bClockwise;
			if (m_pIndicator)
			{
				//已经旋转了图片，旋转到相反的方向
				m_pIndicator->RotateFlip(Rotate180FlipNone);
			}
			
		}
	}
 
	void CCircleProgressUI::SetCircleWidth(DWORD dwCircleWidth)
	{
		m_dwCircleWidth = dwCircleWidth;
		Invalidate();
	}
 
	void CCircleProgressUI::SetBgColor(DWORD dwBgColor)
	{
		m_dwBgColor = dwBgColor;
		Invalidate();
	}
 
	DWORD CCircleProgressUI::GetBgColor() const
	{
		return m_dwBgColor;
	}
 
	void CCircleProgressUI::SetFgColor(DWORD dwFgColor)
	{
		m_dwFgColor = dwFgColor;
		Invalidate();
	}
 
	DWORD CCircleProgressUI::GetFgColor() const
	{
		return m_dwFgColor;
	}
 
	void CCircleProgressUI::SetIndicator(LPCTSTR lpIndicatorImage)
	{
		ASSERT(lpIndicatorImage);
		if (m_sIndicatorImage != lpIndicatorImage)
		{		
			m_sIndicatorImage = lpIndicatorImage;
			const TImageInfo* imgInfo = GetManager()->GetImageEx(m_sIndicatorImage);
			BITMAP bmp;
			GetObject(imgInfo->hBitmap, sizeof(BITMAP), &bmp);
 
			m_pIndicator = new Bitmap(imgInfo->nX, imgInfo->nY, imgInfo->nX * 4, PixelFormat32bppARGB, (BYTE*)bmp.bmBits);
			Status state = m_pIndicator->GetLastStatus();
			if (Ok == state)
			{
				// 假定图片指向上
				m_pIndicator->RotateFlip(m_bClockwise ? Rotate90FlipNone : Rotate270FlipNone);
				Invalidate();
			}		
		}
	}
 
	void CCircleProgressUI::SetEnableCircleEffect(BOOL bEnableCircleEffect)
	{
		m_bEnableCircleEffect = bEnableCircleEffect;
	}
 
	void CCircleProgressUI::SetCircleGradientColor1(DWORD dwColor)
	{
		m_dwGradientColor1 = dwColor;
		Invalidate();
	}
 
	void CCircleProgressUI::SetCircleGradientColor2(DWORD dwColor)
	{
		m_dwGradientColor2 = dwColor;
		Invalidate();
	}
 
	void CCircleProgressUI::PaintBkColor(HDC hDC)
	{
		CProgressUI::PaintBkColor(hDC);
		if (m_bCircular)
		{
			if( m_nMax <= m_nMin ) m_nMax = m_nMin + 1;
			if( m_nValue > m_nMax ) m_nValue = m_nMax;
			if( m_nValue < m_nMin ) m_nValue = m_nMin;
 
			int direction = m_bClockwise ? 1 : -1;
			Gdiplus::REAL bordersize = 1.0;
			Graphics graphics( hDC );
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
 
			// 圆形中心
			PointF center;
			center.X = m_rcItem.left + (m_rcItem.right - m_rcItem.left) / 2;
			center.Y = m_rcItem.top + (m_rcItem.bottom - m_rcItem.top) / 2;
 
			// 控件矩形内的最大正方形的边界
			int side = min(m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top);
			RectF rcBorder;
			rcBorder.X = center.X - side / 2;
			rcBorder.Y = center.Y - side / 2;
			rcBorder.Width = side;
			rcBorder.Height = side;
 
			// 进度弧形的边界
			Gdiplus::RectF outer = rcBorder;
			if (m_pIndicator)
			{
				outer.Inflate( -1.0F * m_pIndicator->GetWidth() / 2, -1.0F * m_pIndicator->GetWidth() / 2);
			}
			else
			{
				outer.Inflate( -0.5 * m_dwCircleWidth - m_nBorderSize, -0.5 * m_dwCircleWidth - m_nBorderSize );		
			}
 
			outer.Inflate( -1.0F, -1.0F );
 
			Pen borderPen(Color::White, bordersize);
			Pen bgPen(m_dwBgColor, m_dwCircleWidth);			
			
			//graphics.DrawEllipse(&borderPen, outer);		
			
			RectF rcArk;
			rcArk.X = outer.X + outer.Width / 2 - m_dwCircleWidth / 2;
			rcArk.Y = outer.Y - m_dwCircleWidth / 2;
			rcArk.Width = rcArk.Height = m_dwCircleWidth;
 
			if (!m_bEnableCircleEffect)
			{
				Pen fgPen(m_dwFgColor, m_dwCircleWidth);
				graphics.DrawArc(&bgPen, outer, 270, 360);
				graphics.DrawArc(&fgPen, outer, 270, direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin));
			}
			else
			{
				REAL factors[4]={ 0.0f, 0.9f, 0.0f };  
				REAL position[4]={ 0.0f, 0.5f, 1.0f };  
 
				LinearGradientBrush lgbrush(rcBorder, ARGB2Color(m_dwGradientColor1), ARGB2Color(m_dwGradientColor2), LinearGradientModeVertical);  
				lgbrush.SetBlend(factors, position, 3); 
				//graphics.FillRectangle(&lgbrush, rcBorder); 
				graphics.DrawArc(&bgPen, outer, 270, 360);
 
				Pen fgPen(&lgbrush, m_dwCircleWidth);
				graphics.DrawArc(&fgPen, outer, 270, direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin));
			}	
 
			Matrix matrix;
			matrix.RotateAt(direction * 360 * (m_nValue - m_nMin) / (m_nMax - m_nMin), center, MatrixOrderAppend);
			graphics.SetTransform(&matrix);			
 
			if (m_pIndicator)
			{
				RectF rectf;
				rectf.X = center.X - m_pIndicator->GetWidth() / 2;
				rectf.Y = outer.Y + bordersize / 2 - m_pIndicator->GetHeight() / 2;
				rectf.Width = m_pIndicator->GetWidth();
				rectf.Height = m_pIndicator->GetHeight();
				graphics.DrawImage(m_pIndicator, rectf);
			}
		}
	}
 
	void CCircleProgressUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("circular")) == 0 ) {
			SetCircular(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("clockwise")) == 0 ) {
			SetClockwiseRotation(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("circlewidth")) == 0 ) SetCircleWidth(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("bgcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBgColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("fgcolor")) == 0 )	{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFgColor(clrColor);
		}
		else if (_tcscmp(pstrName, _T("indicator")) == 0)
		{
			SetIndicator(pstrValue);
		}
		else if( _tcscmp(pstrName, _T("enablecircleeffect")) == 0 ) {
			SetEnableCircleEffect(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("gradientcolor1")) == 0 )	{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetCircleGradientColor1(clrColor);
		}
		else if( _tcscmp(pstrName, _T("gradientcolor2")) == 0 )	{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetCircleGradientColor2(clrColor);
		}
		else CProgressUI::SetAttribute(pstrName, pstrValue);
	}
 
}		// namespace Duilib
