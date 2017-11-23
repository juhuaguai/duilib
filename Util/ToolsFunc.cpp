#include <winsock2.h>
#include <Windows.h>
#include <time.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <commoncontrols.h>
#include <OleCtl.h>
#include <ShellAPI.h>
#include "ToolsFunc.h"
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

BOOL CreateDir(const xstring& strDir)
{
	if (PathIsDirectory(strDir.c_str()) == FALSE)
		return CreateDirectory(strDir.c_str(),NULL);

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
	WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
	return GetFileAttributesEx(szPath, GetFileExInfoStandard, &attrs);
}

bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue)
{
	strValue.clear();
	HKEY hKeyHander = NULL ;
	if (RegOpenKeyEx(hKey,strSubKey.c_str(),NULL,KEY_QUERY_VALUE,&hKeyHander) != ERROR_SUCCESS)
		return false;

	TCHAR szData[1024] = {0};
	DWORD dwSize = 1024;
	DWORD dwTp = dwType;
	if (RegQueryValueEx(hKeyHander,strKeyName.c_str(),NULL,&dwTp,(LPBYTE)szData,&dwSize) == ERROR_SUCCESS)
	{
		if (dwType == REG_SZ)
			strValue = szData;
		else if (dwType==REG_DWORD||dwType==REG_BINARY)
		{
			_sntprintf_s(szData,_countof(szData),_TRUNCATE,_T("%ld"),(long)*(long *)szData);
			strValue = szData;
		}

		RegCloseKey(hKeyHander);
		return true;
	}

	RegCloseKey(hKeyHander);
	return false;
}

xstring GetAppPath(HMODULE hModul)
{
	//初始化App路径
	TCHAR szAppPath[512] = {0};
	GetModuleFileName(hModul,szAppPath,512);	
	xstring strApp = szAppPath;
	unsigned int nPos = strApp.find_last_of(_T('\\'));
	if (nPos == std::string::npos)
		nPos = strApp.find_last_of(_T('/'));
	if (nPos != std::string::npos)
		strApp = strApp.substr(0,nPos);
	else
		OutputDebugString(_T("获取程序路径出错\n"));

	return strApp;
}


string GetLocalIp()
{
	WSADATA     wsaData; 
	WSAStartup(0x0202, &wsaData);
	//Before calling AddIPAddress we use GetIpAddrTable to get an adapter to which we can add the IP.
	PMIB_IPADDRTABLE pIPAddrTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	pIPAddrTable = (MIB_IPADDRTABLE*) malloc( sizeof( MIB_IPADDRTABLE) );

	// Make an initial call to GetIpAddrTable to get the necessary size into the dwSize variable
	if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
	{
		free( pIPAddrTable );
		pIPAddrTable = (MIB_IPADDRTABLE *) malloc ( dwSize );
	}
	else
	{
		free( pIPAddrTable );
		WSACleanup();
		return "";
	}

	// Make a second call to GetIpAddrTable to get the actual data we want
	dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 );
	if ( dwRetVal != NO_ERROR ) 
	{
		free( pIPAddrTable );
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

			free(pIPAddrTable);
			string strIP = inet_ntoa(inAddr);
			WSACleanup();
			return strIP;
		}
	}

	free( pIPAddrTable );
	WSACleanup();
	return "";
}

string GetMAC()
{
	string strMac;
	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	if (pAddresses == NULL) 
		return strMac;
	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAddresses);
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
			strMac = acMAC;
			break;
		}
	} 
	free(pAddresses);
	return strMac;
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
	xstring strRet = strAppPath;
	int nPos = strRet.rfind(_T('\\'));
	if (nPos == string::npos)
	{
		nPos = strRet.rfind(_T('/'));
	}
	if (nPos != string::npos)
	{
		strRet = strRet.substr(nPos+1);
	}
	return strRet;
}