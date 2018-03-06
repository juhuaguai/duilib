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
#if !defined(_LIMITSINGLEINSTANCE_H_)
#define _LIMITSINGLEINSTANCE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>

class CSingleInstance
{
protected:
	DWORD  m_dwLastError;
	HANDLE m_hMutex;

public:
	CSingleInstance(char* pszMutexName);
	~CSingleInstance();
	BOOL IsAnotherInstanceRunning();
};

#endif // !defined(_LIMITSINGLEINSTANCE_H_)
