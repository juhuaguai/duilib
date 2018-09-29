#pragma once
#include <windows.h>
#include <tchar.h>

#include "zip.h"
#include "unzip.h"

#include <string>
using std::string;
using std::wstring;

#ifdef  _UNICODE
#define xstring wstring
#else
#define xstring string
#endif

class CZipx
{
public:
	CZipx(void);
	~CZipx(void);

private:
	xstring m_strSrcFileDir;
	xstring m_strDstFilePath;

public:
	
	//压缩一个文件
	bool ZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstFile=NULL, const char *pszPassword=NULL);
	BOOL ZipFiles(LPCTSTR lpszSrcDir, LPCTSTR lpszDstFile, const char *pszPassword=NULL) ;

	//压缩一个文件夹
	bool UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstDir, const char *pszPassword=NULL);
	bool UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszFileName, LPCTSTR lpszDstDir, const char *pszPassword=NULL);
	bool UnZipFileFromMem(void* pData,DWORD dwDataSize,LPCTSTR lpszDstDir, const char *pszPassword=NULL);

private:
	bool CreateDir(LPCTSTR lpszDir);
	//bool IsFolderExist(LPCTSTR lpszPath) ;
	void GetRelativePath(const xstring& strFilePath, xstring& strSubPath) ;
	void AddFileFromDir(HZIP hz, const xstring &strFileDir) ;
	

};

void PathAddBackslash2(xstring &strFilePath);
bool IsFileExist(const xstring& strFilePath);
bool IsDirExist(const xstring & strFilePath);
bool IsPathExist(const xstring & strFilePath);
