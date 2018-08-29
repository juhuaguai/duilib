#pragma once

#define  DUI_CTR_CONTAINER_EX							 (_T("ContainerEx"))

class CContainerExUI :	public CContainerUI
{
public:
	CContainerExUI(void);
	~CContainerExUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);
	void DoEvent(TEventUI& event);
	bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	void SetHotBkColor(DWORD dwColor);
	DWORD GetHotBkColor() const;
	void SetDisabledBkColor(DWORD dwBkColor);
	DWORD GetDisabledBkColor() const;

	void PaintBkColor(HDC hDC);
	void PaintStatusImage(HDC hDC);

	void SetMustHotChildVisible(bool bMustHot=true);
	bool GetMustHotChildVisible();
	void SetChildVisible(bool bVisible=true);
	bool GetChildVisible();

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	bool m_bMustHotChildVisible;
	bool m_bChildVisible;

	UINT m_uMouseState;

	TDrawInfo m_diNormal;
	TDrawInfo m_diHot;
	TDrawInfo m_diDisabled;

	DWORD m_dwHotBkColor;
	DWORD m_dwDisabledBkColor;
};
