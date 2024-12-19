// stdafx.cpp : source file that includes just the standard includes
//	UIlib.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "StdAfx.h"


#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )

#ifdef USE_OPENCC
typedef void*(*POpencc_open_w)(const wchar_t* configFileName);
typedef int (*POpencc_close)(void* opencc);
typedef size_t(*POpencc_convert_utf8_to_buffer)(void* opencc, const char* input, size_t length, char* output);

opencc_t g_opencc = NULL;
POpencc_open_w openccOpenFunc = NULL;
POpencc_close openccCloseFunc = NULL;
POpencc_convert_utf8_to_buffer openConvertUtft8 = NULL;

opencc_t OpenccOpenW(const wchar_t* pwszCfgFile)
{
	if (g_opencc == NULL)
	{
		wchar_t szAppPath[512] = {0};
		GetModuleFileNameW(NULL,szAppPath,512);	

		//初始化App路径
		wchar_t szDrive[32] = {0},szDir[512] = {0},szFname[512]={0},szExt[128]={0};
		_wsplitpath_s(szAppPath,szDrive,szDir,szFname,szExt);
		wchar_t szDll[512] = {0};
		swprintf_s(szDll,L"%s%s%s",szDrive,szDir,L"opencc.dll");
	
		HMODULE hDll = LoadLibraryW(szDll);
		if (hDll)
		{
			openccOpenFunc = (POpencc_open_w)GetProcAddress(hDll, "opencc_open_w");
			openccCloseFunc = (POpencc_close)GetProcAddress(hDll, "opencc_close");
			openConvertUtft8 = (POpencc_convert_utf8_to_buffer)GetProcAddress(hDll, "opencc_convert_utf8_to_buffer");
			if (openccOpenFunc)
			{
				if (pwszCfgFile && wcslen(pwszCfgFile)>0)
				{
					g_opencc = openccOpenFunc(pwszCfgFile);
				}
				else
				{
					wchar_t szCfg[512] = {0};
					swprintf_s(szCfg,L"%s%s%s",szDrive,szDir,L"s2t.json");
					g_opencc = openccOpenFunc(szCfg);
				}
			}
		}
	}

	return g_opencc;
}
opencc_t OpenccOpenA(const char* pszCfgFile)
{
	if (g_opencc == NULL)
	{
		wchar_t szAppPath[512] = {0};
		GetModuleFileNameW(NULL,szAppPath,512);	

		//初始化App路径
		wchar_t szDrive[32] = {0},szDir[512] = {0},szFname[512]={0},szExt[128]={0};
		_wsplitpath_s(szAppPath,szDrive,szDir,szFname,szExt);
		wchar_t szDll[512] = {0};
		swprintf_s(szDll,L"%s%s%s",szDrive,szDir,L"opencc.dll");

		HMODULE hDll = LoadLibraryW(szDll);
		if (hDll)
		{
			openccOpenFunc = (POpencc_open_w)GetProcAddress(hDll, "opencc_open_w");
			openccCloseFunc = (POpencc_close)GetProcAddress(hDll, "opencc_close");
			openConvertUtft8 = (POpencc_convert_utf8_to_buffer)GetProcAddress(hDll, "opencc_convert_utf8_to_buffer");
			if (openccOpenFunc)
			{
				if (pszCfgFile && strlen(pszCfgFile)>0)
				{
					//ansi转unicode
					int nUnicodeLength = ::MultiByteToWideChar(CP_ACP, 0, pszCfgFile, -1, NULL, 0);
					if (nUnicodeLength < 1)
						return NULL;
					wchar_t szCfg[1024] = {0};
					::MultiByteToWideChar(CP_ACP, 0, pszCfgFile, strlen(pszCfgFile), szCfg, nUnicodeLength);
					szCfg[nUnicodeLength] = L'\0';

					g_opencc = openccOpenFunc(szCfg);
				}
				else
				{
					wchar_t szCfg[512] = {0};
					swprintf_s(szCfg,L"%s%s%s",szDrive,szDir,L"s2t.json");
					g_opencc = openccOpenFunc(szCfg);
				}
			}
		}
	}

	return g_opencc;
}
int OpenccClose()
{
	int nErr = 0;
	if (g_opencc && openccCloseFunc)
	{
		nErr = openccCloseFunc(g_opencc);
		g_opencc = NULL;
	}
	return nErr;
}
const wchar_t* OpenccConvertW(const wchar_t* pszText)
{
	if (g_opencc==NULL)
		return pszText;

	if (pszText && wcslen(pszText)>0)
	{
		//转utf8
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, pszText, wcslen(pszText), NULL, 0, NULL, FALSE);
		if (nLength < 1)
			return pszText;
		static char* pszUtf8 = NULL;
		pszUtf8 = (char*)realloc(pszUtf8,nLength+1);
		::WideCharToMultiByte(CP_UTF8, 0, pszText, wcslen(pszText), pszUtf8, nLength, NULL, FALSE);
		pszUtf8[nLength] = '\0';

		//转换
		static char* pszRetUtf8 = NULL;
		pszRetUtf8 = (char*)realloc(pszRetUtf8,nLength*1.5+1);
		size_t retConv = openConvertUtft8(g_opencc,pszUtf8,nLength,pszRetUtf8);
		pszRetUtf8[retConv] = '\0';
		
		//转回unicode
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv, NULL, 0);
		if (nLength < 1)
			return pszText;

		static wchar_t* pszwUnicode = NULL;
		pszwUnicode = (wchar_t*)realloc(pszwUnicode,nLength*sizeof(wchar_t)+sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv, pszwUnicode, nLength);
		pszwUnicode[nLength] = L'\0';

		return pszwUnicode;
	}
	
	return pszText;
}
const char* OpenccConvertA(const char* pszAnsiText)
{
	if (g_opencc==NULL)
		return pszAnsiText;

	if (pszAnsiText && strlen(pszAnsiText)>0)
	{
		//ansi转unicode
		int nUnicodeLength = ::MultiByteToWideChar(CP_ACP, 0, pszAnsiText, strlen(pszAnsiText), NULL, 0);
		if (nUnicodeLength < 1)
			return pszAnsiText;
		static wchar_t* pszwUnicode = NULL;
		pszwUnicode = (wchar_t*)realloc(pszwUnicode,nUnicodeLength*sizeof(wchar_t)+sizeof(wchar_t));
		::MultiByteToWideChar(CP_ACP, 0, pszAnsiText, strlen(pszAnsiText), pszwUnicode, nUnicodeLength);
		pszwUnicode[nUnicodeLength] = L'\0';

		//转utf8
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, pszwUnicode, nUnicodeLength, NULL, 0, NULL, FALSE);
		if (nLength < 1)
			return pszAnsiText;
		static char* pszUtf8 = NULL;
		pszUtf8 = (char*)realloc(pszUtf8,nLength+1);
		::WideCharToMultiByte(CP_UTF8, 0, pszwUnicode, nUnicodeLength, pszUtf8, nLength, NULL, FALSE);
		pszUtf8[nLength] = '\0';

		//转换
		static char* pszRetUtf8 = NULL;
		pszRetUtf8 = (char*)realloc(pszRetUtf8,nLength*1.5+1);
		size_t retConv = openConvertUtft8(g_opencc,pszUtf8,nLength,pszRetUtf8);
		pszRetUtf8[retConv] = '\0';

		//转回unicode
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv, NULL, 0);
		if (nLength < 1)
			return pszAnsiText;

		static wchar_t* pszwUnicode2 = NULL;
		pszwUnicode2 = (wchar_t*)realloc(pszwUnicode2,nLength*sizeof(wchar_t)+sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv, pszwUnicode2, nLength);
		pszwUnicode2[nLength] = L'\0';

		//转回ansi
		int nAnsiLength = ::WideCharToMultiByte(CP_ACP, 0, pszwUnicode2, nLength, NULL, 0, NULL, FALSE);
		if (nAnsiLength < 1)
			return pszAnsiText;

		static char* pszRetAnsi = NULL;
		pszRetAnsi = (char*)realloc(pszRetAnsi,nAnsiLength+1);
		::WideCharToMultiByte(CP_ACP, 0, pszwUnicode2, nLength, pszRetAnsi, nAnsiLength, NULL, FALSE);
		pszRetAnsi[nAnsiLength] = '\0';

		return pszRetAnsi;
	}

	return pszAnsiText;
}
#endif