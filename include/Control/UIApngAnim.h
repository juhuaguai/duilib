#ifndef ApngAnimUI_h__
#define ApngAnimUI_h__

#ifdef SUPPORT_APNG

#pragma once

//修改为在 项目属性>链接器>输入>附加依赖项 中设置，这样可以将libpng.lib合并到duilib.lib中,外部仅链接duilib.lib即可使用
//#pragma comment(lib,"../apng/static/libpng.lib")

namespace DuiLib
{
#define EVENT_TIME_ID	100

	typedef struct tagApngFrame
	{
		tagApngFrame()
		{
			nFrameID=0;
			pframeBits = NULL;
			nDelayMS=0;
		};
		~tagApngFrame()
		{
			if (pframeBits)
			{
				free(pframeBits);
				pframeBits = NULL;
			}
		}
		void mallocByte(DWORD dwSize)
		{
			pframeBits = (BYTE*)malloc(dwSize);
		};
		void freeByte()
		{
			if (pframeBits)
			{
				free(pframeBits);
				pframeBits = NULL;
			}
		};

		UINT32 nFrameID;            //id
		BYTE* pframeBits;				//rgb
		UINT32 nDelayMS;            //
	}ApngFrame;

	class DUILIB_API CApngAnimUI : public CControlUI
	{
	public:
		CApngAnimUI(void);
		~CApngAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(const CDuiString& strImage);
		CDuiString GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	Play();
		void	Pause();
		void	Stop();

		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		void SetTextRenderingAlias(int nTextRenderingAlias);
		TextRenderingHint GetTextRenderingAlias();
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine = true);

	protected:
		void	InitApngImage();
		void	DeleteApng();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );		// 绘制每帧
		void	LoadApngFromFile(LPCTSTR pstrApngPath);
		void	LoadApngFromMemory( LPVOID pBuf,size_t dwSize );
		void BlendOver(BYTE** rows_dst, BYTE** rows_src, UINT32 x, UINT32 y, UINT32 w, UINT32 h);
	protected:
		CDuiPtrArray	m_ApngImage;

		Bitmap* m_pgdipBmp; 
		int m_nPngWidth;
		int m_nPngHeight;
		DWORD m_nRowSize;	//一行像素大小,乘以m_nPngHeight就是一帧图片的像素大小
		UINT m_nPlays;		//播放几次
		UINT32 m_nChannels;     //1,3,4

		UINT			m_nFramePosition;			// 当前放到第几帧
		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// 是否自动播放
		bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
		bool			m_bIsPlaying;

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//文字抗锯齿级别
	};
}

#endif	//SUPPORT_APNG
#endif // ApngAnimUI_h__
