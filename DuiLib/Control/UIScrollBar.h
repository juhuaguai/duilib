#ifndef __UISCROLLBAR_H__
#define __UISCROLLBAR_H__

#pragma once

namespace DuiLib
{
	class DUILIB_API CScrollBarUI : public CControlUI
	{
	public:
		CScrollBarUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		CContainerUI* GetOwner() const;
		void SetOwner(CContainerUI* pOwner);

		void SetVisible(bool bVisible = true);
		void SetEnabled(bool bEnable = true);
		void SetFocus();

		bool IsHorizontal();
		void SetHorizontal(bool bHorizontal = true);
		int GetScrollRange() const;
		void SetScrollRange(int nRange);
		int GetScrollPos() const;
		void SetScrollPos(int nPos, bool bTriggerEvent=true);
		int GetLineSize() const;
		void SetLineSize(int nSize);
        int GetScrollUnit() const;
        void SetScrollUnit(int iUnit);

		bool GetShowButton1();
		void SetShowButton1(bool bShow);
		DWORD GetButton1Color() const;
		void SetButton1Color(DWORD dwColor);
		CDuiString GetButton1NormalImage();
		void SetButton1NormalImage(const CDuiString& strImage);
		CDuiString GetButton1HotImage();
		void SetButton1HotImage(const CDuiString& strImage);
		CDuiString GetButton1PushedImage();
		void SetButton1PushedImage(const CDuiString& strImage);
		CDuiString GetButton1DisabledImage();
		void SetButton1DisabledImage(const CDuiString& strImage);

		bool GetShowButton2();
		void SetShowButton2(bool bShow);
		DWORD GetButton2Color() const;
		void SetButton2Color(DWORD dwColor);
		CDuiString GetButton2NormalImage();
		void SetButton2NormalImage(const CDuiString& strImage);
		CDuiString GetButton2HotImage();
		void SetButton2HotImage(const CDuiString& strImage);
		CDuiString GetButton2PushedImage();
		void SetButton2PushedImage(const CDuiString& strImage);
		CDuiString GetButton2DisabledImage();
		void SetButton2DisabledImage(const CDuiString& strImage);

		DWORD GetThumbColor() const;
		void SetThumbColor(DWORD dwColor);
		CDuiString GetThumbNormalImage();
		void SetThumbNormalImage(const CDuiString& strImage);
		CDuiString GetThumbHotImage();
		void SetThumbHotImage(const CDuiString& strImage);
		CDuiString GetThumbPushedImage();
		void SetThumbPushedImage(const CDuiString& strImage);
		CDuiString GetThumbDisabledImage();
		void SetThumbDisabledImage(const CDuiString& strImage);

		CDuiString GetRailNormalImage();
		void SetRailNormalImage(const CDuiString& strImage);
		CDuiString GetRailHotImage();
		void SetRailHotImage(const CDuiString& strImage);
		CDuiString GetRailPushedImage();
		void SetRailPushedImage(const CDuiString& strImage);
		CDuiString GetRailDisabledImage();
		void SetRailDisabledImage(const CDuiString& strImage);

		CDuiString GetBkNormalImage();
		void SetBkNormalImage(const CDuiString& strImage);
		CDuiString GetBkHotImage();
		void SetBkHotImage(const CDuiString& strImage);
		CDuiString GetBkPushedImage();
		void SetBkPushedImage(const CDuiString& strImage);
		CDuiString GetBkDisabledImage();
		void SetBkDisabledImage(const CDuiString& strImage);

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void PaintBk(HDC hDC);
		void PaintButton1(HDC hDC);
		void PaintButton2(HDC hDC);
		void PaintThumb(HDC hDC);
		void PaintRail(HDC hDC);

	protected:

		enum
		{ 
			DEFAULT_SCROLLBAR_SIZE = 16,
			DEFAULT_TIMERID = 10,
		};

		bool m_bHorizontal;
		int m_nRange;
		int m_nScrollPos;
		int m_nLineSize;
        int m_nScrollUnit;
		CContainerUI* m_pOwner;
		POINT ptLastMouse;
		int m_nLastScrollPos;
		int m_nLastScrollOffset;
		int m_nScrollRepeatDelay;

		TDrawInfo m_diBkNormal;
		TDrawInfo m_diBkHot;
		TDrawInfo m_diBkPushed;
		TDrawInfo m_diBkDisabled;

		bool m_bShowButton1;
		RECT m_rcButton1;
		UINT m_uButton1State;
		DWORD m_dwButton1Color;
		TDrawInfo m_diButton1Normal;
		TDrawInfo m_diButton1Hot;
		TDrawInfo m_diButton1Pushed;
		TDrawInfo m_diButton1Disabled;

		bool m_bShowButton2;
		RECT m_rcButton2;
		UINT m_uButton2State;
		DWORD m_dwButton2Color;
		TDrawInfo m_diButton2Normal;
		TDrawInfo m_diButton2Hot;
		TDrawInfo m_diButton2Pushed;
		TDrawInfo m_diButton2Disabled;

		RECT m_rcThumb;
		UINT m_uThumbState;
		DWORD m_dwThumbColor;
		TDrawInfo m_diThumbNormal;
		TDrawInfo m_diThumbHot;
		TDrawInfo m_diThumbPushed;
		TDrawInfo m_diThumbDisabled;

		TDrawInfo m_diRailNormal;
		TDrawInfo m_diRailHot;
		TDrawInfo m_diRailPushed;
		TDrawInfo m_diRailDisabled;
	};
}

#endif // __UISCROLLBAR_H__