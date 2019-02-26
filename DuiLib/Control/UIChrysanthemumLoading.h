#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class CChrysanthemumLoadingUI : public CControlUI
	{
	public:
		CChrysanthemumLoadingUI(void);
		~CChrysanthemumLoadingUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void	DoInit();
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);		
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetTimeInterval(const int& nValue);
		int GetTimeInterval();
		void SetNumberOfSpoke(const int& nValue);
		int GetNumberOfSpoke();
		void SetSpokeThickness(const int& nValue);
		int GetSpokeThickness();
		void SetOuterCircleRadius(const int& nValue);
		int GetOuterCircleRadius();
		void SetInnerCircleRadius(const int& nValue);
		int GetInnerCircleRadius();

		void SetBkColor(DWORD dwBackColor);

	protected:
		void InitChrysanthemumLoading();
	
	protected:
		int		m_nTimeInterval;			//时间间隔 单位ms
		int		m_nNumberOfSpoke;			//辐条数量	
		int		m_nSpokeThickness;			//辐条粗细
		int		m_nProgressValue;			//
		int		m_nOuterCircleRadius;		//外圈半径 单位：像素
		int		m_nInnerCircleRadius;		//内圈半径
		PointF	m_CenterPoint;				//中心圆点

		Color*	m_pColors;					//辐条的颜色
		double* m_pAngles;					//辐条的角度
	};
}	// namespace DuiLib
