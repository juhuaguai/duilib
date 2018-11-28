//写日志的类
#pragma once

#include <Windows.h>
#include "StringTools.h"

using namespace std;

class CWriteLogFile
{
public:
	//pszLogFileName日志文件名字,nLogFileMaxSize-日志文件最大大小(KB),默认1024KB,超过大小时下次调用构造函数时会删除日志文件
	CWriteLogFile(const wstring& strLogFileName,int nLogFileMaxSize = 1024);
	CWriteLogFile();
	virtual ~CWriteLogFile();
	
	//pszLogFileName日志文件名字,nLogFileMaxSize-日志文件最大大小(KB),默认1024KB,超过大小时下次调用构造函数时会删除日志文件
	void SetLogFilePathName(const wstring& strLogFileName,int nLogFileMaxSize = 1024);
	wstring GetLogFilePathName();
	void WriteLogW(bool bPrintToConsole,LPCWSTR format, ...);
	void SetWriteLog(bool bIsWrite);	

protected:
	//获取日志文件当前大小
	DWORD GetLogFileCurSize(const wstring& strLogFileName);

private:
	HANDLE m_hEvent;
	wstring m_wstrFileName;
	DWORD m_dwLogFileMaxSize;		//日志文件最大大小(KB)
	bool m_bWrite;
};