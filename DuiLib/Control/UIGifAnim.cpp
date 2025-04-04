#include "StdAfx.h"
#include "UIGifAnim.h"

namespace DuiLib
{
#define RES_TYPE_COLOR _T("*COLOR*")

	extern Color ARGB2Color(DWORD dwColor);

	CGifAnimUI::CGifAnimUI(void)
	{
		m_nFramePosition	=	0;	
		m_bIsAutoPlay		=	true;
		m_bIsAutoSize		=	false;
		m_bIsPlaying		=	false;

		m_nLoopCout = 0;
		m_nLoopedNum = 0;
	}


	CGifAnimUI::~CGifAnimUI(void)
	{
		for (int i=0;i<m_Frames.GetSize();i++)
		{
			Frame* pFrame = (Frame*)m_Frames[i];
			delete pFrame;
		}
		if (m_pManager)
			m_pManager->KillTimer( this, EVENT_TIME_ID );
	}

	LPCTSTR CGifAnimUI::GetClass() const
	{
		return DUI_CTR_GIFANIM;
	}

	LPVOID CGifAnimUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_GIFANIM) == 0 ) return static_cast<CGifAnimUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CGifAnimUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == EVENT_TIME_ID)
			OnTimer( (UINT_PTR)event.wParam );
		else
			CLabelUI::DoEvent(event);
	}

	void CGifAnimUI::SetVisible(bool bVisible /* = true */)
	{
		CLabelUI::SetVisible(bVisible);
		if (bVisible && m_bIsAutoPlay)
			Play();
		else
			Stop();
	}

	void CGifAnimUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("autoplay")) == 0 ) {
			SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("autosize")) == 0 ) {
			SetAutoSize(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else
			CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CGifAnimUI::SetBkImage(const CDuiString& strImage)
	{
		if( m_sBkImage == strImage) return;
		m_sBkImage = strImage;

		Stop();
		//清理
		if ( m_Frames.IsEmpty())
		{
			if( m_diBk.pImageInfo != NULL ) 
				m_diBk.Clear();
		}
		else
		{
			for (int i=0;i<m_Frames.GetSize();i++)
			{
				Frame* pFrame = (Frame*)m_Frames[i];
				delete pFrame;
			}
			m_Frames.Empty();
			m_nFramePosition	=	0;	
		}
		//重新载入
		if (LoadFromFile(strImage.GetData()))
		{
			if (m_bIsAutoSize)
			{
				SetFixedWidth(m_nFrameWidth);
				SetFixedHeight(m_nFrameHeight);
			}

			if (m_bIsAutoPlay && m_Frames.GetSize() > 0)
			{
				Play();
			}
		}
		else
		{
			CLabelUI::SetBkImage(strImage);
		}		
	}

	void CGifAnimUI::SetAutoPlay(bool bIsAuto)
	{
		if (m_bIsAutoPlay != bIsAuto)
		{
			m_bIsAutoPlay = bIsAuto;
			if (m_bIsAutoPlay)
				Play();
			else
				Stop();
		}
	}

	bool CGifAnimUI::IsAutoPlay() const
	{
		return m_bIsAutoPlay;
	}

	void CGifAnimUI::SetAutoSize(bool bIsAuto)
	{
		m_bIsAutoSize = bIsAuto;
		if (m_Frames.IsEmpty() == false)
		{
			if (m_bIsAutoSize)
			{
				SetFixedWidth(m_nFrameWidth);
				SetFixedHeight(m_nFrameHeight);
			}
		}
	}

	bool CGifAnimUI::IsAutoSize() const
	{
		return m_bIsAutoSize;
	}

	void CGifAnimUI::Play()
	{
		if (m_bIsPlaying || m_Frames.IsEmpty())
			return;

		m_nLoopedNum = 0;

		long lPause = ((Frame*)m_Frames[m_nFramePosition])->nDelayMs;
		if ( lPause == 0 ) lPause = 100;
		m_pManager->SetTimer( this, EVENT_TIME_ID, lPause );

		m_bIsPlaying = true;
		Invalidate();
	}

	void CGifAnimUI::Pause()
	{
		if (!m_bIsPlaying || m_Frames.IsEmpty())
			return;

		m_pManager->KillTimer(this, EVENT_TIME_ID);
		this->Invalidate();
		m_bIsPlaying = false;
	}

	void CGifAnimUI::Stop()
	{
		if (!m_bIsPlaying)
			return;

		m_pManager->KillTimer(this, EVENT_TIME_ID);
		m_nFramePosition = 0;
		this->Invalidate();
		m_bIsPlaying = false;
		m_nLoopedNum = 0;
	}

	void CGifAnimUI::OnTimer( UINT_PTR idEvent )
	{
		m_pManager->KillTimer( this, EVENT_TIME_ID );
		this->Invalidate();

		if (m_nLoopCout>0)
		{
			if (m_nFramePosition==0)
				m_nLoopedNum++;

			if (m_nLoopedNum <= m_nLoopCout)
			{
				if (m_Frames.GetSize() > 0)
					m_nFramePosition = (++m_nFramePosition) % m_Frames.GetSize();

				long lPause = ((Frame*)m_Frames[m_nFramePosition])->nDelayMs;
				if ( lPause == 0 ) lPause = 100;
				m_pManager->SetTimer( this, EVENT_TIME_ID, lPause );
			}
		}
		else
		{
			if (m_Frames.GetSize() > 0)
				m_nFramePosition = (++m_nFramePosition) % m_Frames.GetSize();

			long lPause = ((Frame*)m_Frames[m_nFramePosition])->nDelayMs;
			if ( lPause == 0 ) lPause = 100;
			m_pManager->SetTimer( this, EVENT_TIME_ID, lPause );
		}
	}

	void CGifAnimUI::PaintBkImage(HDC hDC)
	{
		//绘制当前帧
		if (m_Frames.IsEmpty())
		{
			CLabelUI::PaintBkImage(hDC);
			return;
		}
		else
		{
			Frame* pFrame = (Frame*)m_Frames[m_nFramePosition];

			RECT rcDest = m_rcItem;
			if( m_diBk.rcDestOffset.left != 0 || m_diBk.rcDestOffset.top != 0 ||
				m_diBk.rcDestOffset.right != 0 || m_diBk.rcDestOffset.bottom != 0 ) {
					rcDest.left = m_rcItem.left + m_diBk.rcDestOffset.left;
					rcDest.top = m_rcItem.top + m_diBk.rcDestOffset.top;
					rcDest.right = m_rcItem.left + m_diBk.rcDestOffset.right;
					if( rcDest.right > m_rcItem.right ) rcDest.right = m_rcItem.right;
					rcDest.bottom = m_rcItem.top + m_diBk.rcDestOffset.bottom;
					if( rcDest.bottom > m_rcItem.bottom ) rcDest.bottom = m_rcItem.bottom;
			}

			CRenderEngine::DrawImage(hDC,pFrame->hBmpFrame,rcDest,m_rcPaint,m_diBk.rcBmpPart,m_diBk.rcScale9,true, m_diBk.uFade, m_diBk.bHole, m_diBk.bTiledX, m_diBk.bTiledY);
		}
	}

	bool CGifAnimUI::LoadFromFile(LPCTSTR pstrGifPath)
	{
		if ( (pstrGifPath==NULL) || (_tcslen(pstrGifPath)==0) )
			return false;

		//填充TDrawInfo
		m_diBk.sDrawString = pstrGifPath;
		m_diBk.sImageName = m_diBk.sDrawString;
		DWORD dwMask = 0;

		bool bUseRes = false;
		CDuiString sImageName = m_diBk.sDrawString;
		CDuiString sImageResType;

		CDuiString sItem;
		CDuiString sValue;
		LPTSTR pstr = NULL;
		LPCTSTR pstrImage = m_diBk.sDrawString.GetData();
		while( *pstrImage != _T('\0') ) {
			sItem.Empty();
			sValue.Empty();
			while( *pstrImage > _T('\0') && *pstrImage <= _T(' ') ) pstrImage = ::CharNext(pstrImage);
			while( *pstrImage != _T('\0') && *pstrImage != _T('=') && *pstrImage > _T(' ') ) {
				LPTSTR pstrTemp = ::CharNext(pstrImage);
				while( pstrImage < pstrTemp) {
					sItem += *pstrImage++;
				}
			}
			while( *pstrImage > _T('\0') && *pstrImage <= _T(' ') ) pstrImage = ::CharNext(pstrImage);
			if( *pstrImage++ != _T('=') ) break;
			while( *pstrImage > _T('\0') && *pstrImage <= _T(' ') ) pstrImage = ::CharNext(pstrImage);
			if( *pstrImage++ != _T('\'') ) break;
			while( *pstrImage != _T('\0') && *pstrImage != _T('\'') ) {
				LPTSTR pstrTemp = ::CharNext(pstrImage);
				while( pstrImage < pstrTemp) {
					sValue += *pstrImage++;
				}
			}
			if( *pstrImage++ != _T('\'') ) break;
			if( !sValue.IsEmpty() ) {
				if( sItem == _T("file") ) {
					m_diBk.sImageName = sValue;
				}
				else if (sItem == _T("res")) {
					bUseRes = true;
					sImageName = sValue;
				}
				else if (sItem == _T("restype")) {
					sImageResType = sValue;
				}
				else if (sItem == _T("color")) {
					bUseRes = true;
					sImageResType = RES_TYPE_COLOR;
					sImageName = sValue;
				}
				else if( sItem == _T("dest") ) {
					m_diBk.rcDestOffset.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
					m_diBk.rcDestOffset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
					m_diBk.rcDestOffset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
					m_diBk.rcDestOffset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
				}
				else if( sItem == _T("source") ) {
					m_diBk.rcBmpPart.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
					m_diBk.rcBmpPart.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
					m_diBk.rcBmpPart.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
					m_diBk.rcBmpPart.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);  
				}
				else if( sItem == _T("corner") || sItem == _T("scale9")) {
					m_diBk.rcScale9.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
					m_diBk.rcScale9.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
					m_diBk.rcScale9.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
					m_diBk.rcScale9.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
				}
				else if( sItem == _T("mask") ) {
					if( sValue[0] == _T('#')) dwMask = _tcstoul(sValue.GetData() + 1, &pstr, 16);
					else dwMask = _tcstoul(sValue.GetData(), &pstr, 16);
				}
				else if( sItem == _T("fade") ) {
					m_diBk.uFade = (BYTE)_tcstoul(sValue.GetData(), &pstr, 10);
				}
				else if( sItem == _T("hole") ) {
					m_diBk.bHole = (_tcscmp(sValue.GetData(), _T("true")) == 0);
				}
				else if( sItem == _T("xtiled") ) {
					m_diBk.bTiledX = (_tcscmp(sValue.GetData(), _T("true")) == 0);
				}
				else if( sItem == _T("ytiled") ) {
					m_diBk.bTiledY = (_tcscmp(sValue.GetData(), _T("true")) == 0);
				}
			}
			if( *pstrImage++ != _T(' ') ) break;
		}

		LPBYTE pData = NULL;
		DWORD dwSize = 0;

		do 
		{
			if (bUseRes) {
				if (_tcsicmp(sImageResType.GetData(), RES_TYPE_COLOR) == 0) {
					pData = (PBYTE)0x1;  /* dummy pointer */
				}else {
					HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(_ttoi(sImageName.GetData())), sImageResType.GetData());
					if (hResource == NULL) break;
					HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
					if (hGlobal == NULL) {
						FreeResource(hResource);
						break;
					}

					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if (dwSize == 0) break;
					pData = new BYTE[dwSize+1];
					memset(pData, 0, dwSize + 1);
					::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
					::FreeResource(hResource);
				}
			}
			else
			{
				CDuiString sFile = CPaintManagerUI::GetResourcePath();
				if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
					sFile += m_diBk.sImageName;
					HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
						FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile == INVALID_HANDLE_VALUE) break;
					dwSize = ::GetFileSize(hFile, NULL);
					if (dwSize == 0)
					{
						::CloseHandle(hFile);
						break;
					}

					DWORD dwRead = 0;
					pData = new BYTE[dwSize + 1];
					pData[dwSize] = '\0';
					::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
					::CloseHandle(hFile);

					if (dwRead != dwSize) {
						delete[] pData;
						pData = NULL;
						break;
					}
				}
				else {
					sFile += CPaintManagerUI::GetResourceZip();
					HZIP hz = NULL;
					if (CPaintManagerUI::IsCachedResourceZip())
						hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
					else
						hz = OpenZip(sFile.GetData(), CPaintManagerUI::GetResourceZipPassword());
					if (hz == NULL)
						break;
					ZIPENTRY ze;
					int i;
					if (FindZipItem(hz, m_diBk.sImageName.GetData(), true, &i, &ze) != 0)
						break;
					dwSize = ze.unc_size;
					if (dwSize == 0)
						break;
					pData = new BYTE[dwSize];
					int res = UnzipItem(hz, i, pData, dwSize);
					if (res != 0x00000000 && res != 0x00000600)
					{
						delete[] pData;
						pData = NULL;
						if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
						break;
					}
					if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
				}
			}			

		} while (0);

		while (!pData)
		{
			//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
			HANDLE hFile = ::CreateFile(m_diBk.sImageName.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
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
		{
			return false;
		}

		bool bRet = LoadFromMemory(pData, dwSize,dwMask);
		delete[] pData;

		if( m_diBk.rcBmpPart.left == 0 && m_diBk.rcBmpPart.right == 0 && 
			m_diBk.rcBmpPart.top == 0 && m_diBk.rcBmpPart.bottom == 0 ) {
				m_diBk.rcBmpPart.right = m_nFrameWidth;
				m_diBk.rcBmpPart.bottom = m_nFrameHeight;
		}

		return bRet;
	}

	bool CGifAnimUI::LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask )
	{
		int* delays = NULL;
		int nFrameCount = 0;
		unsigned char* pData = CRenderEngine::ParseGifFromMemory((unsigned char*)pBuf,dwSize,&delays,&m_nFrameWidth,&m_nFrameHeight,&nFrameCount,&m_nLoopCout);
		if (pData)
		{
			BITMAPINFO bmi;
			::ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = m_nFrameWidth;
			bmi.bmiHeader.biHeight = -m_nFrameHeight;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = (m_nFrameWidth*32+31)/32*4*m_nFrameHeight;

			for (int nFram=0;nFram<nFrameCount;nFram++)
			{
				LPBYTE pDest = NULL;
				HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
				if( !hBitmap ) {
					CRenderEngine::FreeGifFromMemory(pData);
					return false;
				}

				Frame* pframe = new Frame;
				pframe->nFrameId = nFram;
				pframe->nDelayMs = delays[nFram];			

				LPBYTE pImage = pData+m_nFrameWidth*m_nFrameHeight*nFram*4;
				for( int i = 0; i < m_nFrameWidth * m_nFrameHeight; i++ ) 
				{
					pDest[i*4 + 3] = pImage[i*4 + 3];		
					if( pDest[i*4 + 3] < 255 )
					{
						pDest[i*4] = (BYTE)(DWORD(pImage[i*4 + 2])*pImage[i*4 + 3]/255);
						pDest[i*4 + 1] = (BYTE)(DWORD(pImage[i*4 + 1])*pImage[i*4 + 3]/255);
						pDest[i*4 + 2] = (BYTE)(DWORD(pImage[i*4])*pImage[i*4 + 3]/255); 
					}
					else
					{
						pDest[i*4] = pImage[i*4 + 2];
						pDest[i*4 + 1] = pImage[i*4 + 1];
						pDest[i*4 + 2] = pImage[i*4]; 
					}

					if( *(DWORD*)(&pDest[i*4]) == dwMask ) {
						pDest[i*4] = (BYTE)0;
						pDest[i*4 + 1] = (BYTE)0;
						pDest[i*4 + 2] = (BYTE)0; 
						pDest[i*4 + 3] = (BYTE)0;					
					}
				}

				pframe->hBmpFrame = hBitmap;
				m_Frames.Add(pframe);
			}

			CRenderEngine::FreeGifFromMemory(pData);
			return true;
		}

		return false;
	}
}