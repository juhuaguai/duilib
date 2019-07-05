#include "LibCurlAux.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

size_t data_callback(void* data, size_t size, size_t nmemb, void *userPtr)
{
	size_t len  = size * nmemb; 

	if (userPtr == NULL)
		return len;
	
	CLibCurlAux *pThis = (CLibCurlAux *)userPtr;

	if (pThis->m_strDestFile.empty() == false)
	{
		FILE* fp = NULL;
		_wfopen_s(&fp,pThis->m_strDestFile.c_str(),L"ab+");	
		if (fp)
		{
			fwrite(data,nmemb,size,fp);
			fclose(fp);
		}
	}

	pThis->ReceiveData(data, size, nmemb);

	return len;
}

size_t header_callback(void  *data, size_t size, size_t nmemb,  void *userPtr)    
{
	size_t len  = size * nmemb; 

	if (userPtr == NULL)
		return len;

	CLibCurlAux *pThis = (CLibCurlAux *)userPtr;

	pThis->ReceiveHdr(data, size, nmemb);
	return len;  
}  

CLibCurlAux::CLibCurlAux(void)
{
	m_easy_handle = NULL;
	m_pPostStart = NULL;
	m_pPostEnd = NULL;
}

CLibCurlAux::~CLibCurlAux(void)
{
	UninitLibCurl();
}


void CLibCurlAux::ReceiveData(void* buffer, size_t size, size_t nmemb)
{
	size_t len  = size * nmemb;  

	m_httpDataRecved.append((char*)buffer, len);
}


void CLibCurlAux::ReceiveHdr(void* buffer, size_t size, size_t nmemb)
{
	size_t len  = size * nmemb;  

	m_strhttpHdrRecved.append((char*)buffer, len);
}

bool CLibCurlAux::InitGlobalCurl()
{
	CURLcode returnCode = curl_global_init(CURL_GLOBAL_ALL);
	if(CURLE_OK != returnCode)
	{
		return false;
	}

	return true;
}

void CLibCurlAux::UninitGlobalCurl()
{
	curl_global_cleanup();
}


bool CLibCurlAux::InitLibCurl(LPCSTR lpszCookieFile/* =NULL */)
{
	m_strErrTxt = "";
//	CURLcode returnCode;

	//建立easy handle
	m_easy_handle = curl_easy_init();
	if(!m_easy_handle)
	{
		//m_strErrTxt = curl_easy_strerror(returnCode);
		return false;
	}

	curl_easy_setopt(m_easy_handle, CURLOPT_SSL_VERIFYPEER,0L); 
	curl_easy_setopt(m_easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	//	curl_easy_setopt(curl, CURLOPT_CAPATH, "server.crt");

	//设置下载回调函数
	curl_easy_setopt(m_easy_handle, CURLOPT_WRITEFUNCTION, data_callback); 
	curl_easy_setopt(m_easy_handle, CURLOPT_WRITEDATA,this); 
	curl_easy_setopt(m_easy_handle, CURLOPT_HEADERFUNCTION, header_callback );
	curl_easy_setopt(m_easy_handle, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(m_easy_handle, CURLOPT_HEADER, 0L);//输出内容时也输出信息头

	curl_easy_setopt(m_easy_handle, CURLOPT_VERBOSE, 1L);//调试信息打开 
	curl_easy_setopt(m_easy_handle, CURLOPT_FOLLOWLOCATION, 1L);//跟踪重定向 

	if (lpszCookieFile && strlen(lpszCookieFile)>0)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_COOKIEFILE, lpszCookieFile);
		curl_easy_setopt(m_easy_handle, CURLOPT_COOKIEJAR,lpszCookieFile); 
	}

	//curl_easy_setopt(m_easy_handle, CURLOPT_HEADER, 0);//输出内容时也输出信息头

	m_pPostStart = NULL;
	m_pPostEnd = NULL;	

	//设置超时时间
	SetTimeOut(30, 30);
	return true;
}

bool CLibCurlAux::SetTimeOut(int nTransTimeOut, int nConnTimeOut)
{
	if (m_easy_handle != NULL)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(m_easy_handle, CURLOPT_TIMEOUT, nTransTimeOut);        //设置超时
		curl_easy_setopt(m_easy_handle, CURLOPT_CONNECTTIMEOUT, nConnTimeOut);        //设置超时
	}
	
	return true;
}

void CLibCurlAux::UninitLibCurl()
{
	m_pPostStart = NULL;
	m_pPostEnd = NULL;

	//释放资源
	if(m_easy_handle != NULL)
	{
		curl_easy_cleanup(m_easy_handle);
		m_easy_handle = NULL;
	}	
}

bool CLibCurlAux::SendHttpQuest(const char *pszUrl, int nHttpType/* =1 */, const char *pszAppendHeaders /* = NULL */)
{
	m_strhttpHdrRecved.clear();
	m_httpDataRecved.clear();

	string strHost;
	string strUrl = pszUrl;

	//去掉首尾空格
	strUrl.erase(0, strUrl.find_first_not_of(" "));
	strUrl.erase(strUrl.find_last_not_of(" ") + 1);	

	//查找主机地址
	size_t stStartPos = strUrl.find("://");	
	if(stStartPos != string::npos)
	{
		size_t stEndPos = strUrl.find('/', stStartPos + 3);
		strHost = "Host: " + strUrl.substr(stStartPos + 3, stEndPos - stStartPos - 3);
	}

	//设置HTTP头，没有加上，如有修改
	curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, "Accept: */*");
	http_headers = curl_slist_append(http_headers, "Accept-Language: zh-CN");
	http_headers = curl_slist_append(http_headers, "User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
	http_headers = curl_slist_append(http_headers, "Accept-Encoding: gzip, deflate");
	http_headers = curl_slist_append(http_headers, strHost.c_str());
	http_headers = curl_slist_append(http_headers, "Connection: Keep-Alive");	
	if (nHttpType == 3)
		http_headers = curl_slist_append(http_headers, "X-HTTP-Method-Override: PATCH");	
	if (pszAppendHeaders != NULL)
	{
		char szAppendHeader[1024]={0};
		strcpy_s(szAppendHeader, pszAppendHeaders);
		char* context = NULL;
		char *pszSubString = strtok_s(szAppendHeader, "\r\n;", &context);
		while (pszSubString)
		{
			if (strlen(pszSubString) > 1)
			{
				http_headers = curl_slist_append(http_headers, pszSubString);
			}
			pszSubString = strtok_s(NULL, "\r\n;", &context);
		}
	}
	curl_easy_setopt(m_easy_handle, CURLOPT_HTTPHEADER, http_headers);

	curl_easy_setopt(m_easy_handle, CURLOPT_ACCEPT_ENCODING, "gzip");


	if(nHttpType == 0)
	{
		if(m_pPostStart != NULL)
			curl_easy_setopt(m_easy_handle, CURLOPT_HTTPPOST, m_pPostStart);		
	}
	else if (nHttpType == 1)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_HTTPGET, 1);
	}
	else if (nHttpType == 3)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_CUSTOMREQUEST, "PATCH");
	}
	else
	{
		if(m_pPostStart != NULL)
			curl_easy_setopt(m_easy_handle, CURLOPT_HTTPPOST, m_pPostStart);	
	}

	//设置访问IP
	curl_easy_setopt(m_easy_handle, CURLOPT_URL, pszUrl);

	//执行数据请求
	CURLcode res = curl_easy_perform(m_easy_handle);

	double dlength = 0.0;
	curl_easy_getinfo(m_easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD,  &dlength);

	if(m_pPostStart != NULL)
	{
		curl_formfree(m_pPostStart);
		m_pPostStart = NULL;
		m_pPostEnd = NULL;
	}

	if (res != CURLE_OK) 
	{
		m_strErrTxt = curl_easy_strerror(res);
		curl_slist_free_all(http_headers);
		return false;
	}	

	//释放消息头
	curl_slist_free_all(http_headers);
	return true;
}

void CLibCurlAux::SetSimplePostData(const char *pszPostData)
{
	int nPostLen = 0;
	if (pszPostData != NULL)
	{
		nPostLen = strlen(pszPostData);
	}
	if (nPostLen>0)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_POST, 1);
		curl_easy_setopt(m_easy_handle, CURLOPT_POSTFIELDS, pszPostData);
		curl_easy_setopt(m_easy_handle, CURLOPT_POSTFIELDSIZE, nPostLen);
	}
}
void CLibCurlAux::SetSimpleData(const char* pszData)
{
	int nDataLen = 0;
	if (pszData != NULL)
	{
		nDataLen = strlen(pszData);
	}
	if (nDataLen>0)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_POSTFIELDS, pszData);
		curl_easy_setopt(m_easy_handle, CURLOPT_POSTFIELDSIZE, nDataLen);
	}
}

void CLibCurlAux::AddMultiPartPostData(const char *pszKey, const char *pszVal)
{
	curl_formadd(&m_pPostStart, &m_pPostEnd,CURLFORM_COPYNAME, pszKey,   CURLFORM_COPYCONTENTS, pszVal, CURLFORM_END);
}

long CLibCurlAux::GetResponseInfo()
{
	long lInfo = 0;

	CURLcode retCode = curl_easy_getinfo(m_easy_handle, CURLINFO_RESPONSE_CODE, &lInfo); 
	if (retCode != CURLE_OK)
	{
		return 0;			
	}

	return lInfo;
}

string& CLibCurlAux::GetResponseHdr()
{
	return m_strhttpHdrRecved;
}


string& CLibCurlAux::GetResponseData()
{
	return m_httpDataRecved;
}

long CLibCurlAux::OpenUrl(const string& strWebUrl, int nHttpType, string& strRepData, const char *pszAppendHeads/* =NULL */,const wstring& strFile/* =L"" */)
{
	long lRetCode = 0;
	m_strDestFile = strFile;

	DeleteFileW(strFile.c_str());

	int nPos = strFile.find_last_of(L"\\/");
	if (nPos!=string::npos)
	{
		wstring strDir = strFile.substr(0,nPos);
		CreateDirW(strDir);
	}

	std::string szbuffer;  
	std::string szheader_buffer;  
	string strUrl;

	if (!strstr(strWebUrl.c_str(), "http://") && !strstr(strWebUrl.c_str(), "https://"))
	{
		strUrl = "http://";
		strUrl += strWebUrl; 
	}
	else
	{
		strUrl = strWebUrl;
	}

	bool bRet = SendHttpQuest(strUrl.c_str(), nHttpType, pszAppendHeads);
	lRetCode = GetResponseInfo();
	if (!bRet)
	{
		if (m_easy_handle == NULL)
		{
			strRepData = "InitLibCurl is not invoked";
		}
		else
		{
			char errbuf[CURL_ERROR_SIZE]={0};
			curl_easy_setopt(m_easy_handle, CURLOPT_ERRORBUFFER, errbuf);
			if (strlen(errbuf)>0)
				strRepData = errbuf;
			else
				strRepData = m_strErrTxt;
		}
		return lRetCode;
	}

	strRepData = GetResponseData();
	if (strRepData.empty())
	{
		char errbuf[CURL_ERROR_SIZE]={0};
		curl_easy_setopt(m_easy_handle, CURLOPT_ERRORBUFFER, errbuf);
		if (strlen(errbuf)>0)
			strRepData = errbuf;
	}

	szheader_buffer = GetResponseHdr();

	return lRetCode;
}

string CLibCurlAux::sendSyncRequest(const string &url,const map<string,string> &paramsMap,const map<string, string> &headers /* = map<string, string>()*/)
{
	string requestEntity;
	string item;
	for (map<string, string>::const_iterator iter = paramsMap.begin(); iter != paramsMap.end(); ++iter) 
	{
		const char *key = iter->first.c_str();
		char *encodedKey = curl_easy_escape(m_easy_handle, key, strlen(key));
		if (encodedKey) 
		{
			item = encodedKey;
		}
		item += "=";
		const char *value = iter->second.c_str();
		char *encodedValue = curl_easy_escape(m_easy_handle, value, strlen(value));
		if (encodedValue) 
		{
			item += encodedValue;
		}
		if (!requestEntity.empty())
		{
			requestEntity.push_back('&');
		}
		requestEntity.append(item);
		item.clear();
		if (encodedKey) 
		{
			curl_free(encodedKey);
		}
		if (encodedValue) 
		{
			curl_free(encodedValue);
		}
	}

	curl_slist *http_headers = NULL;
	http_headers = curl_slist_append(http_headers, "Accept: */*");
	http_headers = curl_slist_append(http_headers, "Accept-Language: zh-CN");
	http_headers = curl_slist_append(http_headers, "User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
	http_headers = curl_slist_append(http_headers, "Accept-Encoding: gzip, deflate");
	http_headers = curl_slist_append(http_headers, "Connection: Keep-Alive");

	string headitem;
	for (map<string, string>::const_iterator iter = headers.begin(); iter != headers.end(); ++iter) 
	{
		headitem = iter->first;
		headitem += " : ";
		headitem += iter->second;
		http_headers = curl_slist_append(http_headers, headitem.c_str());
		headitem.clear();
	}
	curl_easy_setopt(m_easy_handle, CURLOPT_HTTPHEADER, http_headers);

	//curl_easy_setopt(m_easy_handle, CURLOPT_ACCEPT_ENCODING, "gzip");

	curl_easy_setopt(m_easy_handle, CURLOPT_URL,url.c_str());
	curl_easy_setopt(m_easy_handle, CURLOPT_POST,1);

	curl_easy_setopt(m_easy_handle, CURLOPT_POSTFIELDS,requestEntity.c_str());


	CURLcode res = curl_easy_perform(m_easy_handle);
	if (res != CURLE_OK) 
	{
		m_strErrTxt = curl_easy_strerror(res);
		curl_slist_free_all(http_headers);
		return false;
	}	
	//释放消息头
	curl_slist_free_all(http_headers);

	string strRetContent = GetResponseData();
	string strRetHeader = GetResponseHdr();

	return strRetContent;
}

bool CLibCurlAux::ConnectOnly(const string& strUrl,int nTimeOut)
{
	CURL *curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nTimeOut);        //设置超时
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeOut);        //设置超时
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,0L); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());		
		CURLcode ret = curl_easy_perform(curl);
		if(ret == CURLE_OK) 
		{
			/* only connected! */
			long lInfo = 0;
			CURLcode retCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lInfo); 
			if (lInfo<400)
			{
				curl_easy_cleanup(curl);
				curl = NULL;
				return true;
			}
		}

		curl_easy_cleanup(curl);
		curl = NULL;
	}
	
	return false;
}


size_t write_callback(char *buffer, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = static_cast<FILE *>(userdata);
	size_t length = fwrite(buffer, size, nmemb, fp);
	if (length != nmemb)
	{
		return length;
	}

	return size * nmemb;
}
int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	typedef struct tagUserData
	{
		LPVOID pVoid;
		CURL *handle;
		PDownloadProcessingCallback pCallBack;
	}Progress_User_Data;
	Progress_User_Data *pdata = static_cast<Progress_User_Data *>(userdata);
	CURL *easy_handle = pdata->handle;

	// Defaults to bytes/second
	double speed = 0;
	curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &speed);	// 用以接收下载的平均速度，这个速度不是即时速度，而是下载完成后的速度，单位是 字节/秒 
	if (pdata->pCallBack(pdata->pVoid,dltotal,dlnow,speed) == false)
	{
		return 1;	//返回非0值将终止传输
	}

	return 0;		//返回0继续传输
}

BOOL CLibCurlAux::CreateDirW(const wstring& strDir)
{
	if (PathIsDirectoryW(strDir.c_str()))
		return TRUE;

	int nPos = strDir.find_last_of(L"\\/");
	if (nPos!=string::npos)
	{
		wstring strParentDir = strDir.substr(0,nPos);
		if (PathIsDirectoryW(strParentDir.c_str()) == FALSE)
		{
			if (CreateDirW(strParentDir)==FALSE)
			{
				return FALSE;
			}
		}

		if (CreateDirectoryW(strDir.c_str(), NULL) == FALSE)
		{
			return FALSE;
		}		
	}
	else
		return FALSE;

	return TRUE;
}

bool CLibCurlAux::SyncDownLoadFile(LPVOID pUserData, const char *szWebUrl, const wstring& strFile,PDownloadProcessingCallback pDownloadProingCall/* =NULL */)
{
	DeleteFileW(strFile.c_str());

	int nPos = strFile.find_last_of(L"\\/");
	if (nPos!=string::npos)
	{
		wstring strDir = strFile.substr(0,nPos);
		CreateDirW(strDir);
	}

	FILE* fp = _wfopen(strFile.c_str(),L"ab+");
	if (fp == NULL)
		return false;

	CURL *curl;
	curl = curl_easy_init();
	if(curl == NULL) 
		return false;
	curl_easy_setopt(curl, CURLOPT_URL, szWebUrl);	//下载地址
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,0L);				//禁止CURL验证对等证书(也有说是证书时效性)
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);				//不检查证书
	curl_easy_setopt(curl, CURLOPT_HEADER, 0L);						//输出内容时不输出信息头

	//设置下载回调函数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,fp); 
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);					//调试信息打开 
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);				//跟踪重定向 

	if (pDownloadProingCall)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);					//跟踪进度,开启进度回调
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, &progress_callback);		//设置进度回调函数
		typedef struct tagUserData
		{
			LPVOID pVoid;
			CURL *handle;
			PDownloadProcessingCallback pCallBack;
		}Progress_User_Data;
		Progress_User_Data theUserData;
		theUserData.pVoid = pUserData;
		theUserData.handle = curl;
		theUserData.pCallBack = pDownloadProingCall;
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &theUserData);				//进度回调函数的第一个参数
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);						//当HTTP返回值大于等于300的时候，请求失败
	}	

	//curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, resumeByte);	//断点续传用.告诉服务器本地已经下载多少字节了

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);			//设置超时
	//curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);	//设置超时

	CURLcode ret = curl_easy_perform(curl);
	long lInfo = 0;
	CURLcode retCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lInfo); 
	curl_easy_cleanup(curl);
	fclose(fp);
	if (ret != CURLE_OK)
		return false;

	if (lInfo<200 || lInfo>=400)
	{
		return false;
	}

	return true;
}
