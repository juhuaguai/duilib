//CKCritSec.h
//
#ifndef __KCRITSEC_H__
#define __KCRITSEC_H__

#include <windows.h>

#pragma warning(disable:4800)

class CKCritSec
{
public:
	CKCritSec() 
	{
		InitializeCriticalSection(&m_CritSec);
	};

    ~CKCritSec() 
	{
		DeleteCriticalSection(&m_CritSec);         
    };

    void Lock() 
	{
		EnterCriticalSection(&m_CritSec);
    };

    void Unlock() 
	{
		LeaveCriticalSection(&m_CritSec);
    };

protected:
		CRITICAL_SECTION    m_CritSec;
};


class CKAutoLock
{
public:
    CKAutoLock(CKCritSec * plock)
    {
        m_pLock = plock;
		if (m_pLock)
		{
			m_pLock->Lock();
		}
    };

    ~CKAutoLock() {
		if (m_pLock)
		{
			m_pLock->Unlock();
		}
    };

protected:
    CKCritSec * m_pLock;
};


class CKEvent
{
public:
    CKEvent(bool bManualReset = false)
	{
		m_hEvent = CreateEvent(NULL, bManualReset, false, NULL);
	};

    virtual ~CKEvent()
	{
		CloseHandle(m_hEvent);
	};

public:
    bool Set(void) 
	{
		bool    bRet = false;

		bRet = ::SetEvent(m_hEvent);
		return bRet;
	};

    bool Wait(unsigned long ulTimeout)
	{
		if (ulTimeout == 0)
			ulTimeout = 0xffffffff;

		bool    bRet = false;

		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvent, ulTimeout))
			bRet = true;
		return bRet;
    };

    bool Reset(void) 
	{ 
		bool    bRet = false;

		bRet = ::ResetEvent(m_hEvent);
		return bRet;
	};

protected:
	HANDLE  m_hEvent;
};

#endif

