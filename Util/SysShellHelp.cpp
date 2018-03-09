#include "SysShellHelp.h"

typedef void (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO);


CSysShellHelp::CSysShellHelp(void)
{
}

CSysShellHelp::~CSysShellHelp(void)
{
}

//***************************************************
//名称：ExeFile
//功能：启动可执行文件
//参数：
//      strExePath - 可执行文件
//      strCmd     - 命令行
//返回值：0 失败，1 - 成功
//***************************************************
int CSysShellHelp::ExeFile(const string& strExePath, const string& strCmd, int nTimeout)
{
	PROCESS_INFORMATION  pi;
	STARTUPINFOA StartInfo;
	ZeroMemory(&StartInfo, sizeof(STARTUPINFOA));
	StartInfo.cb = sizeof(STARTUPINFOA);
	StartInfo.dwX = 20;
	StartInfo.dwY = 20;
	StartInfo.dwXSize = 200;
	StartInfo.dwYSize = 200;
	StartInfo.lpDesktop = NULL;
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_SHOW;//SW_HIDE;

	string strExe(strExePath);

	if (!strExe.empty())
	{
		strExe += " ";
	}

	strExe += strCmd;
	if (!CreateProcessA(NULL, //strExe.c_str(),// strExePath.c_str(),
		const_cast<char*>(strExe.c_str()),//NULL,//const_cast<char*>(strCmd.c_str()),
		NULL, 
		NULL,
		FALSE, 
		0,
		NULL,
		NULL,
		&StartInfo,
		&pi))
	{
		DWORD dwErr = GetLastError();

		return -1;
	}
	else   
	{
		//WaitForSingleObject(pi.hProcess,nTimeout);   
		CloseHandle(pi.hProcess);   
		CloseHandle(pi.hThread);   
	} 

	return 1;
}

int  CSysShellHelp::Source2File(const char* sFname, HMODULE hModule, DWORD dwID, const char* sType)
{
	HRSRC res=FindResourceA(hModule, MAKEINTRESOURCEA(dwID), sType);

	if (!res)
	{
		return -1;
	}
	{
		DWORD size = SizeofResource(hModule,res);
		HGLOBAL gl = LoadResource(hModule, res);
		LPVOID  lp = LockResource(gl);
		{
			FILE* pfile = fopen(sFname, "wb");
			if(pfile)
			{
				fwrite(lp, size, 1, pfile);
				fclose(pfile);
			}
		}
		UnlockResource(gl);
		FreeResource(gl);
	}

	return 0;
}

void CSysShellHelp::GetSystemName(char* pszRet, unsigned uSize)
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	std::string strOsName = "unknown";

	if (GetVersionEx((OSVERSIONINFO*)&os))
	{
		switch (os.dwMajorVersion)
		{
		case 4:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
				{
					strOsName = "Microsoft Windows NT 4.0";
				}
				else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				{
					strOsName = "Microsoft Windows 95";
				}
				break;
			case 10:
				{
					strOsName = "Microsoft Windows 98";
				}
				break;
			case 90:
				{
					strOsName = "Microsoft Windows Me";
				}
				break;
			}
			break;

		case 5:
			switch (os.dwMinorVersion)
			{
			case 0:
				strOsName = "Microsoft Windows 2000";
				break;
			case 1:
				strOsName = "Microsoft Windows XP";
				break;
			case 2:
				if (os.wProductType == VER_NT_WORKSTATION
					&& info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					strOsName = "Microsoft Windows XP Professional x64 Edition";
				}
				else if (GetSystemMetrics(SM_SERVERR2) == 0)
				{
					strOsName = "Microsoft Windows Server 2003";
				}
				else if (GetSystemMetrics(SM_SERVERR2) != 0)
				{
					strOsName = "Microsoft Windows Server 2003 R2";
				}
				break;
			}
			break;

		case 6:
			switch (os.dwMinorVersion)
			{
			case 0:
				if (os.wProductType == VER_NT_WORKSTATION)
				{
					strOsName = "Microsoft Windows Vista";
				}
				else
				{
					strOsName = "Microsoft Windows Server 2008";
				}
				break;
			case 1:
				if (os.wProductType == VER_NT_WORKSTATION)
				{
					strOsName = "Microsoft Windows 7";
				}
				else
				{
					strOsName = "Microsoft Windows Server 2008 R2";
				}
				break;
			case 2:
				strOsName = "Microsoft Windows 8";
				break;
			}
			break;
		}
	}

	strncpy(pszRet, strOsName.data(), uSize);
}

BOOL  CSysShellHelp::Is64Bit_OS()
{
	BOOL bRetVal = FALSE;
	SYSTEM_INFO si = { 0 };
	LPFN_PGNSI pGNSI = (LPFN_PGNSI) GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetNativeSystemInfo");
	if (pGNSI == NULL)
	{
		return FALSE;
	}
	pGNSI(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || 
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		bRetVal = TRUE;
	}
	else
	{
		// 32 位操作系统
		//_tprintf(_T("is 32 bit OS\r\n"));
	}
	return bRetVal;
}

BOOL CSysShellHelp::CheckIEVer(int &nVerNum)
{
	CRegKey regkey;//定义注册表类对象
	LONG lResult = regkey.Open(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Internet Explorer"),KEY_QUERY_VALUE); //打开 注册表键 
	if( lResult != ERROR_SUCCESS ) 
		return FALSE;

	TCHAR szIEVer[50];
	DWORD dwSize = 50; 
	
	regkey.QueryValue(szIEVer,_T("Version"),&dwSize);//获取Version字段值
	regkey.Close();//关闭注册表

	USES_CONVERSION;

	string strIEVer = T2A(szIEVer);
	int nPos = strIEVer.find_first_of(".");
	nVerNum = atoi(strIEVer.substr(0,nPos).c_str());
	return true;
}

//描述：运行命令行命令
//
void CSysShellHelp::RunCmd(string& strCmdText)
{
	//system(strCmdText.c_str());
	WinExec(strCmdText.c_str(), SW_HIDE);
}

BOOL CSysShellHelp::EnableDebugPriv()
{
	HANDLE hToken; 
	LUID sedebugnameValue; 
	TOKEN_PRIVILEGES tkp; 

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) { 
		return FALSE;
	} 
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) { 
		CloseHandle(hToken); 
		return FALSE;
	} 
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Luid = sedebugnameValue; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) { 
		CloseHandle(hToken);
		return FALSE;
	} 
	return TRUE;
}

//描述：取消Win7 64bit运行程序前安全警告(.bat;.exe;.reg;.vbs)
//      注：需管理员权限，可写入
void CSysShellHelp::DisableSafetyWarn()
{
	HKEY hKey;
	LRESULT lRet = 0;
	xstring strValue = _T(".exe");

	lRet= RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Associations"), 0, KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if(lRet == ERROR_SUCCESS) 
	{     
		//创建或者修改键值
		RegSetValueEx(hKey, _T("ModRiskFileTypes"),0,REG_SZ,(LPBYTE)strValue.c_str(), strValue.length() * 2);
	}
	else
	{
		DWORD dwDisposition;
		lRet = RegCreateKeyEx(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Associations"), 0,NULL, REG_OPTION_NON_VOLATILE, NULL,NULL, &hKey, &dwDisposition);
		if (lRet == ERROR_SUCCESS)
		{
			lRet = RegSetValueEx(hKey, _T("ModRiskFileTypes"),0,REG_SZ,(LPBYTE)strValue.c_str(), strValue.length() * 2);
		}
	}
	RegCloseKey(hKey);
	RunCmd(string("gpupdate /force"));
}

//描述：
//    枚举系统进程，发现某个进程，并杀掉
//参数：
//    strAppExeName - 要杀到的进程的应用程序的文件名
//    bIsAll        - 是否所有的，若为FALSE，找到一个即退出
BOOL CSysShellHelp::KillProcess(const xstring &strAppExeName, BOOL bIsAll/* = TRUE*/)
{
	try{
		HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if( hProcess == INVALID_HANDLE_VALUE )
		{
			return( FALSE );
		}

		TCHAR szShortPath[MAX_PATH] = { 0 };
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

					TCHAR *pFind = _tcsrchr(minfo.szExePath, _T('\\'));
					xstring strCurExeName = (++pFind);

					if (strAppExeName.compare(strCurExeName) == 0)
					{
						HANDLE hExitProcess = OpenProcess( PROCESS_TERMINATE, FALSE, pinfo.th32ProcessID );
						if( hExitProcess)
						{
							::TerminateProcess(hExitProcess, 0);
							::CloseHandle(hExitProcess);
						}

						if (!bIsAll)
						{
							CloseHandle( hModule );
							break;
						}
					}
				}

				CloseHandle( hModule );
			}

			report = Process32Next(hProcess, &pinfo);
		}

		CloseHandle( hProcess );
	}
	catch(...)
	{}

	return TRUE;
}

BOOL SystemShutDown()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) 
		return FALSE; 

	return TRUE;

}
BOOL SystemReboot()
{
	
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		return( FALSE ); 

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	// Shut down the system and force all applications to close. 

	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) 
		return FALSE; 

	return TRUE;
}

void CSysShellHelp::StartExeByCmd(const char* szCmd)
{
	string opt("cmd.exe /c \"");

	opt += szCmd;
	opt += "\"";

	::WinExec(opt.c_str(), SW_HIDE);
}
