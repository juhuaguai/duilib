
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
			long nSize = _filelength(_fileno(file));
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

#if 0
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
		_tfopen_s(&fp,m_strFileName.c_str(),_T("ab+"));	
		if (fp)
		{

#ifdef UNICODE
			//_wsetlocale(0, L"chs");
			string strAnsiLog = UnicodeToAnsi(strLog);
			fwrite(strAnsiLog.c_str(),strAnsiLog.length(),1,fp);
#else
			//setlocale(0, L"chs");
			fwrite(strLog.c_str(),strLog.length(),1,fp);
#endif

			//_ftprintf(fp,strLog.c_str());
			fclose(fp);
		}
		if (bPrintToConsole)
			OutputDebugString(strLog.c_str());
	}
	catch (...)
	{}
	SetEvent(m_hEvent);
}
#endif

void CWriteLogFile::WriteLogA(bool bPrintToConsole,LPCSTR format, ...)
{
	WaitForSingleObject(m_hEvent,3000);
	if (m_hEvent == INVALID_HANDLE_VALUE)
		return ;
	ResetEvent(m_hEvent);
	try
	{
		if ( GetLogFileCurSize(m_strFileName)>m_dwLogFileMaxSize*1024)
			DeleteFile(m_strFileName.c_str());

		char sLogOutput[10240] = {0};
		string strLog;
		//时间前缀
		time_t nTime;  
		struct tm tmDT;  
		char sDateTime[128] = {0};  
		time(&nTime);  
		localtime_s(&tmDT,&nTime);  
		strftime(sDateTime,_countof(sDateTime),"[%Y-%m-%d %H:%M:%S]",&tmDT);//strftime(sLogOutput, 128, "[%Y-%m-%d %H:%M:%S]",tmDateTime);
		strLog = sDateTime;
		//内容
		va_list arg_ptr;  
		va_start(arg_ptr,format);
		_vsnprintf_s(sLogOutput,_countof(sLogOutput),_TRUNCATE, format, arg_ptr);
		va_end(arg_ptr);
		strLog.append(sLogOutput);
		//换行
		strLog.append("\r\n");
		//输出
		FILE* fp = NULL;
		int nLength = ::WideCharToMultiByte(CP_ACP, 0, m_strFileName.data(), -1, NULL, 0, NULL, FALSE);
		std::vector<char> vecResult(nLength);
		::WideCharToMultiByte(CP_ACP, 0, m_strFileName.data(), -1, &vecResult[0], nLength, NULL, FALSE);
		string strFile = std::string(vecResult.begin(), vecResult.end()-1);
		fopen_s(&fp,strFile.c_str(),"ab+");	
		if (fp)
		{
			//setlocale(0, "chs");
			fwrite(strLog.c_str(),strLog.length(),1,fp);
			fclose(fp);
		}
		if (bPrintToConsole)
			OutputDebugStringA(strLog.c_str());
	}
	catch (...)
	{}
	SetEvent(m_hEvent);
}

void CWriteLogFile::WriteLogW(bool bPrintToConsole,LPCWSTR format, ...)
{
	WaitForSingleObject(m_hEvent,3000);
	if (m_hEvent == INVALID_HANDLE_VALUE)
		return ;
	ResetEvent(m_hEvent);
	try
	{
		if ( GetLogFileCurSize(m_strFileName)>m_dwLogFileMaxSize*1024)
			DeleteFile(m_strFileName.c_str());

		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		BOOL bHave = GetFileAttributesEx(m_strFileName.c_str(), GetFileExInfoStandard, &attrs);

		//输出
		FILE* fp = NULL;
		_wfopen_s(&fp,m_strFileName.c_str(),L"ab+");	
		if (fp)
		{
			
			if ( !bHave )  
			{  
				// 新创建的日志文件，则写入Unicode头  
				BYTE chUnicodeHead[2] = { 0xff, 0xfe }; // Unicode头  
				fwrite( chUnicodeHead, sizeof(BYTE), sizeof(chUnicodeHead), fp );  
			}  

			//内容
			wchar_t sLogOutput[10240] = {0};
			va_list arg_ptr;  
			va_start(arg_ptr,format);
			_vsnwprintf_s(sLogOutput,_countof(sLogOutput),_TRUNCATE, format, arg_ptr);
			va_end(arg_ptr);

			SYSTEMTIME time;  
			::GetLocalTime( &time );  
			fwprintf(fp, L"[%04d-%02d-%02d %02d:%02d:%02d]%s\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, sLogOutput );  
			fclose( fp );  

			if (bPrintToConsole)
				OutputDebugString(sLogOutput);
		}
	}
	catch (...)
	{}
	SetEvent(m_hEvent);
}