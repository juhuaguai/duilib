#pragma once

class CMainWnd : public CWindowWnd, public INotifyUI,public IDialogBuilderCallback//继承了IDialogBuilderCallback用于统一识别创建自定义的控件
{
public:
	CMainWnd();
	~CMainWnd();

	LPCTSTR GetWindowClassName() const { return _T("CMainWnd"); }
	UINT GetClassStyle() const { return CS_DBLCLKS; };	//参考360DEMO
	void OnFinalMessage(HWND /*hWnd*/) { /*delete this;*/ };
	void Notify(TNotifyUI& msg);

	//IDialogBuilderCallback中用于统一识别创建自定义控件的接口
	CControlUI* CreateControl(LPCTSTR pstrClass);

public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
public:
	void InitDlg();

public:
	CPaintManagerUI m_PM;
	
	CButtonUI* m_pTestBtn;
};