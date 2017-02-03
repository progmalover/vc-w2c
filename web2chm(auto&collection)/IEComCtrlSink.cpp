// IEComCtrlSink.cpp : implementation file
//

#include "stdafx.h"
//#include "IERefreshSampleDlg.h"
#include "IEComCtrlSink.h"
#include <Mshtmdid.h>
#include <Afxctl.h>
#include "shlwapi.h"
#include "mfcievw.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEComCtrlSink

IMPLEMENT_DYNCREATE(CIEComCtrlSink, CCmdTarget)
 
CIEComCtrlSink::CIEComCtrlSink()
{
	// enable OLE
	EnableAutomation();
	m_ipDocument = NULL;
	m_pParentWnd = NULL;
	m_pUnkSink = NULL; 
	m_dwCookie = 0;
	m_dwRef = 0;
}

CIEComCtrlSink::~CIEComCtrlSink()
{
	HRESULT hr = 0;
	// kill event sink
	//UnAdviseSink();
	//m_pUnkSink->Release();
}

// using MFC CCmdTarget to catch DWebBrowserEvents2 events
BEGIN_MESSAGE_MAP(CIEComCtrlSink, CCmdTarget)
	//{{AFX_MSG_MAP(CIEComCtrlSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// these are the events we want to monitor. 
// The events are explained below above their function implementations.
BEGIN_DISPATCH_MAP(CIEComCtrlSink, CCmdTarget)
	DISP_FUNCTION_ID(CIEComCtrlSink,"OnHtmlDocumentMouseMove",DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEMOVE,
	OnHtmlDocumentMouseMove,VT_EMPTY,VTS_DISPATCH)
	DISP_FUNCTION_ID(CIEComCtrlSink,"OnHtmlDocumentContextMenu",DISPID_EVMETH_ONCONTEXTMENU,
	OnHtmlDocumentContextMenu,VT_EMPTY,VTS_DISPATCH)

	DISP_FUNCTION_ID(CIEComCtrlSink,"OnHtmlDocumentMouseOut",DISPID_HTMLDOCUMENTEVENTS_ONMOUSEOUT,
	OnHtmlDocumentMouseOut,VT_EMPTY,VTS_DISPATCH)
	 
END_DISPATCH_MAP()


void   CIEComCtrlSink::OnHtmlDocumentMouseOut(IDispatch *pDisp)
{
	if(m_pParentWnd)
		m_pParentWnd->OnMouseOut();
	
}

void  CIEComCtrlSink::OnHtmlDocumentContextMenu(IDispatch *pDisp)
{
	CComQIPtr<IHTMLEventObj> evObj(pDisp);
	POINT  pt;
	
	//evObj->get_x(&pt.x);
	//evObj->get_y(&pt.y); //为了确定坐标正确。
	::GetCursorPos(&pt);
	evObj->put_returnValue(CComVariant(S_OK));
	if(m_pParentWnd)
	{
		//m_pParentWnd->ScreenToClient(&pt);
		m_pParentWnd->OnShowContextMenu((LPPOINT)&pt,NULL,pDisp);
	}
}
void  CIEComCtrlSink::OnHtmlDocumentMouseMove(IDispatch *pDisp)
{
	CComQIPtr<IHTMLEventObj> evObj(pDisp);
	if(m_pParentWnd)
		 m_pParentWnd->OnHtmlDocumentMouseMove( evObj);
	
}
// start capture of DWebBrowserEvents2 events
BOOL CIEComCtrlSink::MyAdviseSink(IHTMLDocument2 *ipWebDoc,CMfcieView *pParent)
{

	
	ASSERT(pParent != NULL);
 
	m_pParentWnd = pParent;
	if(ipWebDoc == NULL) return FALSE ;
	if(ipWebDoc == m_ipDocument)
		return FALSE;
	TRACE("***MyAdviseSink():::\n");
	

	if(m_ipDocument && m_dwCookie)
	{
		UnAdviseSink();
		m_ipDocument = NULL;
		m_dwCookie = 0;
	}
	m_ipDocument = ipWebDoc;
	m_pUnkSink = GetIDispatch(FALSE);
	BOOL bRet = AfxConnectionAdvise((LPUNKNOWN)m_ipDocument,DIID_HTMLDocumentEvents2,m_pUnkSink,FALSE,&m_dwCookie); 
	if(bRet)
		m_dwRef++;
	return bRet;
}

// stop capture of DWebBrowserEvents2 events
BOOL CIEComCtrlSink::UnAdviseSink()
{
	BOOL bOK = TRUE;
	// kill event sink if cookie is not 0
	if(m_dwCookie != 0 && m_dwRef > 0)
	{
		bOK = AfxConnectionUnadvise((LPUNKNOWN)m_ipDocument, DIID_HTMLDocumentEvents2, m_pUnkSink, FALSE, m_dwCookie);
		m_dwCookie = 0;
		m_dwRef--;
	}
	return bOK;
}

// browser is quiting so kill events
void CIEComCtrlSink::OnQuit()
{
	//if(m_dwCookie != 0)

	UnAdviseSink();	

}
