// stdafx.cpp : source file that includes just the standard includes
//	App.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


void LoadMultiLanguageString(const wstring strLanguageJsonFile)
{
	string strLanguages = ReadAllFromFile(strLanguageJsonFile);
	Json::Value jsonValue1;
	Json::Reader read1;
	if (read1.parse(strLanguages, jsonValue1))
	{
		for (Json::ValueIterator itr=jsonValue1.begin();itr!=jsonValue1.end();itr++)
		{
			CPaintManagerUI::AddMultiLanguageString( (*itr)["id"].asInt(),Utf8ToUnicode( (*itr)["txt"].asString() ).c_str());
		}
	}	
}