#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class DUILIB_API CGifAnimUI : public CControlUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

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
		void	PlayGif();
		void	PauseGif();
		void	StopGif();

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
		void	InitGifImage();
		void	DeleteGif();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );		// 绘制GIF每帧
		Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
		Gdiplus::Image* LoadGifFromMemory( LPVOID pBuf,size_t dwSize );
	protected:
		Gdiplus::Image	*m_pGifImage;
		UINT			m_nFrameCount;				// gif图片总帧数
		UINT			m_nFramePosition;			// 当前放到第几帧
		Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// 是否自动播放gif
		bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
		bool			m_bIsPlaying;
		IStream*	m_pStream;

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//文字抗锯齿级别
	};
}

#endif // GifAnimUI_h__
