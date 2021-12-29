#pragma once


#define _CRT_SECURE_NO_DEPRECATE
#include <WinSock2.h>
#include <windows.h>

#include "../../include/UIlib.h"
using namespace DuiLib;
#pragma comment(lib, "../../lib/Duilib_u.lib")

#include "../../Util/StringTools.h"
#include "../../Util/ToolsFunc.h"

//JSON
#include "../json/include/json/json.h"
#pragma comment(lib,"../json/lib/jsoncpp.lib")

extern wstring g_strAppPath;
//∂‡”Ô—‘¥¶¿Ì
void LoadMultiLanguageString(const wstring strLanguageJsonFile);
