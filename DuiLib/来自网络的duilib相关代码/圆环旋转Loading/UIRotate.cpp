#include "StdAfx.h"
#include "UIRotate.h"

namespace DuiLib
{
	//DWORD WINAPI EnDrawThread(LPVOID lpParameter )
	//{
	//	DWORD dwResult;
	//	CRotateUI *pRotate = (CRotateUI*)lpParameter;
	//	while(1)
	//	{
	//		if(pRotate->m_fCurAngle > 359)
	//			pRotate->m_fCurAngle = 0;
	//		pRotate->m_fCurAngle += 36.0;
	//		pRotate->Invalidate();
	//		Sleep(100);
	//	}
	//	return 0;
	//}

	CRotateUI::CRotateUI()
		: m_fCurAngle(0.0f),
		m_pBkimage(NULL),
		m_gdiplusToken(0)
	{
		GdiplusStartup( &m_gdiplusToken,&m_gdiplusStartupInput, NULL);
	}

	CRotateUI::~CRotateUI()
	{
		if(m_pBkimage != NULL)
			delete m_pBkimage;
		try
		{
			GdiplusShutdown( m_gdiplusToken );
		}
		catch (...)
		{
			throw "CRotateUI::~CRotateUI";
		}
	}
/*
	LPCTSTR CRotateUI::GetClass() const
	{
		return _T("RotateUI");
	}

	LPVOID CRotateUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, _T("Rotate")) == 0 ) return static_cast<CRotateUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}*/

	void CRotateUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if( _tcscmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CRotateUI::SetBkImage( LPCTSTR pStrImage )
	{
		if (m_sBkImage==pStrImage)
			return;
		m_sBkImage=pStrImage;

		CDuiString cFullImagePath;
		cFullImagePath.Format(_T("%s\%s"),CPaintManagerUI::GetResourcePath().GetData(),m_sBkImage.GetData());
		m_pBkimage = new Gdiplus::Image(cFullImagePath.GetData());
		this->GetManager()->SetTimer(this,GIFCONTROL_TIMERID,100);
		//CloseHandle(CreateThread(NULL,0,EnDrawThread, this,  0,NULL));
		Invalidate();
	}

	void CRotateUI::PaintBkImage( HDC hDC )
	{
		RECT rcItem = m_rcItem;
		int iWidth = rcItem.right - rcItem.left;
		int iHeight = rcItem.bottom - rcItem.top;
		PointF centerPos(rcItem.left + iWidth/2, rcItem.top + iHeight/2);
		
		Graphics graphics(hDC);
		graphics.TranslateTransform(centerPos.X,centerPos.Y);
		graphics.RotateTransform(m_fCurAngle);
		graphics.TranslateTransform(-centerPos.X, -centerPos.Y);//还原源点
		graphics.DrawImage(m_pBkimage,rcItem.left,rcItem.top,iWidth,iHeight);
	}

	void CRotateUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == GIFCONTROL_TIMERID )
		{
			if(m_fCurAngle > 359)
				m_fCurAngle = 0;
			m_fCurAngle += 36.0;
			this->Invalidate();
		}
		else
			CControlUI::DoEvent(event);
	}

	void CRotateUI::SetVisible( bool bVisible /*= true */ )
	{
		if (m_bVisible==bVisible)
			return;
		m_bVisible=bVisible;
		if (m_bVisible==false)
			this->GetManager()->KillTimer(this,GIFCONTROL_TIMERID);
		else
			this->GetManager()->SetTimer(this,GIFCONTROL_TIMERID,100);
        NeedParentUpdate();
	}
}
