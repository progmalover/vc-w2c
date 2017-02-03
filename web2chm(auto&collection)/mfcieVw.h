// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// mfcieVw.h : interface of the CMfcieView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCIEVIEW_H__47FF4085_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_)
#define AFX_MFCIEVIEW_H__47FF4085_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "HTMLEditControlSite.h"
#include <afxocc.h>
#include "staticinstance.h"
class CIEComCtrlSink;
class CWorkModeDlg;
class CMfcieView : public CHtmlView,public StaticInst<CMfcieView>
{

	friend class CWorkModeDlg;
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CMfcieView)

// Attributes
public:

	CMfcieView();
	virtual ~CMfcieView();
// Operations
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcieView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
	void OnTitleChange(LPCTSTR lpszText);
	void OnDocumentComplete(LPCTSTR lpszUrl);
	void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
CHTMLControlSite *m_pEditSite;
protected:
	virtual void PostNcDestroy();
// Generated message map functions
protected:
	//{{AFX_MSG(CMfcieView)
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnGoStartPage();
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
	afx_msg void OnHelpWebTutorial();
	afx_msg void OnHelpOnlineSupport();
	afx_msg void OnHelpMicrosoftOnTheWebFreeStuff();
	afx_msg void OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions();
	afx_msg void OnHelpMicrosoftOnTheWebGetFasterInternetAccess();
	afx_msg void OnHelpMicrosoftOnTheWebMicrosoftHomePage();
	afx_msg void OnHelpMicrosoftOnTheWebSearchTheWeb();
	afx_msg void OnHelpMicrosoftOnTheWebSendFeedback();
	afx_msg void OnHelpMicrosoftOnTheWebInternetStartPage();
	afx_msg void OnHelpMicrosoftOnTheWebBestOfTheWeb();
	afx_msg void OnViewFontsLargest();
	afx_msg void OnViewFontsLarge();
	afx_msg void OnViewFontsMedium();
	afx_msg void OnViewFontsSmall();
	afx_msg void OnViewFontsSmallest();
	afx_msg void OnFileOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnCopyWithAllURLS(void);
	afx_msg void OnCopyWithRefURLS(void);
	afx_msg void OnCopyWithSrcURLS(void);
	afx_msg void CopyAndTakeNoURLS(void);
	afx_msg void OnElementProperty(void);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(CWnd *pParent);
	VARIANT_BOOL __stdcall OnHtmlDocumentMouseMove(IHTMLEventObj* pIEvt);

	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	void OnShowContextMenu(LPPOINT ppt,LPUNKNOWN pcmdtReserved,LPDISPATCH pdispReserved );	
	//virtual LRESULT OnShowContextMenu(DWORD dwID,LPPOINT ppt,LPUNKNOWN pcmdtReserved,LPDISPATCH pdispReserved ){return S_OK;};	
	BOOL CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT /* nID */, REFCLSID /* clsid */);

	virtual void OnDownloadComplete();
	virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);
	void InstallSink(CIEComCtrlSink* pSink);
	void EnableCatch(BOOL bEnable);
	BOOL IsErrorDocPage(void);
	void CopyToAnchors(int nMode);
	void OnMouseOut(void);
public:
	static BOOL s_bIsEnableSink;
	
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCIEVIEW_H__47FF4085_CE1B_11D0_BEB6_00C04FC99F83__INCLUDED_)
