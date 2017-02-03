// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// mfcieVw.cpp : implementation of the CMfcieView class
//

#include "stdafx.h"

#include "resource.h"

#include "mfcieVw.h"
#include ".\mfcievw.h"
#include <atlbase.h>
#include  "Mshtml.h"
#include  "SimIeBar.h"
#include  "comutil.h"
#include "exdisp.h"
#include "ExDispid.h"
#include "mshtmdid.h"
#include "IEComCtrlSink.h"
#include "TIESinkManager.h"
#include ".\tanchormanager.h"
#include "tanchorpage.h"
#include "AnchorTree.h"
#include "TAnchorPiece.h"
#include "tcatalogbuilder.h"
#include "ConfigManager.h"
#include "ElementProperSheet.h"
#include "DialogIEToolBar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static CPoint st_CurPos;
void SaveCursorPos(void);
void RestoreCursorPos(void);
////////////////////////////////////////////////////////////////////////////
void SaveCursorPos(void)
{
	::GetCursorPos(&st_CurPos);
}

void RestoreCursorPos(void)
{
	::SetCursorPos(st_CurPos.x,st_CurPos.y);
}
/////////////////////////////////////////////////////////////////////////////
// CMfcieView
/*
Detected memory leaks!
Dumping objects ->
{162} client block at 0x00FAA9F0, subtype c0, 236 bytes long.
a CMfcieView object at $00FAA9F0, 236 bytes long
Object dump complete.
*/
IMPLEMENT_DYNCREATE(CMfcieView, CHtmlView)
//
//BEGIN_SINK_MAP(CMfcieView)
//SINK_ENTRY_EX(1, DIID_HTMLDocumentEvents2, DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEMOVE, OnHtmlDocumentMouseMove)
//END_SINK_MAP()

BEGIN_MESSAGE_MAP(CMfcieView, CHtmlView)
	//{{AFX_MSG_MAP(CMfcieView)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_HELP_WEB_TUTORIAL, OnHelpWebTutorial)
	ON_COMMAND(ID_HELP_ONLINE_SUPPORT, OnHelpOnlineSupport)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREE_STUFF, OnHelpMicrosoftOnTheWebFreeStuff)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREQUENTLY_ASKED_QUESTIONS, OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_GET_FASTER_INTERNET_ACCESS, OnHelpMicrosoftOnTheWebGetFasterInternetAccess)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_MICROSOFT_HOME_PAGE, OnHelpMicrosoftOnTheWebMicrosoftHomePage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEARCH_THE_WEB, OnHelpMicrosoftOnTheWebSearchTheWeb)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEND_FEEDBACK, OnHelpMicrosoftOnTheWebSendFeedback)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_INTERNET_START_PAGE, OnHelpMicrosoftOnTheWebInternetStartPage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_BEST_OF_THE_WEB, OnHelpMicrosoftOnTheWebBestOfTheWeb)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	//ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)

	ON_COMMAND(IDM_COPY_ALL_URLS,OnCopyWithAllURLS)
	ON_COMMAND(IDM_COPY_REF_URLS,OnCopyWithRefURLS)
	ON_COMMAND(IDM_COPY_SRC_URLS,OnCopyWithSrcURLS)
	ON_COMMAND(IDM_COPY_NO_URLS,CopyAndTakeNoURLS)
	ON_COMMAND(IDM_ELEMENT_PROPERTY,OnElementProperty)
	ON_WM_SIZE()

	ON_WM_SHOWWINDOW()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcieView construction/destruction
BOOL CMfcieView::s_bIsEnableSink;
CMfcieView::CMfcieView()
{
//	m_pMyIESink = NULL;
	m_pEditSite = NULL;
	 
}

CMfcieView::~CMfcieView()
{
	//m_pEditSite->RemoveBehavior();
	TIESinkManager::ReleaseSinkInstance();
	//m_pEditSite->RemoveBehavior();
}

BOOL CMfcieView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(0);
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcieView drawing

void CMfcieView::OnDraw(CDC* /*pDC*/)
{
	//CMfcieDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcieView diagnostics

#ifdef _DEBUG
void CMfcieView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcieView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CMfcieDoc* CMfcieView::GetDocument() // non-debug version is inline
//{
//	//ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcieDoc)));
//	return NULL;//(CMfcieDoc*)m_pDocument;
//}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcieView message handlers

void CMfcieView::OnTitleChange(LPCTSTR lpszText)
{
	

}
void CMfcieView::EnableCatch(BOOL bEnable)
{
	
	if(CMfcieView::s_bIsEnableSink != bEnable)
	{
		if(CMfcieView::s_bIsEnableSink)
		{
			//TIESinkManager::ReleaseSinkInstance();
			InstallSink(NULL);
			CMfcieView::s_bIsEnableSink = FALSE;

		}else
		{
			CMfcieView::s_bIsEnableSink = TRUE;
			TIESinkManager::CreateSinkInstance();
		    InstallSink(TIESinkManager::SinkInstance());
		}

	}
}

void CMfcieView::OnDocumentComplete(LPCTSTR lpszUrl)
{
 
	
	CComBSTR  bStr;
	m_pBrowserApp->get_LocationURL(&bStr);
	
#if 0 //get body html text
	CComPtr<IDispatch> iDsp;
	this->m_pBrowserApp->get_Document(&iDsp);
	CComQIPtr<IHTMLDocument2>iDoc2(iDsp);
	CComPtr<IHTMLElement> iBody;
	iDoc2->get_body(&iBody);
	CComBSTR bstr;
	iBody->get_innerHTML(&bstr);
#endif
	//zxy
	CString  cStr(bStr);
    CDialogIEToolBar::Instance()->m_URLAdressComboBox.SetWindowText(cStr);
	if( -1 == CDialogIEToolBar::Instance()->m_URLAdressComboBox.FindString(0,cStr))
		CDialogIEToolBar::Instance()->m_URLAdressComboBox.AddString(cStr);
	if(IsErrorDocPage()) 
	  return ;

	 
	if(m_pEditSite)//清理上次绘画的区域 
	{
		m_pEditSite->Unlock();
		m_pEditSite->SetElement(NULL,0);
	}
	if(CMfcieView::s_bIsEnableSink)
	{
		TIESinkManager::CreateSinkInstance();
		InstallSink(TIESinkManager::SinkInstance());
	}

}

void CMfcieView::OnInitialUpdate()
{
	// Go to the home page initially
	CHtmlView::OnInitialUpdate();

	m_pBrowserApp->put_Silent(_variant_t(TRUE));

	if(m_pEditSite)//清理上次绘画的区域 
	{
		m_pEditSite->Unlock();
		m_pEditSite->SetElement(NULL,0);
	}

	/*if(TIESinkManager::SinkInstance() != NULL)
	{
		AfxMessageBox("Sorry just one browser in this sample :)");
		return ;
	}*/

	
	//TIESinkManager::CreateSinkInstance();
	//GoHome();

}

void CMfcieView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD /*nFlags*/,
	LPCTSTR /*lpszTargetFrameName*/, CByteArray& /*baPostedData*/,
	LPCTSTR /*lpszHeaders*/, BOOL* /*pbCancel*/)
{
	//TIESinkManager::CreateSinkInstance();
	TRACE(_T("url:%s \n"),lpszURL);
	if(m_pEditSite)//清理上次绘画的区域 
  {
	 m_pEditSite->SetElement(NULL,CRect(0,0,0,0));
	 m_pEditSite->Lock();
  }
}

// these are all simple one-liners to do simple controlling of the browser
void CMfcieView::OnGoBack()
{
	GoBack();
	
}

void CMfcieView::OnGoForward()
{
	GoForward();
}

void CMfcieView::OnGoSearchTheWeb()
{
	GoSearch();
}

void CMfcieView::OnGoStartPage()
{
	GoHome();
}

void CMfcieView::OnViewStop()
{
	Stop();
}

void CMfcieView::OnViewRefresh()
{
	Refresh();
}

// these all go to specific web pages, just like Internet Explorer's help menu
void CMfcieView::OnHelpWebTutorial()
{
	CString str;

	str.LoadString(IDS_WEB_TUTORIAL);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpOnlineSupport()
{
	CString str;

	str.LoadString(IDS_ONLINE_SUPPORT);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebBestOfTheWeb()
{
	CString str;

	str.LoadString(IDS_BEST_OF_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebFreeStuff()
{
	CString str;

	str.LoadString(IDS_FREE_STUFF);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions()
{
	CString str;

	str.LoadString(IDS_FREQUENTLY_ASKED_QUESTIONS);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebGetFasterInternetAccess()
{
	CString str;

	str.LoadString(IDS_GET_FASTER_INTERNET_ACCESS);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebMicrosoftHomePage()
{
	CString str;

	str.LoadString(IDS_MICROSOFT_HOME_PAGE);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebSearchTheWeb()
{
	CString str;

	str.LoadString(IDS_SEARCH_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebSendFeedback()
{
	CString str;

	str.LoadString(IDS_SEND_FEEDBACK);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebInternetStartPage()
{
	CString str;

	str.LoadString(IDS_INTERNET_START_PAGE);
	Navigate2(str, 0, NULL);
}

// these functions control the font size.  There is no explicit command in the
// CHtmlView class to do this, but we can do it by using the ExecWB() function.
void CMfcieView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

// This demonstrates how we can use the Navigate2() function to load local files
// including local HTML pages, GIFs, AIFF files, etc.
void CMfcieView::OnFileOpen()
{
	CString str;

	str.LoadString(IDS_FILETYPES);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);

	if(fileDlg.DoModal() == IDOK)
		Navigate2(fileDlg.GetPathName(), 0, NULL);
}

BOOL CMfcieView::Create(CWnd *pParent)
{
    CRect  cltRect ;
	pParent->GetWindowRect(&cltRect);
	pParent->ScreenToClient(&cltRect);
	
	return CHtmlView::Create("htmlViewClass","IE",WS_CHILD|WS_VISIBLE|WS_BORDER,cltRect,pParent,0,0);
}

 
void CMfcieView::OnShowContextMenu( LPPOINT ppt,LPUNKNOWN pcmdtReserved,LPDISPATCH pdispReserved )
{
	if(!TIESinkManager::SinkInstance())
	  return ;
    if(IsErrorDocPage()) 
	  return ;
	if(GP_CurrentSession)
	  return ;

	SaveCursorPos();
	CMenu menu;
	CString ManuStr;
	menu.CreatePopupMenu();

	ManuStr.LoadString(IDM_COPY_NO_URLS);
	menu.AppendMenu((MF_ENABLED | MF_STRING), IDM_COPY_NO_URLS, ManuStr);
	menu.AppendMenu((MF_ENABLED | MF_SEPARATOR));

	ManuStr.LoadString(IDM_COPY_ALL_URLS);
	menu.AppendMenu((MF_ENABLED | MF_STRING), IDM_COPY_ALL_URLS, ManuStr);

	menu.AppendMenu((MF_ENABLED | MF_SEPARATOR));
	ManuStr.LoadString(IDM_COPY_REF_URLS);
	menu.AppendMenu((MF_ENABLED | MF_STRING), IDM_COPY_REF_URLS, ManuStr);

	ManuStr.LoadString(IDM_COPY_SRC_URLS);
	menu.AppendMenu((MF_ENABLED | MF_STRING), IDM_COPY_SRC_URLS, ManuStr);

	menu.AppendMenu((MF_ENABLED | MF_SEPARATOR));
	ManuStr.LoadString(IDM_ELEMENT_PROPERTY);
	menu.AppendMenu((MF_ENABLED | MF_STRING), IDM_ELEMENT_PROPERTY, ManuStr);

	//this->ClientToScreen(ppt);
	menu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_VERTICAL,
		ppt->x, ppt->y, AfxGetMainWnd(), NULL);

	//::SetCursorPos(ppt->x,ppt->y);
	RestoreCursorPos();
	//return S_OK;
}


void CMfcieView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	// TODO: Add your specialized code here and/or call the base class
	CComPtr<IDispatch > iDsp;
	if(*ppDisp == NULL)
		iDsp = this->GetHtmlDocument();
	else
		iDsp = *ppDisp;
	if( iDsp == NULL) return;
	CComQIPtr< IHTMLDocument2, &IID_IHTMLDocument2 > spPSI( iDsp );
    if(spPSI == NULL) return;

	CComPtr<IHTMLElement> pElement;
	spPSI->get_activeElement(&pElement);
	if(pElement == NULL)return;

	CComQIPtr<IHTMLAnchorElement>  pAnchor(pElement);
	if(pAnchor == NULL ) return;
	CComBSTR  cbStr;
	pAnchor->get_href(&cbStr);
	
	/*char txt[400];
	BOOL b;
    WideCharToMultiByte( CP_ACP, 0, cbStr.m_str,cbStr.Length(),txt,
		400 ,NULL,&b );*/

	CString   csStr(cbStr);

    TRACE("Get Href :%s\n",csStr);

    Navigate2(csStr, 0, NULL);
	
	CDialogIEToolBar::Instance()->m_URLAdressComboBox.SetWindowText(csStr);
	*Cancel = TRUE;
	//CHtmlView::OnNewWindow2(ppDisp, Cancel);
}

void GetElementAbsolutePos(IHTMLElement *pElement, CRect &rcElement)
{
	long x= 0, y = 0, w = 0, h = 0;
	
	pElement->get_offsetLeft(&x);
	pElement->get_offsetTop(&y);
	pElement->get_offsetWidth(&w);
	pElement->get_offsetHeight(&h);
	
	if(rcElement.IsRectNull())	{
		rcElement = CRect(x, y, x+w, y+h);
	}else {
		rcElement.OffsetRect(x, y);
	}
	
	// Calc relative position about parent
	CComPtr<IHTMLElement> spParent;
	pElement->get_offsetParent(&spParent);
	if(!spParent) return;

	return GetElementAbsolutePos(spParent, rcElement);
}


VARIANT_BOOL __stdcall CMfcieView::OnHtmlDocumentMouseMove(IHTMLEventObj* pIEvt)
{
	if(GP_CurrentSession != NULL)
		return VARIANT_FALSE;
	if(!TIESinkManager::SinkInstance())
	{
		m_pEditSite->RemoveBehavior();
		return VARIANT_FALSE;
	}
	if(IsErrorDocPage()) 
	{
        m_pEditSite->RemoveBehavior();
		return VARIANT_FALSE;
	}

	IHTMLElement*	pElement;
	HRESULT	hr = pIEvt->get_srcElement(&pElement);
	if(!pElement) 
		return VARIANT_FALSE;

	/*if(ElementHasNoUrl(pElement))
		return VARIANT_FALSE;*/

	CRect rcElement = CRect(0,0,0,0);
	GetElementAbsolutePos(pElement, rcElement);
	
	CComPtr<IDispatch> iDsp;
	this->m_pBrowserApp->get_Document(&iDsp);
	CComQIPtr<IHTMLDocument2>iDoc2(iDsp);
	CComPtr<IHTMLElement> iBody;
	iDoc2->get_body(&iBody);

	if(!iBody) return VARIANT_FALSE;
	long  left,top;
	iBody->get_offsetLeft(&left);
	iBody->get_offsetTop(&top);

	rcElement.OffsetRect(-left,-top);

	/*CRect cliRect;
	GetClientRect(&cliRect);
	m_pEditSite->Lock();
	m_pEditSite->SetElement(NULL,0);
	m_pEditSite->Unlock();*/
	m_pEditSite->SetElement(pElement,rcElement);
	
	return VARIANT_TRUE;
}

BOOL CMfcieView:: CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT /* nID */, REFCLSID /* clsid */)
{
	ASSERT(ppSite != NULL);

	*ppSite = m_pEditSite = new CHTMLControlSite(pContainer);
	return TRUE;

}
void CMfcieView::OnDownloadComplete()
{
	// TODO: Add your specialized code here and/or call the base class

	
	CHtmlView::OnDownloadComplete();
   if(m_pEditSite)
	m_pEditSite->SetElement(NULL,CRect(0,0,0,0));
}


void CMfcieView::InstallSink(CIEComCtrlSink* pSink)
{

	if(pSink ==	 NULL)
	{
		m_pEditSite->RemoveBehavior();
		return;
	}

	CComPtr<IDispatch> pDocument;
	HRESULT hr = this->m_pBrowserApp->get_Document(&pDocument);
	ATLASSERT(SUCCEEDED(hr) && L"Get Document fail!");
			
	CComQIPtr<IHTMLDocument2> spDoc(pDocument);
	TIESinkManager::SinkInstance()->MyAdviseSink(spDoc,this);	
	m_pEditSite->SetElement(NULL,0);
	m_pEditSite->AddBehavior(spDoc);
}

void  CMfcieView::OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
{
	TRACE("ERROR :%d\n",dwError);
	switch(dwError)
	{
	case INET_E_RESOURCE_NOT_FOUND:
		{
			//TIESinkManager::ReleaseSinkInstance();
			m_pEditSite->Lock();
		}
		break;
	}

	// 
	 __super::OnNavigateError(lpszURL,lpszFrame,dwError,pbCancel);
}

BOOL  ParseElmentUrls(IHTMLElement *piElement,URL_LIST &list,int nMode)
{
	
	if(nMode <= 0)
		return TRUE;

	CComQIPtr<IHTMLAnchorElement > pt(piElement);
	CComBSTR bstr;
	switch(nMode)
	{
		case 1://copy all urls to anchors
		{
			CString str;
			if(pt != NULL)
			{
				pt->get_href(&bstr);
				bstr.ToLower();
				str = bstr;
				if(!str.IsEmpty())
					list.push_back(CString(bstr.m_str));	
			}

			CComQIPtr<IHTMLImgElement> spImg(piElement);
			if(spImg) 
			{		// <IMG>
				spImg->get_src(&bstr);
				bstr.ToLower();

				str = bstr;
				 
				str.Replace(_T("about:blank"), _T(""));
				if(!str.IsEmpty())
					list.push_back(str); 
			}

		}break;
		case 2://copy ref urls to anchors  
		{
			if(pt != NULL)
			{
				CString str;
				pt->get_href(&bstr);
				bstr.ToLower();
				str = bstr;
				if(!str.IsEmpty())
					list.push_back(CString(bstr.m_str));	
			}
		}break;
		case 3://copy src urls to anchors
		{
			CComQIPtr<IHTMLImgElement> spImg(piElement);
			if(spImg) 
			{		// <IMG>
				CString str;
				spImg->get_src(&bstr);

				bstr.ToLower();
				str = bstr;
				 
				str.Replace(_T("about:blank"), _T(""));
				if(!str.IsEmpty())
					list.push_back(str); 
			}
		}
		break;

	 } 
			
		CComPtr<IDispatch> spDispatch;
		CComQIPtr<IHTMLElementCollection> spCollection;
		if(piElement) 
		{
			piElement->get_all(&spDispatch);
			spCollection = spDispatch;
		}

		long l;
		spCollection->get_length(&l);
		for(long i=0; i<l; i++) 
		{
			CComPtr<IDispatch> spDispatchElement;
			spCollection->item(CComVariant(i), CComVariant(i), &spDispatchElement);
			CComQIPtr<IHTMLAnchorElement> spAnchor(spDispatchElement);

			if(spAnchor && (nMode == 1 || nMode == 2)) 
			{	// <A>
					
				spAnchor->get_href(&bstr);

				bstr.ToLower();
				CString str(bstr);

				 
				str.Replace(_T("about:blank"), _T(""));
				if(str.Find(_T("mailto:")) == 0) continue;
				if(str.Find(_T("javascript:")) >= 0) continue;
				
				if(!str.IsEmpty()) 
					list.push_back(str); 
				
			}

			CComQIPtr<IHTMLImgElement> spImg(spDispatchElement);
			if(spImg && (nMode == 3||nMode == 1)) 
			{		// <IMG>
					
				spImg->get_src(&bstr);

				bstr.ToLower();
				CString str(bstr);  
				
				 
				str.Replace(_T("about:blank"), _T(""));
			  
				if(!str.IsEmpty())
					list.push_back(str); 

			}

			CComQIPtr<IHTMLLinkElement> spLink(spDispatchElement);
			if(spLink && (nMode == 1 || nMode == 2)) 
			{	// <LINK>
					
				spLink->get_rel(&bstr);
				bstr.ToLower();
				if(bstr == L"stylesheet") 
				{
					::SysFreeString(bstr);
					spLink->get_href(&bstr);
					CString str(bstr);
					 
					str.Replace(_T("about:blank"), _T(""));
					
					if(!str.IsEmpty())
						list.push_back(str); 
				}
			}

			CComQIPtr<IHTMLFrameBase> spFrame(spDispatchElement);
			if(spFrame && nMode == 3) 
			{	
					
				spFrame->get_src(&bstr);
				bstr.ToLower();
				CString str(bstr);
				 
				str.Replace(_T("about:blank"), _T(""));
				if(!str.IsEmpty())
					list.push_back(str); 
			}
		}
		
	return list.size() > 0 ;
}

 void CMfcieView::CopyToAnchors(int nMode)
 {
	 CComPtr<IHTMLElement> pCurElement(m_pEditSite->GetElement());
	 if(pCurElement == NULL) return ;

	 CComPtr<IDispatch>piDsp;
	 CComPtr<IHTMLDocument2> pCurDoc2; 
	 this->m_pBrowserApp->get_Document(&piDsp);
	 piDsp->QueryInterface(__uuidof(IHTMLDocument2),(void **)&pCurDoc2);
	 if(pCurDoc2 == NULL) return;

	 /*judge */
	 CComBSTR  bsUrl;
	 pCurDoc2->get_URL(&bsUrl);
	 TAnchorPage *pPage = TAnchorManager::Instance()->GetPage(CString(bsUrl));

	 /*页面不存在就建立新的叶 及 anchors clip结构 。*/
	 if(NULL == pPage)
	 {
	    CComBSTR  bstr;
		pPage = new TAnchorPage();
		pCurDoc2->get_title(&bstr);
		pPage->m_PageTitle = CString(bstr);

		pPage->m_PageTitle.Trim();
		if(pPage->m_PageTitle.IsEmpty())
			pPage->m_PageTitle.Format("Page%d",TAnchorManager::Instance()->GetPageCount());
		
		pPage->m_BindPageUrl = CString(bsUrl);
		
		
		TAnchorPiece *pPiece = new TAnchorPiece(pPage);

		pCurElement->get_outerHTML(&bstr);
		pPiece->m_rawHtml = CString(bstr);
	
		pPiece->m_rawHtml = bstr;
		pPiece->m_rootPage = pPage;
		 
		 
		ParseElmentUrls(pCurElement,pPiece->m_urlsList,nMode);

		HTREEITEM htItem = GO_AnchorTree.InsertItem(pPage->m_PageTitle,GO_AnchorTree.m_hroot);
		
		TAnchorManager::Instance()->AddPage(htItem,pPage);
		GO_AnchorTree.SetItemData(htItem,(DWORD_PTR)pPage);

		pCurElement->get_innerText(&bstr);
		
		CString		innerctxt(bstr);
		HTREEITEM   hItem;

		innerctxt.Trim();
		if(innerctxt.IsEmpty())
			innerctxt.Format("Anchor%d",pPage->GetPieceCount());
			
		hItem = GO_AnchorTree.InsertItem(innerctxt,htItem);
		GO_AnchorTree.SetItemData(hItem,(DWORD_PTR)pPiece);
		pPiece->m_hitem = hItem;
		pPiece->m_PieceTitle = innerctxt;	

		pPage->AddPiece(pPiece);

	 }else
	 {
		CComBSTR bstr;

		TAnchorPiece *pPiece = new TAnchorPiece(pPage);
		pCurElement->get_outerHTML(&bstr);
		pPiece->m_rawHtml = CString(bstr);
		pPiece->m_rootPage = pPage;

		if(pPage->HasThisPiece(pPiece))
		{
			delete pPiece;
			return;
		}
		
		ParseElmentUrls(pCurElement,pPiece->m_urlsList,nMode);
		/*插入 tree ctrl*/
	
		pCurElement->get_innerText(&bstr);
		CString		innerctxt(bstr);
		HTREEITEM   hItem;
		
		innerctxt.Trim();
		if(innerctxt.IsEmpty())
			innerctxt.Format("Anchor%d",pPage->GetPieceCount());
		hItem = GO_AnchorTree.InsertItem(innerctxt,
		TAnchorManager::Instance()->GetPageItemHandle(pPage));
		
		GO_AnchorTree.SetItemData(hItem,(DWORD_PTR)pPiece);
		pPiece->m_hitem = hItem;
		pPiece->m_PieceTitle = innerctxt;
		
		pPage->AddPiece(pPiece);
	 }
	
	GO_AnchorTree.Invalidate();
 }
 void CMfcieView::OnSize(UINT nType, int cx, int cy)
 {
	 CHtmlView::OnSize(nType, cx, cy);
//	 if(NULL == m_pEditSite)
//		 return;
//
//	 pSYSTEM_CONFIG  pconf = ConfigManager::Instance()->GetSystemConfig();
//	 if(pconf->dwWorkMode) return ;
//
//	 CRect rcElement = CRect(0,0,0,0);
////	 m_pEditSite->getsta
//	 IHTMLElement *pElement = m_pEditSite->GetElement();
//	 	 
//	 if(NULL == pElement)
//		 return ;
//
//	 GetElementAbsolutePos(pElement, rcElement);
//	 m_pEditSite->SetElement(pElement,rcElement);
	 // TODO: Add your message handler code here
 if(m_pEditSite)
	 m_pEditSite->SetElement(NULL,CRect(0,0,0,0));
 }

 BOOL CMfcieView::IsErrorDocPage(void)
 {
	 CComPtr<IDispatch> piDsp;
	 CComQIPtr<IHTMLDocument2> piDoc2; 
	 CComBSTR bstrTitle;

	 this->m_pBrowserApp->get_Document(&piDsp);
	 READYSTATE readState;
	// this->m_pBrowserApp->gete( &readState);
	 if(piDsp == NULL) return TRUE;
	 piDoc2 = piDsp;
	// piDoc2->get
	 if(piDoc2 == NULL)return TRUE;
	 piDoc2->get_title(&bstrTitle);
//web page unavailable while offline
	 CString  cstrTitle(bstrTitle);
	 cstrTitle.Trim();	
	 cstrTitle.MakeLower();//Invalid syntax error
	 if(cstrTitle.Compare("no page to display") == 0)
	 return TRUE;
	 if(cstrTitle.Compare("cannot find server") == 0)
	 return TRUE;
	 if(cstrTitle.Compare("web page unavailable while offline") == 0)
	 return TRUE;
	 if(cstrTitle.Compare("invalid syntax error") == 0)
	 return TRUE;
	 if(cstrTitle.Compare("http 404 not found") == 0)
	 return TRUE;


	 return FALSE;
 }


 void CMfcieView::PostNcDestroy()
 {
	 // TODO: Add your specialized code here and/or call the base class
	//TIESinkManager::ReleaseSinkInstance();
	/*if( TIESinkManager::SinkInstance())
		delete TIESinkManager::SinkInstance();*/

 }

 void CMfcieView::OnCopyWithAllURLS(void)
 {
	 CopyToAnchors(1);
 }

 void CMfcieView::OnCopyWithRefURLS(void)
 {
	 CopyToAnchors(2);
 }

 void CMfcieView::OnCopyWithSrcURLS(void)
 {
	 CopyToAnchors(3);
 }

 void CMfcieView::CopyAndTakeNoURLS(void)
 {
	 CopyToAnchors(0);
 }

 void CMfcieView::OnElementProperty(void)
 {
	 CComPtr<IHTMLElement> pCurElement(m_pEditSite->GetElement());
	 if(pCurElement == NULL) return ;

	 //BSTR  bstr;
	 //pCurElement->get_tagName(&bstr);
	 CString  strCap;
	 strCap.LoadString(IDS_ELEMENT_CAP);

	 CElementPropertySheet  sheet(strCap);
	 sheet.m_pCurElement = pCurElement;
	 sheet.DoModal();
	 RestoreCursorPos();
	 //AfxMessageBox(CString(bstr));
 }


 void CMfcieView::OnMouseOut(void)
 {

	//  if(m_pEditSite)
	// m_pEditSite->SetElement(NULL,CRect(0,0,0,0));
 }
