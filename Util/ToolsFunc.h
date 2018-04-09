#pragma once
#include <Shlwapi.h>
#include <WinBase.h>
#include <string>
#include <deque>
#include <IPHlpApi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库  
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
//文件或文件夹是否存在
BOOL IsPathExistA(const string& strPath);

//读取注册表
bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue);
//写注册表
bool WriteRegValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
//删除注册表键值
bool DeleteRegKeyValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName);
//删除注册表的项以及下面的所有子项
bool DeleteRegSubKey(HKEY hKey,const xstring& strSubKey);

//获取App所在目录(结尾没有'/'或者'\\')
xstring GetAppPath(HMODULE hModul=NULL);

//获取本机IP
string GetLocalIp();
//获取本机多个网卡的信息
void GetIpAdapterInfoList(deque<IP_ADAPTER_INFO>& theIpAdapterList);

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

//加入开机启动		//strName注册表启动项中的键名 strFile-启动项中的键值,需要绝对路径
bool AddFollowSystemStart(const xstring& strName,const xstring& strFile);
//删除开机启动		//strName注册表启动项中的键名 strFile-启动项中的键值,需要绝对路径,可以为空
bool DelFollowSystemStart(const xstring& strName,const xstring& strFile);

//获取guid字符串 bUpper-是否大写
xstring GetGUID(bool bUpper=true);

//从文件中读取全部内容
string ReadAllFromFile(const xstring& strFile);
string ReadAllFromFileA(const string& strFile);

//获取当前操作系统名字
string GetOSName();
//是否是64位OS
bool Is64BitOS();

//复制目录 //如果是目录不要以"\\"或者"/"结尾
int CopyFolder(const xstring& strSource,const xstring& strDest);
int CopyFolderA(const string& strSource,const string& strDest);

//查找进程ID (32位进程只能查找32位进程)
int GetProcesssIdFromName(const xstring& strPorcessName);
void GetProcesssIdFromName(const xstring& strPorcessName,deque<int>& dequeOutID);

//获取文件大小(字节) //最大2G
long GetFileSizeByte(const xstring& strFile);
long GetFileSizeByteA(const string& strFile);