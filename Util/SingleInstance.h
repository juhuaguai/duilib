//---------------------------------------------------------------------------
//
// LimitSingleInstance.h
//
// SUBSYSTEM:   Hook system
//				
// MODULE:      Hook server
//
// DESCRIPTION: Controls number of the process instances
//              This code is from Q243953 in case you lose the article 
//              and wonder where this code came from...
//             
//
// AUTHOR:		Ivo Ivanov (ivopi@hotmail.com)
// DATE:		2001 December v1.00
//
//---------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class CSingleInstance
{
protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;

public:
	CSingleInstance(const string& strMutexName);
	~CSingleInstance();
	BOOL IsAnotherInstanceRunning();
};


