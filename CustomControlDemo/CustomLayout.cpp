#include "StdAfx.h"
#include "CustomLayout.h"

CCustomLayoutUI::CCustomLayoutUI(void)
{
}

CCustomLayoutUI::~CCustomLayoutUI(void)
{
}

LPCTSTR CCustomLayoutUI::GetClass() const
{
	return _T("CustomLayout");
}
LPVOID CCustomLayoutUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("CustomLayout")) == 0 ) return static_cast<CCustomLayoutUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}
UINT CCustomLayoutUI::GetControlFlags() const
{
	return CContainerUI::GetControlFlags();
}

void CCustomLayoutUI::DoInit()
{
	CDialogBuilder builder;
	CContainerUI* pChildWindow = static_cast<CHorizontalLayoutUI*>(builder.Create(_T("Header.xml"), (UINT)0, NULL, m_pManager));
	if (pChildWindow)
	{
		this->Add(pChildWindow);
		m_pManager->AddNotifier(this);
	}
	else
	{
		this->RemoveAll();
	}

	m_pMinBtn = static_cast<CButtonUI*>(m_pManager->FindControl(_T("min_btn")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_pManager->FindControl(_T("close_btn")));
}
void CCustomLayoutUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender == m_pMinBtn)
		{
			MessageBox(m_pManager->GetPaintWindow(),_T("点击了min_btn按钮"),_T("提示"),MB_OK);
		}
		else if (msg.pSender == m_pCloseBtn)
		{
			MessageBox(m_pManager->GetPaintWindow(),_T("点击了close_btn按钮"),_T("提示"),MB_OK);
		}
	}
}