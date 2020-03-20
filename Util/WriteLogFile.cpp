
#include "WriteLogFile.h"
#include <time.h>
#include <io.h>

CWriteLogFile::CWriteLogFile(const wstring& strLogFileName,int nLogFileMaxSize /* = 1024 */)
{
	try
	{
		//开始的时候检测一下文件大小
		int nFileSize = (GetLogFileCurSize(strLogFileName));
		if ( nFileSize>nLogFileMaxSize*1024 || nFileSize==0)
			DeleteFileW(strLogFileName.c_str());
		
		m_wstrFileName = strLogFileName;

		m_dwLogFileMaxSize = nLogFileMaxSize;
	}
	catch (...)
	{}
	m_bWrite = true;

	InitializeCriticalSection(&m_CritSec);
}

CWriteLogFile::CWriteLogFile()
{
	m_bWrite = true;

	InitializeCriticalSection(&m_CritSec);
}

void CWriteLogFile::SetLogFilePathName(const wstring& strLogFileName,int nLogFileMaxSize /* = 1024 */)
{
	//开始的时候检测一下文件大小
	int nFileSize = (GetLogFileCurSize(strLogFileName));
	if ( nFileSize>nLogFileMaxSize*1024 || nFileSize==0)
		DeleteFileW(strLogFileName.c_str());

	m_wstrFileName = strLogFileName;
	m_dwLogFileMaxSize = nLogFileMaxSize;
}
wstring CWriteLogFile::GetLogFilePathName()
{
	return m_wstrFileName;
}

CWriteLogFile::~CWriteLogFile()
{
	DeleteCriticalSection(&m_CritSec);  
};

DWORD CWriteLogFile::GetLogFileCurSize(const wstring& strLogFileName)
{
	EnterCriticalSection(&m_CritSec);
	DWORD fileSize=0x0;
	try
	{
		FILE* file = NULL;
		_wfopen_s(&file,strLogFileName.c_str(),L"r");	
		if (file)
		{
			fileSize = _filelength(_fileno(file));
			fclose(file);
		}
	}
	catch (...)
	{}

	LeaveCriticalSection(&m_CritSec);
	return fileSize;
}

void CWriteLogFile::WriteLogPrintW(LPCWSTR lpszCodeFile,const int& nCodeLine,LPCWSTR format, ...)
{
	if (m_bWrite == false)
		return ;

	EnterCriticalSection(&m_CritSec);

	try
	{
		if ( GetLogFileCurSize(m_wstrFileName)>m_dwLogFileMaxSize*1024)
			DeleteFileW(m_wstrFileName.c_str());

		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		BOOL bHave = GetFileAttributesExW(m_wstrFileName.c_str(), GetFileExInfoStandard, &attrs);

		//输出
		FILE* fp = NULL;
		_wfopen_s(&fp,m_wstrFileName.c_str(),L"ab+");	
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
			fwprintf(fp, L"[%04d-%02d-%02d %02d:%02d:%02d][%s %d]%s\r\n",time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, lpszCodeFile, nCodeLine, sLogOutput );  
			fclose( fp );  
		}
	}
	catch (...)
	{}

	LeaveCriticalSection(&m_CritSec);
}

void CWriteLogFile::SetWriteLog(bool bIsWrite)
{
	m_bWrite = bIsWrite;
}

void CWriteLogFile::WriteLogW(LPCWSTR lpszCodeFile,const int& nCodeLine,LPCWSTR lpszLog)
{
	if (m_bWrite == false)
		return ;

	EnterCriticalSection(&m_CritSec);

	try
	{
		if ( GetLogFileCurSize(m_wstrFileName)>m_dwLogFileMaxSize*1024)
			DeleteFileW(m_wstrFileName.c_str());

		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		BOOL bHave = GetFileAttributesExW(m_wstrFileName.c_str(), GetFileExInfoStandard, &attrs);

		//输出
		FILE* fp = NULL;
		_wfopen_s(&fp,m_wstrFileName.c_str(),L"ab+");	
		if (fp)
		{

			if ( !bHave )  
			{  
				// 新创建的日志文件，则写入Unicode头  
				BYTE chUnicodeHead[2] = { 0xff, 0xfe }; // Unicode头  
				fwrite( chUnicodeHead, sizeof(BYTE), sizeof(chUnicodeHead), fp );  
			}  

			//内容
			SYSTEMTIME time;  
			::GetLocalTime( &time );  
			fwprintf(fp, L"[%04d-%02d-%02d %02d:%02d:%02d][%s %d]",time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,lpszCodeFile, nCodeLine);  
			fwrite(lpszLog,wcslen(lpszLog)*2,1,fp);
			fwrite(L"\r\n",wcslen(L"\r\n")*2,1,fp);			
			fclose( fp );  
		}
	}
	catch (...)
	{}

	LeaveCriticalSection(&m_CritSec);
}