#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <tchar.h>

#pragma warning(disable:4190)
using namespace std;

#ifndef xstring 
#ifdef UNICODE
#define xstring wstring
#else
#define xstring string
#endif
#endif

std::string  UnicodeToAnsi(const std::wstring& strSource);
std::wstring AnsiToUnicode(const std::string& strSource);
std::wstring Utf8ToUnicode(const std::string& strSrouce);
std::string UnicodeToUtf8(const std::wstring& strSource);
std::string AnsiToUtf8(const std::string& strSource);
std::string Utf8ToAnsi(const std::string& strSource);
//std::string URLEncodeGB2312(const char* szSrc/*, char* pBuf, int cbBufLen*/);			//空格转为'+'
std::string URLEncodeGB2312Forspace(const char* szSrc/*, char* pBuf, int cbBufLen*/);	//空格转为'%20'
char* Utf8ToGBK(const char* strSource);
char* GBKToUtf8(const char* strSource);
//bool UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, bool bUpperCase);
//bool Utf8UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);
//bool UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);
std::string EscapeToAnsi(const std::string& strSource);

//尽量用这个编码来代替 URLEncodeGB2312 URLEncodeGB2312Forspace如果发现编码结果不对,可以尝试传入utf8编码参数
//如果仍然不对则需要视情况而定来对函数进行修正了
string escape(const string &URL);
string escapeURL(const string &URL);
string deescapeURL(const std::string &URL);

#ifndef _W
#define _W(x) AnsiToUnicode(x).data()
#endif

#ifndef _A
#define _A(x) UnicodeToAnsi(x).data()
#endif

//字符串替换 会对strText中的所有strOld都进行替换,但不会对替换后的串再次检查替换.例如将"1001"中的"1"替换为"1234",那么返回值为"1234001234"
//strText-全部内容 strOld-将要被替换的内容 strNew-新的用来替换的
xstring StringReplace(const xstring& strText, const xstring& strOld, const xstring& strNew);

void SplitString(const char *pszSrc, std::vector<std::string>& vec, const char *pszDelim=",");
void SplitStringW(const wstring& src,vector<wstring> &dst,const wstring& strDelim);

void TrimStringA(string &strVal, LPCSTR lpszOneOfFinds);
void TrimString(xstring &strVal, LPCTSTR lpszOneOfFinds=_T(" "));

void ReplaceSubString(xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd, LPCTSTR lpszNew);

xstring GetSubString(const xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd);

//大小写转换
xstring StringConvertUpperOrLower(bool bUpper,const xstring& strValue);

inline bool IsGB2312(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0xB0 && *pszIn<=0xF7 && *(pszIn+1)>=0xA0 && *(pszIn+1)<=0xFE) 
		return true; 
	else 
		return false; 
}

inline bool IsGBK(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0x81 && *pszIn<=0xFE && *(pszIn+1)>=0x40 && *(pszIn+1)<=0xFE) 
		return true; 
	else 
		return false; 
}

inline bool IsBIG5(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0x81 && *pszIn<=0xFE && ((*(pszIn+1)>=0x40 && *(pszIn+1)<=0x7E) || (*(pszIn+1)>=0xA1 && *(pszIn+1)<=0xFE)))
		return true; 
	else 
		return false; 
}

inline bool IsValidEnglishChar(const unsigned char *pszIn) 
{
	if((*pszIn>='0' && *pszIn <= '9') || (*pszIn>='a' && *pszIn <= 'z') || (*pszIn>='A' && *pszIn <= 'Z'))
	{
		return true;
	}
	else
		return false;
}

inline bool IsValidName(const unsigned char *pszIn, int nLen) 
{
	for (int i=0; i < nLen; )
	{
		if(IsValidEnglishChar(pszIn+i))
		{
			i++;
			continue;
		}
		else if(((i+1) < nLen) && IsGBK(pszIn+i))
		{
			i += 2;
			continue;
		}
		else
			return false;
	}
	return true;
}

inline bool IsValidQQ(const unsigned char *pszIn, int nLen) 
{
	for (int i=0; i < nLen; )
	{
		if(isdigit(*(pszIn+i)))
		{
			i++;
			continue;
		}
		else
			return false;
	}
	return true;

}

inline void CutChineseChar(char *pszSrc,unsigned int nNeedLen)
{
	if (strlen(pszSrc) >= nNeedLen)
	{
		//如果后两位不是英文字符，也不是一个有效的汉字，则后三位是一个汉字多最后一个字节
		if(!IsValidEnglishChar((const unsigned char *)(pszSrc+nNeedLen-2)) && !IsGBK((const unsigned char *)(pszSrc+nNeedLen-2)))
		{
			*(pszSrc+nNeedLen-3) = '.';
		}
		*(pszSrc+nNeedLen-2) = '.';
		*(pszSrc+nNeedLen-1) = '.';
		*(pszSrc+nNeedLen) = '\0';
	}
}