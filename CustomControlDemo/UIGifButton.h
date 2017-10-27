#pragma once

#define  DUI_CTR_GIF_BUTTON                          (_T("GifButton"))

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

	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	CDuiString GetCursor();
	void SetCursor(LPCTSTR pStrCursor);
protected:

	UINT m_uButtonState;
	CDuiString m_sCursor;			//光标形状 arrow/hand 其他根据需要添加
};
