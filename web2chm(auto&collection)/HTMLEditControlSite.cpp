/*
 *	$Header: $
 *
 *	$History: $
 */
#include "stdafx.h"
#include "AfxOcc.h"
#include "HTMLEditControlSite.h"
#include "resource.h"
DEFINE_GUID(SID_SHTMLEditHost, 0x3050f6a0, 0x98b5, 0x11cf, 0xbb, 0x82, 0x00, 0xaa, 0x00, 0xbd, 0xce, 0x0b);

//////////////////////////////////////////////////////////////////////
//	Implementation of the CHTMLControlSite class, including the
//	IDocHostUIHandler implementation
BEGIN_INTERFACE_MAP(CHTMLControlSite, COleControlSite)
	INTERFACE_PART(CHTMLControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CHTMLControlSite, IID_IServiceProvider, ServiceProvider)
	INTERFACE_PART(CHTMLControlSite, IID_IHTMLEditHost, HTMLEditHost)
	INTERFACE_PART(CHTMLControlSite, IID_IHTMLPainter, HTMLPainter)
	INTERFACE_PART(CHTMLControlSite, IID_IElementBehavior, HTMLElementBehavior)
	INTERFACE_PART(CHTMLControlSite, IID_IElementBehaviorFactory, HTMLElementBehaviorFactory)
END_INTERFACE_MAP()
BOOL CHTMLControlSite::m_bLocked;
CHTMLControlSite::CHTMLControlSite(COleControlContainer* pContainer) : COleControlSite(pContainer)
{
 	m_gridCookie = NULL;
	m_pCurHTMLDoc = NULL;
	m_xHTMLElementBehavior.m_spPaintSite = NULL;
	m_bLocked = FALSE;
}

CHTMLControlSite::~CHTMLControlSite(void)
{
		//RemoveBehavior();
}

inline CHtmlView* CHTMLControlSite::GetView() const
{
	return STATIC_DOWNCAST(CHtmlView, m_pCtrlCont->m_pWnd);
}

IHTMLElement*   CHTMLControlSite::GetElement()
{
	return m_xHTMLPainter.m_spElement;
}
 void CHTMLControlSite::SetElement(IHTMLElement* pElement, const CRect & rc)
{

	/*if(m_bLocked)
		return;*/

	m_xHTMLPainter.m_rcElement = rc;
	m_xHTMLPainter.m_spElement = pElement;
	if (m_xHTMLElementBehavior.m_spPaintSite != (IHTMLPaintSite *) NULL)
		m_xHTMLElementBehavior.m_spPaintSite->InvalidateRect(NULL);
	
	//m_spPaintSite->InvalidateRect(NULL);
}
void CHTMLControlSite::RemoveBehavior()
{
	HRESULT hr;
	IHTMLElement  *pBody = NULL;
	IHTMLElement2 *pBody2;
    VARIANT		  vFactory;

	m_xHTMLPainter.m_rcElement.SetRectEmpty();
	if(NULL == m_pCurHTMLDoc)
		return;
	if(0 == m_gridCookie)
		return;

	// Get IHTMLElement and IHTMLElement2 interfaces for the body
	hr = m_pCurHTMLDoc->get_body(&pBody);

	if (pBody == (IHTMLElement *) NULL)
		return;

	hr = pBody->QueryInterface(IID_IHTMLElement2, (void **) &pBody2);

	if (pBody2 == (IHTMLElement2 *) NULL)
	{
		pBody->Release();
		return;
	}

	VARIANT_BOOL dummy;
	hr = pBody2->removeBehavior(m_gridCookie, &dummy);
	m_xHTMLElementBehaviorFactory.Release();
	m_gridCookie = NULL;
	
    m_pCurHTMLDoc = NULL;

	hr = pBody->Release();
	hr = pBody2->Release();
	return;

}

void CHTMLControlSite::AddBehavior(IHTMLDocument2 *pDoc)
{
	
	HRESULT hr;

	CComPtr<IHTMLElement>pBody  ;
	CComPtr<IHTMLElement2>pBody2;
    VARIANT		  vFactory;
    m_pCurHTMLDoc = NULL;
	if (pDoc == (IHTMLDocument2 *) NULL)
		return;

	// Get IHTMLElement and IHTMLElement2 interfaces for the body
	hr = pDoc->get_body(&pBody);

	if (pBody == (IHTMLElement *) NULL)
		return;

	//hr = pBody->QueryInterface(IID_IHTMLElement2, (void **) &pBody2);
	pBody2 = pBody;

	if (pBody2 == (IHTMLElement2 *) NULL)
	{
		//pBody->Release();
		return;
	}

	if (m_gridCookie)
	{
		VARIANT_BOOL dummy;
		hr = pBody2->removeBehavior(m_gridCookie, &dummy);

		//	MSHTML does an AddRef when you add a behaviour
		//	but it doesn't do a Release when you remove it.
		m_xHTMLElementBehaviorFactory.Release();
		m_gridCookie = NULL;
	}

	// Convert the grid factory pointer to the proper VARIANT data type 
	// for IHTMLElement2::AddBehavior
	V_VT(&vFactory) = VT_UNKNOWN;
    V_UNKNOWN(&vFactory) = &m_xHTMLElementBehaviorFactory;

	// Add Grid behavior
	hr = pBody2->addBehavior(NULL, &vFactory, &m_gridCookie);

    m_pCurHTMLDoc = pDoc;
	// Release resources
	//hr = pBody->Release();
	//hr = pBody2->Release();
}


//	DocHostUIHandler methods.  These all delegate through the owner view
//	except for the standard IUnknown methods which delegate to the
//	CCmdTarget base class.
STDMETHODIMP_(ULONG) CHTMLControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler);
	
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XDocHostUIHandler::Release()
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	return pThis->ExternalRelease();
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::GetExternal(LPDISPATCH *lppDispatch)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnGetExternal(lppDispatch);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::ShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	CHtmlView* pView = pThis->GetView();
	return pView->OnGetHostInfo(pInfo);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::ShowUI(DWORD dwID, LPOLEINPLACEACTIVEOBJECT pActiveObject, LPOLECOMMANDTARGET pCommandTarget, LPOLEINPLACEFRAME pFrame, LPOLEINPLACEUIWINDOW pDoc)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	return pView->OnShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	
	return pView->OnHideUI();
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();
	
	return pView->OnUpdateUI();
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnEnableModeless(fEnable);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnDocWindowActivate(fActivate);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnFrameWindowActivate(fActivate);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::ResizeBorder(LPCRECT prcBorder, LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnResizeBorder(prcBorder, pUIWindow, fFrameWindow);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnTranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)

	CHtmlView* pView = pThis->GetView();

	return pView->OnGetOptionKeyPath(pchKey, dwReserved);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::GetDropTarget(LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnGetDropTarget(pDropTarget, ppDropTarget);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnTranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

STDMETHODIMP CHTMLControlSite::XDocHostUIHandler::FilterDataObject(LPDATAOBJECT pDataObject, LPDATAOBJECT* ppDataObject)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, DocHostUIHandler)
	
	CHtmlView* pView = pThis->GetView();
	
	return pView->OnFilterDataObject(pDataObject, ppDataObject);
}

//	The service provider implementation. This is called by MSHTML to get
//	a pointer to an instance of IHTMLEditHost
STDMETHODIMP_(ULONG) CHTMLControlSite::XServiceProvider::AddRef()
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, ServiceProvider);

	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XServiceProvider::Release()
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, ServiceProvider)

	return pThis->ExternalRelease();
}

STDMETHODIMP CHTMLControlSite::XServiceProvider::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, ServiceProvider)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CHTMLControlSite::XServiceProvider::QueryService(REFGUID guidService, REFIID riid, void **ppObj)
{
	METHOD_PROLOGUE_EX_(CHTMLControlSite, ServiceProvider)

	HRESULT hr = E_NOINTERFACE;

	*ppObj = NULL;

    if (guidService == SID_SHTMLEditHost && riid == IID_IHTMLEditHost)
    {
		*ppObj = (void **) &pThis->m_xHTMLEditHost;
		hr = S_OK;
	}

	return hr;
}

//	The edit host implementation, which is where the snap-to-grid is done
CHTMLControlSite::XHTMLEditHost::XHTMLEditHost()
{
	m_iSnap = 8;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLEditHost::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLEditHost::Release()
{
	return 1;
}

STDMETHODIMP CHTMLControlSite::XHTMLEditHost::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLEditHost);

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CHTMLControlSite::XHTMLEditHost::SnapRect(IHTMLElement * /*pIElement*/, RECT * prcNew, ELEMENT_CORNER eHandle)
{
	if (GetAsyncKeyState(VK_CONTROL) & 0x10000000)
		//	If the control key is down return (no snap).
		return S_OK;

	LONG lWidth = prcNew->right - prcNew->left;
	LONG lHeight = prcNew->bottom - prcNew->top;

	switch (eHandle)
	{
	case ELEMENT_CORNER_NONE:
		prcNew->top = ((prcNew->top + (m_iSnap / 2)) / m_iSnap) * m_iSnap - 2;
		prcNew->left = ((prcNew->left + (m_iSnap / 2)) / m_iSnap) * m_iSnap - 2;
		prcNew->bottom = prcNew->top + lHeight;
		prcNew->right = prcNew->left + lWidth;
		break;

	case ELEMENT_CORNER_TOP:
		prcNew->top = ((prcNew->top + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_LEFT:
		prcNew->left = ((prcNew->left + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_BOTTOM:
		prcNew->bottom = ((prcNew->bottom + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_RIGHT:
		prcNew->right = ((prcNew->right + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_TOPLEFT:
		prcNew->top = ((prcNew->top + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		prcNew->left = ((prcNew->left + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_TOPRIGHT:
		prcNew->top = ((prcNew->top + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		prcNew->right = ((prcNew->right + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_BOTTOMLEFT:
		prcNew->bottom = ((prcNew->bottom + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		prcNew->left = ((prcNew->left + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;

	case ELEMENT_CORNER_BOTTOMRIGHT:
		prcNew->bottom = ((prcNew->bottom + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		prcNew->right = ((prcNew->right + (m_iSnap / 2)) / m_iSnap) * m_iSnap;
		break;
	}

	return S_OK;
}

//	The behavior implementation. This is queried by MSHTML to get an instance 
//	of an IHTMLPainter. Our painter draws the grid.
CHTMLControlSite::XHTMLElementBehavior::XHTMLElementBehavior()
{
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLElementBehavior::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLElementBehavior::Release()
{
	return 1;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehavior::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLElementBehavior);

	if (IsEqualIID(iid, IID_IUnknown))
	{
		*ppvObj = (IUnknown *) pThis;
		return S_OK;
	}
	else if (IsEqualIID(iid, IID_IHTMLPainter))
	{
		*ppvObj = (IUnknown *) &pThis->m_xHTMLPainter;
		return S_OK;
	}

	*ppvObj = NULL;
	return E_NOTIMPL;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehavior::Init(IElementBehaviorSite *pBehaviorSite)
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLElementBehavior);
	m_spPaintSite = NULL;
	HRESULT	hr = pBehaviorSite->QueryInterface(IID_IHTMLPaintSite, (void **) &m_spPaintSite);
	
	pBehaviorSite->GetElement(&pThis->m_xHTMLPainter.m_spElement);
	//

	//hr = m_spSite->QueryInterface(IID_IHTMLPaintSite, (void**)&m_spPaintSite);
	if (m_spPaintSite != (IHTMLPaintSite *) NULL)
		m_spPaintSite->InvalidateRect(NULL);

	return hr;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehavior::Notify(LONG /*lEvent*/, VARIANT *pVar)
{
	if (pVar == NULL)
		return E_POINTER;

	return E_NOTIMPL;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehavior::Detach()
{
	m_spPaintSite->InvalidateRect(NULL);
	return S_OK;
}

//	The behavior factory
STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLElementBehaviorFactory::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLElementBehaviorFactory::Release()
{
	return 1;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehaviorFactory::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLElementBehaviorFactory);

	*ppvObj = NULL;

	if (IsEqualIID(iid, IID_IElementBehaviorFactory))
	{
		*ppvObj = (IUnknown *) this;
		return S_OK;
	}
	else if (IsEqualIID(iid, IID_IElementBehavior))
	{
		*ppvObj = (IUnknown *) &pThis->m_xHTMLElementBehavior;
		return S_OK;
	}

	return E_NOTIMPL;
}

STDMETHODIMP CHTMLControlSite::XHTMLElementBehaviorFactory::FindBehavior(BSTR /*bstrBehavior*/, BSTR /*bstrBehaviorUrl*/, IElementBehaviorSite * /*pSite*/, IElementBehavior** ppBehavior)
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLElementBehaviorFactory)

	*ppBehavior = &pThis->m_xHTMLElementBehavior;
	return S_OK;
}

//	The painter part of the class
CHTMLControlSite::XHTMLPainter::XHTMLPainter()
{
	 m_spElement  = NULL;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLPainter::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG) CHTMLControlSite::XHTMLPainter::Release()
{
	return 1;
}

STDMETHODIMP CHTMLControlSite::XHTMLPainter::QueryInterface(REFIID iid, LPVOID far* ppvObj)     
{
	METHOD_PROLOGUE(CHTMLControlSite, HTMLPainter);

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

void BltMaskBlockTo(HDC hDesk,CRect rc,COLORREF clr,int trans,LPCTSTR strPrompt)
{
	CDC		memDC;
	CBitmap memBmp;
	CBrush  fillBrush;
	CDC *pCDC = CDC::FromHandle(hDesk);
	fillBrush.CreateSolidBrush(clr);
	memBmp.CreateCompatibleBitmap(pCDC,rc.Width(),rc.Height());
	memDC.CreateCompatibleDC(pCDC);
	
	memDC.SelectObject(memBmp);
	memDC.FillRect(CRect(0,0,rc.Width(),rc.Height()),&fillBrush);

	if(strPrompt)
	{
		CSize szTxt = memDC.GetTextExtent(strPrompt);
		if(szTxt.cx > rc.Width() || szTxt.cy > rc.Height());
		else
		{
			CRect  drawRect;
			drawRect.left = (rc.Width() - szTxt.cx)/2;
			drawRect.top = (rc.Height() - szTxt.cy)/2;
			drawRect.right = drawRect.left + szTxt.cx;
			drawRect.bottom = drawRect.top + szTxt.cy;
			memDC.SelectObject(&afxGlobalData.fontBold);
			memDC.SetTextColor(RGB(0,0,255));
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(strPrompt,drawRect,DT_SINGLELINE );
		}

	}

	BLENDFUNCTION bnc={0,0,trans};
	::AlphaBlend(hDesk,rc.left,rc.top,rc.Width(),rc.Height(),memDC.GetSafeHdc(),0,0,rc.Width(),rc.Height(),bnc);

}

void BltDotTo(HDC hDesk,HDC hSrc,POINT ptStart ,POINT ptEnd)
{
	int stage = (ptEnd.x - ptStart.x)/2;
	int t =  (ptEnd.x - ptStart.x) / 2;
	
	::BitBlt(hDesk,ptStart.x,ptEnd.y ,5,5,hSrc,0,0,SRCCOPY); 
	::BitBlt(hDesk,ptStart.x + stage,ptEnd.y ,5,5,hSrc,0,0,SRCCOPY);
	::BitBlt(hDesk,ptEnd.x - 2,ptEnd.y ,5,5,hSrc,0,0,SRCCOPY);
	
}

STDMETHODIMP CHTMLControlSite::XHTMLPainter::Draw(RECT rcBounds, RECT /*rcUpdate*/, LONG /*lDrawFlags*/, HDC hdc, LPVOID /*pvDrawObject*/)
{

		if(m_rcElement.IsRectEmpty())
		return S_OK;
		if(this->m_spElement == NULL)
		return S_OK;
		if( CHTMLControlSite::m_bLocked)
		return S_OK;

		CPen redPen;
		CBitmap dotMap;
		CDC memDC;

		dotMap.LoadBitmap(IDB_BITMAP_DOT);
		redPen.CreatePen(PS_DOT, 1, RGB(0xff, 0, 0));
		::SaveDC(hdc);
		memDC.Attach(::CreateCompatibleDC(hdc));
		memDC.SelectObject(dotMap.m_hObject);

		BITMAP bmp;
		dotMap.GetObject(sizeof(BITMAP),&bmp);
		//::SelectObject(hdc,dotMap.m_hObject);
		//SetROP2(hdc,R2_XORPEN      );
		SelectObject(hdc, redPen);

	
		CRect rc1,rc2;
		rc1 = rcBounds;
		rc2 = m_rcElement;
		rc1.DeflateRect(2,2);
		rc2.DeflateRect(2,2);

		CRect realRect(rc1.left + rc2.left, rc1.top + rc2.top,rc1.left + rc2.right,rc1.top + rc2.bottom);
		realRect.DeflateRect(1,1);
		BltMaskBlockTo(hdc,realRect,afxGlobalData.clrBarDkShadow,120,_T("Right click to catch source."));

		MoveToEx(hdc, rc1.left + rc2.left, rc1.top + rc2.top , NULL);
		
		LineTo(hdc, rc1.left + rc2.right , rc1.top + rc2.top);
		LineTo(hdc, rc1.left + rc2.right , rc1.top + rc2.bottom);
		LineTo(hdc, rc1.left + rc2.left , rc1.top + rc2.bottom);
		LineTo(hdc, rc1.left + rc2.left , rc1.top + rc2.top);
#if 0
		POINT ptStart,ptEnd;	
		
		rc1.InflateRect(1,1);
		rc2.InflateRect(1,1);

		ptStart.x =  rc1.left + rc2.left;
		ptStart.y =  rc1.top + rc2.top;
		ptEnd.x =	rc1.left + rc2.right ;
		ptEnd.y =	 rc1.top + rc2.top;
	//	BltDotTo(hdc,memDC.m_hDC,ptStart,ptEnd);

		rc2.OffsetRect(0,-2);
		//rc1.OffsetRect(0,-2);
		ptStart.x =  rc1.left + rc2.left;
		ptStart.y =  rc1.top + rc2.bottom ;
		ptEnd.x =	rc1.left + rc2.right ;
		ptEnd.y =	 rc1.top + rc2.bottom ;
	//	BltDotTo(hdc,memDC.m_hDC,ptStart,ptEnd);
		TRACE("***Draw Line Here!\n");
#endif
		 
		::RestoreDC(hdc,0);
		

	return S_OK;
}

STDMETHODIMP CHTMLControlSite::XHTMLPainter::OnResize(SIZE /*size*/)
{
	return S_OK;
}

STDMETHODIMP CHTMLControlSite::XHTMLPainter::GetPainterInfo(HTML_PAINTER_INFO *pInfo)
{
	if (pInfo == NULL)
		return E_POINTER;

	pInfo->lFlags = HTMLPAINTER_TRANSPARENT;
	pInfo->lZOrder = HTMLPAINT_ZORDER_WINDOW_TOP;

	memset(&pInfo->iidDrawObject, 0, sizeof(IID));

	pInfo->rcExpand.left = 0;
	pInfo->rcExpand.right = 0;
	pInfo->rcExpand.top = 0;
	pInfo->rcExpand.bottom = 0;

	return S_OK;
}

STDMETHODIMP CHTMLControlSite::XHTMLPainter::HitTestPoint(POINT /*pt*/, BOOL *pbHit, LONG *plPartID)
{
	if (pbHit == NULL)
		return E_POINTER;

	if (plPartID == NULL)
		return E_POINTER;

	return E_NOTIMPL;
}

void CHTMLControlSite::Lock(void)
{
	m_bLocked = TRUE;
}

void CHTMLControlSite::Unlock(void)
{
	m_bLocked = FALSE;
}
