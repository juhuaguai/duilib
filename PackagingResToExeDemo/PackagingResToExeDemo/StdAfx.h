#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>

#include "..\..\DuiLib\UIlib.h"

using namespace DuiLib;

#ifndef UILIB_STATIC

#ifdef _UNICODE
#pragma comment(lib, "..\\..\\Lib\\DuiLib_u.lib")
#endif

#else

#pragma comment(lib, "..\\..\\Lib\\Duilib_uStatic.lib")

#endif






//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include <string>
using namespace std;

#include "resource.h"
