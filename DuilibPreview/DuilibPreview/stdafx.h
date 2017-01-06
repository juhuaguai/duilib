// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

// Windows 头文件:
#include <windows.h>
#include <objbase.h>

// C 运行时头文件
#include <stdlib.h>
#include <string>


#include "..\\..\\Duilib\\UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG 
#   ifdef _UNICODE 
#       pragma comment(lib, "..\\..\\Lib\\Duilib_ud.lib") 
#   else 
#       pragma comment(lib, "..\\..\\Lib\\Duilib_d.lib") 
#   endif 
#else 
#   ifdef _UNICODE 
#       pragma comment(lib, "..\\..\\Lib\\Duilib_u.lib") 
#   else 
#       pragma comment(lib, "..\\..\\Lib\\Duilib.lib") 
#   endif 
#endif