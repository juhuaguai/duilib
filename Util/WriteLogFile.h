//写日志的类
#pragma once

#include <Windows.h>
#include "StringTools.h"

using namespace std;

class CWriteLogFile
{
public:
	//pszLogFileName日志文件名字,nLogFileMaxSize-日志文件最大大小(KB),默认1024KB,超过大小时下次调用构造函数时会删除日志文件
	CWriteLogFile(const xstring& strLogFileName,int nLogFileMaxSize = 1024);
	CWriteLogFile();
	virtual ~CWriteLogFile();
	
	//pszLogFileName日志文件名字,nLogFileMaxSize-日志文件最大大小(KB),默认1024KB,超过大小时下次调用构造函数时会删除日志文件
	void SetLogFilePathName(const xstring& strLogFileName,int nLogFileMaxSize = 1024);
	////写日志 bPrintToConsole-是否输出到控制台
	//void WriteLog(bool bPrintToConsole,LPCTSTR format, ...);
	void WriteLogA(bool bPrintToConsole,LPCSTR format, ...);
	void WriteLogW(bool bPrintToConsole,LPCWSTR format, ...);

protected:
	//获取日志文件当前大小
	DWORD GetLogFileCurSize(const xstring& strLogFileName);

private:
	HANDLE m_hEvent;
	xstring m_strFileName;
	DWORD m_dwLogFileMaxSize;		//日志文件最大大小(KB)
};