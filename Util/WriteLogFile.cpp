
#include "WriteLogFile.h"
#include <time.h>
#include <io.h>

CWriteLogFile::CWriteLogFile(const xstring& strLogFileName,int nLogFileMaxSize /* = 1024 */)
{
	try
	{
		//开始的时候检测一下文件大小
		int nFileSize = (GetLogFileCurSize(strLogFileName));
		if ( nFileSize>nLogFileMaxSize*1024 || nFileSize==0)
			DeleteFile(strLogFileName.c_str());
		
		m_strFileName = strLogFileName;
		m_hEvent = CreateEvent(NULL,TRUE,TRUE,NULL);	//需要手动设置有/无信号,初始有信号

		m_dwLogFileMaxSize = nLogFileMaxSize;
	}
	catch (...)
	{
		if (m_hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(m_hEvent);
		m_hEvent = INVALID_HANDLE_VALUE;
	}
}

CWriteLogFile::CWriteLogFile()
{
	try
	{
		m_hEvent = CreateEventA(NULL,TRUE,TRUE,NULL);	//需要手动设置有/无信号,初始有信号
	}
	catch (...)
	{
		if (m_hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(m_hEvent);
		m_hEvent = INVALID_HANDLE_VALUE;
	}
}

void CWriteLogFile::SetLogFilePathName(const xstring& strLogFileName,int nLogFileMaxSize /* = 1024 */)
{
	//开始的时候检测一下文件大小
	int nFileSize = (GetLogFileCurSize(strLogFileName));
	if ( nFileSize>nLogFileMaxSize*1024 || nFileSize==0)
		DeleteFile(strLogFileName.c_str());

	m_strFileName = strLogFileName;
	m_dwLogFileMaxSize = nLogFileMaxSize;
}

CWriteLogFile::~CWriteLogFile()
{
	if (m_hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(m_hEvent);
	m_hEvent = INVALID_HANDLE_VALUE;
};

DWORD CWriteLogFile::GetLogFileCurSize(const xstring& strLogFileName)
{
	try
	{
		WIN32_FIND_DATA fileInfo;
		HANDLE hFind;
		DWORD fileSize=0x0;
		hFind = FindFirstFile(strLogFileName.c_str() ,&fileInfo);
		if(hFind != INVALID_HANDLE_VALUE)
			fileSize = fileInfo.nFileSizeLow;
		FindClose(hFind);
		
		//换另外一种测试文件大小的方法再次获取
		FILE* file = NULL;
#ifdef UNICODE
		fopen_s(&file,_A(strLogFileName), "r");
#else
		fopen_s(&file,strLogFileName.c_str(), "r");
#endif		
		if (file)
		{
			int nSize = _filelength(_fileno(file));
			fclose(file);
			if (fileSize != nSize)
				return nSize;
		}
		return fileSize;
	}
	catch (...)
	{
		return 0;
	}
}

void CWriteLogFile::WriteLog(bool bPrintToConsole,LPCTSTR format, ...)
{
	WaitForSingleObject(m_hEvent,3000);
	if (m_hEvent == INVALID_HANDLE_VALUE)
		return ;
	ResetEvent(m_hEvent);
	try
	{
		if ( GetLogFileCurSize(m_strFileName)>m_dwLogFileMaxSize*1024)
			DeleteFile(m_strFileName.c_str());

		TCHAR sLogOutput[4096] = {0};
		xstring strLog;
		//时间前缀
		time_t nTime;  
		struct tm tmDT;  
		TCHAR sDateTime[128] = {0};  
		time(&nTime);  
		localtime_s(&tmDT,&nTime);  
		_tcsftime(sDateTime,_countof(sDateTime),_T("[%Y-%m-%d %H:%M:%S]"),&tmDT);//strftime(sLogOutput, 128, "[%Y-%m-%d %H:%M:%S]",tmDateTime);
		strLog = sDateTime;
		//内容
		va_list arg_ptr;  
		va_start(arg_ptr,format);
		_vsntprintf_s(sLogOutput,_countof(sLogOutput),_TRUNCATE, format, arg_ptr);
		va_end(arg_ptr);
		strLog.append(sLogOutput);
		//换行
		strLog.append(_T("\r\n"));
		//输出
		FILE* fp = NULL;
#ifdef UNICODE
		fopen_s(&fp,_A(m_strFileName),"a+");
#else
		fopen_s(&fp,m_strFileName.c_str(),"a+");
#endif		
		if (fp)
		{
			_ftprintf(fp,strLog.c_str());
			fclose(fp);
		}
		if (bPrintToConsole)
			OutputDebugString(strLog.c_str());
	}
	catch (...)
	{}
	SetEvent(m_hEvent);
}