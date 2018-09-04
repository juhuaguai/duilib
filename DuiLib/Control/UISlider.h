#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#pragma once

namespace DuiLib
{
	class DUILIB_API CSliderUI : public CProgressUI
	{
	public:
		CSliderUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetEnabled(bool bEnable = true);

		int GetChangeStep();
		void SetChangeStep(int step);
		void SetThumbSize(SIZE szXY);
		RECT GetThumbRect() const;
		bool IsImmMode() const;
		void SetImmMode(bool bImmMode);
		CDuiString GetThumbImage() const;
		void SetThumbImage(const CDuiString& strImage);
		CDuiString GetThumbHotImage() const;
		void SetThumbHotImage(const CDuiString& strImage);
		CDuiString GetThumbPushedImage() const;
		void SetThumbPushedImage(const CDuiString& strImage);

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void PaintStatusImage(HDC hDC);

	protected:
		SIZE m_szThumb;
		UINT m_uButtonState;
		int m_nStep;
		bool m_bImmMode;

		TDrawInfo m_diThumb;
		TDrawInfo m_diThumbHot;
		TDrawInfo m_diThumbPushed;
	};
}

#endif // __UISLIDER_H__