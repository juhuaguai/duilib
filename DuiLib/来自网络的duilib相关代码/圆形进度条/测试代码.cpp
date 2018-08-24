void CMainFrame::OnClick(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
 
        if (sCtrlName == _T("start"))
	{
		CCircleProgressUI* progress = static_cast<CCircleProgressUI*>(m_PaintManager.FindControl(L"progress1"));
		progress->SetValue(1);
		CString sAttrs;
		bool bChecked = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"direction"))->GetCheck();
		sAttrs.Format(L"clockwise=\"%s\"", bChecked ? L"true" : L"false");
		progress->ApplyAttributeList(sAttrs);
 
		progress = static_cast<CCircleProgressUI*>(m_PaintManager.FindControl(L"progress2"));
		progress->SetValue(1);
		bChecked = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(L"direction"))->GetCheck();
		sAttrs.Format(L"clockwise=\"%s\"", bChecked ? L"true" : L"false");
		progress->ApplyAttributeList(sAttrs);
		m_PaintManager.SetTimer(m_PaintManager.GetRoot(), IDT_PROGRESS, 100);
	}
}
 
void CMainFrame::OnTimer(TNotifyUI& msg)
{
    switch (msg.wParam)
    {
    case IDT_PROGRESS:
        {
            CCircleProgressUI* progress = static_cast<CCircleProgressUI*>(m_PaintManager.FindControl(L"progress1"));
            progress->SetValue(progress->GetValue() + 1);
            CString percent;
            percent.Format(L"%d%%", 100 * (progress->GetValue() - progress->GetMinValue()) / (progress->GetMaxValue() - progress->GetMinValue()));
            progress->SetText(percent);
 
            progress = static_cast<CCircleProgressUI*>(m_PaintManager.FindControl(L"progress2"));
            progress->SetValue(progress->GetValue() + 1);
            percent.Format(L"%d%%", 100 * (progress->GetValue() - progress->GetMinValue()) / (progress->GetMaxValue() - progress->GetMinValue()));
            progress->SetText(percent);
        }
    default:
        ;        
    }
}
