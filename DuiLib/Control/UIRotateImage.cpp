#include "StdAfx.h"
#include "UIRotateImage.h"

namespace DuiLib
{
	CRotateImageUI::CRotateImageUI()
		: m_nTimeInterval(40),
		m_nAngle(1),
		m_nCurAngle(0),
		m_pBkimage(NULL),
		m_pStream(NULL),
		m_sBkImage(_T("")),
		m_bStretch(false),
		m_bIsAutoStart(true),
		m_nFixAngle(0),
		m_bIsPlaying(false)
	{
	}

	CRotateImageUI::~CRotateImageUI()
	{
		if(m_pBkimage)
			delete m_pBkimage;

		if (m_pStream != NULL )
		{
			m_pStream->Release();
			m_pStream = NULL;
		}
		
		if (m_pManager)
		{
			m_pManager->KillTimer(this);
			m_bIsPlaying = false;
		}
	}

	LPCTSTR CRotateImageUI::GetClass() const
	{
		return DUI_CTR_ROTATEIMAGE;
	}

	LPVOID CRotateImageUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcscmp(pstrName, DUI_CTR_ROTATEIMAGE) == 0 ) return static_cast<CRotateImageUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CRotateImageUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if( _tcscmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
		else if( _tcscmp(pstrName, _T("timeinterval")) == 0 ) SetTimeInterval(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("eachtimeangle")) == 0 ) SetEachTimeAngle(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("stretch")) == 0 ) SetStretch(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("autostart")) == 0 ) SetAutoStart(_tcscmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("fixangle")) == 0 ) SetFixAngle(_ttoi(pstrValue));
		else 
			CControlUI::SetAttribute(pstrName, pstrValue);
	}

	Gdiplus::Image* CRotateImageUI::LoadImg(LPCTSTR pstrImgPath)
	{
		LPBYTE pData = NULL;
		DWORD dwSize = 0;
		do 
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if( CPaintManagerUI::GetResourceZip().IsEmpty() ) {
				sFile += pstrImgPath;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if( hFile == INVALID_HANDLE_VALUE ) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if( dwSize == 0 ) 
				{
					::CloseHandle( hFile );
					break;
				}

				DWORD dwRead = 0;
				pData = new BYTE[ dwSize+1 ];
				pData[dwSize] = '\0';
				::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
				::CloseHandle( hFile );

				if( dwRead != dwSize ) {
					delete[] pData;
					pData = NULL;
					break;
				}
			}
			else {
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if( CPaintManagerUI::IsCachedResourceZip() ) 
					hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else
					hz = OpenZip(sFile.GetData(),CPaintManagerUI::GetResourceZipPassword());
				if( hz == NULL ) 
					break;
				ZIPENTRY ze; 
				int i; 
				if( FindZipItem(hz, pstrImgPath, true, &i, &ze) != 0 ) 
					break;
				dwSize = ze.unc_size;
				if( dwSize == 0 ) 
					break;
				pData = new BYTE[ dwSize ];
				int res = UnzipItem(hz, i, pData, dwSize);
				if( res != 0x00000000 && res != 0x00000600) 
				{
					delete[] pData;
					pData = NULL;
					if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
					break;
				}
				if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
			}

		} while (0);

		while (!pData)
		{
			//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
			HANDLE hFile = ::CreateFile(pstrImgPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
				FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile == INVALID_HANDLE_VALUE ) break;
			dwSize = ::GetFileSize(hFile, NULL);
			if( dwSize == 0 ) 
			{
				CloseHandle(hFile);
				break;
			}

			DWORD dwRead = 0;
			pData = new BYTE[ dwSize+1 ];
			pData[dwSize] = '\0';
			::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
			::CloseHandle( hFile );

			if( dwRead != dwSize ) {
				delete[] pData;
				pData = NULL;
			}
			break;
		}
		if (!pData)
			return NULL;

		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
		BYTE* pMem = (BYTE*)::GlobalLock(hMem);
		memcpy(pMem, pData, dwSize);
		::GlobalUnlock(hMem);
		::CreateStreamOnHGlobal(hMem, TRUE, &m_pStream);
		Gdiplus::Image *pImg = Gdiplus::Image::FromStream(m_pStream);
		if(!pImg || pImg->GetLastStatus() != Gdiplus::Ok)
		{
			m_pStream->Release();
			m_pStream = NULL;

			if (pData)
			{
				delete[] pData;
				pData = NULL;
			}
			return NULL;
		}

		delete[] pData;

		return pImg;
	}

	void CRotateImageUI::InitInfo()
	{
		if (m_sBkImage.IsEmpty())
			return;

		if (m_pBkimage)
		{
			delete m_pBkimage;
			m_pBkimage = NULL;
		}
		if (m_pStream)
		{
			m_pStream->Release();
			m_pStream = NULL;
		}

		m_pBkimage = LoadImg(m_sBkImage.GetData());
	}

	void CRotateImageUI::SetBkImage(const CDuiString& strImage)
	{
		if (m_sBkImage==strImage)
			return;

		if (m_pBkimage)
		{
			delete m_pBkimage;
			m_pBkimage = NULL;
		}

		m_sBkImage = strImage;

		InitInfo();

		Invalidate();
	}
	void CRotateImageUI::SetTimeInterval(const int& nValue)
	{
		if (nValue<1 || m_nTimeInterval==nValue)
			return;

		m_nTimeInterval = nValue;
		if (m_bIsPlaying && m_pManager)
		{
			m_pManager->KillTimer(this,EVENT_TIME_ID);
			m_pManager->SetTimer(this,EVENT_TIME_ID,m_nTimeInterval);
		}
	}
	int CRotateImageUI::GetTimeInterval()
	{
		return m_nTimeInterval;
	}
	void CRotateImageUI::SetEachTimeAngle(const int& nValue)
	{
		if (nValue<1 || m_nAngle == nValue)
			return;

		m_nAngle = nValue;
	}
	int CRotateImageUI::GetEachTimeAngle()
	{
		return m_nAngle;
	}
	void CRotateImageUI::SetStretch(const bool& bStretch)
	{
		m_bStretch = bStretch;
	}
	bool CRotateImageUI::GetStretch()
	{
		return m_bStretch;
	}
	void CRotateImageUI::SetFixAngle(const int& nValue)
	{
		if (nValue<0 || m_nFixAngle==nValue)
			return;

		m_nFixAngle = nValue;
		if (m_bIsPlaying == false)
		{
			m_nCurAngle = m_nFixAngle;
			Invalidate();
		}
	}
	int CRotateImageUI::GetFixAngle()
	{
		return m_nFixAngle;
	}

	void CRotateImageUI::SetAutoStart(const bool& bAutoStart)
	{
		if (m_bIsAutoStart==bAutoStart)
			return;
		m_bIsAutoStart = bAutoStart;
	}
	bool CRotateImageUI::GetAutoStart()
	{
		return m_bIsAutoStart;
	}
	void CRotateImageUI::Stop()
	{
		if (m_bIsPlaying && m_pManager)
		{
			m_pManager->KillTimer(this,EVENT_TIME_ID);	
			m_bIsPlaying = false;
		}

		if (m_nCurAngle != m_nFixAngle)
		{
			m_nCurAngle = m_nFixAngle;
			Invalidate();
		}
	}
	void CRotateImageUI::Start()
	{
		if (GetTimeInterval()<1 || GetEachTimeAngle()<1 || m_bIsPlaying)
			return;

		if (m_pManager)
		{
			m_pManager->SetTimer(this,EVENT_TIME_ID,m_nTimeInterval);
			m_bIsPlaying = true;
		}
	}
	void CRotateImageUI::Pause()
	{
		if (m_bIsPlaying == false)
			return;

		if (m_pManager)
		{
			m_pManager->KillTimer(this,EVENT_TIME_ID);	
			m_bIsPlaying = false;
		}
	}

	void CRotateImageUI::PaintBkImage( HDC hDC )
	{
		if (m_pBkimage==NULL)
			return;

		if (m_bStretch)
		{
			Graphics graphics(hDC);
			graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
			//graphics.SetCompositingQuality(CompositingQualityHighQuality);
			//graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			//graphics.SetInterpolationMode(InterpolationModeHighQuality);

			graphics.TranslateTransform(m_centerPos.X,m_centerPos.Y);
			graphics.RotateTransform(m_nCurAngle);
			graphics.TranslateTransform(-m_centerPos.X, -m_centerPos.Y);//还原源点

			graphics.DrawImage(m_pBkimage,m_rcItem.left,m_rcItem.top,m_rcItem.right - m_rcItem.left,m_rcItem.bottom - m_rcItem.top);
		}
		else
		{
			float imgWidth = m_pBkimage->GetWidth();
			float imgHeight = m_pBkimage->GetHeight();

			Graphics graphics(hDC);
			graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
			//graphics.SetCompositingQuality(CompositingQualityHighQuality);
			//graphics.SetSmoothingMode(SmoothingMode::SmoothingModeHighQuality);
			//graphics.SetInterpolationMode(InterpolationModeHighQuality);

			graphics.TranslateTransform(m_centerPos.X,m_centerPos.Y);
			graphics.RotateTransform(m_nCurAngle);
			graphics.TranslateTransform(-m_centerPos.X, -m_centerPos.Y);//还原源点

			graphics.DrawImage(m_pBkimage,m_centerPos.X-imgWidth/2.0,m_centerPos.Y-imgHeight/2.0,imgWidth,imgHeight);
		}		
	}

	void CRotateImageUI::DoInit()
	{
		InitInfo();

		m_nCurAngle = m_nFixAngle;
		if (m_bIsAutoStart)
			Start();
	}

	void CRotateImageUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER)
		{
			if (event.wParam == EVENT_TIME_ID)
			{
				m_nCurAngle += m_nAngle;
				m_nCurAngle = m_nCurAngle % 360;		
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

	void CRotateImageUI::SetVisible(bool bVisible/* = true*/ )
	{
		//if( IsVisible() == bVisible ) 
		//	return;

		if (bVisible)
		{
			Start();
		}
		else
		{
			Pause();
		}

		__super::SetVisible(bVisible);
	}

	void CRotateImageUI::SetPos(RECT rc, bool bNeedInvalidate/* = true*/)
	{
		__super::SetPos(rc,bNeedInvalidate);

		int nW = m_rcItem.right-m_rcItem.left;
		int nH = m_rcItem.bottom-m_rcItem.top;
		if (nW>0 && nH>0)
		{
			m_centerPos.X = m_rcItem.left + nW/2.0;
			m_centerPos.Y = m_rcItem.top + nH/2.0;
		}
	}
}
