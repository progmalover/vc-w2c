
// includes
#include <exdisp.h> //For IWebBrowser2* and others
#include <exdispid.h>

/////////////////////////////////////////////////////////////////////////////
// CIEComCtrlSink command target
class CMfcieView;
class CIEComCtrlSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CIEComCtrlSink)

	CIEComCtrlSink();           

public:
	virtual ~CIEComCtrlSink();
	void OnQuit();
protected:	
	CMfcieView  *m_pParentWnd;
	IHTMLDocument2 * m_ipDocument;
	LPUNKNOWN		m_pUnkSink;
	// member we use to keep track of the event sink to Advise and Unadvise it.
	DWORD m_dwCookie;	
	int   m_dwRef;
// methods
protected:
	// start capture of DWebBrowserEvents2 events
	BOOL MyAdviseSink(IHTMLDocument2 *ipWebDoc,CMfcieView *pParent);
	BOOL UnAdviseSink();
	void OnHtmlDocumentMouseMove(IDispatch *pDisp);
	void OnHtmlDocumentContextMenu(IDispatch *pDisp);
	void OnHtmlDocumentMouseOut(IDispatch *pDisp);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEComCtrlSink)
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIEComCtrlSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()

	friend class CMfcieView;
};



