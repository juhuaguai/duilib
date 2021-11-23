#pragma once
#include <Shlwapi.h>
#include <WinBase.h>
#include <string>
#include <deque>
#include <map>
//�±ߵ�˳��Ҫ��
#include <ws2ipdef.h>
#include <IPHlpApi.h>
//#include <netioapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��  
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

//���е���Ļ����
void CenterWindowToScreen(HWND hWnd);
//�Զ����д��ڡ�hParent-������,hWnd-Ҫ���еĴ���,��������С������ΪNULLʱ�����е���Ļ
void CenterWindowEx(HWND hParent,HWND hWnd);

//����:����ȡһ���ַ����е���������
std::string GetNumFromStrBuf(const char* pszStrBuf);

void PrintfLog(const TCHAR * format, ...);

//�ļ����ļ����Ƿ����
BOOL IsPathExist(const TCHAR* szPath);
//�ļ����ļ����Ƿ����
BOOL IsPathExistA(const string& strPath);
//�ļ��Ƿ����
bool IsDirOrFileExist(const TCHAR* szPath);
//�ļ��Ƿ����
bool IsDirOrFileExistA(const char* szPath);
//����һ��Ŀ¼,������Ҳ�᷵��true,���������Ҵ���ʧ��ʱ����false
BOOL CreateDirW(const wstring& strDir);
//����Ŀ¼ //�����Ŀ¼��Ҫ��"\\"����"/"��β
int CopyFolder(const xstring& strSource,const xstring& strDest);
int CopyFolderA(const string& strSource,const string& strDest);
//ɾ���ļ��м���������ļ����ɹ�����0
int DeleteFolder(const xstring& strDest);
// �ݹ��ѯ�ļ�
// ����ֵtrue��ʾ�ɹ���false��ʾʧ��
bool RecursionSearchFile(const wchar_t* pszSource,const wchar_t* pszDestFileName,wchar_t* pszOutMsg,int nOutMaxLen);
// �ļ����Ƿ�Ϊ��
bool IsEmptyFolder(const wchar_t* pszSource, const wchar_t* pszDestFileName, wchar_t* pszOutMsg, int nOutMaxLen);
// ��ѯ·���µ��ļ���
deque<wstring> RecursionSearchFloder(const wchar_t* pszSource,const wchar_t* pszDestFileName,wchar_t* pszOutMsg,int nOutMaxLen);
// ��ѯ·���µ�ָ����׺�ļ�
deque<wstring> RecursionSearchNameSuffix(const wchar_t* pszSource,const wchar_t* pszNameSuffix,wchar_t* pszOutMsg,int nOutMaxLen);

//��ȡע���
bool ReadRegString(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,xstring& strValue);
//дע���
bool WriteRegValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
bool WriteRegValueA(HKEY hKey,const string& strSubKey,const string& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
bool WriteRegValueNo6432(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName,const DWORD& dwType ,const BYTE* lpData,DWORD cbData);
//ɾ��ע����ֵ
bool DeleteRegKeyValue(HKEY hKey,const xstring& strSubKey,const xstring& strKeyName);
bool DeleteRegKeyValueA(HKEY hKey,const string& strSubKey,const string& strKeyName);
//ɾ��ע�������Լ��������������
bool DeleteRegSubKey(HKEY hKey,const xstring& strSubKey);

//��ȡApp����Ŀ¼(��β��'/'����'\\')
xstring GetAppPath(HMODULE hModul=NULL);
xstring GetExeName(HMODULE hModul=NULL);

//���˿��Ƿ�ռ�� ����ֵ:-1-��ʾδ��ʹ��,���򷵻�ʹ�õĽ���PID
int CheckPortUsed(int nPort);

//��ȡ����IP
string GetLocalIp();
string getLocalIp();
//��ȡ���������������Ϣ
void GetIpAdapterInfoList(deque<IP_ADAPTER_INFO>& theIpAdapterList);
// ��ȡ����ʹ�õ�����
string GetInnerIp(wstring& strAlias,ULONG& uIndex, string& strGetway, string& strAdapterName);
//string GetInnerIp(ULONG& uIndex, string& strGetway);

//��ȡ����MAC
string GetMAC();

//��ȡ�ļ���׺ (����ֵͳһΪСд) ������.
xstring GetExtensionFromFileName(const xstring& strFileName);

//��ȡ��ǰ��Ĭ�������
xstring GetDefaultBrowser();

//��������ͼ��
void CreateDesktopIcon(const xstring& strIconName,const xstring& strExeFile,const xstring& strCmdLine,const xstring& strIconFile);
// �ɿ�ݷ�ʽ�ҵ�Դ�ļ�·��
bool GetShellPath(const char *Src,wchar_t *ShellPath);
// �ж��ǲ��ǿ�ݷ�ʽ
bool IsLineFile(const wstring wstr);

//��ȡexe�е�Icon������Ϊ�ļ� (strDestFileһ����ico��β) (���������1����С��ͼ��,�����Ҫ���,�ο�ʵ�ִ�������޸ļ���)
bool SaveIconFileFromExeFile(const xstring& strExe,const xstring& strDestFile);
//��ȡexe�е�Icon������Ϊ�ļ� (strDestFileһ����ico��β) (���������1����С��ͼ��,�����Ҫ���,�ο�ʵ�ִ�������޸ļ���)
bool SaveIconToPngFromExeFile(const xstring& strExe, const xstring& strDestFile,int nInWidth,int nInHeight);

//�ɾ���·����ȡ�ļ���
xstring GetAppNameFromPath(const xstring& strAppPath);

//��·����ȡ�ļ���
wstring GetFileNameFromPath(const wstring& strPath);
//��·����ȡ�ļ���
string GetFileNameFromPathA(const string& strPath);

//�Ƿ�����Ч�����֤�� (����1������Ч,������Ϊ����)
int IsValidIdCardNumber(const xstring& strIdCardNumber);

//���뿪������		//strNameע����������еļ��� strFile-�������еļ�ֵ,��Ҫ����·��
bool AddFollowSystemStart(const xstring& strName,const xstring& strFile);
//ɾ����������		//strNameע����������еļ��� strFile-�������еļ�ֵ,��Ҫ����·��,����Ϊ��
bool DelFollowSystemStart(const xstring& strName,const xstring& strFile);

//��ȡguid�ַ��� bUpper-�Ƿ��д
xstring GetGUID(bool bUpper=true);

//���ļ��ж�ȡȫ������(utf8д���ȡ��������utf8,Ϊ�˼��ݷǼ������ĵĲ���ϵͳ,���ʹ��Unicode�汾����)
string ReadAllFromFile(const wstring& strFile);
//��Utf8�����ʽд���ļ�
int WriteUtf8File(const string& strUtf8,const wstring& strFile);
int WriteUtf8FileW(const wstring& strUnicode,const wstring& strFile);

//��ȡ��ǰ����ϵͳ����
xstring GetOSName();
//�Ƿ���64λOS
BOOL Is64BitOS();
//��ȡ����ϵͳ�汾
int GetOSVersion(int& nMajorVersion,int& nMinorVersion);
bool IsXp();
bool IsXpUp();
bool IsWin7();
bool IsWin7Up();
bool IsWin10();

//���ҽ���ID //�˺�����ȡ����ǰ���̵�PID������0
int GetProcesssIdFromName(const xstring& strPorcessName,bool bCaseSensitive = false);
void GetProcesssIdFromName(const xstring& strPorcessName,deque<int>& dequeOutID,bool bCaseSensitive = false);
//���ҽ���PID,����ȡ����·��(32λ����ֻ�ܲ���32λ����)
void GetProcesssInfoFromName(const xstring& strPorcessName,map<int,wstring>& mapOutID, DWORD& dwErrorCode, wstring& wstrErrMsg,bool bCaseSensitive = false);

//��ȡ�ļ���С(�ֽ�) //���2G
long GetFileSizeByte(const xstring& strFile);
long GetFileSizeByteA(const string& strFile);

//��ȡϵͳ��ǰʱ��
xstring GetTimeZoneNow();
//����ʱ��
//����Ϊ SetTimeZone(_T("Korea Standard Time"));
//Korea Standard Time (UTC+09:00)�׶�
//Tokyo Standard Time (UTC+09:00)���棬���ϣ�����
//China Standard Time ����ʱ��
bool SetTimeZone(const xstring& subKey);
	
#if 0	//�����⣬���Ų�
//��Ĭִ��������
bool DealExecCmd(const xstring& strCommandLine);
//��ӳ��򵽷���ǽ
bool AddFSystemFireWallRuler(const xstring& strfileName, const xstring& strName);
#endif

//����֤�� ����0��ʾ�ɹ�
int ImportCACertFile(const xstring& strFileName);
int ImportCACertString(const string& strTxt);

//��ȡ��ǰ���̵���������PID
long GetCurStartProcessPID();
//��ȡָ�����̵���������PID
long GetStartProcessPID(DWORD dwId);

/*���õ�ǰ����Ĭ��ʹ�õ�webbrowser�ں˰汾 win7ϵͳĬ��Ӧ����ie7�汾,8000��ʾie8
10001 (0x2711)	Internet Explorer 10����ҳ��IE 10�ı�׼ģʽչ�֣�ҳ��!DOCTYPE��Ч
10000 (0x02710)	Internet Explorer 10����IE 10��׼ģʽ�а�����ҳ��!DOCTYPEָ������ʾ��ҳ��Internet Explorer 10 Ĭ��ֵ��
9999 (0x270F)	Windows Internet Explorer 9. ǿ��IE9��ʾ������!DOCTYPEָ��
9000 (0x2328)	Internet Explorer 9. Internet Explorer 9Ĭ��ֵ����IE9��׼ģʽ�а�����ҳ��!DOCTYPEָ������ʾ��ҳ��
8888 (0x22B8)	Internet Explorer 8��ǿ��IE8��׼ģʽ��ʾ������!DOCTYPEָ��
8000 (0x1F40)	Internet Explorer 8Ĭ�����ã���IE8��׼ģʽ�а�����ҳ��!DOCTYPEָ��չʾ��ҳ
7000 (0x1B58)	ʹ��WebBrowser Control�ؼ���Ӧ�ó�����ʹ�õ�Ĭ��ֵ����IE7��׼ģʽ�а�����ҳ��!DOCTYPEָ����չʾ��ҳ��
*/
void SetIEWebbrowserVersion(DWORD dwIEVersion = 8000);

//CPU���к�
string GetCpuIndex();
//CPU����
wstring GetCpuDescr();
//CPU������
int GetCpuCoreNum();
// cpu�ͺ�
string GetCPUType();
// ��ȡϵͳ��Ϣ
wstring GetSysInfo();

//��ȡ�ڴ��С //sprintf(szMemorySize,"%I64d",dwMemorySize);
DWORDLONG GetMemorySize();

//��ȡ����GUID,��ע����ж�ȡ������,��װϵͳ���ܸı�
xstring GetMachineGUID();

//��ȡ�����UUID,���ܻ�ȡʧ��,��ʧ��ʱ,�ַ����н�����ȫ������F����ȫ������0
string GetBIOSUUID();

// �����ݷ�ʽͼ��
bool ChangeLinkIcon(const wstring &strLnkName, const wstring &strIconPath);

//�����ַ��������а�
bool CopyStringToClipboard(const wstring& strValue);

// ����ϵͳȨ��
// ����һ��ͨ�õ�����Ȩ�޺����������Ҫ��������Ȩ��  
// ����LookupPrivilegeValue�ĵڶ�������SE_SHUTDOWN_NAME������  
BOOL EnableShutDownPriv();
// ReSetWindows(EWX_LOGOFF, false);		// ע��
// ReSetWindows(EWX_REBOOT, true);		// ���� 
// ReSetWindows(EWX_SHUTDOWN, true);	// �ػ� 
BOOL ReSetWindows(DWORD dwFlags, BOOL bForce);
//���ã���ȡ���������ǩ��
//������
//		v_pwszFilePath  ---   �����ȫ·��
//		v_pwszSign      ---   ���ڷ�������ǩ���Ļ�����,���ΪNULL��
//					   	      ��ô������Ҫ�Ļ�������С
//      v_iBufSize      ---   v_pwszSign�������Ĵ�С
//����ֵ��
//		-1		---  ʧ��
//		0       ---  �ɹ�
//		����ֵ  ---  ��Ҫ�Ļ�������С
LONG GetSoftSign(const wchar_t* v_pszFilePath,wchar_t * v_pszSign,int v_iBufSize, wchar_t* szSerialNumber, int nSerialSize);

// �ɽ���id��ȡ��������·��
wstring GetProcessPathByPid(const DWORD dwProcessId);

// ��������Ƿ�����
bool IsConnectTheAdapt(const int& nIndex);