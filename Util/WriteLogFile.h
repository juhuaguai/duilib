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
	//设置是否写到日志文件中
	void SetWriteLog(bool bIsWrite);	
	//lpszCodeFile使用宏__FILEW__, nCodeLine使用宏__LINE__
	void WriteLogPrintW(LPCWSTR lpszCodeFile,const int& nCodeLine,LPCWSTR format, ...);
	//lpszCodeFile使用宏__FILEW__, nCodeLine使用宏__LINE__
	void WriteLogW(LPCWSTR lpszCodeFile,const int& nCodeLine,LPCWSTR lpszLog);

protected:
	//获取日志文件当前大小
	DWORD GetLogFileCurSize(const wstring& strLogFileName);

private:
	wstring m_wstrFileName;
	DWORD m_dwLogFileMaxSize;		//日志文件最大大小(KB)
	bool m_bWrite;

	CRITICAL_SECTION    m_CritSec;
};