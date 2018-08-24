#pragma once
#include "..\DuiLib\Core\UIControl.h"
#include <mutex>
#include <condition_variable>

enum StylePresets
{
	Custom,
	MacOSX,
	Firefox,
	IE7
};

class CLoadingCircle :
	public CControlUI
{
public:
	CLoadingCircle();
	~CLoadingCircle();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual void PaintBkImage(HDC hDC);
	void Start();
	void Stop();
protected:
	virtual void Init();
	void ThreadAni();
	Color* GenerateColorsPallet(Color* _objColor, bool _blnShadeColor, int _intNbSpoke);
	Color* GenerateColorsPallet(Color _objColor, bool _blnShadeColor, int _intNbSpoke);

protected:
	int                 m_nNumber;
	int                 m_nTime;
	std::atomic<bool>   m_bStop;
	std::atomic<bool>   m_bExit;
	std::thread*        m_pTrdAni;

	int m_NumberOfSpoke = 10;//辐条数量
	int m_SpokeThickness = 4;//辐条粗细
	int m_ProgressValue;//------------------------
	int m_OuterCircleRadius = 10;//外圈
	int m_InnerCircleRadius = 8;//内圈
	PointF m_CenterPoint;//二维平面的点
	Color m_Color;
	Color* m_Colors;
	double* m_Angles;

	std::condition_variable m_condQueue;
	std::mutex m_mutx;
	std::unique_lock<std::mutex>* m_pLock;
};

CControlUI* CreateLoadingControl(LPCTSTR pstrType);
