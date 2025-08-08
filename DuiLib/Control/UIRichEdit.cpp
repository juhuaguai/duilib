#include "stdafx.h"
#ifdef _USEIMM
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#endif
// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0

namespace DuiLib {

const LONG cInitTextMax = (32 * 1024) - 1;

EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
    0x8d33f740,
    0xcf58,
    0x11ce,
    {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
    0xc5bdd8d0,
    0xd26e,
    0x11ce,
    {0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>

class CTxtWinHost : public ITextHost
{
public:
    CTxtWinHost();
    BOOL Init(CRichEditUI *re , const CREATESTRUCT *pcs);
    virtual ~CTxtWinHost();

    ITextServices* GetTextServices(void) { return m_pserv; }
    void SetClientRect(RECT *prc);
    RECT* GetClientRect() { return &m_rcClient; }
    BOOL IsWordWrap(void) { return m_fWordWrap; }
    void SetWordWrap(BOOL fWordWrap);
    BOOL IsReadOnly();
    void SetReadOnly(BOOL fReadOnly);

	LONG GetWinStyle();
	void SetWinStyle(LONG lStyle);
	void SetPassword(bool bPassword);

	void SetMultLine(bool bMultLine = true);
	bool GetMultLine();

	void SetVscrollbar(bool bVscrollbar);
	bool GetVscrollbar();
	void SetHscrollbar(bool bHscrollbar);
	bool GetHsrcollbar();
	void SetAutoVscroll(bool bAutoVscroll);
	bool GetAutoVscroll();
	void SetAutoHscroll(bool bAutoHscroll);
	bool GetAutoHscroll();

    void SetFont(HFONT hFont);
    void SetColor(DWORD dwColor);
    SIZEL* GetExtent();
    void SetExtent(SIZEL *psizelExtent);
    void LimitText(LONG nChars);
    BOOL IsCaptured();
	BOOL IsShowCaret();
	void NeedFreshCaret();
	INT GetCaretWidth();
	INT GetCaretHeight();

    BOOL GetAllowBeep();
    void SetAllowBeep(BOOL fAllowBeep);
    WORD GetDefaultAlign();
    void SetDefaultAlign(WORD wNewAlign);
    BOOL GetRichTextFlag();
    void SetRichTextFlag(BOOL fNew);
    LONG GetDefaultLeftIndent();
    void SetDefaultLeftIndent(LONG lNewIndent);
    BOOL SetSaveSelection(BOOL fSaveSelection);
    HRESULT OnTxInPlaceDeactivate();
    HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
    BOOL GetActiveState(void) { return m_fInplaceActive; }
    BOOL DoSetCursor(RECT *prc, POINT *pt);
    void SetTransparent(BOOL fTransparent);
    void GetControlRect(LPRECT prc);
    LONG SetAccelPos(LONG laccelpos);
    WCHAR SetPasswordChar(WCHAR chPasswordChar);
    void SetDisabled(BOOL fOn);
    LONG SetSelBarWidth(LONG lSelBarWidth);
    BOOL GetTimerState();

    void SetCharFormat(CHARFORMAT2W &c);
    void SetParaFormat(PARAFORMAT2 &p);

    // -----------------------------
    //	IUnknown interface
    // -----------------------------
    virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG _stdcall AddRef(void);
    virtual ULONG _stdcall Release(void);

    // -----------------------------
    //	ITextHost interface
    // -----------------------------
    virtual HDC TxGetDC();
    virtual INT TxReleaseDC(HDC hdc);
    virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
    virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
    virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
    virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
    virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
    virtual void TxViewChange(BOOL fUpdate);
    virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
    virtual BOOL TxShowCaret(BOOL fShow);
    virtual BOOL TxSetCaretPos(INT x, INT y);
    virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
    virtual void TxKillTimer(UINT idTimer);
    virtual void TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
    virtual void TxSetCapture(BOOL fCapture);
    virtual void TxSetFocus();
    virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
    virtual BOOL TxScreenToClient (LPPOINT lppt);
    virtual BOOL TxClientToScreen (LPPOINT lppt);
    virtual HRESULT TxActivate( LONG * plOldState );
    virtual HRESULT TxDeactivate( LONG lNewState );
    virtual HRESULT TxGetClientRect(LPRECT prc);
    virtual HRESULT TxGetViewInset(LPRECT prc);
    virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
    virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
    virtual COLORREF TxGetSysColor(int nIndex);
    virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
    virtual HRESULT TxGetMaxLength(DWORD *plength);
    virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
    virtual HRESULT TxGetPasswordChar(TCHAR *pch);
    virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
    virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
    virtual HRESULT OnTxCharFormatChange (const CHARFORMATW * pcf);
    virtual HRESULT OnTxParaFormatChange (const PARAFORMAT * ppf);
    virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
    virtual HRESULT TxNotify(DWORD iNotify, void *pv);
    virtual HIMC TxImmGetContext(void);
    virtual void TxImmReleaseContext(HIMC himc);
    virtual HRESULT TxGetSelectionBarWidth (LONG *lSelBarWidth);

private:
    CRichEditUI *m_re;
    ULONG	m_cRefs;					// Reference Count
    ITextServices	*m_pserv;		    // pointer to Text Services object
    // Properties

    DWORD		m_dwStyle;				// style bits

    unsigned	m_fEnableAutoWordSel	:1;	// enable Word style auto word selection?
    unsigned	m_fWordWrap			:1;	// Whether control should word wrap
    unsigned	m_fAllowBeep			:1;	// Whether beep is allowed
    unsigned	m_fRich				:1;	// Whether control is rich text
    unsigned	m_fSaveSelection		:1;	// Whether to save the selection when inactive
    unsigned	m_fInplaceActive		:1; // Whether control is inplace active
    unsigned	m_fTransparent		:1; // Whether control is transparent
    unsigned	m_fTimer				:1;	// A timer is set
    unsigned    m_fCaptured           :1;
	unsigned    m_fShowCaret          :1;
	unsigned    m_fNeedFreshCaret     :1; // �����ı��С��������λ��ԭ����겻������������

	INT         m_iCaretWidth;
	INT         m_iCaretHeight;
	INT         m_iCaretLastWidth;
	INT         m_iCaretLastHeight;
    LONG		m_lSelBarWidth;			// Width of the selection bar
    LONG  		m_cchTextMost;			// maximum text size
    DWORD		m_dwEventMask;			// DoEvent mask to pass on to parent window
    LONG		m_icf;
    LONG		m_ipf;
    RECT		m_rcClient;				// Client Rect for this control
    SIZEL		m_sizelExtent;			// Extent array
    CHARFORMAT2W m_cf;					// Default character format
    PARAFORMAT2	m_pf;					    // Default paragraph format
    LONG		m_laccelpos;				// Accelerator position
    WCHAR		m_chPasswordChar;		    // Password character
};

// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
    return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
    return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}

HRESULT InitDefaultCharFormat(CRichEditUI* re, CHARFORMAT2W* pcf, HFONT hfont) 
{
    memset(pcf, 0, sizeof(CHARFORMAT2W));
    LOGFONT lf;
    if( !hfont )
        hfont = re->GetManager()->GetFont(re->GetFont());
    ::GetObject(hfont, sizeof(LOGFONT), &lf);

    DWORD dwColor = re->GetTextColor();
	if (dwColor == 0 || dwColor == 0xFF000000)
		dwColor = 0xFF000001;
    pcf->cbSize = sizeof(CHARFORMAT2W);
    pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    LONG yPixPerInch = GetDeviceCaps(re->GetManager()->GetPaintDC(), LOGPIXELSY);
    if (yPixPerInch)
        yPixPerInch = 96;
    pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    pcf->yOffset = 0;
    pcf->dwEffects = 0;
    pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
    if(lf.lfWeight >= FW_BOLD)
        pcf->dwEffects |= CFE_BOLD;
    if(lf.lfItalic)
        pcf->dwEffects |= CFE_ITALIC;
    if(lf.lfUnderline)
        pcf->dwEffects |= CFE_UNDERLINE;
	if(lf.lfStrikeOut)
		pcf->dwEffects |= CFE_STRIKEOUT;
    pcf->bCharSet = lf.lfCharSet;
    pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

    return S_OK;
}

HRESULT InitDefaultParaFormat(CRichEditUI* re, PARAFORMAT2* ppf) 
{	
    memset(ppf, 0, sizeof(PARAFORMAT2));
    ppf->cbSize = sizeof(PARAFORMAT2);
    ppf->dwMask = PFM_ALL;
    ppf->wAlignment = PFA_LEFT;
    ppf->cTabCount = 1;
    ppf->rgxTabs[0] = lDefaultTab;

    return S_OK;
}

HRESULT CreateHost(CRichEditUI *re, const CREATESTRUCT *pcs, CTxtWinHost **pptec)
{
    HRESULT hr = E_FAIL;
    //GdiSetBatchLimit(1);

    CTxtWinHost *phost = new CTxtWinHost();
    if(phost)
    {
        if (phost->Init(re, pcs))
        {
            *pptec = phost;
            hr = S_OK;
        }
    }

    if (FAILED(hr))
    {
        delete phost;
    }

    return TRUE;
}

CTxtWinHost::CTxtWinHost() : m_re(NULL)
{
    ::ZeroMemory(&m_cRefs, sizeof(CTxtWinHost) - offsetof(CTxtWinHost, m_cRefs));
    m_cchTextMost = cInitTextMax;
    m_laccelpos = -1;
	m_dwStyle = 0;
}

CTxtWinHost::~CTxtWinHost()
{
    m_pserv->OnTxInPlaceDeactivate();
    m_pserv->Release();
}

////////////////////// Create/Init/Destruct Commands ///////////////////////

BOOL CTxtWinHost::Init(CRichEditUI *re, const CREATESTRUCT *pcs)
{
	IUnknown *pUnk = NULL;
    HRESULT hr;

    m_re = re;
    // Initialize Reference count
    m_cRefs = 1;

    // Create and cache CHARFORMAT for this control
    if(FAILED(InitDefaultCharFormat(re, &m_cf, NULL)))
        goto err;

    // Create and cache PARAFORMAT for this control
    if(FAILED(InitDefaultParaFormat(re, &m_pf)))
        goto err;

    //// edit controls created without a window are multiline by default
    //// so that paragraph formats can be
    //dwStyle = ES_MULTILINE;

    // edit controls are rich by default
    m_fRich = re->IsRich();

    m_cchTextMost = re->GetLimitText();

    if (pcs )
    {
        m_dwStyle = pcs->style;

        if ( !(m_dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)) )
        {
            m_fWordWrap = TRUE;
        }
    }

    if( !(m_dwStyle & ES_LEFT) )
    {
        if(m_dwStyle & ES_CENTER)
            m_pf.wAlignment = PFA_CENTER;
        else if(m_dwStyle & ES_RIGHT)
            m_pf.wAlignment = PFA_RIGHT;
    }

    m_fInplaceActive = TRUE;

    // Create Text Services component
    //if(FAILED(CreateTextServices(NULL, this, &pUnk)))
    //    goto err;

	PCreateTextServices TextServicesProc = NULL;
	HMODULE hmod = LoadLibrary(_T("msftedit.dll"));
	if (hmod)
	{
		TextServicesProc = (PCreateTextServices)GetProcAddress(hmod,"CreateTextServices");
	}

	if (TextServicesProc)
	{
		HRESULT hr = TextServicesProc(NULL, this, &pUnk);
	}

    hr = pUnk->QueryInterface(IID_ITextServices,(void **)&m_pserv);

    // Whether the previous call succeeded or failed we are done
    // with the private interface.
    pUnk->Release();

    if(FAILED(hr))
    {
        goto err;
    }

    // Set window text
    if(pcs && pcs->lpszName)
    {
#ifdef _UNICODE		
        if(FAILED(m_pserv->TxSetText((TCHAR *)pcs->lpszName)))
            goto err;
#else
        size_t iLen = _tcslen(pcs->lpszName);
        LPWSTR lpText = new WCHAR[iLen + 1];
        ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
        ::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
        if(FAILED(m_pserv->TxSetText((LPWSTR)lpText))) {
            delete[] lpText;
            goto err;
        }
        delete[] lpText;
#endif
    }

    return TRUE;

err:
    return FALSE;
}

/////////////////////////////////  IUnknown ////////////////////////////////


HRESULT CTxtWinHost::QueryInterface(REFIID riid, void **ppvObject)
{
    HRESULT hr = E_NOINTERFACE;
    *ppvObject = NULL;

    if (IsEqualIID(riid, IID_IUnknown) 
        || IsEqualIID(riid, IID_ITextHost)) 
    {
        AddRef();
        *ppvObject = (ITextHost *) this;
        hr = S_OK;
    }

    return hr;
}

ULONG CTxtWinHost::AddRef(void)
{
    return ++m_cRefs;
}

ULONG CTxtWinHost::Release(void)
{
    ULONG c_Refs = --m_cRefs;

    if (c_Refs == 0)
    {
        delete this;
    }

    return c_Refs;
}

/////////////////////////////////  Far East Support  //////////////////////////////////////

HIMC CTxtWinHost::TxImmGetContext(void)
{
    return NULL;
}

void CTxtWinHost::TxImmReleaseContext(HIMC himc)
{
    //::ImmReleaseContext( hwnd, himc );
}

//////////////////////////// ITextHost Interface  ////////////////////////////

HDC CTxtWinHost::TxGetDC()
{
    return m_re->GetManager()->GetPaintDC();
}

int CTxtWinHost::TxReleaseDC(HDC hdc)
{
    return 1;
}

BOOL CTxtWinHost::TxShowScrollBar(INT fnBar, BOOL fShow)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    else if( fnBar == SB_BOTH ) {
        if( pVerticalScrollBar ) pVerticalScrollBar->SetVisible(fShow == TRUE);
        if( pHorizontalScrollBar ) pHorizontalScrollBar->SetVisible(fShow == TRUE);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
{
    if( fuSBFlags == SB_VERT ) {
        m_re->EnableScrollBar(true, m_re->GetHorizontalScrollBar() != NULL);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_HORZ ) {
        m_re->EnableScrollBar(m_re->GetVerticalScrollBar() != NULL, true);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    else if( fuSBFlags == SB_BOTH ) {
        m_re->EnableScrollBar(true, true);
        m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
        m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        if( nMaxPos - nMinPos - m_rcClient.bottom + m_rcClient.top <= 0 ) {
            pVerticalScrollBar->SetVisible(false);
        }
        else {
            pVerticalScrollBar->SetVisible(true);
            pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - m_rcClient.bottom + m_rcClient.top);
        }
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        if( nMaxPos - nMinPos - m_rcClient.right + m_rcClient.left <= 0 ) {
            pHorizontalScrollBar->SetVisible(false);
        }
        else {
            pHorizontalScrollBar->SetVisible(true);
            pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - m_rcClient.right + m_rcClient.left);
        }   
    }
    return TRUE;
}

BOOL CTxtWinHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
{
    CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
    CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
    if( fnBar == SB_VERT && pVerticalScrollBar ) {
        pVerticalScrollBar->SetScrollPos(nPos);
    }
    else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
        pHorizontalScrollBar->SetScrollPos(nPos);
    }
    return TRUE;
}

void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
{
    if( prc == NULL ) 
	{
		RECT rc = m_rcClient;
        m_re->GetManager()->Invalidate(rc);
        return;
    }
	else
	{
		RECT rc = *prc;
		m_re->GetManager()->Invalidate(rc);
	}
}

void CTxtWinHost::TxViewChange(BOOL fUpdate) 
{
    if( m_re->OnTxViewChanged() ) m_re->Invalidate();
}

BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
{
	if (m_re->GetCaret())
	{
		m_iCaretWidth = xWidth;
		m_iCaretHeight = yHeight;
		return ::CreateCaret(m_re->GetManager()->GetPaintWindow(), hbmp, xWidth, yHeight);
	}

	return FALSE;
}

BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
{
	m_fShowCaret = fShow;
    if(fShow)
        return ::ShowCaret(m_re->GetManager()->GetPaintWindow());
    else
        return ::HideCaret(m_re->GetManager()->GetPaintWindow());
}

BOOL CTxtWinHost::TxSetCaretPos(INT x, INT y)
{
	POINT ptCaret = { 0 };
	::GetCaretPos(&ptCaret);
	RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x + m_iCaretLastWidth, ptCaret.y + m_iCaretLastHeight };
	if( m_re->GetManager()->IsLayered() ) m_re->GetManager()->Invalidate(rcCaret);
	else if( m_fNeedFreshCaret == TRUE ) {
		m_re->GetManager()->Invalidate(rcCaret);
		m_fNeedFreshCaret = FALSE;
	}
	rcCaret.left = x;
	rcCaret.top = y;
	rcCaret.right = x + m_iCaretWidth;
	rcCaret.bottom = y + m_iCaretHeight;
	if( m_re->GetManager()->IsLayered() ) m_re->GetManager()->Invalidate(rcCaret);
	m_iCaretLastWidth = m_iCaretWidth;
	m_iCaretLastHeight = m_iCaretHeight;
	return ::SetCaretPos(x, y);
}

BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
{
    m_fTimer = TRUE;
    return m_re->GetManager()->SetTimer(m_re, idTimer, uTimeout) == TRUE;
}

void CTxtWinHost::TxKillTimer(UINT idTimer)
{
    m_re->GetManager()->KillTimer(m_re, idTimer);
    m_fTimer = FALSE;
}

void CTxtWinHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
{
    return;
}

void CTxtWinHost::TxSetCapture(BOOL fCapture)
{
    if (fCapture) m_re->GetManager()->SetCapture();
    else m_re->GetManager()->ReleaseCapture();
    m_fCaptured = fCapture;
}

void CTxtWinHost::TxSetFocus()
{
    m_re->SetFocus();
}

void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
{
    ::SetCursor(hcur);
}

BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
{
    return ::ScreenToClient(m_re->GetManager()->GetPaintWindow(), lppt);	
}

BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
{
    return ::ClientToScreen(m_re->GetManager()->GetPaintWindow(), lppt);
}

HRESULT CTxtWinHost::TxActivate(LONG *plOldState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
{
    *prc = m_rcClient;
    GetControlRect(prc);
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc) 
{
    prc->left = prc->right = prc->top = prc->bottom = 0;
    return NOERROR;	
}

HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW **ppCF)
{
    *ppCF = &m_cf;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT **ppPF)
{
    *ppPF = &m_pf;
    return NOERROR;
}

COLORREF CTxtWinHost::TxGetSysColor(int nIndex) 
{
    return ::GetSysColor(nIndex);
}

HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
{
    *pstyle = m_fTransparent ? TXTBACK_TRANSPARENT : TXTBACK_OPAQUE;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetMaxLength(DWORD *pLength)
{
    *pLength = m_cchTextMost;
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetScrollBars(DWORD *pdwScrollBar)
{
    *pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
        ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

    return NOERROR;
}

HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR *pch)
{
#ifdef _UNICODE
    *pch = m_chPasswordChar;
#else
    ::WideCharToMultiByte(CP_ACP, 0, &m_chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
    return NOERROR;
}

HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG *pcp)
{
    *pcp = m_laccelpos;
    return S_OK;
} 										   

HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
{
    return S_OK;
}

HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
{
    return S_OK;
}

HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
{
    DWORD dwProperties = 0;

    if (m_fRich)
    {
        dwProperties = TXTBIT_RICHTEXT;
    }

    if (m_dwStyle & ES_MULTILINE)
    {
        dwProperties |= TXTBIT_MULTILINE;
    }

    if (m_dwStyle & ES_READONLY)
    {
        dwProperties |= TXTBIT_READONLY;
    }

    if (m_dwStyle & ES_PASSWORD)
    {
        dwProperties |= TXTBIT_USEPASSWORD;
    }

    if (!(m_dwStyle & ES_NOHIDESEL))
    {
        dwProperties |= TXTBIT_HIDESELECTION;
    }

    if (m_fEnableAutoWordSel)
    {
        dwProperties |= TXTBIT_AUTOWORDSEL;
    }

    if (m_fWordWrap)
    {
        dwProperties |= TXTBIT_WORDWRAP;
    }

    if (m_fAllowBeep)
    {
        dwProperties |= TXTBIT_ALLOWBEEP;
    }

    if (m_fSaveSelection)
    {
        dwProperties |= TXTBIT_SAVESELECTION;
    }

    *pdwBits = dwProperties & dwMask; 
    return NOERROR;
}


HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void *pv)
{
    if( iNotify == EN_REQUESTRESIZE ) {
        RECT rc;
        REQRESIZE *preqsz = (REQRESIZE *)pv;
        GetControlRect(&rc);
        rc.bottom = rc.top + preqsz->rc.bottom;
        rc.right  = rc.left + preqsz->rc.right;
        SetClientRect(&rc);
        return S_OK;
    }
    m_re->OnTxNotify(iNotify, pv);
    return S_OK;
}

HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
{
    *lpExtent = m_sizelExtent;
    return S_OK;
}

HRESULT	CTxtWinHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
{
    *plSelBarWidth = m_lSelBarWidth;
    return S_OK;
}

void CTxtWinHost::SetWordWrap(BOOL fWordWrap)
{
    m_fWordWrap = fWordWrap;
    m_pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, m_fWordWrap ? TXTBIT_WORDWRAP : 0);
}

BOOL CTxtWinHost::IsReadOnly()
{
    return (m_dwStyle & ES_READONLY) != 0;
}

void CTxtWinHost::SetReadOnly(BOOL fReadOnly)
{
    if (fReadOnly)
    {
        m_dwStyle |= ES_READONLY;
    }
    else
    {
        m_dwStyle &= ~ES_READONLY;
    }

    m_pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, fReadOnly ? TXTBIT_READONLY : 0);
}

bool CTxtWinHost::GetMultLine()
{
	if (m_dwStyle & ES_MULTILINE)
		return true;
	else
		return false;
}

void CTxtWinHost::SetMultLine(bool bMultLine /* = true */)
{
	if (bMultLine)
	{
		m_dwStyle = m_dwStyle|ES_MULTILINE;
	}
	else
	{
		m_dwStyle &= ~ES_MULTILINE;
	}
	m_pserv->OnTxPropertyBitsChange(TXTBIT_MULTILINE, bMultLine ? TXTBIT_MULTILINE : 0);
}

LONG CTxtWinHost::GetWinStyle()
{
	return m_dwStyle;
}

void CTxtWinHost::SetWinStyle(LONG lStyle)
{
	m_dwStyle = lStyle;
}
void CTxtWinHost::SetPassword(bool bPassword)
{
	m_pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, bPassword ? TXTBIT_USEPASSWORD : 0);
}

void CTxtWinHost::SetVscrollbar(bool bVscrollbar)
{
	if (bVscrollbar)
	{
		m_dwStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
	}
	else
	{
		m_dwStyle &= ~WS_VSCROLL;
		if ( (m_dwStyle&WS_HSCROLL)==0)
		{
			m_dwStyle &= ~ES_DISABLENOSCROLL;
		}
	}
}

bool CTxtWinHost::GetVscrollbar()
{
	if (m_dwStyle & WS_VSCROLL)
		return true;
	else
		return false;
}

void CTxtWinHost::SetHscrollbar(bool bHscrollbar)
{
	if (bHscrollbar)
	{
		m_dwStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
	}
	else
	{
		m_dwStyle &= ~WS_HSCROLL;
		if ( (m_dwStyle&WS_VSCROLL)==0)
		{
			m_dwStyle &= ~ES_DISABLENOSCROLL;
		}
	}
}

bool CTxtWinHost::GetHsrcollbar()
{
	if (m_dwStyle & WS_HSCROLL)
		return true;
	else
		return false;
}

void CTxtWinHost::SetAutoVscroll(bool bAutoVscroll)
{
	if (bAutoVscroll)
	{
		m_dwStyle |= ES_AUTOVSCROLL;
	}
	else
	{
		m_dwStyle &= ~ES_AUTOVSCROLL;
	}
}
bool CTxtWinHost::GetAutoVscroll()
{
	if (m_dwStyle & ES_AUTOVSCROLL)
		return true;
	else
		return false;
}
void CTxtWinHost::SetAutoHscroll(bool bAutoHscroll)
{
	if (bAutoHscroll)
	{
		m_dwStyle |= ES_AUTOHSCROLL;
	}
	else
	{
		m_dwStyle &= ~ES_AUTOHSCROLL;
	}
}
bool CTxtWinHost::GetAutoHscroll()
{
	if (m_dwStyle & ES_AUTOHSCROLL)
		return true;
	else
		return false;
}

void CTxtWinHost::SetFont(HFONT hFont) 
{
    if( hFont == NULL ) return;
    LOGFONT lf;
    ::GetObject(hFont, sizeof(LOGFONT), &lf);
    LONG yPixPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY);
    m_cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
    if(lf.lfWeight >= FW_BOLD) 
		m_cf.dwEffects |= CFE_BOLD;
	else 
		m_cf.dwEffects &= ~CFE_BOLD;
    if(lf.lfItalic)
		m_cf.dwEffects |= CFE_ITALIC;
	else 
		m_cf.dwEffects &= ~CFE_ITALIC;
    if(lf.lfUnderline) 
		m_cf.dwEffects |= CFE_UNDERLINE;
	else 
		m_cf.dwEffects &= ~CFE_UNDERLINE;
	if(lf.lfStrikeOut) 
		m_cf.dwEffects |= CFE_STRIKEOUT;
	else 
		m_cf.dwEffects &= ~CFE_STRIKEOUT;
    m_cf.bCharSet = lf.lfCharSet;
    m_cf.bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
    _tcscpy(m_cf.szFaceName, lf.lfFaceName);
#else
    //need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
    MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, m_cf.szFaceName, LF_FACESIZE);
#endif

    m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
}

void CTxtWinHost::SetColor(DWORD dwColor)
{
	if (dwColor == 0 || dwColor == 0xFF000000)
		dwColor = 0xFF000001;
    m_cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
    m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
}

SIZEL* CTxtWinHost::GetExtent() 
{
    return &m_sizelExtent;
}

void CTxtWinHost::SetExtent(SIZEL *psizelExtent) 
{ 
    m_sizelExtent = *psizelExtent; 
    m_pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
}

void CTxtWinHost::LimitText(LONG nChars)
{
    m_cchTextMost = nChars;
    if( m_cchTextMost <= 0 ) 
		m_cchTextMost = cInitTextMax;
    m_pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
}

BOOL CTxtWinHost::IsCaptured()
{
    return m_fCaptured;
}

BOOL CTxtWinHost::IsShowCaret()
{
	return m_fShowCaret;
}

void CTxtWinHost::NeedFreshCaret()
{
	m_fNeedFreshCaret = TRUE;
}

INT CTxtWinHost::GetCaretWidth()
{
	return m_iCaretWidth;
}

INT CTxtWinHost::GetCaretHeight()
{
	return m_iCaretHeight;
}

BOOL CTxtWinHost::GetAllowBeep()
{
    return m_fAllowBeep;
}

void CTxtWinHost::SetAllowBeep(BOOL fAllowBeep)
{
    m_fAllowBeep = fAllowBeep;

    m_pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
}

WORD CTxtWinHost::GetDefaultAlign()
{
    return m_pf.wAlignment;
}

void CTxtWinHost::SetDefaultAlign(WORD wNewAlign)
{
    m_pf.wAlignment = wNewAlign;

    // Notify control of property change
    m_pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

BOOL CTxtWinHost::GetRichTextFlag()
{
    return m_fRich;
}

void CTxtWinHost::SetRichTextFlag(BOOL fNew)
{
    m_fRich = fNew;

    m_pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, fNew ? TXTBIT_RICHTEXT : 0);
}

LONG CTxtWinHost::GetDefaultLeftIndent()
{
    return m_pf.dxOffset;
}

void CTxtWinHost::SetDefaultLeftIndent(LONG lNewIndent)
{
    m_pf.dxOffset = lNewIndent;

    m_pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
}

void CTxtWinHost::SetClientRect(RECT *prc) 
{
    m_rcClient = *prc;

    LONG xPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSX); 
    LONG yPerInch =	::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY); 
    m_sizelExtent.cx = DXtoHimetricX(m_rcClient.right - m_rcClient.left, xPerInch);
    m_sizelExtent.cy = DYtoHimetricY(m_rcClient.bottom - m_rcClient.top, yPerInch);

    m_pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
}

BOOL CTxtWinHost::SetSaveSelection(BOOL f_SaveSelection)
{
    BOOL fResult = f_SaveSelection;

    m_fSaveSelection = f_SaveSelection;

    // notify text services of property change
    m_pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, m_fSaveSelection ? TXTBIT_SAVESELECTION : 0);

    return fResult;		
}

HRESULT	CTxtWinHost::OnTxInPlaceDeactivate()
{
    HRESULT hr = m_pserv->OnTxInPlaceDeactivate();

    if (SUCCEEDED(hr))
    {
        m_fInplaceActive = FALSE;
    }

    return hr;
}

HRESULT	CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
{
    m_fInplaceActive = TRUE;

    HRESULT hr = m_pserv->OnTxInPlaceActivate(prcClient);

    if (FAILED(hr))
    {
        m_fInplaceActive = FALSE;
    }

    return hr;
}

BOOL CTxtWinHost::DoSetCursor(RECT *prc, POINT *pt)
{
    RECT rc = prc ? *prc : m_rcClient;

    // Is this in our rectangle?
    if (PtInRect(&rc, *pt))
    {
        RECT *prcClient = (!m_fInplaceActive || prc) ? &rc : NULL;
        m_pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_re->GetManager()->GetPaintDC(), NULL, prcClient, pt->x, pt->y);

        return TRUE;
    }

    return FALSE;
}

void CTxtWinHost::GetControlRect(LPRECT prc)
{
    prc->top = m_rcClient.top;
    prc->bottom = m_rcClient.bottom;
    prc->left = m_rcClient.left;
    prc->right = m_rcClient.right;
}

void CTxtWinHost::SetTransparent(BOOL f_Transparent)
{
    m_fTransparent = f_Transparent;

    // notify text services of property change
    m_pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
}

LONG CTxtWinHost::SetAccelPos(LONG l_accelpos)
{
    LONG laccelposOld = m_laccelpos;

    m_laccelpos = l_accelpos;

    // notify text services of property change
    m_pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

    return laccelposOld;
}

WCHAR CTxtWinHost::SetPasswordChar(WCHAR ch_PasswordChar)
{
    WCHAR chOldPasswordChar = m_chPasswordChar;

    m_chPasswordChar = ch_PasswordChar;

    // notify text services of property change
    m_pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, (m_chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

    return chOldPasswordChar;
}

void CTxtWinHost::SetDisabled(BOOL fOn)
{
    m_cf.dwMask	 |= CFM_COLOR | CFM_DISABLED;
    m_cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

    if( !fOn )
		m_cf.dwEffects &= ~CFE_DISABLED;

    m_pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
}

LONG CTxtWinHost::SetSelBarWidth(LONG l_SelBarWidth)
{
    LONG lOldSelBarWidth = m_lSelBarWidth;

    m_lSelBarWidth = l_SelBarWidth;

    if (m_lSelBarWidth)
    {
        m_dwStyle |= ES_SELECTIONBAR;
    }
    else
    {
        m_dwStyle &= (~ES_SELECTIONBAR);
    }

    m_pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

    return lOldSelBarWidth;
}

BOOL CTxtWinHost::GetTimerState()
{
    return m_fTimer;
}

void CTxtWinHost::SetCharFormat(CHARFORMAT2W &c)
{
    m_cf = c;
}

void CTxtWinHost::SetParaFormat(PARAFORMAT2 &p)
{
    m_pf = p;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CRichEditUI::CRichEditUI() 
		: m_pTwh(NULL),
		m_bVScrollBarFixing(false),
		m_bWantTab(true),
		m_bWantReturn(true), 
		m_bWantCtrlReturn(true),
		m_bTransparent(true), 
		m_bRich(true), 
		m_bReadOnly(false), 
		m_bWordWrap(false), 
		m_dwTextColor(0), 
		m_iFont(-1), 
		m_iLimitText(cInitTextMax),
		m_lTwhStyle(ES_MULTILINE),
		m_bCaret(true),
		m_bDrawCaret(true),
		m_bInited(false),
		m_dwTipValueColor(0xFF8E8E8E)
{
	::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
}

CRichEditUI::~CRichEditUI()
{
    if( m_pTwh ) {
        m_pTwh->Release();
        m_pManager->RemoveMessageFilter(this);
    }
}

LPCTSTR CRichEditUI::GetClass() const
{
    return DUI_CTR_RICHEDIT;
}

LPVOID CRichEditUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_RICHEDIT) == 0 ) return static_cast<CRichEditUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CRichEditUI::GetControlFlags() const
{
    if( !IsEnabled() ) return CControlUI::GetControlFlags();

    return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

bool CRichEditUI::IsWantTab()
{
    return m_bWantTab;
}

void CRichEditUI::SetWantTab(bool bWantTab)
{
    m_bWantTab = bWantTab;
}

bool CRichEditUI::IsWantReturn()
{
    return m_bWantReturn;
}

void CRichEditUI::SetWantReturn(bool bWantReturn)
{
    m_bWantReturn = bWantReturn;
}

bool CRichEditUI::IsWantCtrlReturn()
{
    return m_bWantCtrlReturn;
}

void CRichEditUI::SetWantCtrlReturn(bool bWantCtrlReturn)
{
    m_bWantCtrlReturn = bWantCtrlReturn;
}

bool CRichEditUI::IsTransparent()
{
    return m_bTransparent;
}

void CRichEditUI::SetTransparent(bool bTransparent)
{
    m_bTransparent = bTransparent;
    if( m_pTwh ) m_pTwh->SetTransparent(bTransparent);
}

bool CRichEditUI::IsRich()
{
    return m_bRich;
}

void CRichEditUI::SetRich(bool bRich)
{
    m_bRich = bRich;
    if( m_pTwh ) m_pTwh->SetRichTextFlag(bRich);
}

bool CRichEditUI::IsReadOnly()
{
    return m_bReadOnly;
}

void CRichEditUI::SetReadOnly(bool bReadOnly)
{
    m_bReadOnly = bReadOnly;
    if( m_pTwh ) m_pTwh->SetReadOnly(bReadOnly);
}

bool CRichEditUI::IsWordWrap()
{
    return m_bWordWrap;
}

void CRichEditUI::SetWordWrap(bool bWordWrap)
{
    m_bWordWrap = bWordWrap;
    if( m_pTwh ) m_pTwh->SetWordWrap(bWordWrap);
}

int CRichEditUI::GetFont()
{
    return m_iFont;
}

void CRichEditUI::SetFont(int index)
{
    m_iFont = index;
    if( m_pTwh ) {
        m_pTwh->SetFont(GetManager()->GetFont(m_iFont));
    }
}

void CRichEditUI::SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bStrikeOut)
{
    if( m_pTwh ) {
        LOGFONT lf = { 0 };
        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        _tcsncpy(lf.lfFaceName, pStrFontName, LF_FACESIZE);
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfHeight = -nSize;
        if( bBold ) lf.lfWeight += FW_BOLD;
        if( bUnderline ) lf.lfUnderline = TRUE;
        if( bItalic ) lf.lfItalic = TRUE;
		if( bStrikeOut ) lf.lfStrikeOut = TRUE;
        HFONT hFont = ::CreateFontIndirect(&lf);
        if( hFont == NULL ) return;
        m_pTwh->SetFont(hFont);
        ::DeleteObject(hFont);
    }
}

LONG CRichEditUI::GetWinStyle()
{
    return m_lTwhStyle;
}

void CRichEditUI::SetWinStyle(LONG lStyle)
{
    m_lTwhStyle = lStyle;
	if (m_pTwh)
	{
		m_pTwh->SetWinStyle(m_lTwhStyle);
	}
}
bool CRichEditUI::GetPassword()
{
	if (m_lTwhStyle & ES_PASSWORD)
		return true;
	else
		return false;
}

void CRichEditUI::SetPassword(bool bPassword /* = true */)
{
	if (bPassword)
	{
		m_lTwhStyle = m_lTwhStyle|ES_PASSWORD;
	}
	else
	{
		m_lTwhStyle &= ~ES_PASSWORD;
	}
	if (m_pTwh)
	{
		m_pTwh->SetWinStyle(m_lTwhStyle);
		m_pTwh->SetPassword(bPassword);
	}
}

DWORD CRichEditUI::GetTextColor()
{
    return m_dwTextColor;
}

void CRichEditUI::SetTextColor(DWORD dwTextColor)
{
	if (m_dwTextColor == dwTextColor)
		return;

	if (dwTextColor)
	{
		BYTE A = dwTextColor>>24;
		if (A==0)
			dwTextColor += 0xFF000000;
	}
    m_dwTextColor = dwTextColor;
    if( m_pTwh ) {
        m_pTwh->SetColor(dwTextColor);
    }
}

int CRichEditUI::GetLimitText()
{
    return m_iLimitText;
}

void CRichEditUI::SetLimitText(int iChars)
{
    m_iLimitText = iChars;
    if( m_pTwh ) {
        m_pTwh->LimitText(m_iLimitText);
    }
}

long CRichEditUI::GetTextLength(DWORD dwFlags) const
{
    GETTEXTLENGTHEX textLenEx;
    textLenEx.flags = dwFlags;
#ifdef _UNICODE
    textLenEx.codepage = 1200;
#else
    textLenEx.codepage = CP_ACP;
#endif
    LRESULT lResult;
    TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
    return (long)lResult;
}

CDuiString CRichEditUI::GetText() const
{
    long lLen = GetTextLength(GTL_DEFAULT);
    LPTSTR lpText = NULL;
    GETTEXTEX gt;
    gt.flags = GT_DEFAULT;
#ifdef _UNICODE
    gt.cb = sizeof(TCHAR) * (lLen + 1) ;
    gt.codepage = 1200;
    lpText = new TCHAR[lLen + 1];
    ::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
    gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
    gt.codepage = CP_ACP;
    lpText = new TCHAR[lLen * 2 + 1];
    ::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
    gt.lpDefaultChar = NULL;
    gt.lpUsedDefChar = NULL;
    TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
    CDuiString sText(lpText);
    delete[] lpText;
    return sText;
}

void CRichEditUI::SetText(const CDuiString& strText)
{
#ifdef USE_OPENCC

	CDuiString strConv = OpenccConvert(strText.GetData());

	bool bTextChanged = false;
	if (m_sText!=strConv)
		bTextChanged = true;

	m_sText = strConv;
	if( !m_pTwh ) return;
	SetSel(0, -1);
	ReplaceSel(strConv.GetData(), FALSE);
#else
	bool bTextChanged = false;
	if (m_sText!=strText)
		bTextChanged = true;

	m_sText = strText;
	if( !m_pTwh ) return;
	SetSel(0, -1);
	ReplaceSel(strText.GetData(), FALSE);
#endif

	if (bTextChanged)
		GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED);	
}

DWORD CRichEditUI::GetTipValueTextColor()
{
	return m_dwTipValueColor;
}
void CRichEditUI::SetTipValueTextColor(DWORD dwTextColor)
{
	if (m_dwTipValueColor == dwTextColor)
		return;

	if (dwTextColor)
	{
		BYTE A = dwTextColor>>24;
		if (A==0)
			dwTextColor += 0xFF000000;
	}
	m_dwTipValueColor = dwTextColor;
	Invalidate();
}
CDuiString CRichEditUI::GetTipValueText() const
{
	return m_strTipValue;
}
void CRichEditUI::SetTipValueText(const CDuiString& strText)
{
#ifdef USE_OPENCC
	CDuiString strConv = OpenccConvert(strText.GetData());
	if (m_strTipValue==strConv)
		return;
	m_strTipValue = strConv;
#else
	if (m_strTipValue==strText)
		return;
	m_strTipValue = strText;
#endif	
	Invalidate();
}


CDuiString CRichEditUI::GetFocusedImage()
{
	return m_diFocused.sDrawString;	
}

void CRichEditUI::SetFocusedImage(const CDuiString& strImage)
{
	if( m_diFocused.sDrawString == strImage && m_diFocused.pImageInfo != NULL ) return;
	m_diFocused.Clear();
	m_diFocused.sDrawString = strImage;
	Invalidate();
}

CDuiString CRichEditUI::GetDisabledImage()
{
	return m_diDisabled.sDrawString;	
}

void CRichEditUI::SetDisabledImage(const CDuiString& strImage)
{
	if( m_diDisabled.sDrawString == strImage && m_diDisabled.pImageInfo != NULL ) return;
	m_diDisabled.Clear();
	m_diDisabled.sDrawString = strImage;
	Invalidate();
}

bool CRichEditUI::IsModify() const
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

void CRichEditUI::SetModify(bool bModified) const
{ 
    TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
}

void CRichEditUI::GetSel(CHARRANGE &cr) const
{ 
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
}

void CRichEditUI::GetSel(long& nStartChar, long& nEndChar) const
{
    CHARRANGE cr;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
    nStartChar = cr.cpMin;
    nEndChar = cr.cpMax;
}

int CRichEditUI::SetSel(CHARRANGE &cr)
{ 
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

int CRichEditUI::SetSel(long nStartChar, long nEndChar)
{
    CHARRANGE cr;
    cr.cpMin = nStartChar;
    cr.cpMax = nEndChar;
    LRESULT lResult;
    TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
    return (int)lResult;
}

void CRichEditUI::ReplaceSel(const CDuiString& strNewText, bool bCanUndo)
{
#ifdef _UNICODE		
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)strNewText.GetData(), 0); 
#else
    int iLen = _tcslen(strNewText.GetData());
    LPWSTR lpText = new WCHAR[iLen + 1];
    ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
    ::MultiByteToWideChar(CP_ACP, 0, strNewText.GetData(), -1, (LPWSTR)lpText, iLen) ;
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpText, 0); 
    delete[] lpText;
#endif
}

void CRichEditUI::ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo)
{
    TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
}

CDuiString CRichEditUI::GetSelText() const
{
    if( !m_pTwh ) return CDuiString();
    CHARRANGE cr;
    cr.cpMin = cr.cpMax = 0;
    TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
    LPWSTR lpText = NULL;
    lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
    ::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
    TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CRichEditUI::SetSelAll()
{
    return SetSel(0, -1);
}

int CRichEditUI::SetSelNone()
{
    return SetSel(-1, 0);
}

bool CRichEditUI::GetZoom(int& nNum, int& nDen) const
{
    LRESULT lResult;
    TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetZoom(int nNum, int nDen)
{
    if (nNum < 0 || nNum > 64) return false;
    if (nDen < 0 || nDen > 64) return false;
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetZoomOff()
{
    LRESULT lResult;
    TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

WORD CRichEditUI::GetSelectionType() const
{
    LRESULT lResult;
    TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
    return (WORD)lResult;
}

bool CRichEditUI::GetAutoURLDetect() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetAutoURLDetect(bool bAutoDetect)
{
    LRESULT lResult;
    TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
    return (BOOL)lResult == FALSE;
}

DWORD CRichEditUI::GetEventMask() const
{
    LRESULT lResult;
    TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
    return (DWORD)lResult;
}

DWORD CRichEditUI::SetEventMask(DWORD dwEventMask)
{
    LRESULT lResult;
    TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
    return (DWORD)lResult;
}

CDuiString CRichEditUI::GetTextRange(long nStartChar, long nEndChar) const
{
    TEXTRANGEW tr = { 0 };
    tr.chrg.cpMin = nStartChar;
    tr.chrg.cpMax = nEndChar;
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nEndChar - nStartChar + 1];
    ::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
    tr.lpstrText = lpText;
    TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

void CRichEditUI::HideSelection(bool bHide, bool bChangeStyle)
{
    TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
}

void CRichEditUI::ScrollCaret()
{
    TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
}

int CRichEditUI::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

int CRichEditUI::AppendText(LPCTSTR lpstrText, bool bCanUndo)
{
    int nRet = SetSel(-1, -1);
    ReplaceSel(lpstrText, bCanUndo);
    return nRet;
}

DWORD CRichEditUI::GetDefaultCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetDefaultCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        CHARFORMAT2W cfw;
        cfw.cbSize = sizeof(CHARFORMAT2W);
        TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
        m_pTwh->SetCharFormat(cfw);
        return true;
    }
    return false;
}

DWORD CRichEditUI::GetSelectionCharFormat(CHARFORMAT2 &cf) const
{
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetSelectionCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
    return (BOOL)lResult == TRUE;
}

bool CRichEditUI::SetWordCharFormat(CHARFORMAT2 &cf)
{
    if( !m_pTwh ) return false;
    cf.cbSize = sizeof(CHARFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION|SCF_WORD, (LPARAM)&cf, &lResult); 
    return (BOOL)lResult == TRUE;
}

DWORD CRichEditUI::GetParaFormat(PARAFORMAT2 &pf) const
{
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    return (DWORD)lResult;
}

bool CRichEditUI::SetParaFormat(PARAFORMAT2 &pf)
{
    if( !m_pTwh ) return false;
    pf.cbSize = sizeof(PARAFORMAT2);
    LRESULT lResult;
    TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
    if( (BOOL)lResult == TRUE ) {
        m_pTwh->SetParaFormat(pf);
        return true;
    }
    return false;
}

bool CRichEditUI::Redo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_REDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

bool CRichEditUI::Undo()
{ 
    if( !m_pTwh ) return false;
    LRESULT lResult;
    TxSendMessage(EM_UNDO, 0, 0, &lResult);
    return (BOOL)lResult == TRUE; 
}

void CRichEditUI::Clear()
{ 
    TxSendMessage(WM_CLEAR, 0, 0, 0); 
}

void CRichEditUI::Copy()
{ 
    TxSendMessage(WM_COPY, 0, 0, 0); 
}

void CRichEditUI::Cut()
{ 
    TxSendMessage(WM_CUT, 0, 0, 0); 
}

void CRichEditUI::Paste()
{ 
    TxSendMessage(WM_PASTE, 0, 0, 0); 
}

int CRichEditUI::GetLineCount() const
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
    return (int)lResult; 
}

CDuiString CRichEditUI::GetLine(int nIndex, int nMaxLength) const
{
    LPWSTR lpText = NULL;
    lpText = new WCHAR[nMaxLength + 1];
    ::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
    *(LPWORD)lpText = (WORD)nMaxLength;
    TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
    CDuiString sText;
    sText = (LPCWSTR)lpText;
    delete[] lpText;
    return sText;
}

int CRichEditUI::LineIndex(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
    return (int)lResult;
}

int CRichEditUI::LineLength(int nLine) const
{
    LRESULT lResult;
    TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
    return (int)lResult;
}

bool CRichEditUI::LineScroll(int nLines, int nChars)
{
    LRESULT lResult;
    TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
    return (BOOL)lResult == TRUE;
}

CDuiPoint CRichEditUI::GetCharPos(long lChar) const
{ 
    CDuiPoint pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar, 0); 
    return pt;
}

long CRichEditUI::LineFromChar(long nIndex) const
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
    return (long)lResult;
}

CDuiPoint CRichEditUI::PosFromChar(UINT nChar) const
{ 
    POINTL pt; 
    TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0); 
    return CDuiPoint(pt.x, pt.y); 
}

int CRichEditUI::CharFromPos(CDuiPoint pt) const
{ 
    POINTL ptl = {pt.x, pt.y}; 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
    return (int)lResult; 
}

void CRichEditUI::EmptyUndoBuffer()
{ 
    TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0); 
}

UINT CRichEditUI::SetUndoLimit(UINT nLimit)
{ 
    if( !m_pTwh ) return 0;
    LRESULT lResult;
    TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
    return (UINT)lResult; 
}

long CRichEditUI::StreamIn(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult;
}

long CRichEditUI::StreamOut(int nFormat, EDITSTREAM &es)
{ 
    if( !m_pTwh ) return 0L;
    LRESULT lResult;
    TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
    return (long)lResult; 
}

RECT CRichEditUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CRichEditUI::SetTextPadding(RECT rc)
{
	m_rcTextPadding = rc;
	Invalidate();
}


bool CRichEditUI::GetMultLine()
{
	if (m_lTwhStyle & ES_MULTILINE)
		return true;
	else
		return false;
}

void CRichEditUI::SetMultLine(bool bMultLine /* = true */)
{
	if (bMultLine)
	{
		m_lTwhStyle = m_lTwhStyle|ES_MULTILINE;
	}
	else
	{
		m_lTwhStyle &= ~ES_MULTILINE;
	}
	if (m_pTwh)
	{
		m_pTwh->SetMultLine(bMultLine);
	}
}

void CRichEditUI::SetVscrollbar(bool bVscrollbar)
{
	if (bVscrollbar)
	{
		m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
	}
	else
	{
		m_lTwhStyle &= ~WS_VSCROLL;
		if ( (m_lTwhStyle&WS_HSCROLL)==0)
		{
			m_lTwhStyle &= ~ES_DISABLENOSCROLL;
		}
	}

	if (m_pTwh)
	{
		m_pTwh->SetVscrollbar(bVscrollbar);
	}
}

bool CRichEditUI::GetVscrollbar()
{
	if (m_lTwhStyle & WS_VSCROLL)
	{
		return true;
	}
	else
		return false;
}

void CRichEditUI::SetHscrollbar(bool bHscrollbar)
{
	if (bHscrollbar)
	{
		m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
	}
	else
	{
		m_lTwhStyle &= ~WS_HSCROLL;
		if ( (m_lTwhStyle&WS_VSCROLL)==0)
		{
			m_lTwhStyle &= ~ES_DISABLENOSCROLL;
		}
	}

	if (m_pTwh)
	{
		m_pTwh->SetHscrollbar(bHscrollbar);
	}
}

bool CRichEditUI::GetHsrcollbar()
{
	if (m_lTwhStyle & WS_HSCROLL)
	{
		return true;
	}
	else
		return false;
}

void CRichEditUI::SetAutoVscroll(bool bAutoVscroll)
{
	if (bAutoVscroll)
	{
		m_lTwhStyle |= ES_AUTOVSCROLL;
	}
	else
	{
		m_lTwhStyle &= ~ES_AUTOVSCROLL;
	}

	if (m_pTwh)
	{
		m_pTwh->SetAutoVscroll(bAutoVscroll);
	}
}

bool CRichEditUI::GetAutoVscroll()
{
	if (m_lTwhStyle & ES_AUTOVSCROLL)
	{
		return true;
	}
	else
		return false;
}

void CRichEditUI::SetAutoHscroll(bool bAutoHscroll)
{
	if (bAutoHscroll)
	{
		m_lTwhStyle |= ES_AUTOHSCROLL;
	}
	else
	{
		m_lTwhStyle &= ~ES_AUTOHSCROLL;
	}

	if (m_pTwh)
	{
		m_pTwh->SetAutoHscroll(bAutoHscroll);
	}
}

bool CRichEditUI::GetAutoHscroll()
{
	if (m_lTwhStyle & ES_AUTOHSCROLL)
	{
		return true;
	}
	else
		return false;
}

void CRichEditUI::DoInit()
{
	if(m_bInited)
		return ;

    CREATESTRUCT cs;
    cs.style = m_lTwhStyle;
    cs.x = 0;
    cs.y = 0;
    cs.cy = 0;
    cs.cx = 0;
    cs.lpszName = m_sText.GetData();
    CreateHost(this, &cs, &m_pTwh);
    if( m_pTwh ) {
        if( m_bTransparent ) m_pTwh->SetTransparent(TRUE);
        LRESULT lResult;
        m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
        m_pTwh->OnTxInPlaceActivate(NULL);
        m_pManager->AddMessageFilter(this);
		if( m_pManager->IsLayered() && m_pManager->GetFocus() == this) 
			m_pManager->SetTimer(this, DEFAULT_TIMERID, ::GetCaretBlinkTime());

		TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
		lResult |= ENM_CHANGE;			
		TxSendMessage(EM_SETEVENTMASK, 0,lResult, &lResult);	
    }

	m_bInited= true;
}

HRESULT CRichEditUI::TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const
{
    if( m_pTwh ) {
        if( msg == WM_KEYDOWN && TCHAR(wparam) == VK_RETURN ) {
            if( !m_bWantReturn || (::GetKeyState(VK_CONTROL) < 0 && !m_bWantCtrlReturn) ) {
                if( m_pManager != NULL ) m_pManager->SendNotify((CControlUI*)this, DUI_MSGTYPE_RETURN);
                return S_OK;
            }
        }
        return m_pTwh->GetTextServices()->TxSendMessage(msg, wparam, lparam, plresult);
    }
    return S_FALSE;
}

IDropTarget* CRichEditUI::GetTxDropTarget()
{
    IDropTarget *pdt = NULL;
    if( m_pTwh->GetTextServices()->TxGetDropTarget(&pdt) == NOERROR ) return pdt;
    return NULL;
}

bool CRichEditUI::OnTxViewChanged()
{
    return true;
}

bool CRichEditUI::SetDropAcceptFile(bool bAccept) 
{
	LRESULT lResult;
	TxSendMessage(EM_SETEVENTMASK, 0,ENM_DROPFILES|ENM_LINK, // ENM_CHANGE| ENM_CORRECTTEXT | ENM_DRAGDROPDONE | ENM_DROPFILES | ENM_IMECHANGE | ENM_LINK | ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_REQUESTRESIZE | ENM_SCROLL | ENM_SELCHANGE | ENM_UPDATE,
		&lResult);
	return (BOOL)lResult == FALSE;
}

void CRichEditUI::OnTxNotify(DWORD iNotify, void *pv)
{
	switch(iNotify)
	{ 
	case EN_CHANGE:
		{
			if (m_sText!=GetText())
			{
				GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED);
#ifdef USE_OPENCC
				m_sText = GetText();
#else
				m_sText = GetText();
#endif				
			}
		}
		break;
	case EN_DROPFILES:   
	case EN_MSGFILTER:   
	case EN_OLEOPFAILED:   
	case EN_PROTECTED:   
	case EN_SAVECLIPBOARD:   
	case EN_SELCHANGE:   
	case EN_STOPNOUNDO:   
	case EN_LINK:
		{
			if(pv)                        // Fill out NMHDR portion of pv   
			{   
				//�׳�һ��notify��Ϣ���ӷ��㴦��link
				ENLINK* pEnLink = (ENLINK *)pv;
				if (pEnLink->msg == WM_LBUTTONDOWN)
					GetManager()->SendNotify(this, DUI_MSGTYPE_LINK,0,(LPARAM) pv);

				LONG nId =  GetWindowLong(this->GetManager()->GetPaintWindow(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = this->GetManager()->GetPaintWindow();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				if(SendMessage(this->GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))   
				{   
					//hr = S_FALSE;   
				}   
			}
		}
		break;
	case EN_OBJECTPOSITIONS:   
	case EN_DRAGDROPDONE:   
		{
			if(pv)                        // Fill out NMHDR portion of pv   
			{   
				LONG nId =  GetWindowLong(this->GetManager()->GetPaintWindow(), GWL_ID);   
				NMHDR  *phdr = (NMHDR *)pv;   
				phdr->hwndFrom = this->GetManager()->GetPaintWindow();   
				phdr->idFrom = nId;   
				phdr->code = iNotify;  

				if(SendMessage(this->GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))   
				{   
					//hr = S_FALSE;   
				}   
			}    
		}
		break;
	}
}

// ���з�rich��ʽ��richedit��һ��������bug�������һ���ǿ���ʱ��LineDown��SetScrollPos�޷����������
// ����iPos����Ϊ���������bug
void CRichEditUI::SetScrollPos(SIZE szPos,bool bTriggerEvent/*=true*/)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy,bTriggerEvent);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx,bTriggerEvent);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }
    if( cy != 0 ) {
        int iPos = 0;
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
            iPos = m_pVerticalScrollBar->GetScrollPos();
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pVerticalScrollBar->GetScrollPos());
        TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
        if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
            if( cy > 0 && m_pVerticalScrollBar->GetScrollPos() <= iPos )
                m_pVerticalScrollBar->SetScrollPos(iPos,bTriggerEvent);
        }
    }
    if( cx != 0 ) {
        WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pHorizontalScrollBar->GetScrollPos());
        TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
    }
}

void CRichEditUI::LineUp()
{
    TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
}

void CRichEditUI::LineDown()
{
    int iPos = 0;
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
        iPos = m_pVerticalScrollBar->GetScrollPos();
    TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
    if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        if( m_pVerticalScrollBar->GetScrollPos() <= iPos )
            m_pVerticalScrollBar->SetScrollPos(m_pVerticalScrollBar->GetScrollRange());
    }
}

void CRichEditUI::PageUp()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
}

void CRichEditUI::PageDown()
{
    TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
}

void CRichEditUI::HomeUp()
{
    TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
}

void CRichEditUI::EndDown()
{
    TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
}

void CRichEditUI::LineLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
}

void CRichEditUI::LineRight()
{
    TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
}

void CRichEditUI::PageLeft()
{
    TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
}

void CRichEditUI::PageRight()
{
    TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
}

void CRichEditUI::HomeLeft()
{
    TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
}

void CRichEditUI::EndRight()
{
    TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
}

void CRichEditUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
    {
        if( m_pTwh && m_pTwh->DoSetCursor(NULL, &event.ptMouse) ) {
            return;
        }
    }
    else if( event.Type == UIEVENT_WINDOWSIZE ) {
        if( m_pTwh ) m_pTwh->NeedFreshCaret();
    }
    else if( event.Type == UIEVENT_SETFOCUS ) {
        if( m_pTwh ) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
			if( m_pManager->IsLayered()) 
				m_pManager->SetTimer(this, DEFAULT_TIMERID, ::GetCaretBlinkTime());
        }
		m_bFocused = true;
		Invalidate();
		return;
    }
    if( event.Type == UIEVENT_KILLFOCUS )  {
        if( m_pTwh ) {
            m_pTwh->OnTxInPlaceActivate(NULL);
            m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
			if( m_pManager->IsLayered()) 
				m_pManager->KillTimer(this, DEFAULT_TIMERID);
        }
		m_bFocused = false;
		Invalidate();
		return;
    }
	else if( event.Type == UIEVENT_TIMER ) {
		if( event.wParam == DEFAULT_TIMERID ) {
			if( m_pTwh && m_pManager->IsLayered() && IsFocused() ) {
				if (::GetFocus() != m_pManager->GetPaintWindow()) return;
				m_bDrawCaret = !m_bDrawCaret;
				POINT ptCaret;
				::GetCaretPos(&ptCaret);
				RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x + m_pTwh->GetCaretWidth(), 
					ptCaret.y + m_pTwh->GetCaretHeight() };
				RECT rcTemp = rcCaret;
				if( !::IntersectRect(&rcCaret, &rcTemp, &m_rcItem) ) return;
				CControlUI* pParent = this;
				RECT rcParent;
				while( pParent = pParent->GetParent() ) {
					rcTemp = rcCaret;
					rcParent = pParent->GetPos();
					if( !::IntersectRect(&rcCaret, &rcTemp, &rcParent) ) {
						return;
					}
				}                    
				m_pManager->Invalidate(rcCaret);
			}
			return;
		}
		if( m_pTwh ) {
			m_pTwh->GetTextServices()->TxSendMessage(WM_TIMER, event.wParam, event.lParam, 0);
		} 
		return;
	}
    if( event.Type == UIEVENT_SCROLLWHEEL ) {
        if( (event.wKeyState & MK_CONTROL) != 0  ) {
            return;
        }
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
    {
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE ) 
    {
        return;
    }
    if( event.Type == UIEVENT_BUTTONUP ) 
    {
        return;
    }
    if( event.Type > UIEVENT__KEYBEGIN && event.Type < UIEVENT__KEYEND )
    {
        return;
    }
    CContainerUI::DoEvent(event);
}

SIZE CRichEditUI::EstimateSize(SIZE szAvailable)
{
    //return CDuiSize(m_rcItem); // ���ַ�ʽ�ڵ�һ�����ô�С֮��ʹ�С������
    return CContainerUI::EstimateSize(szAvailable);
}

int CRichEditUI::EstimmateRichEditHeight(const int& nWidth,LPCTSTR lpszText)
{
	if (lpszText==NULL || _tcslen(lpszText)==0)
		return 0;

	RECT rcText = { 0, 0, nWidth, 9999};	
	CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, lpszText, 0, m_iFont, DT_CALCRECT|DT_WORDBREAK);

	int nHeight = (int)((rcText.bottom-rcText.top)*1.3);

	return nHeight;	
}

void CRichEditUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	int oldW = m_rcItem.right-m_rcItem.left;
	int oldH = m_rcItem.bottom-m_rcItem.top;
	if (rc.right-rc.left != oldW)
	{
		if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible())
			m_pHorizontalScrollBar->SetScrollRange(0);
	}
	if (rc.bottom-rc.top != oldH)
	{
		if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
			m_pVerticalScrollBar->SetScrollRange(0);
	}

    CControlUI::SetPos(rc, bNeedInvalidate);
    rc = m_rcItem;

    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    RECT rcScrollView = rc;

    bool bVScrollBarVisiable = false;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        bVScrollBarVisiable = true;
        rc.top -= m_pVerticalScrollBar->GetScrollPos();
        rc.bottom -= m_pVerticalScrollBar->GetScrollPos();
        rc.bottom += m_pVerticalScrollBar->GetScrollRange();
        rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        rcScrollView.right -= m_pVerticalScrollBar->GetFixedWidth();
    }
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        rc.left -= m_pHorizontalScrollBar->GetScrollPos();
        rc.right -= m_pHorizontalScrollBar->GetScrollPos();
        rc.right += m_pHorizontalScrollBar->GetScrollRange();
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
        rcScrollView.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
    }

    if( m_pTwh != NULL ) {
		RECT rcScrollTextView = rcScrollView;
		rcScrollTextView.left += m_rcTextPadding.left;
		rcScrollTextView.right -= m_rcTextPadding.right;
		rcScrollTextView.top += m_rcTextPadding.top;
		rcScrollTextView.bottom -= m_rcTextPadding.bottom;
		RECT rcText = rc;
		rcText.left += m_rcTextPadding.left;
		rcText.right -= m_rcTextPadding.right;
		rcText.top += m_rcTextPadding.top;
		rcText.bottom -= m_rcTextPadding.bottom;
        m_pTwh->SetClientRect(&rcScrollTextView);
        if( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsVisible() || m_bVScrollBarFixing) ) {
            LONG lWidth = rcText.right - rcText.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetManager()->GetPaintDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if( lHeight > rcText.bottom - rcText.top ) {
                m_pVerticalScrollBar->SetVisible(true);
                m_pVerticalScrollBar->SetScrollPos(0);
                m_bVScrollBarFixing = true;
            }
            else {
                if( m_bVScrollBarFixing ) {
                    m_pVerticalScrollBar->SetVisible(false);
                    m_bVScrollBarFixing = false;
                }
            }
        }
    }

    if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rcScrollView.right, rcScrollView.top, 
            rcScrollView.right + m_pVerticalScrollBar->GetFixedWidth(), rcScrollView.bottom};
        m_pVerticalScrollBar->SetPos(rcScrollBarPos, false);
    }
    if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
        RECT rcScrollBarPos = { rcScrollView.left, rcScrollView.bottom, rcScrollView.right, 
            rcScrollView.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
        m_pHorizontalScrollBar->SetPos(rcScrollBarPos, false);
    }

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it);
        }
        else {
			SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy };
			pControl->SetPos(rcCtrl, false);
		}
    }
}

void CRichEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
{
	CContainerUI::Move(szOffset, bNeedInvalidate);
	if( m_pTwh != NULL ) {
		RECT rc = m_rcItem;
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;

		rc.left += m_rcTextPadding.left;
		rc.top += m_rcTextPadding.top;
		rc.right -= m_rcTextPadding.right;
		rc.bottom -= m_rcTextPadding.bottom;

		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
		m_pTwh->SetClientRect(&rc);
	}
}

bool CRichEditUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
    RECT rcTemp = { 0 };
    if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;

    CRenderClip clip;
    CRenderClip::GenerateClip(hDC, rcTemp, clip);
    CControlUI::DoPaint(hDC, rcPaint, pStopControl);

    if( m_pTwh ) {
        RECT rc;
        m_pTwh->GetControlRect(&rc);
        // Remember wparam is actually the hdc and lparam is the update
        // rect because this message has been preprocessed by the window.
        m_pTwh->GetTextServices()->TxDraw(
            DVASPECT_CONTENT,  		// Draw Aspect
            /*-1*/0,				// Lindex
            NULL,					// Info for drawing optimazation
            NULL,					// target device information
            hDC,			        // Draw device HDC
            NULL, 				   	// Target device HDC
            (RECTL*)&rc,			// Bounding client rectangle
            NULL, 		            // Clipping rectangle for metafiles
            (RECT*)&rcPaint,		// Update rectangle
            NULL, 	   				// Call back function
            NULL,					// Call back parameter
            0);				        // What view of the object
        if( m_bVScrollBarFixing ) {
            LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
            LONG lHeight = 0;
            SIZEL szExtent = { -1, -1 };
            m_pTwh->GetTextServices()->TxGetNaturalSize(
                DVASPECT_CONTENT, 
                GetManager()->GetPaintDC(), 
                NULL,
                NULL,
                TXTNS_FITTOCONTENT,
                &szExtent,
                &lWidth,
                &lHeight);
            if( lHeight <= rc.bottom - rc.top ) {
                NeedUpdate();
            }
        }
    }

	if (GetTextLength() == 0 && GetTipValueText().IsEmpty()==false)
	{
		RECT rc = {0,0,0,0},rcTextPadding={0,0,0,0};
		m_pTwh->GetControlRect(&rc);
		if (GetMultLine())
		{
			rcTextPadding = GetTextPadding();
			rc.left += rcTextPadding.left;
			rc.right -= rcTextPadding.right;
			rc.top += rcTextPadding.top;
			rc.bottom -= rcTextPadding.bottom;
		}
		CRenderEngine::DrawText(hDC, m_pManager, rc, GetTipValueText().GetData(), GetTipValueTextColor(), m_iFont, DT_LEFT|DT_VCENTER);
	}

    if( m_items.GetSize() > 0 ) {
        RECT rc = m_rcItem;
        rc.left += m_rcInset.left;
        rc.top += m_rcInset.top;
        rc.right -= m_rcInset.right;
        rc.bottom -= m_rcInset.bottom;
        if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
        if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

        if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
				if( pControl == pStopControl ) return false;
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                }
            }
        }
        else {
            CRenderClip childClip;
            CRenderClip::GenerateClip(hDC, rcTemp, childClip);
            for( int it = 0; it < m_items.GetSize(); it++ ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
				if( pControl == pStopControl ) return false;
                if( !pControl->IsVisible() ) continue;
                if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
                if( pControl->IsFloat() ) {
                    if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
                    CRenderClip::UseOldClipBegin(hDC, childClip);
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                    CRenderClip::UseOldClipEnd(hDC, childClip);
                }
                else {
                    if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
                    if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
                }
            }
        }
    }

	if( m_pTwh && m_pTwh->IsShowCaret() && m_pManager->IsLayered() && IsFocused() && m_bDrawCaret ) {
		POINT ptCaret;
		::GetCaretPos(&ptCaret);
		if( ::PtInRect(&m_rcItem, ptCaret) ) {
			RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y + m_pTwh->GetCaretHeight() };
			CRenderEngine::DrawLine(hDC, rcCaret, m_pTwh->GetCaretWidth(), m_dwTextColor);
		}
	}

    if( m_pVerticalScrollBar != NULL ) {
        if( m_pVerticalScrollBar == pStopControl ) return false;
        if (m_pVerticalScrollBar->IsVisible()) {
            if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
                if( !m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
            }
        }
    }

    if( m_pHorizontalScrollBar != NULL ) {
        if( m_pHorizontalScrollBar == pStopControl ) return false;
        if (m_pHorizontalScrollBar->IsVisible()) {
            if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
                if( !m_pHorizontalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
            }
        }
    }
    return true;
}

void CRichEditUI::PaintStatusImage(HDC hDC)
{
	if( IsFocused() ) 
	{
		if ( DrawImage(hDC, m_diFocused) )
			return ;
	}

	if( !IsEnabled() )
	{
		if ( DrawImage(hDC, m_diDisabled) )
			return ;
	}
}

void CRichEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) 
	{
        if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetVscrollbar(true);
		else
			SetVscrollbar(false);
    }
    if( _tcscmp(pstrName, _T("autovscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetAutoVscroll(true);
		else
			SetAutoVscroll(false);
    }
    else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) 
	{
        if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetHscrollbar(true);
		else
			SetHscrollbar(false);
    }
    if( _tcscmp(pstrName, _T("autohscroll")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetAutoHscroll(true);
		else
			SetAutoHscroll(false);
    }
    else if( _tcscmp(pstrName, _T("wanttab")) == 0 ) {
        SetWantTab(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantreturn")) == 0 ) {
        SetWantReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("wantctrlreturn")) == 0 ) {
        SetWantCtrlReturn(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("transparent")) == 0 ) {
        SetTransparent(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("rich")) == 0 ) {
        SetRich(_tcscmp(pstrValue, _T("true")) == 0);
    }
    else if( _tcscmp(pstrName, _T("multiline")) == 0 ) {
        if( _tcscmp(pstrValue, _T("false")) == 0 )
			SetMultLine(false);
		else
			SetMultLine(true);
    }
    else if( _tcscmp(pstrName, _T("readonly")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) { m_lTwhStyle |= ES_READONLY; m_bReadOnly = true; }
    }
    else if( _tcscmp(pstrName, _T("password")) == 0 ) {
        if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetPassword(true);
		else
			SetPassword(false);
    }
    else if( _tcscmp(pstrName, _T("align")) == 0 ) {
        if( _tcsstr(pstrValue, _T("left")) != NULL ) {
            m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
            m_lTwhStyle |= ES_LEFT;
        }
        if( _tcsstr(pstrValue, _T("center")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
            m_lTwhStyle |= ES_CENTER;
        }
        if( _tcsstr(pstrValue, _T("right")) != NULL ) {
            m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
            m_lTwhStyle |= ES_RIGHT;
        }
    }
    else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
	else if( _tcscmp(pstrName, _T("textpadding")) == 0 ) {
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetTextPadding(rcTextPadding);
	}
	else if( _tcscmp(pstrName, _T("maxchar")) == 0 ) SetLimitText(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
	else if( _tcscmp(pstrName, _T("showcaret")) == 0 ) {
		if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			SetCaret(true);
		else
			SetCaret(false);
	}
	else if( _tcscmp(pstrName, _T("tipvalue")) == 0 ) SetTipValueText(pstrValue);
	else if( _tcscmp(pstrName, _T("tipcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTipValueTextColor(clrColor);
	}
    else CContainerUI::SetAttribute(pstrName, pstrValue);
}

void CRichEditUI::SetCaret(bool bCaret)
{
	if (m_bCaret != bCaret)
	{
		m_bCaret = bCaret;
		if (m_pTwh)
			m_pTwh->TxShowCaret(m_bCaret);

		Invalidate();
	}
}

void CRichEditUI::SetPasswordChar(TCHAR chPasswordChar)
{
#ifdef UNICODE
	if (m_pTwh)
		m_pTwh->SetPasswordChar(chPasswordChar);
#else
	char szValue[16] = {};
	szValue[0] = chPasswordChar;
	int length = strlen(szValue)+1;
	wchar_t wszValue[32] = {0};
	MultiByteToWideChar(CP_ACP,0,szValue,strlen(szValue),wszValue,31);

	if (m_pTwh)
		m_pTwh->SetPasswordChar(wszValue[0]);
#endif	
}

bool CRichEditUI::GetCaret()
{
	return m_bCaret;
}

LRESULT CRichEditUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if( !IsVisible() || !IsEnabled() ) return 0;
    if( !IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) return 0;
    if( uMsg == WM_MOUSEWHEEL && (LOWORD(wParam) & MK_CONTROL) == 0 ) return 0;

    bool bWasHandled = true;
    if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
        if( !m_pTwh->IsCaptured() ) {
            switch (uMsg) {
			case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_LBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                {
                    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                    CControlUI* pHover = GetManager()->FindControl(pt);
                    if(pHover != this) {
                        bWasHandled = false;
                        return 0;
                    }
                }
                break;
            }
        }
        // Mouse message only go when captured or inside rect
        DWORD dwHitResult = m_pTwh->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
        if( dwHitResult == HITRESULT_OUTSIDE ) {
            RECT rc;
            m_pTwh->GetControlRect(&rc);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if( uMsg == WM_SETCURSOR ) {
                ::GetCursorPos(&pt);
                ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
            }
            else if( uMsg == WM_MOUSEWHEEL ) ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
            if( ::PtInRect(&rc, pt) && !GetManager()->IsCaptured() ) dwHitResult = HITRESULT_HIT;
        }
        if( dwHitResult != HITRESULT_HIT ) return 0;
        if( uMsg == WM_SETCURSOR ) bWasHandled = false;
        else if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDOWN ) {
			if (!GetManager()->IsNoActivate()) ::SetFocus(GetManager()->GetPaintWindow());
            SetFocus();
        }
    }
#ifdef _UNICODE
    else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
#else
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR ) {
#endif
        if( !IsFocused() ) return 0;
    }
#ifdef _USEIMM
	else if( uMsg == WM_IME_STARTCOMPOSITION ) {
		if( IsFocused() ) {
			POINT ptCaret;
			::GetCaretPos(&ptCaret);
			HIMC hMic = ::ImmGetContext(GetManager()->GetPaintWindow());
			COMPOSITIONFORM cpf;
			cpf.dwStyle = CFS_FORCE_POSITION;
			cpf.ptCurrentPos.x = ptCaret.x + m_pTwh->GetCaretWidth();
			cpf.ptCurrentPos.y = ptCaret.y;
			::ImmSetCompositionWindow(hMic, &cpf);

			HFONT hFont = GetManager()->GetFont(m_iFont);
			LOGFONT lf;
			::GetObject(hFont, sizeof(LOGFONT), &lf);
			::ImmSetCompositionFont(hMic, &lf);

			::ImmReleaseContext(GetManager()->GetPaintWindow(), hMic);
		}
		bWasHandled = false;
		return 0;
	}
#endif
    else if( uMsg == WM_CONTEXTMENU ) {
		if (IsContextMenuUsed())
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CControlUI* pHover = GetManager()->FindControl(pt);
			if(pHover != this) {
				bWasHandled = false;
				return 0;
			}
			else
			{			
				POINT pt;
				GetCursorPos(&pt);
				HMENU hmenu = CreatePopupMenu();
				AppendMenu(hmenu,MF_STRING,1000,_T("����(&C)"));
				AppendMenu(hmenu,MF_STRING,1001,_T("ճ��(&P)"));
				::SetForegroundWindow(GetManager()->GetPaintWindow());
				int nNum = TrackPopupMenu(hmenu,TPM_RETURNCMD,pt.x,pt.y,NULL,GetManager()->GetPaintWindow(),NULL);
				if (nNum == 1000)
				{
					Copy();
				}
				else if (nNum == 1001)
				{
					Paste();
				}		
			}
		}
    }
    else
    {
        switch( uMsg ) {
        case WM_HELP:
            bWasHandled = false;
            break;
        default:
            return 0;
        }
    }
    LRESULT lResult = 0;
    HRESULT Hr = TxSendMessage(uMsg, wParam, lParam, &lResult);
    if( Hr == S_OK ) bHandled = bWasHandled;
    else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR )
        bHandled = bWasHandled;
    else if( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) {
        if( m_pTwh->IsCaptured() ) bHandled = bWasHandled;
    }
    return lResult;
}



} // namespace DuiLib
