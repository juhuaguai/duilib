#pragma once

class CMainWnd : public CWindowWnd, public INotifyUI ,public IDialogBuilderCallback
{
public:
	CMainWnd();
	~CMainWnd();

	virtual LPCTSTR GetWindowClassName() const { return _T("CMainWnd"); }
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam); 
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	UINT GetClassStyle() const { return CS_DBLCLKS; };	//²Î¿¼360DEMO

	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitDlg();	

	void ChangeLanguage(const int& nLanguageID);


protected:
	CPaintManagerUI m_PM;

};