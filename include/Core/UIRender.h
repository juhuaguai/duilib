#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class DUILIB_API CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class DUILIB_API CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
	static HBITMAP CreateARGB32Bitmap(HDC hDC, int cx, int cy, COLORREF** pBits);
	static void AdjustImage(bool bUseHSL, TImageInfo* imageInfo, short H, short S, short L);
    static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    static void FreeImage(TImageInfo* bitmap, bool bDelete = true);
	static unsigned char* ParseGifFromMemory(unsigned char const *buffer, int len, int **delays, int *width, int *height, int *layers, int* loop);
	static void FreeGifFromMemory(void* pParseGifFromMemoryRetval);
	static unsigned char* ParseApngFromMemory(unsigned char const *buffer, int len, void **pngframes, int *width, int *height, int *layers,int* loop,bool* bFirstHidden);
	static void FreeApngFromMemory(void* pParseApngFromMemoryRetval);
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcScale9, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
	static bool DrawImage(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
		TDrawInfo& drawInfo);
    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

    // 以下函数中的颜色参数alpha值无效
    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
    static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
    static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
    static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, RECT* pLinks, CDuiString* sLinks, int& nLinkRects, int iDefaultFont, UINT uStyle);
	static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, RECT rc, CControlUI* pStopControl = NULL, DWORD dwFilterColor = 0);
    static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc, DWORD dwFilterColor = 0);
	static SIZE GetTextSize(HDC hDC, CPaintManagerUI* pManager , LPCTSTR pstrText, int iFont, UINT uStyle);
	//nSpecifyWidth/nSpecifyHeight指定宽高都大于0时,直接返回指定的宽高
	static SIZE EstimateTextSize(HDC hDC, CPaintManagerUI* pManager, LPCTSTR pstrText,int iFont, UINT uStyle,bool bShowhtml=false,bool bEnabledEffect=false,int nSpecifyWidth=0,int nSpecifyHeight=0);
	//svg文件转png文件
	static bool SvgFile2PngFile(LPCWSTR pstrSvgFile,LPCWSTR pstrPngFile,const int& nPngWidth,const int& nPngHeight);
	static HBITMAP SvgFile2HBITMAP(const LPCWSTR pstrSvgFile,const int& nBmpWidth,const int& nBmpHeight);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
