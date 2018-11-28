#include "LibCurlAux.h"

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

bool CLibCurlAux::SendHttpQuest(const char *pszUrl, bool bHttpGet, const char *pszAppendHeaders/*=NULL*/)
{
	m_strhttpHdrRecved.clear();
	m_httpDataRecved.clear();

	string strHost;
	string strUrl = pszUrl;

	//去掉首尾空格
	strUrl.erase(0, strUrl.find_first_not_of(" "));
	strUrl.erase(strUrl.find_last_not_of(" ") + 1);	

	//必须是HTTP开头
	//if((stricmp(strUrl.substr(0, 7).c_str(), "http://") != 0) && (stricmp(strUrl.substr(0, 8).c_str(), "https://") != 0))
	//{
	//	return false;
	//}

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

	if(bHttpGet)
	{
		curl_easy_setopt(m_easy_handle, CURLOPT_HTTPGET, 1);
	}
	else
	{
		if(m_pPostStart != NULL)
		{
			curl_easy_setopt(m_easy_handle, CURLOPT_HTTPPOST, m_pPostStart);
		}
	}

	//设置访问IP
	curl_easy_setopt(m_easy_handle, CURLOPT_URL, pszUrl);

	//执行数据请求
	CURLcode res = curl_easy_perform(m_easy_handle);

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

void CLibCurlAux::AddMultiPartPostData(const char *pszKey, const char *pszVal)
{
	curl_formadd(&m_pPostStart, &m_pPostEnd,   CURLFORM_COPYNAME, pszKey,   CURLFORM_COPYCONTENTS, pszVal, CURLFORM_END);
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


//描述：打开某个Url，并获取接收到的数据内容
//参数：
//     nTimeOut - 超时(秒)
long CLibCurlAux::OpenUrl(const char *szWebUrl, bool bHttpGet, string& strRepData, const char *pszAppendHeads/* =NULL */,const wstring& strFile/* ="" */)
{
	long lRetCode = 0;
	m_strDestFile = strFile;
	DeleteFileW(strFile.c_str());

	std::string szbuffer;  
	std::string szheader_buffer;  
	string strUrl;

	if (!strstr(szWebUrl, "http://") && !strstr(szWebUrl, "https://"))
	{
		strUrl = "http://";
		strUrl += szWebUrl; 
	}
	else
	{
		strUrl = szWebUrl;
	}

	bool bRet = SendHttpQuest(strUrl.c_str(), bHttpGet, pszAppendHeads);
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
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,0L); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		CURLcode ret;
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());		
		ret = curl_easy_perform(curl);
		if(ret == CURLE_OK) 
		{
			/* only connected! */

			curl_easy_cleanup(curl);
			curl = NULL;
			return true;
		}

		curl_easy_cleanup(curl);
		curl = NULL;
	}
	
	return false;
}