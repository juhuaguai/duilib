#pragma once
#include "netfw.h"
#include <string>
using namespace std;

//描述：windows防火墙，操作辅助类

class CSystemFWHelper
{
public:
	CSystemFWHelper(void);
	~CSystemFWHelper(void);

private:
	INetFwOpenPorts *m_pFwAuthPorts;
	INetFwAuthorizedApplications *m_pFwAuthApps;

private:
	void Init();
	void UnInit();

public:
	//添加到防火墙
	bool AddNewApp(const wstring& strAppPath,const wstring& strAppDisName);
	//检查是否在防火墙
	bool CheckAppAndEnable(const wstring& strAppPath);
};

