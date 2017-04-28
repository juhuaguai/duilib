#pragma once

class CCustomLayoutUI :	public CContainerUI,INotifyUI
{
public:
	CCustomLayoutUI(void);
	virtual ~CCustomLayoutUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void DoInit();
	void Notify(TNotifyUI& msg);
public:
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pCloseBtn;
};
