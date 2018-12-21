#include "SimpleDownloadFile.h"
#include <process.h>
#include <tchar.h>

DWORD WINAPI DownloadFileThreadFunc(LPVOID lpParameter);

CSimpleDownloadFile::CSimpleDownloadFile(void)
{
	m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	//需要手动设置有/无信号,初始无信号
	
	unsigned int nThreadId=0x0;
	m_hThread = (HANDLE)_beginthreadex(0, 0, (unsigned int (__stdcall *)(void *))DownloadFileThreadFunc, (LPVOID)this, 0 , &nThreadId);
}

CSimpleDownloadFile::~CSimpleDownloadFile(void)
{
	TerminateThread(m_hThread,0x1);
	CloseHandle(m_hThread);
	m_hThread = NULL;

	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}

HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::QueryInterface(/* [in] */ REFIID riid,/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	*ppvObject = NULL;

	if( riid == IID_IBindStatusCallback)
		*ppvObject = static_cast<IBindStatusCallback*>(this);
	else if( riid == IID_IHttpSecurity)
		*ppvObject = static_cast<IHttpSecurity*>(this);

	return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
}
ULONG STDMETHODCALLTYPE CSimpleDownloadFile::AddRef( void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CSimpleDownloadFile::Release( void)
{
	return 0;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnStartBinding(/* [in] */ DWORD dwReserved,/* [in] */ __RPC__in_opt IBinding *pib)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::GetPriority(/* [out] */ __RPC__out LONG *pnPriority)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnLowResource(/* [in] */ DWORD reserved)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnStopBinding(/* [in] */ HRESULT hresult,/* [unique][in] */ __RPC__in_opt LPCWSTR szError)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::GetBindInfo(/* [out] */ DWORD *grfBINDF,/* [unique][out][in] */ BINDINFO *pbindinfo)
{
	*grfBINDF = BINDF_PRAGMA_NO_CACHE | BINDF_RESYNCHRONIZE | BINDF_NOWRITECACHE | BINDF_IGNORESECURITYPROBLEM;
	pbindinfo->cbSize = sizeof(BINDINFO);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnDataAvailable(/* [in] */ DWORD grfBSCF,/* [in] */ DWORD dwSize,/* [in] */ FORMATETC *pformatetc,/* [in] */ STGMEDIUM *pstgmed)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnObjectAvailable(/* [in] */ __RPC__in REFIID riid,/* [iid_is][in] */ __RPC__in_opt IUnknown *punk)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnProgress(/* [in] */ ULONG ulProgress,/* [in] */ ULONG ulProgressMax,/* [in] */ ULONG ulStatusCode,/* [unique][in] */ __RPC__in_opt LPCWSTR szStatusText)
{//做下载进度用
	//if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
	//{
	//	OutputDebugStringW(szStatusText);
	//	OutputDebugStringA("\n");
	//}
	//
	//return S_OK;
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::OnSecurityProblem(/* [in] */ DWORD dwProblem)
{
	return RPC_E_RETRY;
};
HRESULT STDMETHODCALLTYPE CSimpleDownloadFile::GetWindow(/* [in] */ REFGUID rguidReason,/* [out] */ HWND *phwnd)
{
	*phwnd = NULL;
	return E_NOTIMPL;
};

void CSimpleDownloadFile::DownloadFile(LPVOID pVoid,LPCTSTR pszUrl,LPCTSTR pszPath,PDownloadResultCallback pCallback /* = NULL */,bool bDeleteOld /* = true */)
{
	TCHAR szUrl[1024] = {0};
	_sntprintf_s(szUrl,_countof(szUrl),_TRUNCATE,_T("%s?skq=%ld"),pszUrl,GetTickCount());
	
	if (pszPath && _tcslen(pszPath)>0)
	{
		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		if (GetFileAttributesEx(pszPath, GetFileExInfoStandard, &attrs))
		{
			if (bDeleteOld)
				DeleteFile(pszPath);
			else
			{
				pCallback(pVoid,true,pszPath);
				return ;
			}
		}
		SDownloadInfo tInfo;
		tInfo.strUrl = szUrl;
		tInfo.strPath = pszPath;
		tInfo.pCallback = pCallback;
		tInfo.pVoid = pVoid;
		AddTailToDownloadDeque(tInfo);
		SetEvent(m_hEvent);		
	}	
	else
	{
		pCallback(pVoid,false,pszPath);
	}
}

void CSimpleDownloadFile::AddTailToDownloadDeque(SDownloadInfo& tInfo)
{
	CKAutoLock theLock(&m_lockDeque);
	m_dequeDownloadInfo.push_back(tInfo);
}

bool CSimpleDownloadFile::GetHeadFromDownloadDeque(SDownloadInfo& tInfo)
{
	CKAutoLock theLock(&m_lockDeque);
	tInfo = m_dequeDownloadInfo.front();
	m_dequeDownloadInfo.pop_front();

	return tInfo.strUrl.empty()==false;	
}

int CSimpleDownloadFile::GetCountForDownloadDeque()
{
	CKAutoLock theLock(&m_lockDeque);
	return m_dequeDownloadInfo.size();
}

DWORD WINAPI DownloadFileThreadFunc(LPVOID lpParameter)
{
	CSimpleDownloadFile* pThis = (CSimpleDownloadFile*)lpParameter;
	HRESULT hRet = S_FALSE;

	SDownloadInfo tInfo;
	while(true)
	{
		WaitForSingleObject(pThis->m_hEvent,INFINITE);
		while (pThis->GetCountForDownloadDeque() > 0)
		{
			if (pThis->GetHeadFromDownloadDeque(tInfo))
			{
				URLDownloadToFile(tInfo.strUrl.c_str());
				if (URLDownloadToFile(NULL,tInfo.strUrl.c_str(),tInfo.strPath.c_str(),0,pThis) == S_OK)
					tInfo.pCallback(tInfo.pVoid,true,tInfo.strPath.c_str());
				else
					tInfo.pCallback(tInfo.pVoid,false,tInfo.strPath.c_str());			
			}
			else
				continue;
		}
		ResetEvent(pThis->m_hEvent);		
	}

	//这里线程退出时先不做句柄清理
}