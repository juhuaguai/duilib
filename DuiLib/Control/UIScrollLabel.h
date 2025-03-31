#ifndef __ScrollLabelUI_h__
#define __ScrollLabelUI_h__

#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class DUILIB_API CScrollLabelUI : public CLabelUI
	{
	public:
		CScrollLabelUI(void);
		~CScrollLabelUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		virtual void	DoEvent(TEventUI& event);
		virtual void	SetVisible(bool bVisible = true );
		virtual void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void DoInit();
		void SetText(const CDuiString& strText);
		void SetTextColor(DWORD dwTextColor);
		void SetTextPadding(RECT rc);

		void PaintText(HDC hDC);

		void SetTimeInterval(const int& nValue);
		int GetTimeInterval();
		void SetStepPix(const int& nValue);
		int GetStepPix();
		virtual void	SetHorScroll(bool bIsHor = true );
		virtual bool	IsHorScroll() const;
		virtual void	SetAutoPlay(bool bIsAuto = true );
		virtual bool	IsAutoPlay() const;
		virtual void	Play();
		virtual void	Pause();
		virtual void	Stop();

		void CreateTextBitmap(HDC hDC);

	protected:
		POINT m_ptTextCurPos;

		int m_nTimeInterval;	//时间间隔 单位ms
		int m_nStepPix;			//步进速度 单位像素
		bool m_bHorScroll;		//横向滚动
		bool m_bIsAutoPlay;
		bool m_bIsPlaying;

		HDC m_hCloneDc;
		HBITMAP m_hTextBitmap;
		int m_nTextBmpW;
		int m_nTextBmpH;
	};
}

#endif // __ScrollLabelUI_h__
