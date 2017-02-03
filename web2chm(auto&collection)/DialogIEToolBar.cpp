// DialogIEToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "DialogIEToolBar.h"
#include "afxdialogex.h"
#include "W2CRibbonButtonEx.h"
#include "mfcievw.h"
// CDialogIEToolBar dialog

IMPLEMENT_DYNAMIC(CDialogIEToolBar, CDialogEx)
IMP_VISUAL_MANAGER(CDialogIEToolBar)
CDialogIEToolBar::CDialogIEToolBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogIEToolBar::IDD, pParent)
{

}

CDialogIEToolBar::~CDialogIEToolBar()
{
}

void CDialogIEToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URL_COMBO, m_URLAdressComboBox);
	 
	m_URLAdressComboBox.Init("History\\URLS");
	m_URLAdressComboBox.Load();
	m_URLAdressComboBox.AddCurrentText();
	m_URLAdressComboBox.SetDlgCtrlID(AFX_IDW_TOOLBAR + 1);
}
BEGIN_MESSAGE_MAP(CDialogIEToolBar, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(AFX_IDW_TOOLBAR + 1, OnCbnSelchangeLocationedit)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
END_MESSAGE_MAP()


// CDialogIEToolBar message handlers

CW2CRibbonButtonEx *pPreBtn = NULL;
CW2CRibbonButtonEx *pNextBtn = NULL;
CW2CRibbonButtonEx *pStopBtn = NULL;
CW2CRibbonButtonEx *pFreshBtn = NULL;;


int CDialogIEToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	this->ModifyStyle(0,WS_CLIPCHILDREN,0);

	 pPreBtn = new CW2CRibbonButtonEx(this,CRect(0,0,0,0));
	 pPreBtn->SetID(ID_GO_BACK);
	 this->AddToManager(pPreBtn);
	 pNextBtn = new CW2CRibbonButtonEx(this,CRect(0,0,0,0));
	 pNextBtn->SetID(ID_GO_FORWARD);
	 this->AddToManager(pNextBtn);
	 pStopBtn = new CW2CRibbonButtonEx(this,CRect(0,0,0,0));
	 pStopBtn->SetID(ID_VIEW_STOP);
	 this->AddToManager(pStopBtn);
	 pFreshBtn = new CW2CRibbonButtonEx(this,CRect(0,0,0,0));
	 pFreshBtn->SetID(ID_VIEW_REFRESH);
	 this->AddToManager(pFreshBtn);

	return 0;
}

#define BTN_WIDTH 28
#define BTN_HEIGHT BTN_WIDTH

void CDialogIEToolBar::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if(!::IsWindow(m_URLAdressComboBox.GetSafeHwnd()))
		return;

	CRect cliRect;
	GetClientRect(&cliRect);

	CRect btnRect;
#if 0
	m_CancelBtn.GetWindowRect(btnRect);
	this->ScreenToClient(btnRect);

	cliRect.left = btnRect.right + 2;
	cliRect.right -= 2;
#endif
	 
	m_URLAdressComboBox.GetWindowRect(btnRect);
	int offset = (cliRect.Height() - btnRect.Height())/2 ;
	cliRect.DeflateRect(0,offset);
	offset = BTN_WIDTH * 2;
	cliRect.DeflateRect(offset,0);
	//cliRect.OffsetRect(offset,0);

	CMFCRibbonButtonEx *pBtns[]={
		pPreBtn,pNextBtn,pStopBtn,pFreshBtn
	};

	for( int i = 0;i < sizeof(pBtns)/sizeof(pFreshBtn) ;i++)
	{
		pBtns[i]->SetRect(CRect(BTN_WIDTH *i,0,BTN_WIDTH *(i+1),BTN_HEIGHT));
		if( i >=2)
		{
			CRect BtnRect(0,0,BTN_WIDTH,BTN_HEIGHT);
			int btnoffset = cliRect.right;
			if(btnoffset <= BTN_WIDTH *2)
				btnoffset = BTN_WIDTH *2;
			BtnRect.OffsetRect(btnoffset + BTN_WIDTH * (i-2),0);
			pBtns[i]->SetRect(BtnRect);
		}
	}
	m_URLAdressComboBox.MoveWindow(cliRect);
	// TODO: Add your message handler code here
}


BOOL CDialogIEToolBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect cliRect;
	GetClientRect(cliRect);

	pDC->FillRect(cliRect,&afxGlobalData.brBarFace);
	return  TRUE;
	//return __super::OnEraseBkgnd(pDC);
}


void CDialogIEToolBar::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//__super::OnOK();
	CString str;

	m_URLAdressComboBox.GetWindowText(str);//GetLBText(CSimIEBar::Instance()->m_wndAddress.GetCurSel(), str);
	CMfcieView::Instance()->Navigate2(str, 0, NULL);
}


void CDialogIEToolBar::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//__super::OnCancel();
}


void CDialogIEToolBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	this->m_VisualManager.Clear();
	__super::OnClose();
}


void  CDialogIEToolBar::OnCbnSelchangeLocationedit()
{
	int nsel =	m_URLAdressComboBox.GetCurSel();
	if(nsel == LB_ERR) return;
	CString str;
	m_URLAdressComboBox.GetLBText(nsel,str);
	
	//CPadFrame *pFrame = (CPadFrame *)GetOwner();
	 
	CMfcieView::Instance()->Navigate(str);
}


void  CDialogIEToolBar::OnGoBack()
{
	CMfcieView::Instance()->GoBack();
}
void  CDialogIEToolBar::OnGoForward()
{
	CMfcieView::Instance()->GoForward();
}

void  CDialogIEToolBar::OnGoSearchTheWeb()
{
	CMfcieView::Instance()->GoSearch();
}

void  CDialogIEToolBar::OnGoStartPage()
{
	CMfcieView::Instance()->GoHome();
}

void  CDialogIEToolBar::OnViewStop()
{
	CMfcieView::Instance()->Stop();
}

void  CDialogIEToolBar::OnViewRefresh()
{
	CMfcieView::Instance()->Refresh();
}

