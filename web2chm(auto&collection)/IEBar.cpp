#include "StdAfx.h"
#include "IEBar.h"
#include "mfcievw.h"
#include "resource.h"
#include "DialogIEToolBar.h"
#define IE_BAR_HEIGHT  30

IMPLEMENT_DYNAMIC(CIEPane, CAppDockablePane)
BEGIN_MESSAGE_MAP(CIEPane, CAppDockablePane)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_MESSAGE(WM_IDLEUPDATECMDUI,OnUpdateUI)
END_MESSAGE_MAP()

CIEPane::CIEPane(void)
{
}


CIEPane::~CIEPane(void)
{
}


int CIEPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAppDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_dwControlBarStyle = 0;
	this->RemoveCaptionButtons();
	
	return InitlizeLayoutCtrls();
}

int CIEPane::InitlizeLayoutCtrls()
{

	if(0 == CMfcieView::Instance()->Create(this))
		return -1;
	if(CDialogIEToolBar::Instance()->Create(IDD_DIALOG_IEBAR,this) == 0)
		return -1;
	CDialogIEToolBar::Instance()->ShowWindow(SW_SHOW);
#if 0
	CMfcieView::Instance()->SetFocus();
	if(0 == m_IeToolBar.Create(this))
	{
		TRACE0("Failed to create m_IeToolBar\n");
		return -1;
	}

	UINT idArray[6] = {ID_GO_BACK,ID_GO_FORWARD,ID_VIEW_STOP,
				ID_VIEW_REFRESH,ID_GO_START_PAGE,ID_GO_SEARCH_THE_WEB};

	m_IeToolBar.LoadBitmap(IDB_HOTTOOLBAR,IDB_COLDTOOLBAR);
	
	m_IeToolBar.SetButtons(idArray, 6);
	m_IeToolBar.SetSizes(CSize(30,30),CSize(24,24));
 
	m_IeToolBar.SetButtonInfo(0, ID_GO_BACK, TBSTYLE_BUTTON, 0);
	m_IeToolBar.SetButtonInfo(1, ID_GO_FORWARD, TBSTYLE_BUTTON, 1);
	m_IeToolBar.SetButtonInfo(2, ID_VIEW_STOP, TBSTYLE_BUTTON, 2);
	m_IeToolBar.SetButtonInfo(3, ID_VIEW_REFRESH, TBSTYLE_BUTTON, 3);
	m_IeToolBar.SetButtonInfo(4, ID_GO_START_PAGE, TBSTYLE_BUTTON, 4);
	m_IeToolBar.SetButtonInfo(5, ID_GO_SEARCH_THE_WEB, TBSTYLE_BUTTON, 5);
	 
	if(!m_stcAddress.Create(_T("Address:"),WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this))
	{
		TRACE0("Failed to create m_stcAddress\n");
		return -1;
	}
	if (!m_wndAddress.Create(CBS_DROPDOWN  | WS_CHILD |WS_VISIBLE, CRect(0, 0, 200, 120), this, AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}
	
	m_stcAddress.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	m_wndAddress.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
#endif
	//m_IeUrlBar.LoadBitmap();
}

void CIEPane::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CAppDockablePane::OnLButtonDown(nFlags, point);
}


void CIEPane::OnSize(UINT nType, int cx, int cy)
{
	CAppDockablePane::OnSize(nType, cx, cy);

	CRect cliRect;
	GetClientRect(&cliRect);

	if(!::IsWindow(CMfcieView::Instance()->m_hWnd))
		return;

	HDWP hDwp = ::BeginDeferWindowPos(2);
	hDwp = ::DeferWindowPos(hDwp,CDialogIEToolBar::Instance()->GetSafeHwnd(),0,0,0,
				cliRect.Width(),IE_BAR_HEIGHT,SWP_SHOWWINDOW);
	

	hDwp = ::DeferWindowPos(hDwp,CMfcieView::Instance()->GetSafeHwnd(),0,0,IE_BAR_HEIGHT,
				cliRect.Width(),cliRect.Height() - IE_BAR_HEIGHT,SWP_SHOWWINDOW);
	::EndDeferWindowPos(hDwp); 

}

 BOOL  CIEPane::SaveAllURL()
 {
	 CHistoryComboBox &urlComboBox = CDialogIEToolBar::Instance()->m_URLAdressComboBox;
	 if(!::IsWindow(urlComboBox.GetSafeHwnd()))
		 return FALSE;

	 urlComboBox.AddCurrentText();
	 urlComboBox.Save();

	 return TRUE;
 }

BOOL   CIEPane::GetCurURL(CString &strURL)
 {
	  CHistoryComboBox &urlComboBox = CDialogIEToolBar::Instance()->m_URLAdressComboBox;

	 strURL.Empty();
	 urlComboBox.GetWindowText(strURL);
	 strURL.Trim();
	 if(strURL.IsEmpty())
		 return FALSE;
 
	 return TRUE;
 }

BOOL  CIEPane::Check(CString strText)
{
	if(!IsValidURL(strText))
		return FALSE;
	CString strLBText;

    CHistoryComboBox &urlComboBox = CDialogIEToolBar::Instance()->m_URLAdressComboBox;

	for (int i = 0; i < urlComboBox.GetCount(); i++)
	{
		urlComboBox.GetLBText(i, strLBText);
		if (_tcscmp(strText, strLBText)==0)
			return FALSE;
	}
 
	return TRUE;
}

void    CIEPane::AddCurURL(BOOL bSave)
{
	CString strText;
	CHistoryComboBox &urlComboBox = CDialogIEToolBar::Instance()->m_URLAdressComboBox;
	urlComboBox.GetWindowText(strText);
	strText.Trim();
	if(Check(strText))
		urlComboBox.AddString(strText);

	if(bSave)
	{
		urlComboBox.AddCurrentText();
		urlComboBox.Save();
	}
 
}

 void CIEPane::OnDestroy()
 {
	 SaveAllURL();
	 __super::OnDestroy();
 }

 HRESULT CIEPane::OnUpdateUI(WPARAM wparam, LPARAM lparam)
 {
	// CDialogIEToolBar::Instance()->UpDateControls();
	 return S_OK;
 }