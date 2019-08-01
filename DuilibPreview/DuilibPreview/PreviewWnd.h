#pragma once
#include "WinImplBase.hpp"

class CPreviewWnd : public WindowImplBase 
{ 
public: 
	CPreviewWnd() { }; 
	~CPreviewWnd() { }; 

	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void OnFinalMessage( HWND );
	virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void SetSkinFile(CDuiString);
	virtual void SetSkinFolder(CDuiString);

private: 
	CDuiString m_skinFolder;
	CDuiString m_skinFile;
}; 