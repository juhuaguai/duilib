
#include "WindowMultiThreadManager.h"
#include <process.h>

extern DWORD WINAPI WindowMultiThreadManagerThreadFunc(LPVOID pParam);

CWindowMultiThreadManager::CWindowMultiThreadManager(void)
{
}

CWindowMultiThreadManager::~CWindowMultiThreadManager(void)
{
	//RemoveAllThread();
}

DWORD CWindowMultiThreadManager::BeginThread(PThreadFuncCallback pFun,ULONG_PTR pParam)
{
	unsigned int dwThrdId = 0;
	HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))WindowMultiThreadManagerThreadFunc,this,CREATE_SUSPENDED,&dwThrdId);
	
	{
		CKAutoLock theLock(&m_lockThreadHandleMap);
		m_mapThreadHandle[dwThrdId] = hThread;

		CKAutoLock theLock1(&m_lockThreadFuncMap);
		m_mapThreadFunc[dwThrdId] = pFun;

		CKAutoLock theLock2(&m_lockThreadParamMap);
		m_mapThreadParam[dwThrdId] = pParam;
	}	

	ResumeThread(hThread);

	return dwThrdId;
}

PThreadFuncCallback CWindowMultiThreadManager::GetThreadFunc(const DWORD& dwThreadId)
{
	CKAutoLock theLock(&m_lockThreadFuncMap);
	map<DWORD,PThreadFuncCallback>::iterator itrFind1 = m_mapThreadFunc.find(dwThreadId);
	if (itrFind1!=m_mapThreadFunc.end())
		return itrFind1->second;

	return NULL;
}
ULONG_PTR CWindowMultiThreadManager::GetThreadParam(const DWORD& dwThreadId)
{
	CKAutoLock theLock(&m_lockThreadParamMap);
	map<DWORD,ULONG_PTR>::iterator itrFind2 = m_mapThreadParam.find(dwThreadId);
	if (itrFind2!=m_mapThreadParam.end())
		return itrFind2->second;

	return NULL;
}

DWORD WINAPI WindowMultiThreadManagerThreadFunc(LPVOID pParam)
{
	CWindowMultiThreadManager* pThis = (CWindowMultiThreadManager*)pParam;
	DWORD dwThreadId = GetCurrentThreadId();
	
	PThreadFuncCallback pFunc = pThis->GetThreadFunc(dwThreadId);
	ULONG_PTR pData = pThis->GetThreadParam(dwThreadId);
	if (pFunc)
		pFunc(pData);
	
	pThis->RemoveThread(dwThreadId);
	return 0;
}

void CWindowMultiThreadManager::RemoveThread(const DWORD& dwThreadId,bool bTerminate /* = false */)
{
	CKAutoLock theLock(&m_lockThreadHandleMap);
	map<DWORD,HANDLE>::iterator itrFind = m_mapThreadHandle.find(dwThreadId);
	if (itrFind!=m_mapThreadHandle.end())
	{
		if (bTerminate)
			TerminateThread(itrFind->second,0);

		CloseHandle(itrFind->second);
		m_mapThreadHandle.erase(itrFind);
	}	

	CKAutoLock theLock1(&m_lockThreadFuncMap);
	map<DWORD,PThreadFuncCallback>::iterator itrFind1 = m_mapThreadFunc.find(dwThreadId);
	if (itrFind1!=m_mapThreadFunc.end())
		m_mapThreadFunc.erase(itrFind1);	

	CKAutoLock theLock2(&m_lockThreadParamMap);
	map<DWORD,ULONG_PTR>::iterator itrFind2 = m_mapThreadParam.find(dwThreadId);
	if (itrFind2!=m_mapThreadParam.end())
		m_mapThreadParam.erase(itrFind2);
}

void CWindowMultiThreadManager::RemoveAllThread()
{
	CKAutoLock theLock(&m_lockThreadHandleMap);

	for (map<DWORD,HANDLE>::iterator itr=m_mapThreadHandle.begin();itr!=m_mapThreadHandle.end();itr++)
	{
		try
		{
			TerminateThread(itr->second,0);
			CloseHandle(itr->second);
		}
		catch (...)
		{
			continue;
		}		
	}
	m_mapThreadHandle.clear();
}
