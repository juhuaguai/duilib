#pragma once
//程序安装为系统服务
//检查服务，启动服务，停止服务
#include <Windows.h>
#include <tchar.h>
#include <WinSvc.h>
#include <algorithm>
#include <string>
using namespace std;

class CSysServiceHandler
{
public:
	CSysServiceHandler(void);
	~CSysServiceHandler(void);

private:
	void SetServiceDescription(const wstring& strServiceName, const wstring& strDesc);
	BOOL RunExec(const string& strCmd, const string& strPara, DWORD dwMilliseconds = INFINITE);

public:
	void make_upper(std::string& str);

	int InstallService(const wstring& strServiceName, const wstring& strServicePath, const wstring& strDesc, bool bIsAutoStart = true );
	bool IsServiceInstalled(const string& strServiceName);
	int IsServiceRunned(const string& strServiceName);
	bool StartServerBySCM(const string& strServiceName);
	bool StopServerBySCM(const string& strerviceName);
	bool UninstallService(const string& strServiceName);

	BOOL StartService(const string& strServiceName);
	BOOL StopService(const string& strServiceName);
	BOOL EnableService(const string& strServiceName,bool bAuto);
	BOOL DisableService(const string& strServiceName);

	bool GetServiceName(const string& strAppPath, string& strServiceName);
	bool GetServicePath(string& strAppPath, const string& strServiceName);
	bool ResetServicePath(const string& strAppPath, const string& strServiceName);


	BOOL RunExec(const string& strCmd, const string& strPara, DWORD dwMilliseconds = INFINITE);
	BOOL RunExec(const wstring& strCmd, const wstring& strPara, DWORD dwMilliseconds = INFINITE);
};
