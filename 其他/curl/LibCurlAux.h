#pragma once

#include <string>
#include <map>
using namespace std;
#include "../curl/include/curl.h"
#pragma comment(lib, "../curl/lib/libcurl.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )

//下载进度回调 下载完成是不走这个回调的 返回值:true-继续 false-取消下载
typedef bool (*PDownloadProcessingCallback)(LPVOID pVoid,__int64 nTotal,__int64 nCurDownload,double dbSpeed);	//下载结果回调

class CLibCurlAux
{
public:
	CLibCurlAux(void);
	~CLibCurlAux(void);

private:
	string  m_strhttpHdrRecved;
	string  m_httpDataRecved;
	CURL*  m_easy_handle;
	string  m_strErrTxt;   //发生错误时文本描述

	struct curl_httppost *m_pPostStart;
	struct curl_httppost *m_pPostEnd;

public:
	wstring m_strDestFile;

public:
	static bool InitGlobalCurl();
	static void UninitGlobalCurl();

	bool InitLibCurl(LPCSTR lpszCookieFile=NULL);
	//nTransTimeOut-接收数据超时(单位:秒) nConnTimeOut-连接超时(单位:秒)
	bool SetTimeOut(int nTransTimeOut, int nConnTimeOut);
	void UninitLibCurl();
	long OpenUrl(const char *szWebUrl, bool bHttpGet, string& strRepData, const char *pszAppendHeads=NULL,const wstring& strFile=L"");
	void SetSimplePostData(const char *pszPostData);
	void AddMultiPartPostData(const char *pszKey, const char *pszVal);

	void ReceiveData(void* buffer, size_t size, size_t nmemb);
	void ReceiveHdr(void* buffer, size_t size, size_t nmemb);
	bool SendHttpQuest(const char *pszUrl, bool bHttpGet=true, const char *pszAppendHeaders = NULL);
	long  GetResponseInfo();
	string& GetResponseHdr();
	string& GetResponseData();
	string GetErrText(){return m_strErrTxt;};	

	string sendSyncRequest(const string &url,const map<string, string> &paramsMap,const map<string, string> &headers = map<string, string>());

public:
	//独立的接口和句柄,无需预先调用InitLibCurl,完毕后也无需调用UninitLibCurl
	bool ConnectOnly(const string& strUrl,int nTimeOut);

	//同步下载函数，返回值表示下载有没有出错。注意：没出错也不代表文件一定下载正确。
	//独立的接口和句柄,无需预先调用InitLibCurl,完毕后也无需调用UninitLibCurl
	bool SyncDownLoadFile(LPVOID pUserData, const char *szWebUrl, const wstring& strFile,PDownloadProcessingCallback pDownloadProingCall=NULL);

	//创建目录,可一次创建多级目录
	BOOL CreateDirW(const wstring& strDir);
};

