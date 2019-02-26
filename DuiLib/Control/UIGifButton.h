#pragma once

namespace DuiLib
{
	class CGifButtonUI :	public CGifAnimUI
	{
	public:
		CGifButtonUI(void);
		~CGifButtonUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void SetBkImageDest(const RECT& rcDest);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CDuiString GetCursor();
		void SetCursor(LPCTSTR pStrCursor);

		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		void SetTextRenderingAlias(int nTextRenderingAlias);
		TextRenderingHint GetTextRenderingAlias();
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine = true);

		void DrawFrame( HDC hDC );		// 绘制GIF每帧
	protected:
		UINT m_uButtonState;
		CDuiString m_sCursor;			//光标形状 arrow/hand 其他根据需要添加

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		DWORD	m_dwHotTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//文字抗锯齿级别

		RECT m_rcBkImageDest;
	};
}