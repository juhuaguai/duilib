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

		void PaintBkColor(HDC hDC);
		void PaintText(HDC hDC);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CDuiString GetCursor();
		void SetCursor(LPCTSTR pStrCursor);

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;

		UINT GetButtonState() {return m_uButtonState;};
	protected:
		UINT m_uButtonState;
		CDuiString m_sCursor;			//光标形状 arrow/hand 其他根据需要添加

		DWORD m_dwHotBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
	};
}