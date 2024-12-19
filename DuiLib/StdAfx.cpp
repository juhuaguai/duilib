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
	if (g_opencc==NULL || pszText==NULL)
		return pszText;

	static int nRetUnicode = 1024*sizeof(wchar_t);
	static wchar_t* pszwUnicode = (wchar_t*)malloc(nRetUnicode);

	int nInLen = wcslen(pszText);
	if (nInLen>1024)
	{
		int nSize = nInLen*sizeof(wchar_t);
		char* pszUtf8 = (char*)malloc(nSize);

		//转utf8
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, pszText, wcslen(pszText), pszUtf8, nSize, NULL, FALSE);
		if (nLength == 0)
		{
			free(pszUtf8);
			return pszText;
		}
		pszUtf8[nLength] = '\0';

		//转换
		char* pszRetUtf8 = (char*)malloc(nLength*1.5+1);
		size_t retConv = openConvertUtft8(g_opencc,pszUtf8,nLength,pszRetUtf8);
		pszRetUtf8[retConv] = '\0';
		
		free(pszUtf8);

		//转回unicode
		if (nRetUnicode < retConv*sizeof(wchar_t))
		{
			free(pszwUnicode);
			nRetUnicode = (retConv+1)*sizeof(wchar_t);
			pszwUnicode = (wchar_t*)malloc(nRetUnicode);
		}
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv,pszwUnicode, retConv);
		if (nLength == 0)
		{
			free(pszRetUtf8);
			return pszText;
		}
		pszwUnicode[nLength] = L'\0';

		free(pszRetUtf8);

		return pszwUnicode;
	}
	else
	{
		char szUtf8[2048] = {0};
		//转utf8
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, pszText, wcslen(pszText), szUtf8, 2048, NULL, FALSE);
		if (nLength == 0)
			return pszText;
		szUtf8[nLength] = '\0';

		//转换
		char szRetUtf8[2048] = {0};
		size_t retConv = openConvertUtft8(g_opencc,szUtf8,nLength,szRetUtf8);
		szRetUtf8[retConv] = '\0';

		//转回unicode
		if (nRetUnicode < retConv*sizeof(wchar_t))
		{
			free(pszwUnicode);
			nRetUnicode = (retConv+1)*sizeof(wchar_t);
			pszwUnicode = (wchar_t*)malloc(nRetUnicode);
		}
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, szRetUtf8, retConv,pszwUnicode, retConv);
		if (nLength == 0)
		{
			return pszText;
		}
		pszwUnicode[nLength] = L'\0';

		return pszwUnicode;
	}
	
	return pszText;
}
const char* OpenccConvertA(const char* pszAnsiText)
{
	if (g_opencc==NULL || pszAnsiText==NULL)
		return pszAnsiText;

	static int nRetAnsi = 1024;
	static char* pszRetAnsi = (char*)malloc(nRetAnsi);

	int nInLen = strlen(pszAnsiText);
	if (nInLen>1024)
	{
		//ansi转unicode
		int nSize = (nInLen+1)*sizeof(wchar_t);
		wchar_t* pszwUnicode = (wchar_t*)malloc(nSize);		
		int nUnicodeLength = ::MultiByteToWideChar(CP_ACP, 0, pszAnsiText, nInLen, pszwUnicode, nInLen);
		if (nUnicodeLength == 0)
		{
			free(pszwUnicode);
			return pszAnsiText;
		}
		pszwUnicode[nUnicodeLength] = L'\0';

		//转utf8
		char* pszUtf8 = (char*)malloc(nSize);
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, pszwUnicode, nUnicodeLength, pszUtf8, nSize, NULL, FALSE);
		if (nLength == 0)
		{
			free(pszwUnicode);
			free(pszUtf8);
			return pszAnsiText;
		}
		pszUtf8[nLength] = '\0';

		free(pszwUnicode);

		//转换
		char* pszRetUtf8 = (char*)malloc(nLength*1.5+1);
		size_t retConv = openConvertUtft8(g_opencc,pszUtf8,nLength,pszRetUtf8);
		pszRetUtf8[retConv] = '\0';

		free(pszUtf8);

		//转回unicode
		wchar_t* pszwRetUnicode = (wchar_t*)malloc((retConv+1)*sizeof(wchar_t));
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, pszRetUtf8, retConv, pszwRetUnicode, retConv);
		if (nLength == 0)
		{
			free(pszRetUtf8);
			free(pszwRetUnicode);
			return pszAnsiText;
		}
		pszwRetUnicode[nLength] = L'\0';

		free(pszRetUtf8);

		//转回ansi
		if (nRetAnsi < nLength*sizeof(wchar_t))
		{
			free(pszRetAnsi);
			nRetAnsi = nLength*sizeof(wchar_t);
			pszRetAnsi = (char*)malloc(nRetAnsi);
		}
		int nAnsiLength = ::WideCharToMultiByte(CP_ACP, 0, pszwRetUnicode, nLength, pszRetAnsi, nRetAnsi, NULL, FALSE);
		if (nAnsiLength == 0)
		{
			free(pszwRetUnicode);
			return pszAnsiText;
		}
		pszRetAnsi[nAnsiLength] = '\0';

		free(pszwRetUnicode);

		return pszRetAnsi;
	}
	else
	{
		//ansi转unicode
		wchar_t szwUnicode[1024] = {0}; 
		int nUnicodeLength = ::MultiByteToWideChar(CP_ACP, 0, pszAnsiText, nInLen, szwUnicode, 1024);
		if (nUnicodeLength == 0)
			return pszAnsiText;
		szwUnicode[nUnicodeLength] = L'\0';

		//转utf8
		char szUtf8[2048] = {0};
		int nLength = ::WideCharToMultiByte(CP_UTF8, 0, szwUnicode, nUnicodeLength, szUtf8, 2048, NULL, FALSE);
		if (nLength == 0)
			return pszAnsiText;
		szUtf8[nLength] = '\0';

		//转换
		char szRetUtf8[2048] = {0};
		size_t retConv = openConvertUtft8(g_opencc,szUtf8,nLength,szRetUtf8);
		szRetUtf8[retConv] = '\0';

		//转回unicode
		wchar_t szwRetUnicode[2048] = {0};
		nLength = ::MultiByteToWideChar(CP_UTF8, 0, szRetUtf8, retConv, szwRetUnicode, 2048);
		if (nLength == 0)
			return pszAnsiText;
		szwRetUnicode[nLength] = L'\0';

		//转回ansi
		if (nRetAnsi < nLength*sizeof(wchar_t))
		{
			free(pszRetAnsi);
			nRetAnsi = nLength*sizeof(wchar_t);
			pszRetAnsi = (char*)malloc(nRetAnsi);
		}
		int nAnsiLength = ::WideCharToMultiByte(CP_ACP, 0, szwRetUnicode, nLength, pszRetAnsi, nRetAnsi, NULL, FALSE);
		if (nAnsiLength == 0)
			return pszAnsiText;
		pszRetAnsi[nAnsiLength] = '\0';

		return pszRetAnsi;
	}

	return pszAnsiText;
}
#endif