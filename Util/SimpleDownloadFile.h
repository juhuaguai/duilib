#pragma once
#include <string>
#include <deque>
#include <urlmon.h>
#include "KCritSec.h"
using namespace std;

#pragma comment(lib, "urlmon.lib")

//pVoid-DownloadFile时传的第一个参数 pszUrl-要下载的文件的url pszPath-存放路径及文件名
typedef void (*PDownloadResultCallback)(LPVOID pVoid,bool bDownloadOK,LPCTSTR pszPath);	//下载结果回调

#ifdef _UNICODE
	#define xstring wstring
#else
	#define xstring string 
#endif

typedef struct tagDownloadInfo
{
	tagDownloadInfo()
	{
		pCallback = NULL;
		pVoid = NULL;
	};

	tagDownloadInfo& operator=(const tagDownloadInfo& value)
	{
		strUrl = value.strUrl;
		strPath = value.strPath;
		pCallback = value.pCallback;
		pVoid = value.pVoid;
		return *this;
	};

	xstring strUrl;
	xstring strPath;
	PDownloadResultCallback pCallback;
	LPVOID pVoid;
}SDownloadInfo;

class CSimpleDownloadFile :	public IBindStatusCallback
{
public:
	CSimpleDownloadFile(void);
	~CSimpleDownloadFile(void);

	//参数:pVoid-回调时将带出该指针 pszUrl-源url pszPath-目标路径及文件名 false-是否删除已经存在的同名文件(不删除有可能导致文件内容不会更新)
	void DownloadFile(LPVOID pVoid,LPCTSTR pszUrl,LPCTSTR pszPath,PDownloadResultCallback pCallback = NULL,bool bDeleteOld = true);

public:
	HRESULT STDMETHODCALLTYPE OnStartBinding(/* [in] */ DWORD dwReserved,/* [in] */ __RPC__in_opt IBinding *pib);
	HRESULT STDMETHODCALLTYPE GetPriority(/* [out] */ __RPC__out LONG *pnPriority);
	HRESULT STDMETHODCALLTYPE OnLowResource(/* [in] */ DWORD reserved);
	HRESULT STDMETHODCALLTYPE OnStopBinding(/* [in] */ HRESULT hresult,/* [unique][in] */ __RPC__in_opt LPCWSTR szError);
	HRESULT STDMETHODCALLTYPE GetBindInfo(/* [out] */ DWORD *grfBINDF,/* [unique][out][in] */ BINDINFO *pbindinfo);
	HRESULT STDMETHODCALLTYPE OnDataAvailable(/* [in] */ DWORD grfBSCF,/* [in] */ DWORD dwSize,/* [in] */ FORMATETC *pformatetc,/* [in] */ STGMEDIUM *pstgmed);
	HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID riid,/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	ULONG STDMETHODCALLTYPE AddRef( void);
	ULONG STDMETHODCALLTYPE Release( void);
	HRESULT STDMETHODCALLTYPE OnObjectAvailable(/* [in] */ __RPC__in REFIID riid,/* [iid_is][in] */ __RPC__in_opt IUnknown *punk);
	HRESULT STDMETHODCALLTYPE OnProgress(/* [in] */ ULONG ulProgress,/* [in] */ ULONG ulProgressMax,/* [in] */ ULONG ulStatusCode,/* [unique][in] */ __RPC__in_opt LPCWSTR szStatusText);

	//添加到下载列表尾部
	void AddTailToDownloadDeque(SDownloadInfo& tInfo);
	//取出下载列表的头部(是取走而不是获取)	返回值:false-获取失败
	bool GetHeadFromDownloadDeque(SDownloadInfo& tInfo);
	//获取下载列表中的节点数量
	int GetCountForDownloadDeque();

public:
	deque<SDownloadInfo> m_dequeDownloadInfo;		//下载文件的列表

	HANDLE m_hEvent;								//等待下载信号
	HANDLE m_hThread;								//下载线程句柄
	CKCritSec m_lockDeque;							//Deque操作锁,保证Deque的线程安全的
};
