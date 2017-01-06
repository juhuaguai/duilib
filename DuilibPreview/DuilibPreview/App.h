#pragma once

#include "resource.h"


int ShowHelpInfo()
{
	MessageBox(NULL, _T("使用时将界面文件(*.xml)拖到DuilibPreview.exe上即可预览，\r\n按\"Esc\"或者\"Alt+F4\"退出。\r\n\r\n看下readme好不好亲 ^_*"), _T("界面预览工具"), MB_OK);
	return 0;
}