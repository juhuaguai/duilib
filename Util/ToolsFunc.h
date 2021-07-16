#pragma once
#include <Shlwapi.h>
#include <WinBase.h>
#include <string>
#include <deque>
#include <map>
//下边的顺序不要错
#include <ws2ipdef.h>
#include <IPHlpApi.h>
//#include <netioapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库  
#include <ShlObj.h>
#pragma comment(lib,"Shell32.lib")

#define MY_ENCODING_TYPE (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#pragma comment(lib,"crypt32.lib") 

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

//#include <wincrypt.h>
//#pragma  comment(lib, "crypt32.lib")

#pragma comment(lib, "Wintrust.lib") 
//#pragma comment(lib, "crypt32.lib")
#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

#ifndef xstring
#ifdef _UNICODE
#define xstring		std::wstring
#else
#define	xstring		std::string
#endif
using namespace std;
#endif

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
//文件是否存在
bool IsDirOrFileExist(const TCHAR* szPath);
//文件是否存在
bool IsDirOrFileExistA(const char* szPath);
//创建一个目录,若存在也会返回true,当不存在且创建失败时返回false
BOOL CreateDirW(const wstring& strDir);
//复制目录 //如果是目录不要以"\\"或者"/"结尾
int CopyFolder(const xstring& strSource,const xstring& strDest);
int CopyFolderA(const string& strSource,const string& strDest);
//删除文件夹及其里面的文件。成功返回0
int DeleteFolder(const xstring& strDest);
// 递归查询文件
// 返回值true表示成功，false表示失败
bool RecursionSearchFile(const wchar_t* pszSource,const wchar_t* pszDestFileName,wchar_t* pszOutMsg,int nOutMaxLen);
// 文件夹是否为空
bool IsEmptyFolder(const wchar_t* pszSource, const wchar_t* pszDestFileName, wchar_t* pszOutMsg, int nOutMaxLen);
// 查询路径下的文件夹
deque<wstring> RecursionSearchFloder(const wchar_t* pszSource,const wchar_t* pszDestFileName,wchar_t* pszOutMsg,int nOutMaxLen);
// 查询路径下的指定后缀文件
deque<wstring> RecursionSearchNameSuffix(const wchar_t* pszSource,const wchar_t* pszNameSuffix,wchar_t* pszOutMsg,int nOutMaxLen);

//读取注册表
bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue);
//写注册表
bool WriteRegValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
bool WriteRegValueA(HKEY hKey,const string& strSubKey,const string& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
bool WriteRegValueNo6432(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
//删除注册表键值
bool DeleteRegKeyValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName);
bool DeleteRegKeyValueA(HKEY hKey,const string& strSubKey,const string& strKeyName);
//删除注册表的项以及下面的所有子项
bool DeleteRegSubKey(HKEY hKey,const xstring& strSubKey);

//获取App所在目录(结尾有'/'或者'\\')
xstring GetAppPath(HMODULE hModul=NULL);
xstring GetExeName(HMODULE hModul=NULL);

//检查端口是否被占用 返回值:-1-表示未被使用,否则返回使用的进程PID
int CheckPortUsed(int nPort);

//获取本机IP
string GetLocalIp();
string getLocalIp();
//获取本机多个网卡的信息
void GetIpAdapterInfoList(deque<IP_ADAPTER_INFO>& theIpAdapterList);
// 获取正在使用的网卡
string GetInnerIp(wstring& strAlias,ULONG& uIndex, string& strGetway, string& strAdapterName);
//string GetInnerIp(ULONG& uIndex, string& strGetway);

//获取本机MAC
string GetMAC();

//获取文件后缀 (返回值统一为小写) 不包含.
xstring GetExtensionFromFileName(const xstring& strFileName);

//获取当前的默认浏览器
xstring GetDefaultBrowser();

//创建桌面图标
void CreateDesktopIcon(const xstring& strIconName,const xstring& strExeFile,const xstring& strCmdLine,const xstring& strIconFile);
// 由快捷方式找到源文件路径
bool GetShellPath(const char *Src,wchar_t *ShellPath);
// 判断是不是快捷方式
bool IsLineFile(const wstring wstr);

//提取exe中的Icon并保存为文件 (strDestFile一般以ico结尾) (里面包含了1个大小的图像,如果需要多个,参考实现代码进行修改即可)
bool SaveIconFileFromExeFile(const xstring& strExe,const xstring& strDestFile);
//提取exe中的Icon并保存为文件 (strDestFile一般以ico结尾) (里面包含了1个大小的图像,如果需要多个,参考实现代码进行修改即可)
bool SaveIconToPngFromExeFile(const xstring& strExe, const xstring& strDestFile,int nInWidth,int nInHeight);

//由绝对路径获取文件名
xstring GetAppNameFromPath(const xstring& strAppPath);

//由路径获取文件名
wstring GetFileNameFromPath(const wstring& strPath);
//由路径获取文件名
string GetFileNameFromPathA(const string& strPath);

//是否是有效的身份证号 (返回1表明有效,错误码为负数)
int IsValidIdCardNumber(const xstring& strIdCardNumber);

//加入开机启动		//strName注册表启动项中的键名 strFile-启动项中的键值,需要绝对路径
bool AddFollowSystemStart(const xstring& strName,const xstring& strFile);
//删除开机启动		//strName注册表启动项中的键名 strFile-启动项中的键值,需要绝对路径,可以为空
bool DelFollowSystemStart(const xstring& strName,const xstring& strFile);

//获取guid字符串 bUpper-是否大写
xstring GetGUID(bool bUpper=true);

//从文件中读取全部内容(utf8写则读取出来就是utf8,为了兼容非简体中文的操作系统,最好使用Unicode版本函数)
string ReadAllFromFile(const wstring& strFile);
//以Utf8编码格式写入文件
int WriteUtf8File(const string& strUtf8,const wstring& strFile);
int WriteUtf8FileW(const wstring& strUnicode,const wstring& strFile);

//获取当前操作系统名字
xstring GetOSName();
//是否是64位OS
BOOL Is64BitOS();
//获取操作系统版本
int GetOSVersion(int& nMajorVersion,int& nMinorVersion);
bool IsXp();
bool IsXpUp();
bool IsWin7();
bool IsWin7Up();
bool IsWin10();

//查找进程ID //此函数获取到当前进程的PID可能是0
int GetProcesssIdFromName(const xstring& strPorcessName,bool bCaseSensitive = false);
void GetProcesssIdFromName(const xstring& strPorcessName,deque<int>& dequeOutID,bool bCaseSensitive = false);
//查找进程PID,并获取绝对路径(32位进程只能查找32位进程)
void GetProcesssInfoFromName(const xstring& strPorcessName,map<int,wstring>& mapOutID, DWORD& dwErrorCode, wstring& wstrErrMsg,bool bCaseSensitive = false);

//获取文件大小(字节) //最大2G
long GetFileSizeByte(const xstring& strFile);
long GetFileSizeByteA(const string& strFile);

//获取系统当前时区
xstring GetTimeZoneNow();
//设置时区
//调用为 SetTimeZone(_T("Korea Standard Time"));
//Korea Standard Time (UTC+09:00)首尔
//Tokyo Standard Time (UTC+09:00)大阪，札幌，东京
//China Standard Time 北京时间
bool SetTimeZone(const xstring& subKey);
	
#if 0	//有问题，待排查
//静默执行命令行
bool DealExecCmd(const xstring& strCommandLine);
//添加程序到防火墙
bool AddFSystemFireWallRuler(const xstring& strfileName, const xstring& strName);
#endif

//导入证书 返回0表示成功
int ImportCACertFile(const xstring& strFileName);
int ImportCACertString(const string& strTxt);

//获取当前进程的启动进程PID
long GetCurStartProcessPID();
//获取指定进程的启动进程PID
long GetStartProcessPID(DWORD dwId);

/*设置当前进程默认使用的webbrowser内核版本 win7系统默认应该是ie7版本,8000表示ie8
10001 (0x2711)	Internet Explorer 10。网页以IE 10的标准模式展现，页面!DOCTYPE无效
10000 (0x02710)	Internet Explorer 10。在IE 10标准模式中按照网页上!DOCTYPE指令来显示网页。Internet Explorer 10 默认值。
9999 (0x270F)	Windows Internet Explorer 9. 强制IE9显示，忽略!DOCTYPE指令
9000 (0x2328)	Internet Explorer 9. Internet Explorer 9默认值，在IE9标准模式中按照网页上!DOCTYPE指令来显示网页。
8888 (0x22B8)	Internet Explorer 8，强制IE8标准模式显示，忽略!DOCTYPE指令
8000 (0x1F40)	Internet Explorer 8默认设置，在IE8标准模式中按照网页上!DOCTYPE指令展示网页
7000 (0x1B58)	使用WebBrowser Control控件的应用程序所使用的默认值，在IE7标准模式中按照网页上!DOCTYPE指令来展示网页。
*/
void SetIEWebbrowserVersion(DWORD dwIEVersion = 8000);

//CPU序列号
string GetCpuIndex();
//CPU描述
wstring GetCpuDescr();
//CPU核心数
int GetCpuCoreNum();
// cpu型号
string GetCPUType();
// 获取系统信息
wstring GetSysInfo();

//获取内存大小 //sprintf(szMemorySize,"%I64d",dwMemorySize);
DWORDLONG GetMemorySize();

//获取机器GUID,从注册表中读取出来的,重装系统可能改变
xstring GetMachineGUID();

//获取主板的UUID,可能获取失败,当失败时,字符串中仅包含全部都是F或者全部都是0
string GetBIOSUUID();

// 更快快捷方式图标
bool ChangeLinkIcon(const wstring &strLnkName, const wstring &strIconPath);

//复制字符串到剪切板
bool CopyStringToClipboard(const wstring& strValue);

// 提升系统权限
// 这是一个通用的提升权限函数，如果需要提升其他权限  
// 更改LookupPrivilegeValue的第二个参数SE_SHUTDOWN_NAME，即可  
BOOL EnableShutDownPriv();
// ReSetWindows(EWX_LOGOFF, false);		// 注销
// ReSetWindows(EWX_REBOOT, true);		// 重启 
// ReSetWindows(EWX_SHUTDOWN, true);	// 关机 
BOOL ReSetWindows(DWORD dwFlags, BOOL bForce);
//作用：获取软件的数字签名
//参数：
//		v_pwszFilePath  ---   程序的全路径
//		v_pwszSign      ---   用于返回数字签名的缓冲区,如果为NULL，
//					   	      那么将会需要的缓冲区大小
//      v_iBufSize      ---   v_pwszSign缓冲区的大小
//返回值：
//		-1		---  失败
//		0       ---  成功
//		其它值  ---  需要的缓冲区大小
LONG GetSoftSign(const wchar_t* v_pszFilePath,wchar_t * v_pszSign,int v_iBufSize, wchar_t* szSerialNumber, int nSerialSize);

// 由进程id获取进程所在路径
wstring GetProcessPathByPid(const DWORD dwProcessId);

// 检测网卡是否连接
bool IsConnectTheAdapt(const int& nIndex);