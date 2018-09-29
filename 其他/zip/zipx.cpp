#include "zipx.h"
#include <stdio.h>

void PathAddBackslash2(xstring &strFilePath)
{
	int nLen = strFilePath.size();
	if(nLen > 0)
	{
		if(strFilePath[nLen-1] != _T('\\'))
		{
			strFilePath += _T('\\');
		}
	}
}

// 判断文件是否存在
bool IsFileExist(const xstring& strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return ((INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}
// 判断文件夹是否存在
bool IsDirExist(const xstring & strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return ((INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)));
}
// 判断文件或文件夹是否存在
bool IsPathExist(const xstring & strFilePath)
{
	DWORD dwAttrib = GetFileAttributes(strFilePath.c_str());
	return (INVALID_FILE_ATTRIBUTES != dwAttrib);
}

// 变变变变变种(听说会更快一点)，见备注1
bool IsPathExistEx(const xstring & strFilePath)
{
	WIN32_FILE_ATTRIBUTE_DATA attrs = { 0 };
	return (0 != GetFileAttributesEx(strFilePath.c_str(), GetFileExInfoStandard, &attrs));
}


CZipx::CZipx(void)
{
}

CZipx::~CZipx(void)
{
}

bool CZipx::ZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstFile, const char *pszPassword)
{
	LPCTSTR pszTempDst = lpszDstFile;
	if(lpszDstFile == NULL)
	{
		pszTempDst = lpszSrcFile;
	}

	//目标文件是否存在
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	// 取得文件名
	LPCTSTR pszFind = _tcsrchr(pszTempDst, _T('\\'));
	xstring strFileName = pszFind;
	xstring strFileDir(pszTempDst, pszFind-pszTempDst);
	if(lpszDstFile == NULL)
	{
		//更改目标文件后缀
		string::size_type nFind = strFileName.find_last_of(_T('.'));
		if(nFind != string::npos)
		{
			strFileName = strFileName.substr(0, nFind) + _T(".zip");
		}
	}

	//建立文件目录
	CreateDir(strFileDir.c_str());

	HZIP hz = CreateZip(lpszDstFile, pszPassword);
	ZipAdd(hz, strFileName.c_str(), lpszSrcFile);
	CloseZip(hz);
}

//目标文件必须指定
BOOL CZipx::ZipFiles(LPCTSTR lpszSrcDir, LPCTSTR lpszDstFile, const char *pszPassword) 
{ 
	if(lpszDstFile == NULL || lpszSrcDir == NULL)
	{
		return FALSE;
	}

	//检查源目录是否存在
	if(!IsDirExist(lpszSrcDir))
	{
		return false;
	}

	// 取得文件名
	LPCTSTR pszFind = _tcsrchr(lpszDstFile, _T('\\'));
	if(pszFind == NULL)
	{
		return FALSE;
	}

	xstring strDstFileName = pszFind+1;
	xstring strDstFileDir(lpszDstFile, pszFind - lpszDstFile+1);
	if(strDstFileName.size() == 0)
	{
		return FALSE;
	}	

	//建立文件目录
	m_strDstFilePath = lpszDstFile;
	m_strSrcFileDir = lpszSrcDir;
	CreateDir(strDstFileDir.c_str());

	//创建ZIP文件 
	HZIP hz=CreateZip(lpszDstFile, pszPassword); 
	if(hz == 0) 
	{ 
		//创建Zip文件失败 
		return FALSE; 
	} 

	//递归文件夹,将获取的文件加入ZIP文件 	
	AddFileFromDir(hz, m_strSrcFileDir); 

	//关闭ZIP文件 
	CloseZip(hz); 

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(lpszDstFile)) 
	{ 
		//压缩失败(未发现压缩后的文件) 
		return FALSE; 
	} 
	return TRUE; 
} 

void CZipx::AddFileFromDir(HZIP hz, const xstring &strFileDir) 
{ 
	WIN32_FIND_DATA wfd = {0}; 
	xstring strDir = strFileDir; 
	PathAddBackslash2(strDir);

	HANDLE hFind = FindFirstFile((strDir+_T("*.*")).c_str(), &wfd); 

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (wfd.cFileName[0] == _T('.'))
		{
			if (wfd.cFileName[1] == _T('\0') ||
				(wfd.cFileName[1] == _T('.') &&	wfd.cFileName[2] == _T('\0')))
			{
				continue;
			}
		}

		xstring strFilePath = strDir + wfd.cFileName;

		//如果是一个子目录，用递归继续往深一层找 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			xstring subPath; 
			GetRelativePath(strFilePath, subPath); 

			ZipAddFolder(hz, subPath.c_str()); 
			AddFileFromDir(hz, strFilePath); 
		} 
		else
		{ 
			//排除本压缩文档
			if(_tcsicmp(m_strDstFilePath.c_str(), strFilePath.c_str()) == 0)
			{
				continue;
			}

			//显示当前访问的文件(完整路径) 
			xstring subPath; 
			GetRelativePath(strFilePath, subPath); 

			//将文件添加到ZIP文件
			ZipAdd(hz, subPath.c_str(), strFilePath.c_str()); 
		} 
	}
	while( 0 != FindNextFile(hFind, &wfd));

	FindClose(hFind);
} 

bool CZipx::UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszDstDir, const char *pszPassword)
{
	if(lpszSrcFile == NULL || lpszDstDir == NULL)
		return false;

	//检查源文件是否存在
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	CreateDir(lpszDstDir);

	//打开ZIP文件 
	HZIP hz = OpenZip(lpszSrcFile, pszPassword); 
	if(hz == 0) 
	{ 
		//打开Zip文件失败 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//打开Zip文件失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	ZIPENTRY ze;
	zr=GetZipItem(hz, -1, &ze); 
	if(zr != ZR_OK) 
	{ 
		//获取Zip文件内容失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	int nNumItems = ze.index; 
	for (int i=0; i<nNumItems; i++) 
	{ 
		zr = GetZipItem(hz, i, &ze); 
		zr = UnzipItem(hz, i, ze.name); 

		if(zr != ZR_OK) 
		{ 
			//获取Zip文件内容失败 
			CloseZip(hz); 
			return false;       
		} 
	} 

	CloseZip(hz); 

	return true;
}

bool CZipx::UnZipFile(LPCTSTR lpszSrcFile, LPCTSTR lpszFileName, LPCTSTR lpszDstDir, const char *pszPassword)
{
	if(lpszSrcFile == NULL || lpszDstDir == NULL)
		return false;

	//检查源文件是否存在
	if(!IsFileExist(lpszSrcFile))
	{
		return false;
	}

	CreateDir(lpszDstDir);

	//打开ZIP文件 
	HZIP hz = OpenZip(lpszSrcFile, pszPassword); 
	if(hz == 0) 
	{ 
		//打开Zip文件失败 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//打开Zip文件失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	int i = 0; 
	ZIPENTRY ze;
	zr = FindZipItem(hz, lpszFileName, true, &i, &ze);
	if(zr == ZR_OK) 
	{
		zr = UnzipItem(hz, i, ze.name);
	}
	CloseZip(hz);
	 
	return (zr == ZR_OK) ;
}

bool CZipx::UnZipFileFromMem(void* pData,DWORD dwDataSize,LPCTSTR lpszDstDir, const char *pszPassword/* =NULL */)
{
	if(pData == NULL || dwDataSize==0)
		return false;

	CreateDir(lpszDstDir);

	//打开ZIP文件 
	HZIP hz = OpenZip(pData,dwDataSize, pszPassword); 
	if(hz == 0) 
	{ 
		//打开Zip文件失败 
		return false; 
	} 

	ZRESULT zr = SetUnzipBaseDir(hz, lpszDstDir); 
	if(zr != ZR_OK) 
	{ 
		//打开Zip文件失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	ZIPENTRY ze;
	zr=GetZipItem(hz, -1, &ze); 
	if(zr != ZR_OK) 
	{ 
		//获取Zip文件内容失败 
		CloseZip(hz); 
		return FALSE;       
	} 

	int nNumItems = ze.index; 
	for (int i=0; i<nNumItems; i++) 
	{ 
		zr = GetZipItem(hz, i, &ze); 
		zr = UnzipItem(hz, i, ze.name); 

		if(zr != ZR_OK) 
		{ 
			//获取Zip文件内容失败 
			CloseZip(hz); 
			return false;       
		} 
	} 

	CloseZip(hz); 

	return true;
}

bool CZipx::CreateDir(LPCTSTR lpszDir)
{
	if (IsDirExist(lpszDir))       //如果目录已存在，直接返回
	{
		return TRUE;
	}

	std::xstring strPath;
	TCHAR pszSrc[MAX_PATH] = { 0 };
	_tcscpy_s(pszSrc, lpszDir);
	TCHAR *pszContent;
	TCHAR *ptoken = _tcstok_s(pszSrc, _T("\\/"), &pszContent);
	while (ptoken)
	{
		strPath += ptoken;
		strPath += _T("\\");
		if (!IsDirExist(strPath.c_str()))
		{
			//创建失败时还应删除已创建的上层目录，此次略
			if (!CreateDirectory(strPath.c_str(), NULL))
			{
				DWORD dw = GetLastError();
				return FALSE;
			}
		}
		ptoken = _tcstok_s(NULL, _T("\\"), &pszContent);
	}
	return TRUE;

}

void CZipx::GetRelativePath(const xstring& strFilePath, xstring& strSubPath) 
{ 
	strSubPath = strFilePath;
	if(_tcsnicmp(strFilePath.c_str(), m_strSrcFileDir.c_str(), m_strSrcFileDir.size()) == 0)
	{
		strSubPath = strFilePath.substr(m_strSrcFileDir.size());
	}
}






