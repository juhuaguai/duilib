#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class DUILIB_API CGifAnimUI : public CLabelUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(const CDuiString& strImage);

		virtual void PaintBkImage(HDC hDC);

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	Play();
		void	Pause();
		void	Stop();

	protected:
		void    OnTimer( UINT_PTR idEvent );
		bool	LoadFromFile(LPCTSTR pstrGifPath);
		bool	LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask );
	protected:
		CDuiPtrArray	m_Frames;
		UINT			m_nFramePosition;			// 当前放到第几帧

		int m_nFrameWidth;
		int m_nFrameHeight;

		int m_nLoopCout;		//播放几次 0无限循环
		int m_nLoopedNum;		

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// 是否自动播放gif
		bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
		bool			m_bIsPlaying;
	};
}

#endif // GifAnimUI_h__
