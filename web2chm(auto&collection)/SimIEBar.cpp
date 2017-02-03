// SimIEBar.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "tabdoc.h"
#include "SimIEBar.h"
#include ".\simiebar.h"
#include "atlimage.h"
#include ".\FlatButton\VisualStylesXP.h"
#include ".\padframe.h"
IMPLEMENT_DYNAMIC(CSimIEBar, CMFCReBar)
CSimIEBar::CSimIEBar()
{
}

CSimIEBar::~CSimIEBar()
{
}


BEGIN_MESSAGE_MAP(CSimIEBar, CMFCReBar)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(AFX_IDW_TOOLBAR + 1, OnCbnSelchangeLocationedit)
END_MESSAGE_MAP()

// CSimIEBar 消息处理程序
BOOL CSimIEBar::Create(CWnd* pParentWnd, DWORD dwCtrlStyle , DWORD dwStyle , UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRet =   CMFCReBar::Create(pParentWnd, dwCtrlStyle, dwStyle, nID);
	if(bRet)
		bRet = CreateClient();
	//CReBarCtrl *p = (CReBarCtrl*)this;
   // CVisualStylesXP  xp;
	//xp.SetWindowTheme(m_hWnd,(LPCWSTR)(" "),(LPCWSTR)(" "));
	return bRet;
}

BOOL CSimIEBar::CreateClient(void)
{
		CImageList img;
		CImage  cimg;
		CString str;
		CBitmap bmp;
	
		if (!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT|WS_BORDER))
		{
			TRACE0("Failed to create toolbar\n");
			return FALSE;      // fail to create
		}

		// set up toolbar properties
		//m_wndToolBar.GetToolBarCtrl().SetButtonWidth(150,  300);
		//m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	
		bmp.LoadBitmap(IDB_HOTTOOLBAR);
		img.Create(24,24,ILC_COLORDDB|ILC_MASK,0,10);
		img.Add(&bmp,RGB(0,0,0));
		//m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
		img.Detach();
		bmp.Detach();

		bmp.LoadBitmap(IDB_COLDTOOLBAR);
		img.Create(24,24,ILC_COLORDDB|ILC_MASK,0,10);
		img.Add(&bmp,RGB(0,0,0));

		//m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
		img.Detach();
		bmp.Detach();
#if 0
		m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
		m_wndToolBar.SetButtons(NULL, 6);

		// set up each toolbar button
		m_wndToolBar.SetButtonInfo(0, ID_GO_BACK, TBSTYLE_BUTTON, 0);
		//str.LoadString(IDS_BACK);
		//m_wndToolBar.SetButtonText(0, str);
		m_wndToolBar.SetButtonInfo(1, ID_GO_FORWARD, TBSTYLE_BUTTON, 1);
		//str.LoadString(IDS_FORWARD);
	//	m_wndToolBar.SetButtonText(1, str);
		m_wndToolBar.SetButtonInfo(2, ID_VIEW_STOP, TBSTYLE_BUTTON, 2);
		//str.LoadString(IDS_STOP);
		//m_wndToolBar.SetButtonText(2, str);
		m_wndToolBar.SetButtonInfo(3, ID_VIEW_REFRESH, TBSTYLE_BUTTON, 3);
		//str.LoadString(IDS_REFRESH);
		//m_wndToolBar.SetButtonText(3, str);
		m_wndToolBar.SetButtonInfo(4, ID_GO_START_PAGE, TBSTYLE_BUTTON, 4);
		//str.LoadString(IDS_HOME);
		//m_wndToolBar.SetButtonText(4, str);
		m_wndToolBar.SetButtonInfo(5, ID_GO_SEARCH_THE_WEB, TBSTYLE_BUTTON, 5);
		//m_wndToolBar.SetButtonInfo(new CMFCToolBarButton(ID_GO_BACK,-1));
#endif
		CRect rectToolBar;
		// set up toolbar button sizes
		m_wndToolBar.GetItemRect(0, &rectToolBar);
		m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(24,24));
	
		// create a combo box for the address bar
		if (!m_wndAddress.Create(CBS_DROPDOWN  | WS_CHILD, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 1))
		{
			TRACE0("Failed to create combobox\n");
			return -1;      // fail to create
		}

		m_wndAddress.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	
		// add the toolbar, animation, and address bar to the rebar
		AddBar(&m_wndToolBar,NULL, NULL, RBBS_GRIPPERALWAYS|RBBS_FIXEDBMP|RBBS_USECHEVRON);
		//m_wndReBar.AddBar(&m_wndAnimate, NULL, NULL, RBBS_FIXEDSIZE | RBBS_FIXEDBMP);
		str.LoadString(IDS_ADDRESS);
		AddBar(&m_wndAddress, str, NULL, RBBS_FIXEDBMP | RBBS_BREAK);
	
		// set up min/max sizes and ideal sizes for pieces of the rebar
		REBARBANDINFO rbbi;
	
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE;
		rbbi.cxMinChild = rectToolBar.Width();
		rbbi.cyMinChild = rectToolBar.Height();
		rbbi.cx = rbbi.cxIdeal = rectToolBar.Width() * 6;
		GetReBarCtrl().SetBandInfo(0, &rbbi);
		rbbi.cxMinChild = 0;
	
		CRect rectAddress;
	
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
		m_wndAddress.GetWindowRect(&rectAddress);
		rbbi.cyMinChild = rectAddress.Height() + 10;
		rbbi.cxIdeal = 200;
		GetReBarCtrl().SetBandInfo(2, &rbbi);
	
		//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		//	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	
		m_wndAddress.Init("History\\URLS");
		m_wndAddress.Load();
		m_wndAddress.AddCurrentText();
		return TRUE;
}

void CSimIEBar::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	CMFCReBar::CalcWindowRect(lpClientRect, nAdjustType);
	CWnd  *pWnd  = GetParent();
	pWnd->Invalidate();
}

 BOOL  CSimIEBar::SaveAllURL()
 {
	 m_wndAddress.AddCurrentText();
	 m_wndAddress.Save();
	 return TRUE;
 }

BOOL   CSimIEBar::GetCurURL(CString &strURL)
 {
	 strURL.Empty();
	 m_wndAddress.GetWindowText(strURL);
	 strURL.Trim();
	 if(strURL.IsEmpty())
		 return FALSE;
	 return TRUE;
 }

BOOL  CSimIEBar::Check(CString strText)
{
	if(!IsValidURL(strText))
		return FALSE;
	CString strLBText;
	for (int i = 0; i < m_wndAddress.GetCount(); i++)
	{
		m_wndAddress.GetLBText(i, strLBText);
		if (_tcscmp(strText, strLBText)==0)
			return FALSE;
	}
	return TRUE;
}

void    CSimIEBar::AddCurURL(BOOL bSave)
{
	CString strText;
	m_wndAddress.GetWindowText(strText);
	strText.Trim();
	if(Check(strText))
		m_wndAddress.AddString(strText);

	if(bSave)
	{
		m_wndAddress.AddCurrentText();
		m_wndAddress.Save();
	}
}

 void CSimIEBar::OnDestroy()
 {
	 SaveAllURL();
	 __super::OnDestroy();
 }

void  CSimIEBar::OnCbnSelchangeLocationedit()
{
	int nsel =	m_wndAddress.GetCurSel();
	if(nsel == LB_ERR) return;
	CString str;
	m_wndAddress.GetLBText(nsel,str);
	
	CPadFrame *pFrame = (CPadFrame *)GetOwner();
	CMfcieView *pView = (CMfcieView *)pFrame->GetActiveView();
	pView->Navigate(str);
}