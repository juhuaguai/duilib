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

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

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

		DWORD GetBkColor() const;
		void SetBkColor(DWORD dwBackColor);
		DWORD GetSpokeColor() const;
		void SetSpokeColor(DWORD dwSpokeColor);

		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		void SetTextRenderingAlias(int nTextRenderingAlias);
		TextRenderingHint GetTextRenderingAlias();
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine = true);

	protected:
		void InitChrysanthemumLoading();
		void PaintBitMap();
	
	protected:
		int		m_nTimeInterval;			//时间间隔 单位ms
		int		m_nNumberOfSpoke;			//辐条数量	
		int		m_nSpokeThickness;			//辐条粗细
		int		m_nProgressValue;			//
		int		m_nOuterCircleRadius;		//外圈半径 单位：像素
		int		m_nInnerCircleRadius;		//内圈半径
		PointF	m_CenterPoint;				//中心圆点
		DWORD	m_dwSpokeColor;				//辐条颜色

		Color*	m_pColors;					//辐条的颜色
		double* m_pAngles;					//辐条的角度

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//文字抗锯齿级别

		Bitmap* m_pgdipBmp;
	};
}	// namespace DuiLib
