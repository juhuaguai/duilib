
#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class CRotateImageUI : public CControlUI
	{
	public:
		CRotateImageUI();
		~CRotateImageUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void	DoInit();
		void	DoEvent(TEventUI& event);		
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void	SetTimeInterval(const int& nValue);
		int		GetTimeInterval();
		void	SetEachTimeAngle(const int& nValue);
		int		GetEachTimeAngle();
		void	SetFixAngle(const int& nValue);
		int		GetFixAngle();
		void	SetBkImage(const CDuiString& strImage);
		void	SetStretch(const bool& bStretch);
		bool	GetStretch();
		void	SetAutoStart(const bool& bAutoStart);
		bool	GetAutoStart();
		void	Stop();
		void	Start();
		void	Pause();

		void	PaintBkImage(HDC hDC);	

	protected:
		Gdiplus::Image*	LoadImg(LPCTSTR pstrImgPath);
		void	InitInfo();

	protected:
		int m_nTimeInterval;			//时间间隔(多少毫秒旋转一次) 单位ms
		int m_nAngle;					//每次旋转角度
		bool m_bStretch;				//是否自动拉伸图片
		bool m_bIsAutoStart;			//是否自动开始旋转
		int m_nFixAngle;				//固定旋转角度		

		int m_nCurAngle;
		Gdiplus::Image*	m_pBkimage;
		IStream* m_pStream;
		CDuiString m_sBkImage;
		bool m_bIsPlaying;				//是否正在旋转
	};
}