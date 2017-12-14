#pragma once
#include <Shlwapi.h>
#include <WinBase.h>
#include <string>
#include <IPHlpApi.h>
#include <ShlObj.h>
#pragma comment(lib,"Shell32.lib")
#ifndef xstring
#ifdef _UNICODE
#define xstring		std::wstring
#else
#define	xstring		std::string
#endif
using namespace std;
#endif

//创建一个目录,若存在也会返回true,当不存在且创建失败时返回false 注意：不能一次创建多级目录
BOOL CreateDir(const xstring& strDir);

//居中到屏幕中央
void CenterWindowToScreen(HWND hWnd);
//自动居中窗口。hParent-父窗口,hWnd-要居中的窗口,父窗口最小化或者为NULL时将居中到屏幕
void CenterWindowEx(HWND hParent,HWND hWnd);

//功能:从提取一串字符串中的所有数字
std::string GetNumFromStrBuf(const char* pszStrBuf);

void PrintfLog(const TCHAR * format, ...);

//文件或文件夹是否存在
BOOL IsPathExist(const TCHAR* szPath);

//读取注册表
bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue);

//获取App所在目录(结尾没有'/'或者'\\')
xstring GetAppPath(HMODULE hModul=NULL);

//获取本机IP
string GetLocalIp();

//获取本机MAC
string GetMAC();

//获取文件后缀 (返回值统一为小写) 不包含.
xstring GetExtensionFromFileName(const xstring& strFileName);

//获取当前的默认浏览器
xstring GetDefaultBrowser();

//创建桌面图标
void CreateDesktopIcon(const xstring& strIconName,const xstring& strExeFile,const xstring& strCmdLine,const xstring& strIconFile);

//提取exe中的Icon并保存为文件 (strDestFile一般以ico结尾) (里面包含了1个大小的图像,如果需要多个,参考实现代码进行修改即可)
bool SaveIconFileFromExeFile(const xstring& strExe,const xstring& strDestFile);

//由绝对路径获取文件名
xstring GetAppNameFromPath(const xstring& strAppPath);

//是否是有效的身份证号 (返回1表明有效,错误码为负数)
int IsValidIdCardNumber(const xstring& strIdCardNumber);