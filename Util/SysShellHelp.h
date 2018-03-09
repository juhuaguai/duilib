#pragma once

#include <Windows.h>
#include <string>
#include <shellapi.h>
#include <shlobj.h>
#include <io.h>
#include <tlhelp32.h> //½ø³ÌÃ¶¾Ù
#include <tchar.h>
#include <atlbase.h>
using namespace std;

#ifndef xstring 
#ifdef UNICODE
#define xstring wstring
#else
#define xstring string
#endif
#endif

class CSysShellHelp
{
public:
	CSysShellHelp(void);
	~CSysShellHelp(void);

	void RunCmd(string& strCmdText);
	BOOL EnableDebugPriv();

	int ExeFile(const string& strExePath, const string& strCmd, int nTimeout = INFINITE);

	int  Source2File(const char* sFname, HMODULE hModule, DWORD dwID, const char* sType);

	BOOL  Is64Bit_OS();
	void GetSystemName(char* pszRet, unsigned uSize);
	BOOL CheckIEVer(int& nVerNum);

	void DisableSafetyWarn();

	BOOL KillProcess(const xstring &strAppExeName, BOOL bIsAll = TRUE);
	void StartExeByCmd(const char* szCmd);
};

extern BOOL SystemShutDown();
extern BOOL SystemReboot();
extern std::string WChar2Ansi(LPCWSTR wstr);
extern std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
