#ifndef __UIROTATE_H__
#define __UIROTATE_H__

#pragma once
#include <GdiPlus.h>
#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;
class Gdiplus::RectF;
struct Gdiplus::GdiplusStartupInput;

namespace DuiLib
{
	class CRotateUI : public CControlUI
	{
		enum
		{ 
			GIFCONTROL_TIMERID = 100,
		};
	public:
		CRotateUI();
		~CRotateUI();

		/*LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);*/
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetBkImage(LPCTSTR pStrImage);	
		virtual void DoEvent(TEventUI& event);
		virtual void PaintBkImage(HDC hDC);	
		virtual void SetVisible(bool bVisible = true );

	public:
		float m_fCurAngle;
		Gdiplus::Image*         m_pBkimage;
		ULONG_PTR				m_gdiplusToken;
		GdiplusStartupInput		m_gdiplusStartupInput;
	};
}

#endif // __UIROTATE_H__