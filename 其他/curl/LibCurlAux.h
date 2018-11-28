#pragma once

#include <string>
#include <map>
using namespace std;
#include "../curl/include/curl.h"
#pragma comment(lib, "../curl/lib/libcurl.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )

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

	void ReceiveData(void* buffer, size_t size, size_t nmemb);
	void ReceiveHdr(void* buffer, size_t size, size_t nmemb);
	//默认设置10秒连接超时10秒接收超时
	bool InitLibCurl(LPCSTR lpszCookieFile=NULL);
	//nTransTimeOut-接收数据超时(单位:秒) nConnTimeOut-连接超时(单位:秒) 
	bool SetTimeOut(int nTransTimeOut, int nConnTimeOut);
	void UninitLibCurl();
	bool SendHttpQuest(const char *pszUrl, bool bHttpGet=true, const char *pszAppendHeaders = NULL);
	long  GetResponseInfo();
	string& GetResponseHdr();
	string& GetResponseData();

	string GetErrText(){return m_strErrTxt;};

	long OpenUrl(const char *szWebUrl, bool bHttpGet, string& strRepData, const char *pszAppendHeads=NULL,const wstring& strFile=L"");

	void SetSimplePostData(const char *pszPostData);
	void AddMultiPartPostData(const char *pszKey, const char *pszVal);

	string sendSyncRequest(const string &url,const map<string, string> &paramsMap,const map<string, string> &headers = map<string, string>());

public:
	//独立的接口和句柄,无需预先调用InitLibCurl,完毕后也无需调用UninitLibCurl
	bool ConnectOnly(const string& strUrl,int nTimeOut);
};

