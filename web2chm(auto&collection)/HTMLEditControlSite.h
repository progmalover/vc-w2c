/*
 *	$Header: $
 *
 *	$History: $
 */
//	This is copied from MSHTML.IDL
#pragma once

#include <afxocc.h>

#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) EXTERN_C const GUID DECLSPEC_SELECTANY name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

class CHTMLControlSite : public COleControlSite
{
public:
					CHTMLControlSite(COleControlContainer* pParentWnd);
					~CHTMLControlSite(void);
	CHtmlView		*GetView() const;
	

protected:
	LONG			m_gridCookie;

// Implementation
	DECLARE_INTERFACE_MAP()

	//	This is the implementation of the IDocHostUIHandler interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
		STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT, LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
		STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
		STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
		STDMETHOD(GetExternal)(LPDISPATCH*);
		STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
		STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
	END_INTERFACE_PART(DocHostUIHandler)

	//	This is the implementation of the IServiceProvider interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(ServiceProvider, IServiceProvider)
		STDMETHOD(QueryService)(REFGUID, REFIID, void **);
	END_INTERFACE_PART(ServiceProvider)

	//	This is the implementation of the IHTMLEditHost interface
	//	MSHMTL gets this interface from us so we have to reference count it.
	BEGIN_INTERFACE_PART(HTMLEditHost, IHTMLEditHost)
		STDMETHOD(SnapRect)(IHTMLElement *pIElement, RECT *prcNew, ELEMENT_CORNER eHandle);

					XHTMLEditHost();

		int			m_iSnap;
	END_INTERFACE_PART(HTMLEditHost)

	//	This is the implementation of the IHTMLPainter interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLPainter, IHTMLPainter)
		STDMETHOD(Draw)(RECT rcBounds, RECT rcUpdate, LONG lDrawFlags, HDC hdc, LPVOID pvDrawObject);
		STDMETHOD(OnResize)(SIZE size);
		STDMETHOD(GetPainterInfo)(HTML_PAINTER_INFO * pInfo);
		STDMETHOD(HitTestPoint)(POINT pt, BOOL* pbHit, LONG * plPartID);
						XHTMLPainter();

	    CRect  m_rcElement;
	    IHTMLElement*		m_spElement;
	END_INTERFACE_PART(HTMLPainter)

	//	This is the implementation of the HTMLElementBehavior interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLElementBehavior, IElementBehavior)
		STDMETHOD(Init)(IElementBehaviorSite *pBehaviorSite);
		STDMETHOD(Notify)(LONG lEvent, VARIANT * pVar);
		STDMETHOD(Detach)();
		XHTMLElementBehavior();
		IHTMLPaintSite		 *m_spPaintSite;
	END_INTERFACE_PART(HTMLElementBehavior)

	//	This is the implementation of the HTMLElementBehaviorFactory interface
	//	We pass this interface to MSHMTL so we don't reference count it.
	//	(It ain't going away unless the entire class goes away)
	BEGIN_INTERFACE_PART(HTMLElementBehaviorFactory, IElementBehaviorFactory)
		STDMETHOD(FindBehavior)(BSTR bstrBehavior, BSTR bstrBehaviorUrl, IElementBehaviorSite* pSite, IElementBehavior** ppBehavior);
	END_INTERFACE_PART(HTMLElementBehaviorFactory)
public:
    void SetElement(IHTMLElement* pElement, const CRect & rc);
	IHTMLElement*   GetElement();
	void AddBehavior(IHTMLDocument2 *pDoc);
	void RemoveBehavior( );
private:
	IHTMLDocument2 *m_pCurHTMLDoc;
	static BOOL m_bLocked;
public:
	void Lock(void);
	void Unlock(void);
};
