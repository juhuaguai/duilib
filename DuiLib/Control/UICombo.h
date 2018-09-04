#ifndef __UICOMBO_H__
#define __UICOMBO_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CComboWnd;

class DUILIB_API CComboUI : public CContainerUI, public IListOwnerUI
{
    friend class CComboWnd;
public:
    CComboUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);

    void DoInit();
    UINT GetControlFlags() const;

    CDuiString GetText() const;
    void SetEnabled(bool bEnable = true);

    CDuiString GetDropBoxAttributeList();
    void SetDropBoxAttributeList(const CDuiString& strList);
    SIZE GetDropBoxSize() const;
    void SetDropBoxSize(SIZE szDropBox);

    int GetCurSel() const;
	bool GetSelectCloseFlag();
	void SetSelectCloseFlag(bool flag);
    bool SelectItem(int iIndex, bool bTakeFocus = false, bool bTriggerEvent=true);
    bool ExpandItem(int iIndex, bool bExpand = true);
    int GetExpandedItem() const;

    bool SetItemIndex(CControlUI* pControl, int iNewIndex);
    bool SetMultiItemIndex(CControlUI* pStartControl, int iCount, int iNewStartIndex);
    bool Add(CControlUI* pControl);
    bool AddAt(CControlUI* pControl, int iIndex);
    bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);
    bool RemoveAt(int iIndex, bool bDoNotDestroy=false);
    void RemoveAll();

    bool Activate();

	bool GetShowText() const;
	void SetShowText(bool flag);
	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;
	void SetDisabledTextColor(DWORD dwTextColor);
	DWORD GetDisabledTextColor() const;
	void SetFont(int index);
	int GetFont() const;
    RECT GetTextPadding() const;
    void SetTextPadding(RECT rc);
    CDuiString GetNormalImage() const;
    void SetNormalImage(const CDuiString& strImage);
    CDuiString GetHotImage() const;
    void SetHotImage(const CDuiString& strImage);
    CDuiString GetPushedImage() const;
    void SetPushedImage(const CDuiString& strImage);
    CDuiString GetFocusedImage() const;
    void SetFocusedImage(const CDuiString& strImage);
    CDuiString GetDisabledImage() const;
    void SetDisabledImage(const CDuiString& strImage);

    TListInfoUI* GetListInfo();
    UINT GetItemFixedHeight();
    void SetItemFixedHeight(UINT nHeight);
    int GetItemFont(int index);
    void SetItemFont(int index);
    UINT GetItemTextStyle();
    void SetItemTextStyle(UINT uStyle);
	RECT GetItemTextPadding() const;
    void SetItemTextPadding(RECT rc);
	DWORD GetItemTextColor() const;
    void SetItemTextColor(DWORD dwTextColor);
	DWORD GetItemBkColor() const;
    void SetItemBkColor(DWORD dwBkColor);
	CDuiString GetItemBkImage() const;
    void SetItemBkImage(const CDuiString& strImage);
    bool IsAlternateBk() const;
    void SetAlternateBk(bool bAlternateBk);
	DWORD GetSelectedItemTextColor() const;
    void SetSelectedItemTextColor(DWORD dwTextColor);
	DWORD GetSelectedItemBkColor() const;
    void SetSelectedItemBkColor(DWORD dwBkColor);
	CDuiString GetSelectedItemImage() const;
    void SetSelectedItemImage(const CDuiString& strImage);
	DWORD GetHotItemTextColor() const;
    void SetHotItemTextColor(DWORD dwTextColor);
	DWORD GetHotItemBkColor() const;
    void SetHotItemBkColor(DWORD dwBkColor);
	CDuiString GetHotItemImage() const;
    void SetHotItemImage(const CDuiString& strImage);
	DWORD GetDisabledItemTextColor() const;
    void SetDisabledItemTextColor(DWORD dwTextColor);
	DWORD GetDisabledItemBkColor() const;
    void SetDisabledItemBkColor(DWORD dwBkColor);
	CDuiString GetDisabledItemImage() const;
    void SetDisabledItemImage(const CDuiString& strImage);
    int GetItemHLineSize() const;
    void SetItemHLineSize(int iSize);
    DWORD GetItemHLineColor() const;
    void SetItemHLineColor(DWORD dwLineColor);
    int GetItemVLineSize() const;
    void SetItemVLineSize(int iSize);
	DWORD GetItemVLineColor() const;
    void SetItemVLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);
	void SetEnabledEffect(bool _EnabledEffect);
	bool GetEnabledEffect();

    SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc, bool bNeedInvalidate = true);
	void Move(SIZE szOffset, bool bNeedInvalidate = true);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    
    bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
    void PaintText(HDC hDC);
    void PaintStatusImage(HDC hDC);
	void SetTextRenderingAlias(int nTextRenderingAlias);
	TextRenderingHint GetTextRenderingAlias();

	CDuiString GetVscrollbar() const;
	CDuiString GetHscrollbar() const;
	CDuiString GetVscrollStyle() const;
	CDuiString GetHscrollStyle() const;

protected:
    CComboWnd* m_pWindow;

    int m_iCurSel;
	bool m_EnableEffect;
	bool m_bShowText;
	bool m_bSelectCloseFlag;
    RECT m_rcTextPadding;
    CDuiString m_sDropBoxAttributes;
	CDuiString m_sVscrollbar;
	CDuiString m_sVscrollbarStyle;
	CDuiString m_sHscrollbar;
	CDuiString m_sHscrollStyle;
    SIZE m_szDropBox;
    UINT m_uButtonState;
	int		m_iFont;
	DWORD	m_dwTextColor;
	DWORD	m_dwDisabledTextColor;
	UINT	m_uTextStyle;

	TDrawInfo m_diNormal;
    TDrawInfo m_diHot;
    TDrawInfo m_diPushed;
    TDrawInfo m_diFocused;
    TDrawInfo m_diDisabled;

    TListInfoUI m_ListInfo;

	TextRenderingHint		m_TextRenderingAlias;	//ÎÄ×Ö¿¹¾â³Ý¼¶±ð
};

} // namespace DuiLib

#endif // __UICOMBO_H__
