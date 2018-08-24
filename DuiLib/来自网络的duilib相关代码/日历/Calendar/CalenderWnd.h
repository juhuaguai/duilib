#pragma once
#include <algorithm>

#define MSG_SELECT_TIME (WM_USER + 9871)

class CalenderWnd : public WindowImplBase
{
public:
    CalenderWnd(POINT pt);
    virtual ~CalenderWnd(void);

    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    CDuiString GetSkinFile();
    CDuiString GetSkinFolder();
    void InitWindow();
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    void Notify(TNotifyUI &msg);
    void OnFinalMessage( HWND hWnd );

    void SetTagTime(std::vector<SYSTEMTIME> &vectTime);

private:
    void InitCtrlPtr();
    void DrawCalendar();
    int GetMonthDays(int iY,int iM);
    void SetBeforMonth();
    void SetNextMonth();
    void DrawTagTime(CButtonUI *pBtn, int nYear, int nMonth, int nDay);
    

private:
    CButtonUI *m_pBtnYears;
    CButtonUI *m_pBtnBeforeY;
    CButtonUI *m_pBtnNextY;
    CButtonUI *m_pBtnBeforeM;
    CButtonUI *m_pBtnNextM;

private:
    POINT m_pt;
    SYSTEMTIME m_sysTime;
    COleDateTime m_cTime;
    int m_nYearOffset;
    int m_nMonthOffset;
    std::vector<SYSTEMTIME> m_vectTagTime;//需要标记的时间
    std::vector<SYSTEMTIME> m_vectTime;//当前页的时间
};

