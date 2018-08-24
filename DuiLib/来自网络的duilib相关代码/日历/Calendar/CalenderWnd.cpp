#include "stdafx.h"
#include "CalenderWnd.h"


CalenderWnd::CalenderWnd(POINT pt)
    : m_pt(pt)
    , m_nYearOffset(0)
    , m_nMonthOffset(0)
{
    
}

CalenderWnd::~CalenderWnd(void)
{
}

LPCTSTR CalenderWnd::GetWindowClassName() const
{
    return _T("CalenderWnd");
}

UINT CalenderWnd::GetClassStyle() const
{
    return WindowImplBase::GetClassStyle() | CS_DROPSHADOW;
}

CDuiString CalenderWnd::GetSkinFile()
{
    return _T("DateTime.xml");
}

CDuiString CalenderWnd::GetSkinFolder()
{
    return _T("");
}

void CalenderWnd::InitWindow()
{
    ::SetWindowPos(*this, NULL, m_pt.x, m_pt.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

    InitCtrlPtr();

    m_cTime = COleDateTime::GetCurrentTime();
    int nYear = m_cTime.GetYear();
    int nMonth = m_cTime.GetMonth();

    m_nYearOffset = m_cTime.GetYear();
    m_nMonthOffset = m_cTime.GetMonth();

    CString strYears;
    strYears.Format(_T("%d年%d月"), nYear, nMonth);
    m_pBtnYears->SetText(strYears);

    DrawCalendar();
}

void CalenderWnd::InitCtrlPtr()
{
    m_pBtnYears = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnYears")));
    assert(m_pBtnYears);

    m_pBtnBeforeY = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnBeforeYear")));
    assert(m_pBtnBeforeY);

    m_pBtnNextY = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnNextYear")));
    assert(m_pBtnNextY);

    m_pBtnBeforeM = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnBeforeMonth")));
    assert(m_pBtnBeforeM);

    m_pBtnNextM = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnNextMonth")));
    assert(m_pBtnNextM);
}

LRESULT CalenderWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    //add your msg
    if (uMsg == WM_KILLFOCUS)
    {
        Close(IDCANCEL);
    }

    if (uMsg == WM_MOUSEWHEEL)
    {
        int nData = GET_WHEEL_DELTA_WPARAM(wParam);
        DUITRACE(_T("WHEEL:%d"), nData);

        if (nData < 0)
        {
            SetNextMonth();
        }
        else
        {
            SetBeforMonth();
        }
    }
    return 0;
}

void CalenderWnd::Notify(TNotifyUI &msg)
{
    //add your nofity
    if (msg.sType == DUI_MSGTYPE_CLICK)
    {
        if (msg.pSender->GetName() == _T("btnBeforeYear"))
        {
            m_nYearOffset--;
            DrawCalendar();
        }
        
        if (msg.pSender->GetName() == _T("btnNextYear"))
        {
            m_nYearOffset++;
            DrawCalendar();
        }

        if (msg.pSender->GetName() == _T("btnBeforeMonth"))
        {
            SetBeforMonth();
        }

        if (msg.pSender->GetName() == _T("btnNextMonth"))
        {
            SetNextMonth();
        }

        for (int i = 0; i < 42; i++)
        {
            CString strBtnName;
            strBtnName.Format(_T("btnDay%d"), i+1);
            if (msg.pSender->GetName() == strBtnName.GetBuffer())
            {
                int wParam =  MAKELONG(m_vectTime[i].wYear, m_vectTime[i].wMonth);
                int lParam = m_vectTime[i].wDay;
                PostMessage(MSG_SELECT_TIME, wParam, lParam);
                Close();
            }
        }
    }

    __super::Notify(msg);
}

void CalenderWnd::SetBeforMonth()
{
    m_nMonthOffset--;
    if (m_nMonthOffset < 1)
    {
        m_nMonthOffset = 12;
        m_nYearOffset--;
    }
    DrawCalendar();
}

void CalenderWnd::SetNextMonth()
{
    m_nMonthOffset++;
    if (m_nMonthOffset > 12)
    {
        m_nMonthOffset = 1;
        m_nYearOffset++;
    }
    DrawCalendar();
}

void CalenderWnd::OnFinalMessage( HWND hWnd )
{
    WindowImplBase::OnFinalMessage(hWnd);

    delete this;
}

void CalenderWnd::DrawCalendar()
{
    m_vectTime.clear();

    int iDay = 0;
    int iNextMonthDays = 0;
    //获取这个月1号是星期几
    COleDateTime cTime;
    
    DUITRACE(_T("Year Offset:%d, Month Offset:%d"), m_nYearOffset, m_nMonthOffset);
    cTime.SetDate(m_nYearOffset, m_nMonthOffset, 1);
    DUITRACE(_T("%d-%d-%d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());

    CString strYears;
    strYears.Format(_T("%d年%d月"), cTime.GetYear(), cTime.GetMonth());
    m_pBtnYears->SetText(strYears);

    int nStartDay = cTime.GetDayOfWeek()-1;
    if (nStartDay == 1)
    {
        nStartDay += 7;
    }
    //这个月天数
    int nDaysInMonth = GetMonthDays(cTime.GetYear(), cTime.GetMonth());
    //上个月显示在上一页的天数
    int nDaysInBeforMonth = GetMonthDays(cTime.GetYear(), cTime.GetMonth()-1) - nStartDay - 1;

    CButtonUI *pBtn = nullptr;
    for (int i = 0; i < 42; i++)
    {
        CString strBtnName;
        strBtnName.Format(_T("btnDay%d"), i+1);
        pBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(strBtnName));
        assert(pBtn);
        
        CString strDays;
        if (i < nStartDay - 1)
        {
            nDaysInBeforMonth++;
            strDays.Format(_T("%d"), nDaysInBeforMonth);
            pBtn->SetText(strDays);
            pBtn->SetEnabled(false);

            SYSTEMTIME sysTime;
            sysTime.wYear = cTime.GetYear();
            sysTime.wMonth = cTime.GetMonth() - 1;
            sysTime.wDay = nDaysInBeforMonth;
            m_vectTime.push_back(sysTime);
        }
        else if (i >= nStartDay - 1 && iDay < nDaysInMonth)
        {
            iDay++;
            strDays.Format(_T("%d"), iDay);
            pBtn->SetText(strDays);
            pBtn->SetEnabled(true);

            

            if (iDay == m_cTime.GetDay() && m_cTime.GetYear() == cTime.GetYear() && m_cTime.GetMonth() == cTime.GetMonth())
            {
                pBtn->SetAttribute(_T("bkcolor"), _T("#FFFF0000"));
            }
            else
            {
                pBtn->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
                DrawTagTime(pBtn, cTime.GetYear(), cTime.GetMonth(), iDay);
            }

            SYSTEMTIME sysTime;
            sysTime.wYear = cTime.GetYear();
            sysTime.wMonth = cTime.GetMonth();
            sysTime.wDay = iDay;
            m_vectTime.push_back(sysTime);
        }
        else
        {
            iNextMonthDays++;
            strDays.Format(_T("%d"), iNextMonthDays);
            pBtn->SetText(strDays);
            pBtn->SetEnabled(false);

            SYSTEMTIME sysTime;
            sysTime.wYear = cTime.GetYear();
            sysTime.wMonth = cTime.GetMonth() + 1;
            sysTime.wDay = iNextMonthDays;
            m_vectTime.push_back(sysTime);
        }
    }
}

int CalenderWnd::GetMonthDays(int iY,int iM)
{
    int iTotalDay = 31;        
    if(iM == 2) 
    {
        //闰年可以被4或者400整除 但是不能被100整除
        if(iY % 4 ==0 && iY % 100 !=0 || iY % 400 ==0) 
            iTotalDay= 29;
        else
            iTotalDay=28;
    }
    else if(iM==4 || iM==6 || iM == 9||iM==11)
    {
        iTotalDay = 30;
    }
    return iTotalDay;
}

void CalenderWnd::DrawTagTime(CButtonUI *pBtn, int nYear, int nMonth, int nDay)
{
    if (m_vectTagTime.size() == 0 || NULL == pBtn)
    {
        return;
    }

    auto itor =  std::find_if(m_vectTagTime.begin(), m_vectTagTime.end(), [=](SYSTEMTIME sysTime){
        if (sysTime.wYear == nYear && sysTime.wMonth == nMonth && sysTime.wDay == nDay)
        {
            return true;
        }
        else
        {
            return false;
        }
    });

    if (itor != m_vectTagTime.end())
    {
        pBtn->SetAttribute(_T("bkcolor"), _T("#FF00FFFF"));
    }
    else
    {
        pBtn->SetAttribute(_T("bkcolor"), _T("#FFFFFFFF"));
    }
}

void CalenderWnd::SetTagTime(std::vector<SYSTEMTIME> &vectTime)
{
    m_vectTagTime.clear();
    m_vectTagTime = vectTime;
}