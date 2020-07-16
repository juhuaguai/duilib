#pragma once

#include <Windows.h>
#include "wke.h"
#include <string>
#include <map>
using namespace std;

#include "../util/KCritSec.h"

#include "../duiLib/include/UIlib.h"
using namespace DuiLib;
#pragma comment(lib, "../duiLib/lib/Duilib_uStatic.lib")


#define DUI_CTR_WEBUI			(_T("WebUI"))

/////离屏渲染的几种处理方式,更多的请参考miniblink的api文档。本控件处理流程：
/////        wkeOnPaintUpdated回调通知有更新=》控件自身设置无效区域=》触发duilib的刷新=》刷新到该控件时,通过wkeGetViewDC获取webview的DC,自己贴到对应位置即可.
/////1.用填充好的像素Buffer来做离屏,需要自己转换为位图,贴到DC上。相关api：wkeOnPaintBitUpdated
/////2.将渲染好的DC直接传过来,自己直接bitblt到需要的位置即可。相关api：wkeOnPaintUpdated
/////3.使用wkePaint接口,然后自己转换为位图,自己贴。相关api：wkePaint
/////4.使用wkePaint2接口,然后自己转换为位图,自己贴。相关api：wkePaint2
/////5.使用wkeGetViewDC获取webview的DC,自己直接bitblt到需要的位置即可。相关api：wkeGetViewDC

class CWebUI;

//创建新的页面时的回调,比如说调用了 window.open 或者点击了 <a target="_blank" .../>
typedef wkeWebView (*PCreateViewCallback)(wkeWebView webView,CWebUI *pWeb,wkeNavigationType navType, const wchar_t* pszUrl,const wkeWindowFeatures* features);
//页面加载完成时的回调（成功，取消，失败）
typedef void (*PLoadFinishCallback)(CWebUI *pWeb,const wchar_t* pszUrl, wkeLoadingResult result,const wchar_t* pszFailedReason);
//页面导航回调
typedef bool (*PNavigationCallback)(wkeWebView webView, CWebUI* pWeb, wkeNavigationType navType, const wchar_t* pszUrl);
//urlLoad之前的回调:返回true表示不处理此网络请求（不会发送网络请求）。返回false表示依然会发送网络请求
typedef bool (*PLoadUrlBeginCallback)(wkeWebView webView, CWebUI* pWeb, const char* url, void *job);

//js回调c++的函数指针类型
typedef jsValue (WKE_CALL_TYPE *PJsCallback)(jsExecState es,void* param);

//js回调c++的函数指针类型。注意：这个专用于window.external.xxx()
typedef jsValue (WKE_CALL_TYPE *PJsExternalCallback)(jsExecState es, jsValue object, jsValue* args, int argCount);

class CWebUI :	public CControlUI
{
public:
	CWebUI();
	~CWebUI(void);

	virtual LPCTSTR	GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void DoInit();
	virtual void SetPos(RECT rc, bool bNeedInvalidate/* = true*/);
	virtual void DoEvent(DuiLib::TEventUI& event);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

	//初始化整个mb。此句必须在所有mb api前最先调用。并且所有mb api必须和调用wkeInit的线程为同个线程
	static void InitGlobal();
	static void UnInitGlobal();

	//全局的js来调用c++函数,必须在对象创建之前调用绑定
	//参数:pszJSFuncName-c++函数对应的在js中调用时的函数名 param-传入自定义数据,在触发时将带出这个指针
	static void JSBindFunction(const char* pszJSFuncName,PJsCallback pFunc,void* param, const int& nParamCount);

	//设置创建新的页面时的回调
	void SetCreateViewCallback(PCreateViewCallback pCallback);
	//设置加载完成的回调
	void SetLoadFinishCallback(PLoadFinishCallback pCallback);
	//设置导航回调
	void SetNavigationCallback(PNavigationCallback pCallback);
	//设置urlLoadBegin的回调
	void SetLoadUrlBeginCallback(PLoadUrlBeginCallback pCallback);
	//设置window.external.xxx的js绑定，注意不要有同名的js函数
	void SetExternalJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc);
	//设置window.xxx的js绑定，注意不要有同名的js函数
	void SetWindowJSBindFunc(const string& strJSFuncName,PJsExternalCallback pFunc);

	//提供一个SetURL的接口,供之前的代码使用
	void SetURL(const wstring& strUrl);
	//加载html代码
	void LoadHtml(const wstring& strHtml);
	//页面操作：后退、前进、停止、刷新
	void Back();
	void Forward();
	void Stop();
	void Reload();

	//设置请求header 要在PLoadUrlBeginCallback回调中调用才可以
	void SetDynamicHeader(void* jobPtr,wchar_t* pszKey,wchar_t* value);

	//提供一个RunJS的接口,供之前的代码使用 c++直接调用js代码
	jsValue RunJS(const wstring& strValue);

	inline wkeWebView GetWebView() {return m_WebView;};
private:
	//创建新的页面时的回调,比如说调用了 window.open 或者点击了 <a target="_blank" .../>
	static wkeWebView OnCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);
	//页面加载完成回调函数（成功，取消，失败）
	static void WKE_CALL_TYPE OnLoadFinish(wkeWebView webView,void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason);
	//跳转导航回调
	static bool OnNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navType, const wkeString urlStr);
	//webview需要刷新的回调
	static void OnPaintUpdatedCallback(wkeWebView webView, void* param, const HDC hdc, int x, int y, int cx, int cy);
	//LoadurlBegin的回调
	static bool OnLoadUrlBeginCallback(wkeWebView webView, void* param, const char* url, void *job);
	//js上下文创建时的回调
	static void WKE_CALL_TYPE OnCreateScriptContextCallback(wkeWebView webView, void* param, wkeWebFrameHandle frameId, void* context, int extensionGroup, int worldId);
	static void ReleaseExternalFunction(jsData* data);


private:
	static bool m_static_bInit_webui;

	wkeWebView m_WebView;		//本身就是个句柄,或者叫做指针
	
	PCreateViewCallback m_pCreateViewCallback;
	PLoadFinishCallback m_pLoadFinishCallback;
	PNavigationCallback m_pNavigationCallback;
	PLoadUrlBeginCallback m_pLoadUrlBeginCallback;

	CKCritSec m_lockExternalJs;
	map<string,PJsExternalCallback> m_mapExternalJs;
	CKCritSec m_lockWindowJs;
	map<string,PJsExternalCallback> m_mapWindowJs;
};
