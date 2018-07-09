#ifndef __UIWEBBROWSER_H__
#define __UIWEBBROWSER_H__

#pragma once

#include "Utils/WebBrowserEventHandler.h"
#include <ExDisp.h>

namespace DuiLib
{
	//js调用c++时,c++的函数类型声明
	/*
		pDispParams:js传递过来的参数数组,cArgs是参数的数量,rgvarg是参数数组的指针。注意,js里面的参数顺序和c++这个参数数组里面的顺序刚好是相反的
		pVarResult:作为Js函数的返回值.要注意判断指针是否为空.另外,如果要返回给js字符串,那么要使用SysAllocString,并且不用SysFreeString
		pExcepInfo:返回给js的异常信息,一般不用
		puArgErr:错误索引,一般不用
	*/
	class CWebBrowserUI;

	typedef void (*PJSCall)(CWebBrowserUI* pBrowser, DISPPARAMS *pDispParams,/* [out] */ VARIANT *pVarResult,/* [out] */ EXCEPINFO *pExcepInfo,/* [out] */ UINT *puArgErr);
	typedef struct tagJSCallInfo
	{
		tagJSCallInfo()
		{
			lDspId=-1;
			strFuncName.Empty();
			pFunc = NULL;
		};
		tagJSCallInfo& operator=(const tagJSCallInfo& value)
		{
			lDspId = value.lDspId;
			strFuncName = value.strFuncName;
			pFunc = value.pFunc;
			return *this;
		};
		DISPID lDspId;
		CDuiString strFuncName;
		PJSCall pFunc;
	}JSCallInfo;

	class DUILIB_API CWebBrowserUI
		: public CActiveXUI
		, public IDocHostUIHandler
		, public IServiceProvider
		, public IOleCommandTarget
		, public IDispatch
		, public ITranslateAccelerator
	{
	public:
		/// 构造函数
		CWebBrowserUI();
		virtual ~CWebBrowserUI();

		void SetHomePage(LPCTSTR lpszUrl);
		LPCTSTR GetHomePage();

		void SetAutoNavigation(bool bAuto = TRUE);
		bool IsAutoNavigation();

		void SetWebBrowserEventHandler(CWebBrowserEventHandler* pEventHandler);
		void Navigate2(LPCTSTR lpszUrl);
		void Refresh();
		void Refresh2(int Level);
		void GoBack();
		void GoForward();
		void NavigateHomePage();
		void NavigateUrl(LPCTSTR lpszUrl);
		virtual bool DoCreateControl();
		IWebBrowser2* GetWebBrowser2(void);
		IDispatch*		   GetHtmlWindow();
		static DISPID FindId(IDispatch *pObj, LPOLESTR pName);
		//这个函数用于c++调用js
		/*
			pObj:一般来讲,传递GetHtmlWindow()的返回值即可,调用的是全局js函数,详细了解请自行搜索
			pMehtod:wchar_t*的指针,js函数名.
			pVarResult:用来接收js函数执行结果.
			ps:参数数组指针.这个参数顺序与js接收到的参数顺序是相反的,另外如果要传递字符串,请使用SysAllocString,在调用js函数结束后,要对应使用SysFreeString。
			cArgs:参数数组的大小
		*/
		static HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pMehtod, VARIANT *pVarResult, VARIANT *ps, int cArgs);
		static HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
		static HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
		//js调用c++, 调用方法为window.external.strJSFuncName() //strJSFuncName-js中调用的函数名 pCallback-函数名对应的函数指针
		void BindJSWindowExternalFunc(const CDuiString& strJSFuncName,PJSCall pCallback);

	protected:
		CDuiPtrArray m_aJsCallInfo;			//js调用c++时,函数信息
		IWebBrowser2*			m_pWebBrowser2; //浏览器指针
		IHTMLWindow2*		_pHtmlWnd2;
		LONG m_dwRef;
		DWORD m_dwCookie;
		virtual void ReleaseControl();
		HRESULT RegisterEventHandler(BOOL inAdvise);
		
		CDuiString m_sHomePage;	// 默认页面
		bool m_bAutoNavi;	// 是否启动时打开默认页面
		CWebBrowserEventHandler* m_pWebBrowserEventHandler;	//浏览器事件处理

		// DWebBrowserEvents2
		void BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel );
		void NavigateError(IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel);
		void NavigateComplete2(IDispatch *pDisp,VARIANT *&url);
		void DocumentComplete(IDispatch *pDisp,VARIANT *&url); 
		void ProgressChange(LONG nProgress, LONG nProgressMax);
		void NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
		void CommandStateChange(long Command,VARIANT_BOOL Enable);
		void WindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL *Cancel);
		
	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface( LPCTSTR pstrName );
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		// IUnknown
		STDMETHOD_(ULONG,AddRef)();
		STDMETHOD_(ULONG,Release)();
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);

		// IDispatch
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( __RPC__out UINT *pctinfo );
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo );
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);
		virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr );

		// IDocHostUIHandler
		STDMETHOD(ShowContextMenu)(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
		STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
		STDMETHOD(HideUI)();
		STDMETHOD(UpdateUI)();
		STDMETHOD(EnableModeless)(BOOL fEnable);
		STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
		STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
		STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow);
		STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);	//浏览器消息过滤
		STDMETHOD(GetOptionKeyPath)(LPOLESTR* pchKey, DWORD dwReserved);
		STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
		STDMETHOD(GetExternal)(IDispatch** ppDispatch);
		STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut);
		STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet);

		// IServiceProvider
		STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void** ppvObject);

		// IOleCommandTarget
		virtual HRESULT STDMETHODCALLTYPE QueryStatus( __RPC__in_opt const GUID *pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds ) OLECMD prgCmds[ ], __RPC__inout_opt OLECMDTEXT *pCmdText);
		virtual HRESULT STDMETHODCALLTYPE Exec( __RPC__in_opt const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT *pvaIn, __RPC__inout_opt VARIANT *pvaOut );

		// IDownloadManager
		STDMETHOD(Download)( 
			/* [in] */ IMoniker *pmk,
			/* [in] */ IBindCtx *pbc,
			/* [in] */ DWORD dwBindVerb,
			/* [in] */ LONG grfBINDF,
			/* [in] */ BINDINFO *pBindInfo,
			/* [in] */ LPCOLESTR pszHeaders,
			/* [in] */ LPCOLESTR pszRedir,
			/* [in] */ UINT uiCP);

		// ITranslateAccelerator
		// Duilib消息分发给WebBrowser
		virtual LRESULT TranslateAccelerator( MSG *pMsg );
	};
} // namespace DuiLib
#endif // __UIWEBBROWSER_H__