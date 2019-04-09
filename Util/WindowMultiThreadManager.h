#pragma once

/*
用于管理与窗口相关的线程,比如开线程来获取数据并在获取成功后显示在窗口上,获取过程中窗口如果关闭,
则线程可以方便的被自动回收掉;
*/

#include "KCritSec.h"
#include <map>
using namespace std;

typedef void (*PThreadFuncCallback)(ULONG_PTR pParam);

class CWindowMultiThreadManager
{
public:
	CWindowMultiThreadManager(void);
	virtual ~CWindowMultiThreadManager(void);

	//返回线程ID
	DWORD BeginThread(PThreadFuncCallback pFun,ULONG_PTR pParam);
	//回收指定线程id的线程
	void RemoveThread(const DWORD& dwThreadId,bool bTerminate = false);
	//回收全部线程
	void RemoveAllThread();


public:	//类内部使用,外部不用管
	PThreadFuncCallback GetThreadFunc(const DWORD& dwThreadId);
	ULONG_PTR GetThreadParam(const DWORD& dwThreadId);

protected:
	CKCritSec m_lockThreadHandleMap;
	map<DWORD,HANDLE> m_mapThreadHandle;

	CKCritSec m_lockThreadFuncMap;
	map<DWORD,PThreadFuncCallback> m_mapThreadFunc;

	CKCritSec m_lockThreadParamMap;
	map<DWORD,ULONG_PTR> m_mapThreadParam;
};
