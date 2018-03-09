#include "SysServiceHandler.h"

CSysServiceHandler::CSysServiceHandler(void)
{
}

CSysServiceHandler::~CSysServiceHandler(void)
{
}


int CSysServiceHandler::InstallService(const string& strServiceName, const string& strServicePath, const string& strDesc, bool bIsAutoStart /* = true */ )
{
	SC_HANDLE scm = OpenSCManagerA(0, 0, SC_MANAGER_CREATE_SERVICE);
	if (!scm) 
	{
		return -1;
	}

	//"Provides secure remote desktop sharing"
	//"Tcpip\0\0"   //指定启动该服务前必须先启动的服务或服务组
	SC_HANDLE service = CreateServiceA(scm, strServiceName.c_str(), strServiceName.c_str(), SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		bIsAutoStart ? SERVICE_AUTO_START : SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, strServicePath.c_str(),
		NULL, NULL, "Tcpip\0\0", NULL, NULL);	
	if (!service) 
	{
		DWORD myerror = GetLastError();
		if (myerror == ERROR_ACCESS_DENIED)
		{
			CloseServiceHandle(scm);
			return -2;
		}
		if (myerror == ERROR_SERVICE_EXISTS)
		{
			CloseServiceHandle(scm);
			return 0;
		}

		CloseServiceHandle(scm);
		return -3;
	}
	else
	{
		SetServiceDescription(strServiceName, strDesc);
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	return 1;
}


void CSysServiceHandler::SetServiceDescription(const string& strServiceName, const string& strDesc)
{
	// Add service description 
	DWORD	dw=0;
	HKEY hKey = NULL;
	char tempName[512] = {0};
	_snprintf(tempName,  sizeof tempName, "SYSTEM\\CurrentControlSet\\Services\\%s", strServiceName.c_str());
	if ( RegCreateKeyExA(HKEY_LOCAL_MACHINE, tempName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &hKey, &dw) == ERROR_SUCCESS )
	{
		RegSetValueExA(hKey, "Description", 0, REG_SZ, (const BYTE *)strDesc.c_str(), strDesc.length()+1);
	}
	
	RegCloseKey(hKey);
}

bool CSysServiceHandler::IsServiceInstalled(const string& strServiceName)
{
	bool bResult = false;
	SC_HANDLE hSCM = ::OpenSCManagerA(NULL, // local machine
		NULL, // ServicesActive database
		SC_MANAGER_ENUMERATE_SERVICE); // full access
	if (hSCM) 
	{
		SC_HANDLE hService = ::OpenServiceA(hSCM, strServiceName.c_str(), SERVICE_QUERY_CONFIG);
		if (hService) 
		{
			bResult = true;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}

	return bResult;
}


int CSysServiceHandler::IsServiceRunned(const string& strServiceName)
{
	SERVICE_STATUS serviceStatus;

	SC_HANDLE scm = OpenSCManagerA(0, 0, SC_MANAGER_CONNECT);
	if(!scm) 
	{
		return -1;
	}

	SC_HANDLE service=OpenServiceA(scm, strServiceName.c_str(), SERVICE_QUERY_STATUS);
	if(!service) 
	{
		DWORD myerror=GetLastError();
		if (myerror==ERROR_ACCESS_DENIED)
		{
			CloseServiceHandle(scm);
			return -2;
		}
		if (myerror==ERROR_SERVICE_DOES_NOT_EXIST)
		{
			CloseServiceHandle(scm);
			return -3;
		}

		CloseServiceHandle(scm);
		return -4;
	}
	if(!QueryServiceStatus(service, &serviceStatus)) 
	{
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return -5;
	}
	if(serviceStatus.dwCurrentState == SERVICE_RUNNING) 
	{
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return 1;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	return -6;
}

bool CSysServiceHandler::StartServerBySCM(const string& strServiceName)
{
	bool bSucess = false;
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {};

	// Open the local default service control manager database
	schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CONNECT);
	if (schSCManager)
	{
		// Open the service with delete, stop, and query status permissions
		schService = OpenServiceA(schSCManager, strServiceName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS);
		if (schService)
		{
			::StartServiceA(schService, 0, NULL);//开始Service
			Sleep(1000);

			while (QueryServiceStatus(schService, &ssSvcStatus))
			{
				if (ssSvcStatus.dwCurrentState == SERVICE_START_PENDING)
				{
					Sleep(1000);
				}
				else 
					break;
			}
			if (ssSvcStatus.dwCurrentState == SERVICE_RUNNING)
			{
				bSucess = true;
			}
		}
	}

	// Centralized cleanup for all allocated resources.
	if (schSCManager)
	{
		::CloseServiceHandle(schSCManager);
		schSCManager = NULL;
	}
	if (schService)
	{
		::CloseServiceHandle(schService);
		schService = NULL;
	}
	return bSucess;
}

bool CSysServiceHandler::StopServerBySCM(const string& strerviceName)
{
	bool bSucess = false;
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {};

	// Open the local default service control manager database
	schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CONNECT);
	if (schSCManager)
	{
		// Open the service with delete, stop, and query status permissions
		schService = OpenServiceA(schSCManager, strerviceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS);
		if (schService)
		{
			// Try to stop the service
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
			{
				Sleep(1000);
				while (QueryServiceStatus(schService, &ssSvcStatus))
				{
					if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
					{
						Sleep(1000);
					}
					else 
						break;
				}

				if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
				{
					bSucess = true;
				}
			}
		}
	}

	// Centralized cleanup for all allocated resources.
	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
	}
	if (schService)
	{
		CloseServiceHandle(schService);
		schService = NULL;
	}
	return bSucess;
}

bool CSysServiceHandler::UninstallService(const string& strServiceName)
{
	bool bSucess = false;
	SC_HANDLE schSCManager = NULL;
	SC_HANDLE schService = NULL;
	SERVICE_STATUS ssSvcStatus = {};

	// Open the local default service control manager database
	schSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CONNECT);
	if (schSCManager)
	{
		// Open the service with delete, stop, and query status permissions
		schService = OpenServiceA(schSCManager, strServiceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
		if (schService)
		{
			// Try to stop the service
			if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
			{
				Sleep(1000);
				while (QueryServiceStatus(schService, &ssSvcStatus))
				{
					if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
					{
						Sleep(1000);
					}
					else 
						break;
				}

				// Now remove the service by calling DeleteService.
				if (DeleteService(schService))
				{
					OutputDebugStringA("\n11111111111111111111111\n");
					bSucess = true;
				}
			}
		}
	}

	//Cleanup:
	// Centralized cleanup for all allocated resources.
	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = NULL;
	}
	if (schService)
	{
		CloseServiceHandle(schService);
		schService = NULL;
	}
	return bSucess;
}

void CSysServiceHandler::StartService(const string& strServiceName)
{
	char command[MAX_PATH*2] = {0};
	_snprintf(command, sizeof command, "start \"%s\"", strServiceName.c_str());
	RunExec("net", command);
}

void CSysServiceHandler::StopService(const string& strServiceName)
{
	char command[MAX_PATH*2] = {0};
	_snprintf(command, sizeof command, "stop \"%s\"", strServiceName.c_str());
	RunExec("net", command);
}

BOOL CSysServiceHandler::RunExec(const string& strCmd, const string& strPara, DWORD dwMilliseconds /* = INFINITE */)
{
	SHELLEXECUTEINFOA ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = strCmd.c_str();
	ShExecInfo.lpParameters = strPara.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteExA(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds);

	return suc;
}

void CSysServiceHandler::make_upper(std::string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
}


bool CSysServiceHandler::GetServicePath(string& strAppPath, const string& strServiceName)
{
	bool bRet = false;	
	// connect to serice control manager
	SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!hManager)
		return false;

	// open service
	SC_HANDLE hService = OpenServiceA(hManager, strServiceName.c_str(), GENERIC_READ);
	if (!hService)
		return bRet;

	// call QueryServiceConfig() the first time to receive buffer size
	DWORD dwBufferSize = 0;
	BOOL bOK = QueryServiceConfigA( hService, NULL, 0, &dwBufferSize);
	if (!bOK && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		// allocate space per results from the first call
		QUERY_SERVICE_CONFIGA *pServiceConfig = (QUERY_SERVICE_CONFIGA *) new UCHAR[dwBufferSize];
		if (pServiceConfig)
		{
			// call EnumServicesStatus() the second time to actually get service config
			bOK = QueryServiceConfigA( hService, pServiceConfig, dwBufferSize, &dwBufferSize);
			if (bOK)
			{
				bRet = true;
				strAppPath = pServiceConfig->lpBinaryPathName;
			}

			delete [] (UCHAR *) pServiceConfig;
		}
	}

	if (hService)
	{
		CloseServiceHandle(hService);
	}

	// disconnect from serice control manager
	CloseServiceHandle(hManager);
	return bRet;
}


bool CSysServiceHandler::ResetServicePath(const string& strAppPath, const string& strServiceName)
{
	bool bRet = false;
	DWORD dwBufferSize = 0;

	// connect to serice control manager
	SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hManager)
		return bRet;

	// Get a handle to the service.  
	SC_HANDLE hService = OpenServiceA( hManager, strServiceName.c_str(), SERVICE_CHANGE_CONFIG);  // need change config access   
	if (hService == NULL) 
	{
		CloseServiceHandle(hManager); 
		return bRet; 
	}

	// Change the service start type.  
	if (ChangeServiceConfigA(  
		hService,        // handle of service   
		SERVICE_NO_CHANGE, // service type: no change   
		SERVICE_NO_CHANGE,  // service start type   
		SERVICE_NO_CHANGE, // error control: no change   
		strAppPath.c_str(),              // binary path
		NULL,              // load order group: no change   
		NULL,              // tag ID: no change   
		NULL,              // dependencies: no change   
		NULL,              // account name: no change   
		NULL,              // password: no change   
		NULL) )            // display name: no change  
	{ 
		bRet = true;
	}

	CloseServiceHandle(hService);  
	CloseServiceHandle(hManager); 

	return bRet;
}


//**************************************************************************
// GetServiceName() looks up service by application path. If found, the function
// fills pszServiceName (must be at least 256+1 characters long).
bool CSysServiceHandler::GetServiceName(const string& strAppPath, string& strServiceName)
{
	// prepare given application path for matching against service list
	std::string appPath(strAppPath);
	// convert to uppercase
	make_upper(appPath);

	// connect to serice control manager
	SC_HANDLE hManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!hManager)
		return false;

	DWORD dwBufferSize = 0;
	DWORD dwCount = 0;
	DWORD dwPosition = 0;
	bool bResult = false;

	// call EnumServicesStatus() the first time to receive services array size
	BOOL bOK = EnumServicesStatusA(
		hManager,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,
		NULL,
		0,
		&dwBufferSize,
		&dwCount,
		&dwPosition);
	if (!bOK && GetLastError() == ERROR_MORE_DATA)
	{
		// allocate space per results from the first call
		ENUM_SERVICE_STATUSA *pServices = (ENUM_SERVICE_STATUSA *) new UCHAR[dwBufferSize];
		if (pServices)
		{
			// call EnumServicesStatus() the second time to actually get the services array
			bOK = EnumServicesStatusA(
				hManager,
				SERVICE_WIN32,
				SERVICE_STATE_ALL,
				pServices,
				dwBufferSize,
				&dwBufferSize,
				&dwCount,
				&dwPosition);
			if (bOK)
			{
				// iterate through all services returned by EnumServicesStatus()
				for (DWORD i = 0; i < dwCount && !bResult; i++)
				{
					// open service
					SC_HANDLE hService = OpenServiceA(hManager,
						pServices[i].lpServiceName,
						GENERIC_READ);
					if (!hService)
						break;

					// call QueryServiceConfig() the first time to receive buffer size
					bOK = QueryServiceConfigA(
						hService,
						NULL,
						0,
						&dwBufferSize);
					if (!bOK && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
					{
						// allocate space per results from the first call
						QUERY_SERVICE_CONFIGA *pServiceConfig = (QUERY_SERVICE_CONFIGA *) new UCHAR[dwBufferSize];
						if (pServiceConfig)
						{
							// call EnumServicesStatus() the second time to actually get service config
							bOK = QueryServiceConfigA(
								hService,
								pServiceConfig,
								dwBufferSize,
								&dwBufferSize);
							if (bOK)
							{
								// match given application name against executable path in the service config
								std::string servicePath(pServiceConfig->lpBinaryPathName);
								make_upper(servicePath);
								if (servicePath.find( appPath ) != -1)
								{
									bResult = true;
									strServiceName = pServices[i].lpServiceName;
								}
							}

							delete [] (UCHAR *) pServiceConfig;
						}
					}

					CloseServiceHandle(hService);
				}
			}

			delete [] (UCHAR *) pServices;
		}
	}

	// disconnect from serice control manager
	CloseServiceHandle(hManager);

	return bResult;
}

