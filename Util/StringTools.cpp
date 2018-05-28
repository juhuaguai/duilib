#include "StringTools.h"

std::string UnicodeToAnsi(const std::wstring& strSource)
{
    int nLength = ::WideCharToMultiByte(CP_ACP, 0, strSource.data(), -1, NULL, 0, NULL, FALSE);

    if (nLength < 1)
    {
        return "";
    }

    std::vector<char> vecResult(nLength);
    ::WideCharToMultiByte(CP_ACP, 0, strSource.data(), -1, &vecResult[0], nLength, NULL, FALSE);

    return std::string(vecResult.begin(), vecResult.end()-1);
}

std::wstring AnsiToUnicode(const std::string& strSource)
{
    int nLength = ::MultiByteToWideChar(CP_ACP, 0, strSource.data(), -1, NULL, 0);

    if (nLength < 1)
    {
        return L"";
    }

    std::wstring strResult(nLength, 0);
    ::MultiByteToWideChar(CP_ACP, 0, strSource.data(), strSource.size(), &strResult[0], nLength);

    return std::wstring(strResult.data(), nLength - 1);
}


std::wstring Utf8ToUnicode(const std::string& strSrouce)
{
    int nLength = ::MultiByteToWideChar(CP_UTF8, 0, strSrouce.data(), -1, NULL, 0);

    if (nLength < 1)
    {
        return L"";
    }

    std::wstring strResult(nLength, 0);
    ::MultiByteToWideChar(CP_UTF8, 0, strSrouce.data(), strSrouce.size(), &strResult[0], nLength);

    return std::wstring(strResult.data(), nLength - 1);
}

std::string UnicodeToUtf8(const std::wstring& strSource)
{
    int nLength = ::WideCharToMultiByte(CP_UTF8, 0, strSource.data(), -1, NULL, 0, NULL, FALSE);

    if (nLength < 1)
    {
        return "";
    }

    std::string strResult(nLength, 0);
    ::WideCharToMultiByte(CP_UTF8, 0, strSource.data(), -1, &strResult[0], nLength, NULL, FALSE);

    return std::string(strResult.data(), nLength - 1);
}

std::string AnsiToUtf8(const std::string& strSource)
{
    std::wstring temp = AnsiToUnicode(strSource);
    return UnicodeToUtf8(temp);
}

std::string Utf8ToAnsi(const std::string& strSource)
{
    std::wstring strTmp = Utf8ToUnicode(strSource);
    return UnicodeToAnsi(strTmp);
}

//UrlEncode
bool UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, bool bUpperCase)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return false;

    size_t len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
		return true;
    }

    //先转换到UTF-8
    char baseChar = bUpperCase ? 'A' : 'a';
    int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if(pUnicode == NULL)
        return false;
    MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

    int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if(pUTF8 == NULL)
    {
        free(pUnicode);
        return false;
    }
    WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
    pUTF8[cbUTF8] = '\0';

    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)pUTF8;
    unsigned char *pDest = (unsigned char*)pBuf;
    while(*pSrc && cbDest < cbBufLen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ')
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > cbBufLen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
            pDest += 3;
            cbDest += 3;
        }
        ++pSrc;
    }
    //null-terminator
    *pDest = '\0';
    free(pUnicode);
    free(pUTF8);
    return true;
}

bool Utf8UrlDecode(const char* szSrc, char* pBuf, int cbBufLen)
{
	if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
		return false;

	size_t len_ascii = strlen(szSrc);
	if(len_ascii == 0)
	{
		pBuf[0] = 0;
		return true;
	}

	char *pUTF8 = (char*)malloc(len_ascii + 1);
	if(pUTF8 == NULL)
		return false;

	int cbDest = 0; //累加
	unsigned char *pSrc = (unsigned char*)szSrc;
	unsigned char *pDest = (unsigned char*)pUTF8;
	while(*pSrc)
	{
		if(*pSrc == '%')
		{
			*pDest = 0;
			//高位
			if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
				*pDest += (pSrc[1] - 'A' + 10) * 0x10;
			else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
				*pDest += (pSrc[1] - 'a' + 10) * 0x10;
			else
				*pDest += (pSrc[1] - '0') * 0x10;

			//低位
			if(pSrc[2] >= 'A' && pSrc[2] <= 'F')
				*pDest += (pSrc[2] - 'A' + 10);
			else if(pSrc[2] >= 'a' && pSrc[2] <= 'f')
				*pDest += (pSrc[2] - 'a' + 10);
			else
				*pDest += (pSrc[2] - '0');

			pSrc += 3;
		}
		else if(*pSrc == '+')
		{
			*pDest = ' ';
			++pSrc;
		}
		else
		{
			*pDest = *pSrc;
			++pSrc;
		}
		++pDest;
		++cbDest;
	}
	//null-terminator
	*pDest = '\0';
	++cbDest;

	if(cbDest < cbBufLen)
	{
		strncpy_s(pBuf, cbBufLen, pUTF8, cbDest);
		return true;
	}
	return false;
}

//解码后是utf-8编码
bool UrlDecode(const char* szSrc, char* pBuf, int cbBufLen)
{
	if(Utf8UrlDecode(szSrc, pBuf, cbBufLen))
	{
		string strTemp = Utf8ToAnsi(pBuf);
		if(strTemp.size() > 0)
		{
			strncpy_s(pBuf, cbBufLen, strTemp.c_str(), strTemp.size());
			return true;
		}
	}
	return false;
}

typedef unsigned char BYTE;

inline BYTE toHex(const BYTE &x)
{

    return x > 9 ? x + 55: x + 48; 

}

std::string URLEncodeGB2312(const char* szSrc/*, char* pBuf, int cbBufLen*/)
{
    std::string sOut;
	for( size_t ix = 0; ix < strlen(szSrc); ix++ )
    {       
        BYTE buf[4]; 
        memset( buf, 0, 4 ); 
        if( isalnum( (BYTE)szSrc[ix] ) || ispunct((BYTE)szSrc[ix]))
        {       
            buf[0] = szSrc[ix];
        }
        else if ( isspace( (BYTE)szSrc[ix] ) )
        {
            buf[0] = '+';
        }
        else
        {
            buf[0] = '%';
            buf[1] = toHex( (BYTE)szSrc[ix] >> 4 );
            buf[2] = toHex( (BYTE)szSrc[ix] % 16);
        }
        sOut += (char *)buf;
    }
	//strncpy(pBuf,sOut.data(),cbBufLen);
    return sOut;
};

std::string URLEncodeGB2312Forspace(const char* szSrc/*, char* pBuf, int cbBufLen*/)
{
	std::string sOut;
	for( size_t ix = 0; ix < strlen(szSrc); ix++ )
	{       
		BYTE buf[4]; 
		memset( buf, 0, 4 ); 
		if( isalnum( (BYTE)szSrc[ix] ) || ispunct((BYTE)szSrc[ix]))
		{       
			buf[0] = szSrc[ix];
		}
		else if ( isspace( (BYTE)szSrc[ix] ) )
		{
			//buf[0] = '+';
			buf[0] = '%';
			buf[1] = '2';
			buf[2] = '0';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)szSrc[ix] >> 4 );
			buf[2] = toHex( (BYTE)szSrc[ix] % 16);
		}
		sOut += (char *)buf;
	}
	//strncpy(pBuf,sOut.data(),cbBufLen);
	return sOut;
};

string Utf8ToGBK(const string& strUtf8)
{
    int len=MultiByteToWideChar(CP_UTF8, 0, /*(LPCTSTR)*/strUtf8.c_str(), -1, NULL,0); 
    unsigned short * wszGBK = new unsigned short[len+1];       
    memset(wszGBK, 0, len * 2 + 2); 
    MultiByteToWideChar(CP_UTF8, 0, /*(LPCTSTR)*/strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1]; 
    memset(szGBK, 0, len + 1); 
    WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL);

	string strGbk = szGBK;
	delete []szGBK;
	delete []wszGBK;

    return strGbk; 
}

char* GBKToUtf8(const char* strGBK)
{ 
    int len=MultiByteToWideChar(CP_ACP, 0, /*(LPCTSTR)*/strGBK, -1, NULL,0); 
    unsigned short * wszUtf8 = new unsigned short[len+1]; 
    memset(wszUtf8, 0, len * 2 + 2); 
    MultiByteToWideChar(CP_ACP, 0, /*(LPCTSTR)*/strGBK, -1, (LPWSTR)wszUtf8, len);
    len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1]; 
    memset(szUtf8, 0, len + 1); 
    WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, (LPSTR)szUtf8, len, NULL,NULL);
    return szUtf8; 
}

std::string EscapeToAnsi(const std::string& strSource)
{
	try
	{
		string strAnsi = strSource;					//"\u50ab",另外一种"%u50ab"暂时没有做处理
		string strValue,strvalue1;
		wchar_t wch;

		unsigned int npos = strAnsi.find("\\u",0);
		while (npos != string::npos)
		{
			strvalue1 = strAnsi.substr(npos,6);		//"\u50ab"
			strValue = strAnsi.substr(npos+2,4);	//"50ab"
			if (strValue.length() == 4)
			{
				if ( (strValue.at(0)>='0'&& strValue.at(0)<='9') || (strValue.at(0)>='a'&& strValue.at(0)<='f') || (strValue.at(0)>='A'&& strValue.at(0)<='F') )
				{
					if ( (strValue.at(1)>='0'&&strValue.at(1)<='9') || (strValue.at(1)>='a'&&strValue.at(1)<='f') || (strValue.at(1)>='A'&&strValue.at(1)<='F') )
					{
						if ( (strValue.at(2)>='0'&& strValue.at(2)<='9') || ((strValue.at(2)>='a'&& strValue.at(2)<='f')) || (strValue.at(2)>='A'&& strValue.at(2)<='F') )
						{
							if ( (strValue.at(3)>='0'&& strValue.at(3)<='9') || ((strValue.at(3)>='a'&& strValue.at(3)<='f')) || (strValue.at(3)>='A'&& strValue.at(3)<='F') )
							{
								sscanf(strValue.c_str(),"%4x",&wch);
								strAnsi.replace(strAnsi.begin()+npos,strAnsi.begin()+npos+6,_A(&wch));

								npos = strAnsi.find("\\u",0);
							}
							else
								npos = strAnsi.find("\\u",npos+2);
						}
						else
							npos = strAnsi.find("\\u",npos+2);
					}
					else
						npos = strAnsi.find("\\u",npos+2);
				}
				else
					npos = strAnsi.find("\\u",npos+2);
			}
			else
				break;			
		}
		return strAnsi;
	}
	catch (...)
	{
		return "";
	}
}

string Native2Ascii(const wstring& strUnicode,bool bPercentPre /* = false */,bool bUpper /* = false */)
{
	string result = "";
	char szValue[8] = {0};
	for (unsigned int i=0;i<strUnicode.length();i++)
	{
		if (bPercentPre)
			result += '%';
		else
			result += '\\';
		if (bUpper)
			sprintf(szValue,"u%4X",strUnicode.at(i));
		else
			sprintf(szValue,"u%4x",strUnicode.at(i));
		result += szValue;	
	}

	return result;
}
wstring Ascii2Native(const string& strAnsi,bool bPercentPre /* = false */)
{
	wstring strRet;
	string strSource = strAnsi;
	try
	{
		string strValue;
		wchar_t wch;

		unsigned int npos = 0;
		while (strSource.length())
		{
			if (bPercentPre)
				npos = strSource.find("%u",0);
			else
				npos = strSource.find("\\u",0);
			if (npos == string::npos)
			{
				strRet += AnsiToUnicode(strSource);
				break;
			}
			else
			{
				if (npos+6<=strSource.length())
				{
					strValue = strSource.substr(npos+2,4);
					if ( ((strValue.at(0)>='0'&& strValue.at(0)<='9') || (strValue.at(0)>='a'&& strValue.at(0)<='f') || (strValue.at(0)>='A'&& strValue.at(0)<='F'))
						&& ((strValue.at(1)>='0'&& strValue.at(1)<='9') || (strValue.at(1)>='a'&& strValue.at(1)<='f') || (strValue.at(1)>='A'&& strValue.at(1)<='F'))
						&& ((strValue.at(2)>='0'&& strValue.at(2)<='9') || (strValue.at(2)>='a'&& strValue.at(2)<='f') || (strValue.at(2)>='A'&& strValue.at(2)<='F'))
						&& ((strValue.at(3)>='0'&& strValue.at(3)<='9') || (strValue.at(3)>='a'&& strValue.at(3)<='f') || (strValue.at(3)>='A'&& strValue.at(3)<='F'))
						)
					{
						strRet += AnsiToUnicode( strSource.substr(0,npos) );

						sscanf(strValue.c_str(),"%4x",&wch);
						strRet += wch;
						strSource.erase(0,npos+6);
					}
					else
					{
						strRet += AnsiToUnicode( strSource.substr(0,npos+2) );
						strSource.erase(0,npos+2);
					}
				}
				else
				{
					strRet += AnsiToUnicode( strSource.substr(0,npos+2) );
					strSource.erase(0,npos+2);
				}	
			}
		}
	}
	catch (...)
	{}
	return strRet;
}

//描述：中文汉字在没在对应的拼音范围
bool  InPYRange(wchar_t start, wchar_t end, wchar_t code)
{
	if (code >= start && code <= end)
	{
		return true;
	}

	return false;
}


//描述：得到当前中文字的拼音的第一个字母
char  GetChineseFirstPY(wchar_t n)
{
	if (InPYRange(0xB0A1,0xB0C4,n)) return 'a';
	if (InPYRange(0XB0C5,0XB2C0,n)) return 'b';
	if (InPYRange(0xB2C1,0xB4ED,n)) return 'c';
	if (InPYRange(0xB4EE,0xB6E9,n)) return 'd';
	if (InPYRange(0xB6EA,0xB7A1,n)) return 'e';
	if (InPYRange(0xB7A2,0xB8c0,n)) return 'f';
	if (InPYRange(0xB8C1,0xB9FD,n)) return 'g';
	if (InPYRange(0xB9FE,0xBBF6,n)) return 'h';
	if (InPYRange(0xBBF7,0xBFA5,n)) return 'j';
	if (InPYRange(0xBFA6,0xC0AB,n)) return 'k';
	if (InPYRange(0xC0AC,0xC2E7,n)) return 'l';
	if (InPYRange(0xC2E8,0xC4C2,n)) return 'm';
	if (InPYRange(0xC4C3,0xC5B5,n)) return 'n';
	if (InPYRange(0xC5B6,0xC5BD,n)) return 'o';
	if (InPYRange(0xC5BE,0xC6D9,n)) return 'p';
	if (InPYRange(0xC6DA,0xC8BA,n)) return 'q';
	if (InPYRange(0xC8BB,0xC8F5,n)) return 'r';
	if (InPYRange(0xC8F6,0xCBF0,n)) return 's';
	if (InPYRange(0xCBFA,0xCDD9,n)) return 't';
	if (InPYRange(0xCDDA,0xCEF3,n)) return 'w';
	if (InPYRange(0xCEF4,0xD188,n)) return 'x';
	if (InPYRange(0xD1B9,0xD4D0,n)) return 'y';
	if (InPYRange(0xD4D1,0xD7F9,n)) return 'z';
	return '\0';
}


//***************************************************
//名称：GetFirstChinesePYCharOfText
//功能：得到中文文本中的第一个中文的拼音的第一个字母 
//参数：
//      szText - 包含有中文的文本
//      firstChar - 返回第一个中文的拼音的首个字母
//返回值：
//     若文本以中文打头，且成功得到其拼音的首个字母，则返回true,否则false
//***************************************************
bool GetFirstChinesePYCharOfText(char* pszText, char &firstChar)
{
	if (!pszText || strlen(pszText) <= 1)
	{
		return false;
	}

	string sChinese = pszText;
	int  nCnt = 1;//sChinese.length()/2   //得到文本中前几个中文的拼音的第一个字母

	char chr[3];
	wchar_t wchr = 0;
	char* buff = new char[nCnt + 1];

	memset(buff, 0x00, sizeof(char) * nCnt + 1);
	for (int i = 0, j = 0; i < nCnt; ++i)
	{
		memset(chr, 0x00, sizeof(chr));
		chr[0] = sChinese[j++];
		chr[1] = sChinese[j++];
		chr[2] = '\0';

		// 单个字符的编码 如：'我' = 0xced2
		wchr = 0;
		wchr = (chr[0] & 0xff) << 8;
		wchr |= (chr[1] & 0xff);

		buff[i] = GetChineseFirstPY(wchr);
	}

	firstChar = buff[0];
	delete []buff;

	if (firstChar == '\0')
	{
		firstChar = pszText[0];
		return false;
	}
	else
	{
		return true;
	}
}

xstring StringReplace(const xstring& strText, const xstring& strOld, const xstring& strNew)
{
	if (strOld == strNew)
		return strText;

	xstring strValue = strText;
	int nFind = 0;
	int nStart = strValue.find(strOld, nFind);
	while (nStart != string::npos)
	{
		strValue.replace(nStart, strOld.length(), strNew);
		nFind = nStart + strNew.length();
		nStart = strValue.find(strOld, nFind);
	}

	return strValue;
}

string StringReplaceA(const string& strText, const string& strOld, const string& strNew)
{
	if (strOld == strNew)
		return strText;

	string strValue = strText;
	int nFind = 0;
	int nStart = strValue.find(strOld, nFind);
	while (nStart != string::npos)
	{
		strValue.replace(nStart, strOld.length(), strNew);
		nFind = nStart + strNew.length();
		nStart = strValue.find(strOld, nFind);
	}

	return strValue;
}

xstring StringConvertUpperOrLower(bool bUpper, const xstring& strValue)
{
	xstring strText = strValue;

#ifdef UNICODE
	if (bUpper)
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::towupper);
	}
	else
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::towlower);
	}
#else
	if (bUpper)
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::toupper);
	}
	else
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::tolower);
	}
#endif
	return strText;
}
string StringConvertUpperOrLowerA(bool bUpper, const string& strValue)
{
	string strText = strValue;
	if (bUpper)
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::toupper);
	}
	else
	{
		transform(strText.begin(), strText.end(), strText.begin(), ::tolower);
	}
	return strText;
}

void SplitString(const char *pszSrc, std::vector<std::string>& vec, const char *pszDelim)
{
	vec.clear();
	int nStrLen = strlen(pszSrc);
	if (pszSrc == NULL || nStrLen >= 2048)
	{
		return;
	}

	char szPszDelim[3];
	if (pszDelim == NULL || strlen(pszDelim) == 0)
	{
		strcpy_s(szPszDelim, ",");
		pszDelim = szPszDelim;
	}

	const char *pszStart = pszSrc;
	const char *pszEnd = pszSrc + nStrLen;
	int nPos = strcspn(pszStart, pszDelim);
	if (nPos < 0 || nPos >= 2048 || pszStart >= pszEnd)
	{
		return;
	}
	while (pszStart + nPos <= pszEnd)
	{
		char szSrc[2048];
		strncpy(szSrc, pszStart, nPos);
		szSrc[nPos] = '\0';
		vec.push_back(szSrc);

		pszStart = pszStart + nPos + 1;
		if (pszStart >= pszEnd)
		{
			break;
		}
		nPos = strcspn(pszStart, pszDelim);		
	}
}

void SplitStringW(const wstring& src,vector<wstring> &dst,const wstring& strDelim)
{
	size_t last = 0; 
	wstring strTmp = src;
	wstring delim = strDelim;
	size_t index = strTmp.find_first_of(delim, last);  

	while (index != std::wstring::npos)  
	{  
		dst.push_back(strTmp.substr(last, index - last));  
		last = strTmp.find_first_not_of(delim, index + 1);
		index = strTmp.find_first_of(delim, last);  
	}  

	if (index-last > 0)  
	{  
		dst.push_back(strTmp.substr(last, index - last));  
	}
}

void TrimLeftStringA(string &strVal, LPCSTR lpszOneOfFinds)
{
	string::size_type nStartPos = strVal.find_first_not_of(lpszOneOfFinds);

	if (nStartPos == string::npos)
	{
		strVal.clear();
		return;
	}
	strVal = strVal.substr(nStartPos);
}

void TrimStringA(string &strVal, LPCSTR lpszOneOfFinds)
{
	string::size_type nStartPos = strVal.find_first_not_of(lpszOneOfFinds);
	string::size_type nEndPos = strVal.find_last_not_of(lpszOneOfFinds);

	if (nStartPos == string::npos)
	{
		strVal.clear();
		return;
	}
	strVal = strVal.substr(nStartPos, nEndPos - nStartPos+1);
}

void TrimString(xstring &strVal, LPCTSTR lpszOneOfFinds/* = _T(" ")*/)
{
	string::size_type nStartPos = strVal.find_first_not_of(lpszOneOfFinds);
	string::size_type nEndPos = strVal.find_last_not_of(lpszOneOfFinds);

	if (nStartPos == string::npos)
	{
		strVal.clear();
		return;
	}
	strVal = strVal.substr(nStartPos, nEndPos - nStartPos + 1);
}

void ReplaceSubString(xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd, LPCTSTR lpszNew)
{
	xstring::size_type nStartPos = strSrc.find(lpszStart);

	int nEndPos = 0;
	if (nStartPos != string::npos)
	{
		nEndPos = strSrc.find(lpszEnd, nStartPos + _tcslen(lpszStart));
		if (nEndPos == string::npos)
		{
			nEndPos = strSrc.size();
		}
		strSrc.replace(nStartPos + _tcslen(lpszStart), nEndPos - nStartPos - _tcslen(lpszStart), lpszNew);
	}
}

xstring GetSubString(const xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd)
{
	xstring::size_type nStartPos = strSrc.find(lpszStart);

	int nEndPos = 0;
	if (nStartPos != string::npos)
	{
		nEndPos = strSrc.find(lpszEnd, nStartPos + _tcslen(lpszStart));
		if (nEndPos == string::npos)
		{
			nEndPos = strSrc.size();
		}
		return strSrc.substr(nStartPos + _tcslen(lpszStart), nEndPos - nStartPos - _tcslen(lpszStart));
	}
	return _T("");
}

char dec2hexChar(short int n) {
	if ( 0 <= n && n <= 9 ) {
		return char( short('0') + n );
	} else if ( 10 <= n && n <= 15 ) {
		return char( short('A') + n - 10 );
	} else {
		return char(0);
	}
}
short int hexChar2dec(char c) {
	if ( '0'<=c && c<='9' ) {
		return short(c-'0');
	} else if ( 'a'<=c && c<='f' ) {
		return ( short(c-'a') + 10 );
	} else if ( 'A'<=c && c<='F' ) {
		return ( short(c-'A') + 10 );
	} else {
		return -1;
	}
}
string escape(const string &URL)
{
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) 
	{
		char c = URL[i];
		if ( ('0'<=c && c<='9')||('a'<=c && c<='z') || ('A'<=c && c<='Z') ) 
		{
			result += c;
		} 
		else 
		{
			int j = (short int)c;
			if ( j < 0 ) 
			{
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0); 
		}
	}
	return result;
}
string escapeURL(const string &URL)
{
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) 
	{
		char c = URL[i];
		if ( ('0'<=c && c<='9')||('a'<=c && c<='z') || ('A'<=c && c<='Z') || c=='/' || c=='.' || c==':' || c=='?' || c=='=') 
		{
			result += c;
		} 
		else 
		{
			int j = (short int)c;
			if ( j < 0 ) 
			{
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0); 
		}
	}
	return result;
}

std::string deescapeURL(const std::string &URL) 
{
	string result = "";		
	unsigned char num;
	int nRemainSize = URL.size();
	for (int i = 0; nRemainSize > 0; nRemainSize--,i++ ) 
	{
		num = URL[i];
		if(('%' == num) && (nRemainSize >= 3))
		{
			char c1 = URL[i+1];
			char c0 = URL[i+2];
			num = hexChar2dec(c1) * 16 + hexChar2dec(c0);			

			nRemainSize -= 2;
			i += 2;
		}

		result += num;
	}
	return result;
}

std::string deescapeUnicodeURL(const std::string &URL) 
{
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) 
	{
		char c = URL[i];
		if ( c == '\\' && URL[i+1] == 'u') 
		{
			i++;
			char c1 = URL[++i];
			char c0 = URL[++i];
			int num = 0;
			num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
			//result += char(num);	

			c1 = URL[++i];
			c0 = URL[++i];
			num = num*256;
			num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
			char szAnsi[8];
			int nLen =  ::WideCharToMultiByte(CP_ACP, 0, (wchar_t*)&num, 1, szAnsi, 7, NULL, FALSE);
			if(nLen > 0)
			{
				szAnsi[nLen] = '\0';
				result += szAnsi;
			}			
		} 
		else 
		{
			result += c;			
		}
	}
	return result;
}

string base64Encode(const char* source,int nLength)
{
	const char Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//由于传入的二进制有可能大于127,因此要用unsigned char，避免传入的串取单个字节值时变为负数,因此采用unsigned char
	unsigned char* src= (unsigned char*)source;

	int i = 0;
	//char *p = dst;
	string strDest;
	int d = nLength - 3;

	char ch = 0;
	for(i = 0;i <= d;i += 3){
		ch = Base64[((*(src + i)) >> 2) & 0x3f];
		strDest.append(1,ch);
		ch = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
		strDest.append(1,ch);
		ch = Base64[((*(src + i + 1) & 0xf) << 2) + ((*(src + i + 2)) >> 6)];
		strDest.append(1,ch);
		ch = Base64[(*(src + i + 2)) & 0x3f];
		strDest.append(1,ch);
		//*p++ = Base64[((*(src + i)) >> 2) & 0x3f];
		//*p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
		//*p++ = Base64[((*(src + i + 1) & 0xf) << 2) + ((*(src + i + 2)) >> 6)];
		//*p++ = Base64[(*(src + i + 2)) & 0x3f];
	}
	if((nLength - i) == 1){
		ch = Base64[((*(src + i)) >> 2) & 0x3f];
		strDest.append(1,ch);
		ch = Base64[((*(src + i)) & 0x3) << 4];
		strDest.append(1,ch);
		ch = '=';
		strDest.append(1,ch);
		ch = '=';
		strDest.append(1,ch);
		//*p++ = Base64[((*(src + i)) >> 2) & 0x3f];
		//*p++ = Base64[((*(src + i)) & 0x3) << 4];
		//*p++ = '=';
		//*p++ = '=';
	}
	if((nLength - i) == 2){
		ch = Base64[((*(src + i)) >> 2) & 0x3f];
		strDest.append(1,ch);
		ch = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
		strDest.append(1,ch);
		ch = Base64[((*(src + i + 1) & 0xf) << 2)];
		strDest.append(1,ch);
		ch = '=';
		strDest.append(1,ch);
		//*p++ = Base64[((*(src + i)) >> 2) & 0x3f];
		//*p++ = Base64[(((*(src + i)) & 0x3) << 4) + ((*(src + i + 1)) >> 4)];
		//*p++ = Base64[((*(src + i + 1) & 0xf) << 2)];
		//*p++ = '=';
	}

	return strDest;
}

int base64Decode(const char* source, unsigned char* pOut)
{
	const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	unsigned char* base64 = (unsigned char*)source;
	int i, j;
	unsigned char k;
	unsigned char temp[4];

	for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
	{
		memset( temp, 0xFF, sizeof(temp) );
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i] )
				temp[0]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+1] )
				temp[1]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+2] )
				temp[2]= k;
		}
		for ( k = 0 ; k < 64 ; k ++ )
		{
			if ( base64char[k] == base64[i+3] )
				temp[3]= k;
		}

		pOut[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
			((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
		if ( base64[i+2] == '=' )
			break;

		pOut[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
			((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
		if ( base64[i+3] == '=' )
			break;

		pOut[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
			((unsigned char)(temp[3]&0x3F));
	}

	return j;
}