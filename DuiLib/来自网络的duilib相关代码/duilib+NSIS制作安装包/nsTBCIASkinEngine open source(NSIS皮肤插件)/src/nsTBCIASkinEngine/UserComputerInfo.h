#pragma  once

//#include "tbproto.pb.h"
//#include "typedefine.h"
//using namespace TbMsgDefine;


class CUserComputerInfo
{
public:
	CUserComputerInfo(BOOL bInstall);
	~CUserComputerInfo();
	
	BOOL PostMac(LPCTSTR lpszURL);
	//BOOL  MakeComputerInfo(string& strFullMsg);

	//void  AddHardwateInfo(stMsgUserComputerInfoC2S& MsgUserComputerInfoC2S, stHardwareInfo_enHardwareType HardwareType);
public:
	BOOL m_bInstall;
};