#include <winsock2.h>
#include <Windows.h>
#include <time.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <commoncontrols.h>
#include <OleCtl.h>
#include <ShellAPI.h>
#include <TlHelp32.h>
#include <io.h>
#include "ToolsFunc.h"
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib,"version.lib")

BOOL CreateDirW(const wstring& strDir)
{
	if (PathIsDirectoryW(strDir.c_str()))
		return TRUE;

	int nPos = strDir.find_last_of(L"\\/");
	if (nPos!=string::npos)
	{
		wstring strParentDir = strDir.substr(0,nPos);
		if (PathIsDirectoryW(strParentDir.c_str()) == FALSE)
		{
			if (CreateDirW(strParentDir)==FALSE)
			{
				return FALSE;
			}
		}

		if (CreateDirectoryW(strDir.c_str(), NULL) == FALSE)
		{
			DWORD dwErr = GetLastError();
			if (dwErr != 183)
				return FALSE;
		}		
	}
	else
		return FALSE;

	return TRUE;
}

//居中到屏幕中央
void CenterWindowToScreen(HWND hWnd)
{
	RECT rcArea;
	RECT rcCenter;

#if WINVER < 0x0500
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
#else
	HMONITOR hMonitor = NULL;
	if(hWnd != NULL)
		hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	else
		hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	MONITORINFO minfo;
	minfo.cbSize = sizeof(MONITORINFO);
	BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);

	rcArea = minfo.rcWork;
#endif
	::GetWindowRect(hWnd, &rcCenter);
	int DlgWidth = rcCenter.right - rcCenter.left;
	int DlgHeight = rcCenter.bottom - rcCenter.top;

	int xLeft = (rcArea.right-rcArea.left-DlgWidth)/2;
	int yTop = (rcArea.bottom-rcArea.top-DlgHeight)/2;

	::SetWindowPos(hWnd, NULL, xLeft, yTop, -1, -1,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

//自动居中窗口。hParent-父窗口,hWnd-要居中的窗口
void CenterWindowEx(HWND hParent,HWND hWnd)
{
	if (::IsWindow(hWnd) == FALSE)
		return;

	if (hParent == NULL || IsIconic(hParent))
	{
		CenterWindowToScreen(hWnd);
	}
	else
	{
		RECT rcDlg = { 0 };
		RECT rcCenter = { 0 };
		RECT rcArea = { 0 };
		::GetWindowRect(hWnd, &rcDlg);
		::GetWindowRect(hParent, &rcCenter);
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// Find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// The dialog is outside the screen, move it inside
		if( xLeft < rcArea.left ) xLeft = rcArea.left;
		else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
		if( yTop < rcArea.top ) yTop = rcArea.top;
		else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
		::SetWindowPos(hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

std::string GetNumFromStrBuf(const char* pszStrBuf)
{
	std::string strNum="";
	try
	{
		char ch;
		for (unsigned int i=0;i<strlen(pszStrBuf);i++)
		{
			ch = *(pszStrBuf+i);
			if (ch >= '0' && ch<='9')
				strNum.append(1,ch);
		}
	}
	catch (...)
	{}
	return strNum;
}

string Guid2StringA(const GUID& theGuid)
{
	char szValue[64] = {0};
	sprintf_s(szValue
		, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"
		, theGuid.Data1
		, theGuid.Data2
		, theGuid.Data3
		, theGuid.Data4[0], theGuid.Data4[1]
	, theGuid.Data4[2], theGuid.Data4[3], theGuid.Data4[4], theGuid.Data4[5]
	, theGuid.Data4[6], theGuid.Data4[7]
	);
	string strGuid = szValue;
	return strGuid;
}

void PrintfLog(const TCHAR * format, ...)
{
	try
	{
		//时间前缀
		time_t nTime;  
		struct tm *tmDateTime;  
		TCHAR szDateTime[128] = {0};  
		time(&nTime);
		localtime_s(tmDateTime,&nTime); 
		_sntprintf_s(szDateTime,_countof(szDateTime),_TRUNCATE,_T("[%04d-%02d-%02d %02d:%02d:%02d]"),tmDateTime->tm_year+1900,tmDateTime->tm_mon+1,tmDateTime->tm_mday,tmDateTime->tm_hour,tmDateTime->tm_min,tmDateTime->tm_sec);

		//内容
		TCHAR szLog[4096] = {0};
		va_list arg_ptr;  
		va_start(arg_ptr, format);
		_vsntprintf_s(szLog,_countof(szLog),_TRUNCATE, format, arg_ptr);
		va_end(arg_ptr);
		TCHAR szText[4224]={0};	//4096+128
		_sntprintf_s(szText,_countof(szText),_TRUNCATE,_T("%s%s\n"),szDateTime,szLog);
		//输出
		OutputDebugString(szText);
	}
	catch (...)
	{}
}

BOOL IsPathExist(const TCHAR* szPath)
{
	if (szPath==NULL || _tcslen(szPath)==0)
		return FALSE;

	WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
	return GetFileAttributesEx(szPath, GetFileExInfoStandard, &attrs);
}

BOOL IsPathExistA(const string& strPath)
{
	if ( strPath.empty() )
		return FALSE;

	WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
	return GetFileAttributesExA(strPath.c_str(), GetFileExInfoStandard, &attrs);
}
bool IsDirOrFileExist(const TCHAR* szPath)
{
	if ( (_taccess(szPath,0)) == -1 )
		return false;

	return true;
}
bool IsDirOrFileExistA(const char* szPath)
{
	if ( (_access(szPath,0)) == -1 )
		return false;
	
	return true;
}

bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue)
{
	strValue.clear();
	HKEY hKeyHander = NULL ;
	if (RegOpenKeyEx(hKey,strSubKey.c_str(),NULL,KEY_QUERY_VALUE,&hKeyHander) != ERROR_SUCCESS)
		return false;

	TCHAR szData[1024*10] = {0};
	DWORD dwSize = 1024*10;
	DWORD dwTp = dwType;
	LSTATUS lret = RegQueryValueEx(hKeyHander,strKeyName.c_str(),NULL,&dwTp,(LPBYTE)szData,&dwSize);
	if (lret == ERROR_SUCCESS)
	{
		if (dwType == REG_SZ)
			strValue = szData;
		else if (dwType==REG_DWORD||dwType==REG_BINARY)
		{
			_sntprintf_s(szData,_countof(szData),_TRUNCATE,_T("%ld"),(long)*(long *)szData);
			strValue = szData;
		}
		else if (dwType == REG_MULTI_SZ)
		{
			strValue.append(szData,dwSize);
		}

		RegCloseKey(hKeyHander);
		return true;
	}
	else if (lret == ERROR_FILE_NOT_FOUND)	//打开失败则从非Wow6432Node下面找
	{
		if (RegOpenKeyEx(hKey,strSubKey.c_str(),NULL,KEY_QUERY_VALUE | KEY_WOW64_64KEY,&hKeyHander) != ERROR_SUCCESS)
			return false;
		lret = RegQueryValueEx(hKeyHander,strKeyName.c_str(),NULL,&dwTp,(LPBYTE)szData,&dwSize);
		if (lret == ERROR_SUCCESS)
		{
			if (dwType == REG_SZ)
				strValue = szData;
			else if (dwType==REG_DWORD||dwType==REG_BINARY)
			{
				_sntprintf_s(szData,_countof(szData),_TRUNCATE,_T("%ld"),(long)*(long *)szData);
				strValue = szData;
			}
			else if (dwType == REG_MULTI_SZ)
			{
				strValue.append(szData,dwSize);
			}

			RegCloseKey(hKeyHander);
			return true;
		}
	}

	RegCloseKey(hKeyHander);
	return false;
}

bool WriteRegValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData)
{
	HKEY   hKeyHander = NULL;   
	//找到  
	long lRet = ::RegOpenKeyEx(hKey,strSubKey.c_str(),0,KEY_READ|KEY_WRITE,&hKeyHander);
	if(lRet != ERROR_SUCCESS)  
	{
		DWORD dwDisposition;
		lRet = RegCreateKeyEx(hKey,strSubKey.c_str(),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, &hKeyHander, &dwDisposition);
	}
	if (lRet == ERROR_SUCCESS)
	{
		lRet = RegSetValueEx(hKeyHander,strKeyName.c_str(),0,dwType,lpData,cbData);
	}
	if (hKeyHander != NULL)
		RegCloseKey(hKeyHander);

	if (lRet == ERROR_SUCCESS)
		return true;

	return false;  
}
bool WriteRegValueNo6432(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData)
{
	HKEY   hKeyHander = NULL;   
	//找到  
	long lRet = ::RegOpenKeyEx(hKey,strSubKey.c_str(),0,KEY_READ|KEY_WRITE|KEY_WOW64_64KEY,&hKeyHander);
	if(lRet != ERROR_SUCCESS)  
	{
		DWORD dwDisposition;
		lRet = RegCreateKeyEx(hKey,strSubKey.c_str(),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, &hKeyHander, &dwDisposition);
	}
	if (lRet == ERROR_SUCCESS)
	{
		lRet = RegSetValueEx(hKeyHander,strKeyName.c_str(),0,dwType,lpData,cbData);
	}
	if (hKeyHander != NULL)
		RegCloseKey(hKeyHander);

	if (lRet == ERROR_SUCCESS)
		return true;

	return false;  
}
bool WriteRegValueA(HKEY hKey,const string& strSubKey,const string& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData)
{
	HKEY   hKeyHander = NULL;   
	//找到  
	long lRet = ::RegOpenKeyExA(hKey,strSubKey.c_str(),0,KEY_READ|KEY_WRITE,&hKeyHander);
	if(lRet != ERROR_SUCCESS)  
	{
		DWORD dwDisposition;
		lRet = RegCreateKeyExA(hKey,strSubKey.c_str(),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, &hKeyHander, &dwDisposition);
	}
	if (lRet == ERROR_SUCCESS)
	{
		lRet = RegSetValueExA(hKeyHander,strKeyName.c_str(),0,dwType,lpData,cbData);
	}
	if (hKeyHander != NULL)
		RegCloseKey(hKeyHander);

	if (lRet == ERROR_SUCCESS)
		return true;

	return false;  
}

bool DeleteRegKeyValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName)
{
	HKEY hKeyHandle = NULL;                             
	LONG lRet = RegOpenKeyEx(hKey, strSubKey.c_str(), 0, KEY_QUERY_VALUE|KEY_WRITE, &hKeyHandle );                                
	if( lRet == ERROR_SUCCESS )    
	{
		lRet = RegDeleteValue(hKeyHandle, strKeyName.c_str());
		RegCloseKey(hKeyHandle);
		if (lRet == ERROR_SUCCESS)
		{
			return true;
		}
	}

	return false;
}
bool DeleteRegKeyValueA(HKEY hKey,const string& strSubKey,const string& strKeyName)
{
	HKEY hKeyHandle = NULL;                             
	LONG lRet = RegOpenKeyExA(hKey, strSubKey.c_str(), 0, KEY_QUERY_VALUE|KEY_WRITE, &hKeyHandle );                                
	if( lRet == ERROR_SUCCESS )    
	{
		lRet = RegDeleteValueA(hKeyHandle, strKeyName.c_str());
		RegCloseKey(hKeyHandle);
		if (lRet == ERROR_SUCCESS)
		{
			return true;
		}
	}

	return false;
}

bool DeleteRegSubKey(HKEY hKey,const xstring& strSubKey)
{
	LONG lRet = SHDeleteKey(hKey,strSubKey.c_str());  
	if (lRet == ERROR_SUCCESS)
	{
		return true;
	}

	return false;
}

xstring GetAppPath(HMODULE hModul)
{
	TCHAR szAppPath[512] = {0};
	GetModuleFileName(hModul,szAppPath,512);	

	//初始化App路径
	TCHAR szDrive[32] = {0},szDir[512] = {0},szFname[512]={0},szExt[128]={0};
	_tsplitpath_s(szAppPath,szDrive,szDir,szFname,szExt);
	xstring strAppName = szDrive;
	strAppName += szDir;
	return strAppName;	//"e:\jiasu\Bin\"
}

void SplitStringA(const string& strSource, std::deque<std::string>& deq, const string& strDelim)
{
	string strSrc = strSource;
	deq.clear();
	if (strSrc.empty())
	{
		return ;
	}
	if (strDelim.empty())
	{
		deq.push_back(strSrc);
		return ;
	}

	int nPos = strSrc.find(strDelim);
	if (nPos == string::npos)
	{
		return ;
	}
	else
	{
		while (nPos != string::npos)
		{
			deq.push_back(strSrc.substr(0,nPos));
			strSrc.erase(0,nPos+strDelim.length());

			nPos = strSrc.find(strDelim);
		}

		if (strSrc.empty() == false)
		{
			deq.push_back(strSrc);
		}
	}	
}

int CheckPortUsed(int nPort)
{
	try
	{
		OutputDebugStringW(L"开始检查端口\n");
		//创建匿名管道
		SECURITY_ATTRIBUTES sa;  
		HANDLE hRead=NULL,hWrite=NULL;  
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
		sa.lpSecurityDescriptor = NULL;  
		sa.bInheritHandle = TRUE;  
		if (!CreatePipe(&hRead,&hWrite,&sa,0))   
		{
			OutputDebugStringW(L"匿名管道创建失败\n");
			return -1;  
		}   
		wchar_t szCmd[128] = {0};
		_snwprintf_s(szCmd,_countof(szCmd),_TRUNCATE,L"cmd.exe /C netstat -ano | find \"%d\"",nPort);
		STARTUPINFOW si;  
		ZeroMemory(&si, sizeof(STARTUPINFOW));
		PROCESS_INFORMATION pi;   
		si.cb = sizeof(STARTUPINFOW);  
		GetStartupInfoW(&si);   
		si.hStdError = hWrite;            //把创建进程的标准错误输出重定向到管道输入  
		si.hStdOutput = hWrite;           //把创建进程的标准输出重定向到管道输入  
		si.wShowWindow = SW_HIDE;  
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  
		if (!CreateProcess(NULL, szCmd,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
		{  
			CloseHandle(hWrite);  
			CloseHandle(hRead);  
			OutputDebugStringW(L"查询端口cmd执行失败,CreateProcess失败\n");
			TerminateProcess(pi.hProcess,0);
			return -1;  
		}  
		
		if (WaitForSingleObject(pi.hProcess,2000) == WAIT_TIMEOUT)
		{
			CloseHandle(hWrite);  
			CloseHandle(hRead);
			CloseHandle(pi.hProcess);
			OutputDebugStringW(L"查询端口cmd执行超时失败\n");
			TerminateProcess(pi.hProcess,0);
			return -1;  
		}

		if (hWrite)
			CloseHandle(hWrite);  

		string strResult;
		if (hRead)
		{
			int nSize = GetFileSize(hRead, NULL)+4;		//加4避免申请的堆内存刚好被文件内容填满后,字符串末尾没有'\0'了
			char* szBuf = (char*)malloc(nSize);
			memset(szBuf,0,nSize);
			DWORD dwReadSize=0;
			
			//char szLog[128] = {0};
			//sprintf(szLog,"数据大小:%d\n",nSize);
			//OutputDebugStringA(szLog);

			ReadFile(hRead, szBuf, nSize, &dwReadSize, NULL);
			strResult = szBuf;
			free(szBuf);
			CloseHandle(hRead);
			OutputDebugStringA(strResult.c_str());
		}
		else
			return -1;

		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		deque<string> theDeq;
		SplitStringA(strResult,theDeq,"\r\n");

		for (deque<string>::iterator itrAll = theDeq.begin();itrAll != theDeq.end();itrAll++)
		{
			deque<string> theDetailDeq;
			SplitStringA(*itrAll,theDetailDeq,"    ");
			deque<string> deqIpPort;
			if (theDetailDeq.size()>1)
			{
				SplitStringA(theDetailDeq.at(1),deqIpPort,":");
				if (deqIpPort.size() > 0)
				{
					string strPort = deqIpPort.back();
					if (atoi(strPort.c_str()) == nPort)
					{
						string strPid = theDetailDeq.back();
						return atoi(strPid.c_str());
					}
				}
			}
		}
	}
	catch (...)
	{}	
 
	return -1;
}

string GetLocalIp()
{
	WSADATA     wsaData; 
	WSAStartup(0x0202, &wsaData);
	//Before calling AddIPAddress we use GetIpAddrTable to get an adapter to which we can add the IP.
	PMIB_IPADDRTABLE pIPAddrTable = NULL;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	// Make an initial call to GetIpAddrTable to get the necessary size into the dwSize variable
	if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
	{
		pIPAddrTable = (MIB_IPADDRTABLE *) malloc ( dwSize );
	}

	// Make a second call to GetIpAddrTable to get the actual data we want
	dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 );
	if ( dwRetVal != NO_ERROR ) 
	{
		if (pIPAddrTable)
		{
			free( pIPAddrTable );
			pIPAddrTable = NULL;
		}
		
		WSACleanup();
		return "";
	}

	for (DWORD i=0; i<pIPAddrTable->dwNumEntries; i++)
	{
		PMIB_IPADDRROW prow = &pIPAddrTable->table[i];
		if ( prow->dwAddr==0 || prow->dwMask ==0 || (prow->dwAddr&0xFF)==127 || (prow->wType & MIB_IPADDR_DISCONNECTED)  )
		{
			continue;
		}

		if (prow->dwMask != 0xFFFFFFFF)
		{
			IN_ADDR inAddr,inMask;
			inAddr.S_un.S_addr  = prow->dwAddr;
			inMask.S_un.S_addr = prow->dwMask;

			if (pIPAddrTable)
			{
				free( pIPAddrTable );
				pIPAddrTable = NULL;
			}
			string strIP = inet_ntoa(inAddr);
			WSACleanup();
			return strIP;
		}
	}

	if (pIPAddrTable)
	{
		free( pIPAddrTable );
		pIPAddrTable = NULL;
	}
	WSACleanup();
	return "";
}

string GetMAC()
{
	string strMac;
	ULONG lIFindex = 0xFFFFFFFF;
	ULONG outBufLen = 0;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (pAddresses == NULL) 
			return strMac;
	}

	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
	{
		// If successful, output some information from the data we received
		for(PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(pCurrAddresses->PhysicalAddressLength != 6)
				continue;

			char acMAC[20] = {0};
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int (pCurrAddresses->PhysicalAddress[0]),
				int (pCurrAddresses->PhysicalAddress[1]),
				int (pCurrAddresses->PhysicalAddress[2]),
				int (pCurrAddresses->PhysicalAddress[3]),
				int (pCurrAddresses->PhysicalAddress[4]),
				int (pCurrAddresses->PhysicalAddress[5]));
			if (pCurrAddresses->IfIndex < lIFindex)
			{
				strMac = acMAC;
				lIFindex = pCurrAddresses->IfIndex;
			}
		}
	} 
	if (pAddresses)
	{
		free(pAddresses);
		pAddresses = NULL;
	}	
	return strMac;
}

void GetIpAdapterInfoList(deque<IP_ADAPTER_INFO>& theIpAdapterList)
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息  
	PIP_ADAPTER_INFO pIpAdapterInfo = NULL;  
	unsigned long stSize = 0;  
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);  
	if (ERROR_BUFFER_OVERFLOW == nRel)  
	{
		pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(stSize);  
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);   
		if (ERROR_SUCCESS == nRel)  
		{  
			//输出网卡信息  
			//可能有多网卡,因此通过循环去判断
			while (pIpAdapterInfo)  
			{  
				theIpAdapterList.push_back(*pIpAdapterInfo);
				pIpAdapterInfo = pIpAdapterInfo->Next;  
			}  
		}   
		if (pIpAdapterInfo)
		{
			free(pIpAdapterInfo);  
			pIpAdapterInfo = NULL;
		}		 
	}
	else
	{
		if (pIpAdapterInfo)
		{
			free(pIpAdapterInfo);  
			pIpAdapterInfo = NULL;
		}	
	}
}

#ifndef __INCLUDE__STRINGTOOLS__H
#define __INCLUDE__STRINGTOOLS__H
#include "StringTools.h"
#endif
xstring GetExtensionFromFileName(const xstring& strFileName)
{
	//PathFindExtension()	//暂时不用这个api
	
	xstring strExtension;
	int nPos = strFileName.rfind(_T('.'));
	if (nPos != string::npos)
	{
		strExtension = strFileName.substr(nPos+1);
	}

	strExtension = StringConvertUpperOrLower(false,strExtension);
	return strExtension;
}


xstring GetDefaultBrowser()
{
	HKEY hDefBrowserPos = NULL;
	xstring strDefBrowserPath = _T("C:\\Program Files\\Internet Explorer\\iexplore.exe");

	xstring strValue;
	bool bRet = ReadRegString(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\Ftp\\UserChoice"),_T("ProgId"),REG_SZ,strValue);
	if (bRet)
	{
		xstring strDefBrowserPos = strValue;
		strDefBrowserPos += _T("\\shell\\open\\command");
		
		bRet = ReadRegString(HKEY_CLASSES_ROOT,strDefBrowserPos,_T(""),REG_SZ,strValue);

		if (bRet)
		{
			// 解出 exe 路径.
			strDefBrowserPath = strValue;
			xstring::size_type leftQuotation = strDefBrowserPath.find(_T('"'));
			if (leftQuotation != string::npos)
			{
				wstring::size_type rightQuotation = strDefBrowserPath.find(L'"', leftQuotation + 1);
				if (rightQuotation != string::npos)
				{
					strDefBrowserPath.assign(strDefBrowserPath.begin() + leftQuotation + 1,strDefBrowserPath.begin() + rightQuotation);
				}
			}
		}
	}
	return strDefBrowserPath;
}

void CreateDesktopIcon(const xstring& strIconName,const xstring& strExeFile,const xstring& strCmdLine,const xstring& strIconFile)
{
	int nSpecialFolder = CSIDL_DESKTOPDIRECTORY;
	LPITEMIDLIST pidl = NULL;
	LPMALLOC pShell = NULL;
	TCHAR szPath[512] = {0};

	TCHAR szLink[1024] = {0};

	if( SUCCEEDED(SHGetMalloc(&pShell)) )
	{
		if( SUCCEEDED(SHGetSpecialFolderLocation(NULL,nSpecialFolder,&pidl)) )
		{
			if(!SHGetPathFromIDList(pidl,szPath))
			{
				pShell->Free(pidl);
				pShell->Release();
				return ;
			}
			pShell->Release();

			_sntprintf_s(szLink,_countof(szLink),_TRUNCATE,_T("%s\\%s.lnk"),szPath,strIconName.c_str());

			IShellLink* psl = NULL;
			if(SUCCEEDED(CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl)))
			{
				psl->SetPath(strExeFile.c_str());
				psl->SetArguments(strCmdLine.c_str());
				psl->SetIconLocation(strIconFile.c_str(),0);
				psl->SetWorkingDirectory(_T("C:\\"));

				IPersistFile* ppf = NULL;
				if(SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(LPVOID*)&ppf)))
				{
#ifdef UNICODE
					ppf->Save(szLink, TRUE);
#else
					ppf->Save(_W(szLink), TRUE);
#endif
					
					ppf->Release();
				}
				psl->Release();
			}
		}
	}
}
bool GetShellPath(const char *Src,wchar_t *ShellPath)
{
	bool blret=false;
	//::CoInitialize(NULL); //初始化COM接口
	IShellLink *psl = NULL;
	//创建COM接口，IShellLink对象创建
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID *)&psl);
	if (SUCCEEDED(hr))
	{
		IPersistFile *ppf ;
		hr=psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
		if (SUCCEEDED(hr))
		{
			WCHAR wsz[MAX_PATH] = {0};
			MultiByteToWideChar( CP_ACP, 0, Src, -1, wsz, MAX_PATH ) ;    //转下宽字节
			hr=ppf->Load(wsz, STGM_READ);    //加载文件
			if (SUCCEEDED(hr))
			{
				WIN32_FIND_DATA wfd ;
				psl->GetPath(ShellPath,MAX_PATH, (WIN32_FIND_DATA*)&wfd,SLGP_SHORTPATH);  //获取目标路径
				blret=true;
			}
			ppf->Release(); 
		}
		psl->Release();  //释放对象
	}
	//::CoUninitialize();   //释放COM接口
	return blret;
}
bool IsLineFile(const wstring wstr)
{
	if (wstr.find(L".lnk") == string::npos)
		return false;

	return true;
}

bool SaveIconFileFromExeFile(const xstring& strExe,const xstring& strDestFile)
{
	SHFILEINFO sfi; 
	ZeroMemory(&sfi, sizeof(SHFILEINFO)); 
	::SHGetFileInfo(strExe.c_str(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX); 

	HIMAGELIST* imageList = NULL; 
	//由大图到小图依次抽取,直到抽取到为止(SHIL_JUMBO可能抽取有问题,具体使用这个时保存出来的ico不对,因此跳过这个抽取)
	HICON hIcon = NULL;
	HRESULT hResult = NULL;
	//HRESULT hResult = ::SHGetImageList(SHIL_JUMBO, IID_IImageList, (void**)&imageList);
	//if (hResult == S_OK)
	//{
	//	((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_NORMAL, &hIcon); 
	//}
	if (hIcon == NULL)
	{
		hResult = ::SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&imageList);
		if (hResult == S_OK)
		{
			((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_NORMAL, &hIcon); 
		}
	}
	if (hIcon == NULL)
	{
		hResult = ::SHGetImageList(SHIL_LARGE, IID_IImageList, (void**)&imageList);
		if (hResult == S_OK)
		{
			((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_NORMAL, &hIcon); 
		}
	}
	if (hIcon == NULL)
	{
		hResult = ::SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)&imageList);
		if (hResult == S_OK)
		{
			((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_NORMAL, &hIcon); 
		}
	}
	if (hIcon == NULL)
	{
		return false;
	}

	//创建文件
	HANDLE hFile = CreateFile(strDestFile.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,NULL,NULL);
	//写入头
	typedef struct
	{
		WORD idReserved; // Reserved (must be 0)
		WORD idType; // Resource Type (1 for icons)
		WORD idCount; // How many images?
	}ICONHEADER;	
	ICONHEADER icoHeader = {0,1,1};
	DWORD dwWrite = 0;
	WriteFile(hFile,&icoHeader,sizeof(icoHeader),&dwWrite,NULL);
	
	typedef struct
	{
		BYTE bWidth;
		BYTE bHeight;
		BYTE bColorCount;
		BYTE bReserved;
		WORD wPlanes; // for cursors, this field = wXHotSpot
		WORD wBitCount; // for cursors, this field = wYHotSpot
		DWORD dwBytesInRes;
		DWORD dwImageOffset; // file-offset to the start of ICONIMAGE
	}ICONDIR;

	SetFilePointer(hFile, sizeof(ICONDIR), 0, FILE_CURRENT);

	int nOffset = SetFilePointer(hFile, 0, 0, FILE_CURRENT);
	//写入图像头
	BITMAPINFOHEADER biHeader;
	ZeroMemory(&biHeader, sizeof(biHeader));
	biHeader.biSize = sizeof(biHeader);

	ICONINFO theIcon;
	GetIconInfo(hIcon,&theIcon);
	BITMAP bmColor,bmMask;
	::GetObject(theIcon.hbmColor, sizeof(BITMAP), &bmColor);
	::GetObject(theIcon.hbmMask, sizeof(BITMAP), &bmMask);
	biHeader.biWidth = bmColor.bmWidth;
	biHeader.biHeight = bmColor.bmHeight*2;
	biHeader.biPlanes = bmColor.bmPlanes;
	biHeader.biBitCount = bmColor.bmBitsPixel;
	DWORD dwBmcolorImageBytes,dwBmMaskImageBytes;
	dwBmcolorImageBytes = bmColor.bmWidthBytes;
	if (dwBmcolorImageBytes & 3)
		dwBmcolorImageBytes = (dwBmcolorImageBytes + 4) & ~3;
	dwBmcolorImageBytes = dwBmcolorImageBytes*bmColor.bmHeight;
	dwBmMaskImageBytes = bmMask.bmWidthBytes;
	if (dwBmMaskImageBytes & 3)
		dwBmMaskImageBytes = (bmMask.bmWidthBytes + 4) & ~3;
	dwBmMaskImageBytes = dwBmMaskImageBytes*bmMask.bmHeight;
	biHeader.biSizeImage = dwBmcolorImageBytes + dwBmMaskImageBytes;
	WriteFile(hFile, &biHeader, sizeof(biHeader), &dwWrite, 0);
	//写入bmColor图像数据
	BYTE* pIconData = (BYTE *)malloc(dwBmcolorImageBytes);
	GetBitmapBits(theIcon.hbmColor, dwBmcolorImageBytes, pIconData);
	for(int i = bmColor.bmHeight - 1; i >= 0; i--)
	{
		WriteFile(hFile,pIconData + (i * bmColor.bmWidthBytes), bmColor.bmWidthBytes, &dwWrite,0);
		// extend to a 32bit boundary (in the file) if necessary
		if(bmColor.bmWidthBytes & 3)
		{
			DWORD padding = 0;
			WriteFile(hFile, &padding, 4 - (bmColor.bmWidthBytes & 3), &dwWrite, 0);
		}
	}
	free(pIconData);
	//写入bmMask图像数据
	BYTE* pIconDataMask = (BYTE *)malloc(dwBmMaskImageBytes);
	GetBitmapBits(theIcon.hbmMask, dwBmMaskImageBytes, pIconDataMask);
	for(int i = bmMask.bmHeight - 1; i >= 0; i--)
	{
		WriteFile(hFile,pIconDataMask + (i * bmMask.bmWidthBytes), bmMask.bmWidthBytes, &dwWrite,0);
		// extend to a 32bit boundary (in the file) if necessary
		if(bmMask.bmWidthBytes & 3)
		{
			DWORD padding = 0;
			WriteFile(hFile, &padding, 4 - (bmMask.bmWidthBytes & 3), &dwWrite, 0);
		}
	}
	free(pIconDataMask);

	SetFilePointer(hFile, sizeof(ICONHEADER), 0, FILE_BEGIN);
	//Lastly, skip back and write the icon directories.Write one icon directory entry - specify the index of the image

	ICONDIR iconDir;
	UINT nColorCount;
	if(bmColor.bmBitsPixel >= 8)
		nColorCount = 0;
	else
		nColorCount = 1 << (bmColor.bmBitsPixel * bmColor.bmPlanes);
	// Create the ICONDIR structure
	iconDir.bWidth = (BYTE)bmColor.bmWidth;
	iconDir.bHeight = (BYTE)bmColor.bmHeight;
	iconDir.bColorCount = nColorCount;
	iconDir.bReserved = 0;
	iconDir.wPlanes = bmColor.bmPlanes;
	iconDir.wBitCount = bmColor.bmBitsPixel;
	iconDir.dwBytesInRes = sizeof(BITMAPINFOHEADER) + dwBmcolorImageBytes + dwBmMaskImageBytes;
	iconDir.dwImageOffset = nOffset;
	WriteFile(hFile, &iconDir, sizeof(iconDir), &dwWrite, 0);

	//释放资源
	::DeleteObject(theIcon.hbmColor);
	::DeleteObject(theIcon.hbmMask);
	DestroyIcon(hIcon);
	//关闭文件句柄
	CloseHandle(hFile);

	return true;
}

//由绝对路径获取文件名
xstring GetAppNameFromPath(const xstring& strAppPath)
{
	TCHAR szDrive[32] = {0},szDir[512] = {0},szFname[512]={0},szExt[512]={0};
	_tsplitpath_s(strAppPath.c_str(),szDrive,szDir,szFname,szExt);
	xstring strAppName = szFname;
	strAppName += szExt;
	return strAppName;
}

int IsValidIdCardNumber(const xstring& strIdCardNumber)
{
	//身份证号长度
	if (strIdCardNumber.length() != 18)
		return -1;

	//非法字符
	xstring strValue1 = strIdCardNumber.substr(0,17);
	if (strValue1.find_first_not_of(_T("0123456789"))!=strValue1.npos )
		return -2;
	xstring strValue2 = strIdCardNumber.substr(17,1);
	if (strValue2.find_first_not_of(_T("0123456789xX"))!=strValue2.npos )
		return -2;

	//出生月份
	int nMonth = _ttoi(strIdCardNumber.substr(10,2).c_str());
	if (nMonth<1 || nMonth>12)
		return -3;
	//出生日期	//不再判断当月是否真的有31号
	int nDay = _ttoi(strIdCardNumber.substr(12,2).c_str());
	if (nDay<1 || nDay>31)
		return -4;

	//〖GB 11643-1999〗中规定，验证校验码
	int weight[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
	xstring strValidate = _T("10X98765432");
	int sum = 0;

	for(unsigned int i=0;i<strValue1.length();i++)
	{
		sum = sum + (strValue1[i] - _T('0')) * weight[i];
	}
	int nIndex = sum % 11;
	if (strValidate[nIndex] != strValue2[0])
		return -5;
	
	return 1;
}

bool AddFollowSystemStart(const xstring& strName,const xstring& strFile)
{
	HKEY   hKey;   
	//找到系统的启动项   
	LPCTSTR lpszRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");   
	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpszRun,0,KEY_READ|KEY_WRITE,&hKey)==ERROR_SUCCESS)  
	{  
		wchar_t lpData[512] = {0};
		DWORD cbData=512;   
		DWORD dwType = REG_SZ;   
		if(::RegQueryValueEx(hKey,strName.c_str(),0,&dwType,(LPBYTE)lpData,&cbData) == ERROR_SUCCESS) //已存在  
		{  
			if (_tcsicmp(strFile.c_str(),lpData) == 0)	//确实是要启动的程序
			{
				::RegCloseKey(hKey);
				return true;
			}			
		}  

		//不存在注册自启动项或同名键的值不对时
#ifdef UNICODE
		if( ::RegSetValueEx(hKey,strName.c_str(),0,REG_SZ,(const unsigned char*)(LPCTSTR)strFile.c_str(),strFile.length()*2) == ERROR_SUCCESS)
#else
		if( ::RegSetValueEx(hKey,strName.c_str(),0,REG_SZ,(const unsigned char*)(LPCTSTR)strFile.c_str(),strFile.length()) == ERROR_SUCCESS)
#endif		  
		{
			::RegCloseKey(hKey); 
			return true;
		}			
		::RegCloseKey(hKey); 
	}
	return false;  
}

bool DelFollowSystemStart(const xstring& strName,const xstring& strFile)
{
	HKEY   hKey;   
	//找到系统的启动项   
	LPCTSTR lpszRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");   
	if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpszRun,0,KEY_READ|KEY_WRITE,&hKey)==ERROR_SUCCESS)  
	{  
		wchar_t lpData[512] = {0};
		DWORD cbData=512;   
		DWORD dwType = REG_SZ;   
		if(::RegQueryValueEx(hKey,strName.c_str(),0,&dwType,(LPBYTE)lpData,&cbData) == ERROR_SUCCESS) //已存在  
		{  
			if (_tcsicmp(strFile.c_str(),lpData) == 0 || strFile.empty())	//确实是要启动的程序,或者只是想删除指定的键
			{
				if( RegDeleteValue(hKey,strName.c_str()) != ERROR_SUCCESS)	//删除失败
				{
					::RegCloseKey(hKey);
					return false;
				}			
			}
		}  
		::RegCloseKey(hKey);
	}
	return true;
}

//获取guid字符串
xstring GetGUID(bool bUpper/*=true*/)
{
	GUID guid;
	CoCreateGuid(&guid); 
	TCHAR szValue[256] = {0};  
	if (bUpper)
	{
		_sntprintf_s(szValue,_countof(szValue),_TRUNCATE,_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"), 
			guid.Data1, guid.Data2, guid.Data3,  
			guid.Data4[0], guid.Data4[1],  
			guid.Data4[2], guid.Data4[3],  
			guid.Data4[4], guid.Data4[5],  
			guid.Data4[6], guid.Data4[7]);  
	}
	else
	{
		_sntprintf_s(szValue,_countof(szValue),_TRUNCATE,_T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"), 
			guid.Data1, guid.Data2, guid.Data3,  
			guid.Data4[0], guid.Data4[1],  
			guid.Data4[2], guid.Data4[3],  
			guid.Data4[4], guid.Data4[5],  
			guid.Data4[6], guid.Data4[7]);  
	}

	return szValue; 
}

string ReadAllFromFile(const wstring& strFile)
{
	string strRet;
	try
	{
		HANDLE hFile = CreateFileW(strFile.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			OutputDebugStringW(L"文件打开失败\n");
		}
		else
		{
			int nSize = GetFileSize(hFile, NULL)+4;		//加4避免申请的堆内存刚好被文件内容填满后,字符串末尾没有'\0'了
			char* szBuf = (char*)malloc(nSize);
			memset(szBuf,0,nSize);
			DWORD dwReadSize=0;
			ReadFile(hFile, szBuf, nSize, &dwReadSize, NULL);
			strRet = szBuf;
			free(szBuf);
			CloseHandle(hFile);
		}
	}
	catch (...)
	{
		OutputDebugStringW(L"文件读写异常\n");
	}
	return strRet;
}
int WriteUtf8File(const string& strUtf8,const wstring& strFile)
{
	if (strUtf8.empty() || strFile.empty())
		return -1;

	FILE* fp = NULL;
	_wfopen_s(&fp,strFile.c_str(),L"wb");
	if (fp)
	{
		fwrite( strUtf8.c_str(),1,strUtf8.length(),fp);
		fclose(fp);
		return 1;
	}
	
	return 0;
}
int WriteUtf8FileW(const wstring& strUnicode,const wstring& strFile)
{
	if (strUnicode.empty() || strFile.empty())
		return -1;

	FILE* fp = NULL;
	_wfopen_s(&fp,strFile.c_str(),L"wb");
	if (fp)
	{
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode.data(), -1, NULL, 0, NULL, FALSE);
		if (nLength < 1)
			return -2;
		std::string strResult(nLength, 0);
		::WideCharToMultiByte(CP_UTF8, 0, strUnicode.data(), -1, &strResult[0], nLength, NULL, FALSE);

		string strUtf8(strResult.data(), nLength - 1);
		fwrite( strUtf8.c_str(),1,strUtf8.length(),fp);
		fclose(fp);
		return 1;
	}

	return 0;
}

xstring GetOSName()
{
	xstring strProductName,strCSDVersion,strBit;
	ReadRegString(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),_T("ProductName"),REG_SZ,strProductName);
	ReadRegString(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),_T("CSDVersion"),REG_SZ,strCSDVersion);
	if (Is64BitOS())
		strBit = _T(" 64Bit");
	else
		strBit = _T(" 32Bit");

	xstring strRet = strProductName + strBit;
	return strRet;
}

BOOL Is64BitOS()
{
	static int static_nRet = -1;
	if (static_nRet==0)
		return false;
	else if (static_nRet==1)
		return true;

	BOOL bIsWow64 = FALSE;
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(_T("kernel32")),"IsWow64Process");
	if(NULL!=fnIsWow64Process)
	{
		if(!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{}
	}

	if (bIsWow64)
		static_nRet = 1;
	else
		static_nRet = 0;

	return bIsWow64;  
}
void GetOSVersion(int& nMajorVersion,int& nMinorVersion)
{
	//获取操作系统版本
	TCHAR szWindows[1024] = {0};
	GetWindowsDirectory(szWindows,1023);
	try
	{
		wstring strNtdll = szWindows;
		strNtdll += L"\\System32\\ntdll.dll";
		VS_FIXEDFILEINFO *pVerInfo = NULL;  
		DWORD dwTemp=0, dwSize=0;  
		BYTE *pData = NULL;  
		UINT uLen=0;
		dwSize = GetFileVersionInfoSize(strNtdll.c_str(), &dwTemp);
		pData = new BYTE[dwSize+1];  
		GetFileVersionInfo(strNtdll.c_str(), 0, dwSize, pData);
		VerQueryValue(pData, TEXT("\\"), (void **)&pVerInfo, &uLen);
		DWORD verMS = pVerInfo->dwFileVersionMS;  
		DWORD verLS = pVerInfo->dwFileVersionLS;  
		DWORD major = HIWORD(verMS);  
		DWORD minor = LOWORD(verMS);  
		DWORD build = HIWORD(verLS);  
		DWORD revision = LOWORD(verLS);  
		if (pData)
		{
			delete[] pData;  
			pData = NULL;
		}

		nMajorVersion = major;
		nMinorVersion = minor;
	}
	catch (...)
	{
		OSVERSIONINFOEX theVerInfo;
		theVerInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX); 
		GetVersionEx((OSVERSIONINFO *)&theVerInfo);
		nMajorVersion = theVerInfo.dwMajorVersion;
		nMinorVersion = theVerInfo.dwMinorVersion;
	}	
}
bool IsXp()
{
	static int static_is_init = 0;
	if (static_is_init == 0)
	{
		int nMajorVer = 0, nMinorVer = 0;
		GetOSVersion(nMajorVer, nMinorVer);
		if (nMajorVer > 5)
		{
			static_is_init = 2;//不是xp
		}
		else
			static_is_init = 1;//是xp
	}

	if (static_is_init == 1)
	{
		return true;
	}

	return false;
}
bool IsXpUp()
{
	bool bIsXpUp = false;
	static int static_is_init = 0;
	if (static_is_init == 0)
	{
		int nMajorVer = 0, nMinorVer = 0;
		GetOSVersion(nMajorVer, nMinorVer);
		if (nMajorVer > 5)
			static_is_init = 2;//不是xp
		else
			static_is_init = 1;//是xp
	}

	if (static_is_init == 1)
		bIsXpUp = false;

	if (static_is_init == 2)
		bIsXpUp = true;

	return bIsXpUp;
}
bool IsWin7Up()
{
	int dwMajorVersion=-1,dwMinorVersion=-1;
	GetOSVersion(dwMajorVersion, dwMinorVersion);
	bool bIsWin7Up = false;
	if (dwMajorVersion == 6)
	{
		if (dwMinorVersion >= 2)
			bIsWin7Up = true;
	}
	else if (dwMajorVersion > 6)
		bIsWin7Up = true;

	return bIsWin7Up;
}

int CopyFolder(const xstring& strSource,const xstring& strDest)
{
	SHFILEOPSTRUCT FileOp;   
	//根据MSDN上，ZeryMerory在当缓冲区的字符串超出生命周期的时候，  
	//会被编译器优化，从而缓冲区的内容会被恶意软件捕捉到。  
	//引起软件安全问题，特别是对于密码这些比较敏感的信息而说。  
	//而SecureZeroMemory则不会引发此问题，保证缓冲区的内容会被正确的清零。  
	//如果涉及到比较敏感的内容，尽量使用SecureZeroMemory函数。 
	xstring strFrom = strSource;
	strFrom.append(1,_T('\0'));
	strFrom.append(1,_T('\0'));
	xstring strTo = strDest;
	strTo.append(1,_T('\0'));
	strTo.append(1,_T('\0'));

	SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));//secureZeroMemory和ZeroMerory的区别  
 
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_NO_UI;		//操作与确认标志   
	FileOp.hNameMappings = NULL;			//文件映射  
	FileOp.hwnd = NULL;						//消息发送的窗口句柄；  
	FileOp.lpszProgressTitle = NULL;		//文件操作进度窗口标题   
	FileOp.pFrom = strFrom.c_str();			//源文件及路径    //必须要以“\0\0”结尾，不然删除不了  
	FileOp.pTo = strTo.c_str();				//目标文件及路径
	FileOp.wFunc = FO_COPY;					//操作类型   
	return SHFileOperation(&FileOp); 
}
int CopyFolderA(const string& strSource,const string& strDest)
{
	SHFILEOPSTRUCTA FileOp;   
	//根据MSDN上，ZeryMerory在当缓冲区的字符串超出生命周期的时候，  
	//会被编译器优化，从而缓冲区的内容会被恶意软件捕捉到。  
	//引起软件安全问题，特别是对于密码这些比较敏感的信息而说。  
	//而SecureZeroMemory则不会引发此问题，保证缓冲区的内容会被正确的清零。  
	//如果涉及到比较敏感的内容，尽量使用SecureZeroMemory函数。 
	SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCTA));//secureZeroMemory和ZeroMerory的区别  

	string strFrom = strSource;
	strFrom.append(1,'\0');
	string strTo = strDest;
	strTo.append(1,'\0');
	SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));//secureZeroMemory和ZeroMerory的区别  

	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_NO_UI;		//操作与确认标志   
	FileOp.hNameMappings = NULL;			//文件映射  
	FileOp.hwnd = NULL;						//消息发送的窗口句柄；  
	FileOp.lpszProgressTitle = NULL;		//文件操作进度窗口标题   
	FileOp.pFrom = strFrom.c_str();			//源文件及路径    //必须要以“\0\0”结尾，不然删除不了  
	FileOp.pTo = strTo.c_str();				//目标文件及路径
	FileOp.wFunc = FO_COPY;					//操作类型   
	return SHFileOperationA(&FileOp); 
}
int DeleteFolder(const xstring& strDest)
{
	SHFILEOPSTRUCT FileOp;   
	//根据MSDN上，ZeryMerory在当缓冲区的字符串超出生命周期的时候，  
	//会被编译器优化，从而缓冲区的内容会被恶意软件捕捉到。  
	//引起软件安全问题，特别是对于密码这些比较敏感的信息而说。  
	//而SecureZeroMemory则不会引发此问题，保证缓冲区的内容会被正确的清零。  
	//如果涉及到比较敏感的内容，尽量使用SecureZeroMemory函数。 
	SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));//secureZeroMemory和ZeroMerory的区别  

	xstring strFrom = strDest;
	strFrom.append(1,_T('\0'));
	strFrom.append(1,_T('\0'));

	//FileOp.fFlags = FOF_ALLOWUNDO|FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_NO_UI;		//操作与确认标志
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_NO_UI;		//操作与确认标志   
	FileOp.hNameMappings = NULL;			//文件映射  
	FileOp.hwnd = NULL;						//消息发送的窗口句柄；  
	FileOp.lpszProgressTitle = NULL;		//文件操作进度窗口标题   
	FileOp.pFrom = strFrom.c_str();			//源文件及路径    //必须要以“\0\0”结尾，不然删除不了  
	FileOp.pTo = NULL;						//目标文件及路径
	FileOp.wFunc = FO_DELETE;				//操作类型   
	return SHFileOperation(&FileOp); 
}
bool CopyFolderRecursive(const wstring& strSource,const wstring& strDest)
{
	WIN32_FIND_DATA fd;
	wchar_t szTempFileFind[512] = { 0 };
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));
	swprintf_s(szTempFileFind, L"%s\\*.*", strSource.c_str());
	HANDLE hFind = FindFirstFile(szTempFileFind, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;

	//先创建目标文件夹
	CreateDirW(strDest);

	while(true)
	{
		if ( FindNextFileW(hFind, &fd) )
		{
			if (_tcsicmp(fd.cFileName,L".")==0)
			{
				continue;						
			}
			else if (_tcsicmp(fd.cFileName,L"..")==0)
			{
				continue;
			}
			else
			{
				if ( (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)//不是目录
				{
					wstring strSrcFile = strSource + L"\\";
					strSrcFile += fd.cFileName;
					wstring strDestFile = strDest + L"\\";
					strDestFile += fd.cFileName; 

					if (CopyFileW(strSrcFile.c_str(),strDestFile.c_str(),FALSE) == FALSE)
					{
						FindClose(hFind);
						return false;
					}	
				}
				else
				{
					wstring strSrcFile = strSource + L"\\";
					strSrcFile += fd.cFileName;
					wstring strDestFile = strDest + L"\\";
					strDestFile += fd.cFileName;

					if (CopyFolderRecursive(strSrcFile,strDestFile) == false)
					{
						FindClose(hFind);
						return false;
					}
				}
			}
	
		}
		else
			break;
	}//while
	FindClose(hFind);
	return true;	
}
// 递归查询文件源目录并复制源目录文件到目标目录
bool RecursionSearchFile(const wchar_t* pszSource,const wchar_t* pszDestFileName,wchar_t* pszOutMsg,int nOutMaxLen)
{
	_wfinddata_t file_info;
	int file_num = 0;
	wstring current_path = pszSource;
	current_path += L"\\*.*";//可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台 
	//wstring target_path = pszDestDir;
	int handle = _wfindfirst(current_path.c_str(),&file_info);
	if (handle == -1)
	{
		if (pszOutMsg)
			swprintf(pszOutMsg,_T("查找文件失败[%s]"),current_path);
		return false;
	}

	do 
	{
		wstring attribute;		
		if (wcscmp(file_info.name,L".") == 0 || wcscmp(file_info.name,L"..") == 0)
		{
			file_num++;
			continue;
		}

		if ((file_info.attrib&_A_SUBDIR)!=0)
		{
			// 是子目录，递归遍历
			wstring sourcePath = pszSource;
			sourcePath += L"\\";
			sourcePath += file_info.name;
			//wstring targetPath = pszDestDir;
			//targetPath += L"\\";
			//targetPath += file_info.name;
			//if ( CreateDir(targetPath) == FALSE)
			//{
			//	if (pszOutMsg)
			//		swprintf(pszOutMsg,_T("创建文件夹[%s]失败"),file_info.name);
			//	return false;
			//}

			bool bRec = RecursionSearchFile(sourcePath.c_str(),pszDestFileName,pszOutMsg,nOutMaxLen);
			if (!bRec)
				return false;
		}
		else
		{
			wstring strNewFile = pszSource;
			strNewFile += L"\\";
			strNewFile += file_info.name;
			wstring wstrFileName = GetFileNameFromPath(strNewFile);
			if (wstrFileName == pszDestFileName)
			{
				if (pszOutMsg)
					swprintf(pszOutMsg,_T("%s"),strNewFile.c_str());
				return true;
			}
			//wstring strOldFile = pszDestDir;
			//strOldFile += L"\\";
			//strOldFile += file_info.name;
			//if (CopyFile(strNewFile.c_str(),strOldFile.c_str(),FALSE) == FALSE)
			//{
			//	if (pszOutMsg)
			//		swprintf(pszOutMsg,_T("[%s]新文件覆盖[%s]失败,err=%ld"),strNewFile.c_str(),strOldFile.c_str(),GetLastError());
			//	return false;
			//}	
		}

		file_num++;

	} while (!_wfindnext(handle,&file_info));
	_findclose(handle);

	return true;
}

int GetProcesssIdFromName(const xstring& strPorcessName,bool bCaseSensitive/* = false*/)
{
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hProcess == INVALID_HANDLE_VALUE )
		return -1;

	PROCESSENTRY32 pinfo; 
	pinfo.dwSize = sizeof(PROCESSENTRY32);

	BOOL report = Process32First(hProcess, &pinfo); 
	while(report) 
	{ 
		if (bCaseSensitive)
		{
			if (strPorcessName == pinfo.szExeFile)
			{
				CloseHandle( hProcess );
				return pinfo.th32ProcessID;
			}
		}
		else
		{
			if (_tcsicmp(strPorcessName.c_str(),pinfo.szExeFile) == 0)
			{
				CloseHandle( hProcess );
				return pinfo.th32ProcessID;
			}
		}

		report = Process32Next(hProcess, &pinfo);
	}

	CloseHandle( hProcess );
	return -1;	
}
void GetProcesssIdFromName(const xstring& strPorcessName,deque<int>& dequeOutID,bool bCaseSensitive/* = false*/)
{
	dequeOutID.clear();
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hProcess == INVALID_HANDLE_VALUE )
		return ;

	PROCESSENTRY32 pinfo; 
	pinfo.dwSize = sizeof(PROCESSENTRY32);

	BOOL report = Process32First(hProcess, &pinfo); 
	while(report) 
	{ 
		if (bCaseSensitive)
		{
			if (strPorcessName == pinfo.szExeFile)
				dequeOutID.push_back(pinfo.th32ProcessID);
		}
		else
		{
			if (_tcsicmp(strPorcessName.c_str(),pinfo.szExeFile) == 0)
				dequeOutID.push_back(pinfo.th32ProcessID);
		}



		report = Process32Next(hProcess, &pinfo);
	}

	CloseHandle( hProcess );
}

void GetProcesssInfoFromName(const xstring& strPorcessName,map<int,wstring>& mapOutID,bool bCaseSensitive/* = false*/)
{
	mapOutID.clear();
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hProcess == INVALID_HANDLE_VALUE )
	{
		return ;
	}

	TCHAR szShortPath[MAX_PATH*2] = { 0 };
	PROCESSENTRY32 pinfo; 
	MODULEENTRY32 minfo;
	pinfo.dwSize = sizeof(PROCESSENTRY32);
	minfo.dwSize = sizeof( MODULEENTRY32);

	BOOL report = Process32First(hProcess, &pinfo); 
	while(report) 
	{ 
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pinfo.th32ProcessID); 
		if( hModule != INVALID_HANDLE_VALUE )
		{
			if( Module32First( hModule, &minfo ) )
			{
				xstring strCurExeName = minfo.szExePath;
				int nPos = strCurExeName.rfind(_T('\\'));
				strCurExeName = strCurExeName.substr(nPos+1);
				if (bCaseSensitive)
				{
					if (strPorcessName == strCurExeName)
					{
						mapOutID[pinfo.th32ProcessID] = minfo.szExePath;
					}
				}
				else
				{
					if (_tcsicmp(strPorcessName.c_str(),strCurExeName.c_str()) == 0)
					{
						mapOutID[pinfo.th32ProcessID] = minfo.szExePath;
					}
				}

			}

			CloseHandle( hModule );
		}
		report = Process32Next(hProcess, &pinfo);
	}

	CloseHandle( hProcess );
}

long GetFileSizeByte(const xstring& strFile)
{
	FILE* file = NULL;
#ifdef UNICODE
	fopen_s(&file,_A(strFile), "r");
#else
	fopen_s(&file,strFile.c_str(), "r");
#endif		
	if (file)
	{
		long nSize = _filelength(_fileno(file));
		fclose(file);
		return nSize;
	}
	return -1;
}

long GetFileSizeByteA(const string& strFile)
{
	FILE* file = NULL;
	fopen_s(&file,strFile.c_str(), "r");
	if (file)
	{
		long nSize = _filelength(_fileno(file));
		fclose(file);
		return nSize;
	}
	return -1;
}


xstring GetTimeZoneNow()
{
	//其中UTC=localtime+bias（UTC时间=本地时间+bias），具体含义参看MSDN例子：
	TIME_ZONE_INFORMATION tzi;
	GetSystemTime(&tzi.StandardDate);  
	GetTimeZoneInformation(&tzi);  
#ifdef UNICODE
	xstring strStandName = tzi.StandardName; 
	xstring strDaylightName = tzi.DaylightName;  
#else
	xstring strStandName = UnicodeToAnsi(tzi.StandardName); 
	xstring strDaylightName = UnicodeToAnsi(tzi.DaylightName);  
#endif
	
	int zone = tzi.Bias/ -60; //时区，如果是中国标准时间则得到8 

	xstring strRetName;

	TCHAR    achKey[1024] = {0};	 // buffer for subkey name  
	DWORD    cbName = 0;                   // size of name string   
	TCHAR    achClass[MAX_PATH] = {0};  // buffer for class name   
	DWORD    cchClassName = MAX_PATH;  // size of class string   
	DWORD    cSubKeys=0;               // number of subkeys   
	DWORD    cbMaxSubKey;              // longest subkey size   
	DWORD    cchMaxClass;              // longest class string   
	DWORD    cValues;              // number of values for key   
	DWORD    cchMaxValue;          // longest value name   
	DWORD    cbMaxValueData;       // longest value data   
	DWORD    cbSecurityDescriptor; // size of security descriptor   
	FILETIME ftLastWriteTime;      // last write time   

	DWORD i, retCode;   

	TCHAR  achValue[1024] = {0};   
	DWORD cchValue = 1024;   
	HKEY hDestKey = NULL;
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones"),0,KEY_READ, &hDestKey) == ERROR_SUCCESS )  
	{  
		// Get the class name and the value count.   
		retCode = RegQueryInfoKey(  
			hDestKey,                    // key handle   
			achClass,                // buffer for class name   
			&cchClassName,           // size of class string   
			NULL,                    // reserved   
			&cSubKeys,               // number of subkeys   
			&cbMaxSubKey,            // longest subkey size   
			&cchMaxClass,            // longest class string   
			&cValues,                // number of values for this key   
			&cchMaxValue,            // longest value name   
			&cbMaxValueData,         // longest value data   
			&cbSecurityDescriptor,   // security descriptor   
			&ftLastWriteTime);       // last write time   

		for (i=0; i<cSubKeys; i++)   
		{   
			cbName = 1024;  
			retCode = RegEnumKeyEx(hDestKey, i,  achKey,&cbName, NULL, NULL, NULL, &ftLastWriteTime);   
			if (retCode == ERROR_SUCCESS)   
			{  
				xstring strSubKsy = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\");
				strSubKsy += achKey;
				xstring strName;
				ReadRegString(HKEY_LOCAL_MACHINE,strSubKsy,_T("Std"),REG_SZ,strName);
				if (strName == strStandName)
				{
					strRetName = achKey;
					break;
				}
			}  
		}  

		RegCloseKey(hDestKey);
	}  

	return strRetName; 
}

bool SetTimeZone(const xstring& subKey)
{
	//提升进程权限
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp; 
	if ( OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken) )
	{
		if ( LookupPrivilegeValue(NULL, TEXT("SeTimeZonePrivilege"), &tkp.Privileges[0].Luid) )
		{
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if ( AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0) )
			{
				//提权完毕,进行修改
				HKEY hKey = NULL;
				TIME_ZONE_INFORMATION tziNew;
				typedef struct tagREG_TZI_FORMAT
				{
					LONG Bias;
					LONG StandardBias;
					LONG DaylightBias;
					SYSTEMTIME StandardDate;
					SYSTEMTIME DaylightDate;
				}REG_TZI_FORMAT;
				REG_TZI_FORMAT regTZI;
				DWORD dwBufLen=sizeof(regTZI);
				LONG lRet;
				xstring strKeyName = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\") + subKey;
				xstring strStd = _T("");//标准时间名称

				xstring strDlt = _T("");//夏令时名称
				unsigned char szData[512] = {0};
				DWORD dwDataType, dwBufSize;
				dwBufSize = 512;
				lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName.c_str(),    0, KEY_QUERY_VALUE, &hKey ); 
				if( lRet != ERROR_SUCCESS )
					return false;

				lRet = RegQueryValueEx( hKey, TEXT("TZI"), NULL, NULL,(LPBYTE)&regTZI, &dwBufLen);

				if(RegQueryValueEx(hKey, _T("Dlt"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
					strDlt = (LPCTSTR)szData;
				dwBufSize = 512;
				if(RegQueryValueEx(hKey, _T("Std"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
					strStd = (LPCTSTR)szData;
				RegCloseKey(hKey);
				if( (lRet != ERROR_SUCCESS) || (dwBufLen > sizeof(regTZI)) )
					return false;
				//设置值
				ZeroMemory(&tziNew, sizeof(tziNew));
				tziNew.Bias = regTZI.Bias;
				tziNew.StandardDate = regTZI.StandardDate;
				wcscpy_s(tziNew.StandardName, strStd.c_str());
				wcscpy_s(tziNew.DaylightName, strDlt.c_str());
				tziNew.DaylightDate = regTZI.DaylightDate;
				tziNew.DaylightBias = regTZI.DaylightBias;

				if( !SetTimeZoneInformation( &tziNew ) )
				{
					RegFlushKey(HKEY_LOCAL_MACHINE);
					return false;
				}

				RegFlushKey(HKEY_LOCAL_MACHINE);
				tkp.Privileges[0].Attributes = 0;
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
				return true;
			}	
			else
			{
				
			}
		}	
		else
		{
			
		}
	}
	else
	{
		
	}
	return false;
}


void FreeHandles(HCERTSTORE hFileStore, PCCERT_CONTEXT pctx,     HCERTSTORE pfxStore, HCERTSTORE myStore )
{
	if (myStore)
		CertCloseStore(myStore, 0);

	if (pfxStore)
		CertCloseStore(pfxStore, CERT_CLOSE_STORE_FORCE_FLAG);

	if(pctx)
		CertFreeCertificateContext(pctx);

	if (hFileStore)
		CertCloseStore(hFileStore, 0);
}
int ImportCACert(BYTE* pBinByte , unsigned long binBytes)
{
	HCERTSTORE pfxStore = 0;
	HCERTSTORE myStore = 0;
	HCERTSTORE hFileStore = 0;
	PCCERT_CONTEXT pctx = NULL;
	DWORD err = 0;

	pctx = CertCreateCertificateContext(MY_ENCODING_TYPE, (BYTE*)pBinByte , binBytes );
	if(pctx == NULL)
	{
		DWORD err = GetLastError();
		FreeHandles(hFileStore,pctx, pfxStore, myStore);   
		return err;
	}

	// we open the store for the CA
	hFileStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, 0, CERT_STORE_OPEN_EXISTING_FLAG | CERT_SYSTEM_STORE_LOCAL_MACHINE, L"Root" );
	if (!hFileStore)
	{
		DWORD err = GetLastError();
		FreeHandles(hFileStore,pctx, pfxStore, myStore);   
		return err;
	}

	if( !CertAddCertificateContextToStore(hFileStore, pctx, CERT_STORE_ADD_NEW, 0) )
	{
		err = GetLastError();
		if( CRYPT_E_EXISTS == err )
		{
			{
				if( !CertAddCertificateContextToStore(hFileStore, pctx , CERT_STORE_ADD_REPLACE_EXISTING, 0))
				{
					err = GetLastError();
					FreeHandles(hFileStore,pctx, pfxStore, myStore);                     
					return err;
				}
			}
		}
		else
		{
			FreeHandles(hFileStore, pctx , pfxStore , myStore);

			return err;
		}
	}
	FreeHandles(hFileStore,pctx, pfxStore, myStore);

	return 0;
}
int ImportCACertFile(const xstring& strFileName)
{
	HANDLE hfile = INVALID_HANDLE_VALUE;

	BYTE pByte[4096] = {0} , pBinByte[8192]={0};
	unsigned long bytesRead = 0;
	unsigned long binBytes = 4096;

	// Open it...
	hfile = CreateFile(strFileName.c_str(), FILE_READ_DATA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == hfile)
		return -1;

	ReadFile( hfile , pByte, 4096, &bytesRead ,NULL );
	CloseHandle(hfile);

	CryptStringToBinaryA( (LPCSTR)pByte , bytesRead ,CRYPT_STRING_BASE64HEADER , pBinByte , &binBytes ,NULL,NULL);

	return ImportCACert(pBinByte , binBytes );
}
int ImportCACertString(const string& strTxt)
{
	BYTE pBinByte[8192]={0};
	unsigned long binBytes = 4096;

	CryptStringToBinaryA( strTxt.c_str(), strTxt.length() ,CRYPT_STRING_BASE64HEADER , pBinByte , &binBytes ,NULL,NULL);

	return ImportCACert(pBinByte , binBytes);
}

long GetCurStartProcessPID()
{
	typedef struct  
	{  
		unsigned long ExitStatus;  
		unsigned long PebBaseAddress;  
		unsigned long AffinityMask;  
		unsigned long BasePriority;  
		unsigned long UniqueProcessId;  
		unsigned long InheritedFromUniqueProcessId;  
	}PROCESS_BASIC_INFORMATION;
	typedef long (__stdcall *PROCNTQSIP)(void*,unsigned int,void*,unsigned long,unsigned long*);

	PROCESS_BASIC_INFORMATION pbi;
	memset(&pbi,0,sizeof(pbi));

	PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress( GetModuleHandleA("ntdll"), "NtQueryInformationProcess" ); 
	if(NtQueryInformationProcess)
	{
		if  ( NtQueryInformationProcess( GetCurrentProcess(),0,(PVOID)&pbi,sizeof(PROCESS_BASIC_INFORMATION),NULL)==0 )
		{
			return pbi.InheritedFromUniqueProcessId;
		}
	}

	return -1;
}
long GetStartProcessPID(DWORD dwId)
{
	typedef struct  
	{  
		unsigned long ExitStatus;  
		unsigned long PebBaseAddress;  
		unsigned long AffinityMask;  
		unsigned long BasePriority;  
		unsigned long UniqueProcessId;  
		unsigned long InheritedFromUniqueProcessId;  
	}PROCESS_BASIC_INFORMATION;
	typedef long (__stdcall *PROCNTQSIP)(void*,unsigned int,void*,unsigned long,unsigned long*);

	PROCESS_BASIC_INFORMATION pbi;
	memset(&pbi,0,sizeof(pbi));

	PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress( GetModuleHandleA("ntdll"), "NtQueryInformationProcess" ); 
	if(NtQueryInformationProcess)
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,dwId);
		if(hProcess)
		{
			if  ( NtQueryInformationProcess( hProcess,0,(PVOID)&pbi,sizeof(PROCESS_BASIC_INFORMATION),NULL)==0 )
			{
				CloseHandle(hProcess);
				return pbi.InheritedFromUniqueProcessId;
			}
			CloseHandle(hProcess);
		}
	}

	return -1;
}

void SetIEWebbrowserVersion(DWORD dwIEVersion /* = 8000 */)
{
	TCHAR szAppPath[512] = {0};
	GetModuleFileName(NULL,szAppPath,512);
	xstring strAppName = GetAppNameFromPath(szAppPath);

	WriteRegValue(HKEY_CURRENT_USER,L"Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION",strAppName,REG_DWORD,(const BYTE *)&dwIEVersion,sizeof(dwIEVersion));
	WriteRegValue(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION",strAppName,REG_DWORD,(const BYTE *)&dwIEVersion,sizeof(dwIEVersion));
}

string GetCpuIndex()
{
	char szCpuIndex[64] = {0};
	unsigned long s1, s2;
	__asm{
		mov eax, 01h
			xor edx, edx
			cpuid 
			mov s1, edx
			mov s2, eax
	}

	_snprintf_s(szCpuIndex,sizeof(szCpuIndex),"%08X%08X",s1,s2);
	return szCpuIndex;
}
wstring GetCpuDescr()
{
	wstring strValue;
	ReadRegString(HKEY_LOCAL_MACHINE,L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",L"ProcessorNameString",REG_SZ,strValue);
	return strValue;
}
int GetCpuCoreNum()
{
	SYSTEM_INFO theSysInfo;
	GetSystemInfo(&theSysInfo);
	return (int)theSysInfo.dwNumberOfProcessors;
}

DWORDLONG GetMemorySize()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	DWORDLONG dwMemorySize = statex.ullTotalPhys;
	return dwMemorySize;
}

//由路径获取文件名
wstring GetFileNameFromPath(const wstring& strPath)
{
	wstring strName = strPath;
	int nPos = strName.find_last_of(L"/\\");
	if (nPos != string::npos)
		strName.erase(0,nPos+1);
	return strName;
}
string GetFileNameFromPathA(const string& strPath)
{
	string strName = strPath;
	int nPos = strName.find_last_of("/\\");
	if (nPos != string::npos)
		strName.erase(0,nPos+1);
	return strName;
}

bool CopyStringToClipboard(const wstring& strValue)
{
	if(OpenClipboard(NULL))
	{  
		EmptyClipboard();
		HANDLE hClip=GlobalAlloc(GMEM_MOVEABLE,strValue.length()*2+2); 
		wchar_t* pszBuf =(wchar_t*)GlobalLock(hClip);
		wcscpy(pszBuf,strValue.c_str());
		GlobalUnlock(hClip);

		SetClipboardData(CF_UNICODETEXT,hClip);
		CloseClipboard();
		
		GlobalFree(hClip);
		hClip = NULL;
		return true;
	} 

	return false;
}

#if 0
string GetBIOSUUID()
{
	try
	{
		//创建匿名管道
		SECURITY_ATTRIBUTES sa;  
		HANDLE hRead=NULL,hWrite=NULL;  
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
		sa.lpSecurityDescriptor = NULL;  
		sa.bInheritHandle = TRUE;  
		if (!CreatePipe(&hRead,&hWrite,&sa,0))   
		{
			OutputDebugStringW(L"查询执行失败0\n");
			return "";  
		}   

		wchar_t szCmd[128] = {0};
		_snwprintf_s(szCmd,_countof(szCmd),_TRUNCATE,L"cmd.exe /C wmic csproduct get UUID");
		STARTUPINFOW si;  
		ZeroMemory(&si, sizeof(STARTUPINFOW));
		PROCESS_INFORMATION pi;   
		si.cb = sizeof(STARTUPINFOW);  
		GetStartupInfoW(&si);   
		si.hStdError = hWrite;            //把创建进程的标准错误输出重定向到管道输入  
		si.hStdOutput = hWrite;           //把创建进程的标准输出重定向到管道输入  
		si.wShowWindow = SW_HIDE;  
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  
		if (!CreateProcess(NULL, szCmd,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
		{  
			CloseHandle(hWrite);  
			CloseHandle(hRead);  
			OutputDebugStringW(L"查询执行失败1\n");
			return "";
		}  

		if (WaitForSingleObject(pi.hProcess,5000) == WAIT_TIMEOUT)
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			if (hWrite)
				CloseHandle(hWrite);  
			return "";
		}

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		if (hWrite)
			CloseHandle(hWrite);  

		string strResult;
		if (hRead)
		{
			int nSize = GetFileSize(hRead, NULL)+4;		//加4避免申请的堆内存刚好被文件内容填满后,字符串末尾没有'\0'了
			char* szBuf = (char*)malloc(nSize);
			memset(szBuf,0,nSize);
			DWORD dwReadSize=0;

			//char szLog[128] = {0};
			//sprintf(szLog,"数据大小:%d\n",nSize);
			//OutputDebugStringA(szLog);

			ReadFile(hRead, szBuf, nSize, &dwReadSize, NULL);
			strResult = szBuf;
			free(szBuf);
			CloseHandle(hRead);

			/*vector<string> vecItem;
			SplitStringA(strResult.c_str(),vecItem,"\r\n");
			for (vector<string>::iterator itr=vecItem.begin();itr!=vecItem.end();itr++)
			{
				string strItem = *itr;	
				TrimStringA(strItem,"\n");
				TrimStringA(strItem,"\r");				
				TrimStringA(strItem," ");
				if (strItem.length()>31 && strItem.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789- ")==string::npos)
				{
					strResult = strItem;
					break;
				}
			}*/
			int nStart = strResult.find_first_not_of(" \n\r\t",4);
			int nEnd = strResult.find_last_not_of(" \n\r\t");
			strResult = strResult.substr(nStart,nEnd-nStart+1);
			if (strResult.empty() == false)
			{
				return strResult;
			}
		}
	}
	catch (...)
	{}	

	return "";   
}
#else
string GetBIOSUUID()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);//NULL, COINIT_APARTMENTTHREADED
	try
	{
		IWbemLocator *pLoc = NULL;
		HRESULT hres = CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator, (LPVOID *) &pLoc);
		if (FAILED(hres))
		{
			CoUninitialize();
			return "";    
		}

		IWbemServices *pSvc = NULL;
		hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"),NULL,NULL,0,NULL,0,0,&pSvc);
		if (FAILED(hres))
		{
			pLoc->Release();     
			CoUninitialize();
			return "";                
		}
		hres = CoSetProxyBlanket(pSvc,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE);
		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();     
			CoUninitialize();
			return "";              
		}

		IEnumWbemClassObject* pEnumerator = NULL;
		hres = pSvc->ExecQuery(_bstr_t("WQL"),_bstr_t("SELECT * FROM Win32_ComputerSystemProduct"),WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumerator);
		if (FAILED(hres))
		{
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			
			return "";              
		}

		IWbemClassObject *pclsObj = NULL;
		char szValue[128] = {0};
		while (pEnumerator)
		{
			ULONG uReturn = 0;
			hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
			if(0 == uReturn)
				break;

			VARIANT vtProp;
			hres = pclsObj->Get(L"UUID", 0, &vtProp, 0, 0); 
			wcstombs(szValue,vtProp.bstrVal,127);
			VariantClear(&vtProp);
			pclsObj->Release();    
		} 
		pEnumerator->Release();
		pEnumerator = NULL;

		CoUninitialize();
		return szValue;

	}
	catch (...)
	{}	

	CoUninitialize();
	return "";   
}
#endif

xstring GetMachineGUID()
{
	xstring strValue;
	ReadRegString(HKEY_LOCAL_MACHINE,L"SOFTWARE\\Microsoft\\Cryptography",L"MachineGuid",REG_SZ,strValue);
	return strValue;
}