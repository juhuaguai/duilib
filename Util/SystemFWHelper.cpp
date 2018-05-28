#include "SystemFWHelper.h"
#include "atlconv.h"
#include "netfw.h"

CSystemFWHelper::CSystemFWHelper(void)
{
	m_pFwAuthPorts = NULL;
	m_pFwAuthApps = NULL;

	Init();
}

CSystemFWHelper::~CSystemFWHelper(void)
{
	UnInit();
}

void CSystemFWHelper::Init()
{
	CoInitialize(NULL);

	INetFwMgr *pFwMgr = NULL;
	INetFwProfile *pFwProfile = NULL;
	INetFwPolicy *pFwProlicy = NULL;

	HRESULT hr = CoCreateInstance(__uuidof(NetFwMgr), 0, CLSCTX_INPROC_SERVER,__uuidof(INetFwMgr),reinterpret_cast<void **>(&pFwMgr));

	if (SUCCEEDED(hr) && (pFwMgr != NULL))
	{
		hr = pFwMgr->get_LocalPolicy( &pFwProlicy );
		if (SUCCEEDED(hr) && (pFwProlicy != NULL))
		{
			hr = pFwProlicy->get_CurrentProfile( &pFwProfile );
			if (SUCCEEDED(hr) && (pFwProfile != NULL))
			{
				pFwProfile->get_GloballyOpenPorts(&m_pFwAuthPorts);
				pFwProfile->get_AuthorizedApplications(&m_pFwAuthApps);
			}
		}
	}

	if (pFwMgr)
	{
		pFwMgr->Release();
	}

	if (pFwProfile)
	{
		pFwProfile->Release();
	}

	if (pFwProlicy)
	{
		pFwProlicy->Release();
	}
}


void CSystemFWHelper::UnInit()
{
	if (m_pFwAuthPorts)
	{
		m_pFwAuthPorts->Release();
		m_pFwAuthPorts = NULL;
	}

	if (m_pFwAuthApps)
	{
		m_pFwAuthApps->Release();
		m_pFwAuthApps = NULL;
	}

	CoUninitialize();
}


//描述：添加新的应用程序，到系统防火墙
//参数：
//      szAppPath - 应用程序的路径
//      szAppDisName - 系统防火墙中的显示名
bool CSystemFWHelper::AddNewApp(const wstring& strAppPath,const wstring& strAppDisName)
{
	if (!m_pFwAuthApps)
	{
		return false;
	}

	if (strAppPath.empty())
	{
		return false;
	}

	INetFwAuthorizedApplication *pFwAppNew;

	HRESULT hr = CoCreateInstance(__uuidof(NetFwAuthorizedApplication), 0, CLSCTX_INPROC_SERVER,__uuidof(INetFwAuthorizedApplication),reinterpret_cast<void **>(&pFwAppNew));
	if (FAILED(hr) || !pFwAppNew)
	{
		return true;
	}

	if ( strAppDisName.empty() )
	{
		wstring strName = strAppPath;
		int nPos = strName.rfind(L'\\');
		if (nPos == string::npos)
		{
			nPos = strName.rfind(L'/');
		}
		if (nPos != string::npos)
		{
			strName = strName.substr(nPos+1);
		}

		if (strName.empty())
		{
			pFwAppNew->put_Name(L"Unknown Name");
		}
		else
		{
			BSTR bstrValue = ::SysAllocString(strName.c_str());
			pFwAppNew->put_Name(bstrValue);
			SysFreeString(bstrValue);
		}
	}
	else
	{
		BSTR bstrValue = ::SysAllocString(strAppDisName.c_str());
		pFwAppNew->put_Name(bstrValue);
		SysFreeString(bstrValue);
	}

	BSTR bstrValue = ::SysAllocString(strAppPath.c_str());
	pFwAppNew->put_ProcessImageFileName(bstrValue);
	SysFreeString(bstrValue);

	hr = m_pFwAuthApps->Add(pFwAppNew);
	pFwAppNew->Release();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool CSystemFWHelper::CheckAppAndEnable(const wstring& strAppPath)
{
	if (!m_pFwAuthApps)
	{
		return false;
	}

	INetFwAuthorizedApplication *pFwApp = NULL;
	HRESULT hr;

	BSTR bstrValue = ::SysAllocString(strAppPath.c_str());
	hr = m_pFwAuthApps->Item(bstrValue, &pFwApp);
	SysFreeString(bstrValue);
	if (SUCCEEDED(hr) && pFwApp)
	{
		VARIANT_BOOL  bIsEnabled;

		pFwApp->get_Enabled(&bIsEnabled);
		if (bIsEnabled == VARIANT_FALSE)
		{
			hr = pFwApp->put_Enabled(VARIANT_TRUE);
			pFwApp->Release();
			if (SUCCEEDED(hr))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		pFwApp->Release();

		return true;
	}

	return false;
}


