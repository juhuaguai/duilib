#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	class DUILIB_API CButtonUI : public CLabelUI
	{
	public:
		CButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		CDuiString GetNormalImage();
		void SetNormalImage(const CDuiString& strImage);
		CDuiString GetHotImage();
		void SetHotImage(const CDuiString& strImage);
		CDuiString GetPushedImage();
		void SetPushedImage(const CDuiString& strImage);
		CDuiString GetFocusedImage();
		void SetFocusedImage(const CDuiString& strImage);
		CDuiString GetDisabledImage();
		void SetDisabledImage(const CDuiString& strImage);
		CDuiString GetForeImage();
		void SetForeImage(const CDuiString& strImage);
		CDuiString GetHotForeImage();
		void SetHotForeImage(const CDuiString& strImage);
		CDuiString GetCursor();
		void SetCursor(const CDuiString& strCursor);
		
		UINT GetButtonState() {return m_uButtonState;};

		// 对应按钮的5个状态图
		void SetFiveStatusImage(const CDuiString& strImage);
		void SetFadeAlphaDelta(BYTE uDelta);
		BYTE GetFadeAlphaDelta();

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetHotBorderColor(DWORD dwColor);
		DWORD GetHotBorderColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		void SetFocusedBkColor(DWORD dwColor);
		DWORD GetFocusedBkColor() const;
		//SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);
		void PaintBkColor(HDC hDC);
		void PaintStatusImage(HDC hDC);
		void PaintBorder(HDC hDC);

	protected:
		enum
		{ 
			FADE_TIMERID = 11,
			FADE_ELLAPSE = 30,
		};

		UINT m_uButtonState;
		CDuiString m_sCursor;			//光标形状 arrow/hand 其他根据需要添加

		DWORD m_dwHotBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwHotBorderColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;
		DWORD m_dwFocusedBkColor;
		
		BYTE m_uFadeAlpha;
		BYTE m_uFadeAlphaDelta;

		TDrawInfo m_diNormal;
		TDrawInfo m_diHot;
		TDrawInfo m_diHotFore;
		TDrawInfo m_diPushed;
		TDrawInfo m_diPushedFore;
		TDrawInfo m_diFocused;
		TDrawInfo m_diDisabled;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__