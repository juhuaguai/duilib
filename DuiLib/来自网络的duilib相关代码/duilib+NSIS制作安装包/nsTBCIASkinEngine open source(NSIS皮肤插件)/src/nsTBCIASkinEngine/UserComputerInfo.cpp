
#include "..\SysInfo\SysInfo.h"
#include "UserComputerInfo.h"
#include "winhttpmodsession.h"

using namespace WinMod;

CUserComputerInfo::CUserComputerInfo(BOOL bInstall)
{
	m_bInstall = bInstall;
}

CUserComputerInfo::~CUserComputerInfo()
{

}

BOOL CUserComputerInfo::PostMac(LPCTSTR lpszURL)
{
	CHAR szBuf[1024] = {0};
	int  nType = 0;
	CEthernetMac EthernetMac;
	if(EthernetMac.GetFirstMac(szBuf, nType))
	{

	}

	CWinHttpSession http;
	http.Open();

	http.HttpRequestEx(lpszURL, 1000, WINMOD_HTTP__APPLICATION__X_OCTET_STREAM, szBuf);

	return TRUE;
}

//
//BOOL CUserComputerInfo::MakeComputerInfo(string& strFullMsg)
//{
//	BOOL bret = FALSE;
//
//	strFullMsg.clear();
//	stMsgUserComputerInfoC2S MsgUserComputerInfoC2S;
//	
//
//	try
//	{
//		MsgUserComputerInfoC2S.mutable_msgheader()->set_msgtype(stMsgHeader_enMsgType_USERCOMPUTERINFO);
//		MsgUserComputerInfoC2S.set_binstall(!!m_bInstall);
//
//		stEncryInfo encryInfo;
//		encryInfo.set_encrytype(stEncryInfo_enEncryType_NONE);
//		encryInfo.mutable_keyinfo()->set_strpublickey("");
//		
//
//		for (int i = stHardwareInfo_enHardwareType_enHardwareType_MIN; i <= stHardwareInfo_enHardwareType_enHardwareType_MAX; ++i)
//		{
//			AddHardwateInfo(MsgUserComputerInfoC2S, (stHardwareInfo_enHardwareType)i);
//		}
//
//		if (CNetWorkMgr::EncryptMsg(MsgUserComputerInfoC2S.SerializeAsString(), encryInfo, stMsgHeader_enMsgType_USERCOMPUTERINFO, strFullMsg))
//		{
//			bret = TRUE;
//		}
//	}
//	catch(...)
//	{
//		bret = FALSE;
//	}
//
//	return bret;
//}
//
//void  CUserComputerInfo::AddHardwateInfo(stMsgUserComputerInfoC2S& MsgUserComputerInfoC2S, stHardwareInfo_enHardwareType HardwareType)
//{
//	string strBody;
//	stHardwareInfo HardwareInfo;
//	HardwareInfo.set_hardwaretype(HardwareType);
//	CHAR szBuf[1024] = {0};
//	int nType = 0;
//
//	switch(HardwareType)
//	{
//	case stHardwareInfo_enHardwareType_HardwareType_Mac:
//		{
//			CEthernetMac EthernetMac;
//			
//			if(EthernetMac.GetFirstMac(szBuf, nType))
//			{
//				HardwareInfo.set_strtext(szBuf);
//			}
//		}
//		break;
//	case stHardwareInfo_enHardwareType_HardwareType_BIOSID:
//		{
//			CBiosInformation BiosInformation;
//			if(BiosInformation.GetBiosID(szBuf, sizeof(szBuf)))
//			{
//				HardwareInfo.set_strtext(szBuf);
//			}
//		}
//		break;
//	case stHardwareInfo_enHardwareType_HardwareType_BIOSUUID:
//		{
//			CBiosInformation BiosInformation;
//			if(BiosInformation.GetBiosUUID(szBuf, sizeof(szBuf)))
//			{
//				HardwareInfo.set_strtext(szBuf);
//			}
//		}
//		break;
//	default:
//		break;
//	}
//
//	if (HardwareInfo.IsInitialized())
//	{
//		MsgUserComputerInfoC2S.add_hardwareinfo()->CopyFrom(HardwareInfo);
//	}
//}