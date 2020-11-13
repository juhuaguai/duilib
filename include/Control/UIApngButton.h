#ifndef ApngButtonUI_h__
#define ApngButtonUI_h__

#ifdef SUPPORT_APNG

namespace DuiLib
{
	class DUILIB_API CApngButtonUI : public CApngAnimUI
	{
	public:
		CApngButtonUI(void);
		~CApngButtonUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		void SetBkImageDest(const RECT& rcDest);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CDuiString GetCursor();
		void SetCursor(LPCTSTR pStrCursor);

		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;

		UINT GetButtonState() {return m_uButtonState;};

	protected:	
		void	DrawFrame( HDC hDC );		// 绘制每帧

	protected:
		UINT m_uButtonState;
		CDuiString m_sCursor;			//光标形状 arrow/hand 其他根据需要添加
		DWORD	m_dwHotTextColor;
		RECT m_rcBkImageDest;
	};
}

#endif	//SUPPORT_APNG
#endif // ApngButtonUI_h__
