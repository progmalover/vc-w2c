// WorkModeDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "WorkModeDlg.h"
#include ".\workmodedlg.h"
#include "ConfigManager.h"
#include "anchorsbar.h"
#include "TIESinkManager.h"
#include "mainfrm.h"
#include "mfcievw.h"
#include "tabdoc.h"
#include "tcatalogbuilder.h"
CWorkModeDlg GO_WorkModeDlg;
// CWorkModeDlg dialog
 
IMPLEMENT_DYNAMIC(CWorkModeDlg, CDialog)
CWorkModeDlg::CWorkModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkModeDlg::IDD, pParent)
{
}

CWorkModeDlg::~CWorkModeDlg()
{
}

void CWorkModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORK_MODE_COMBO, m_cmbWorkMode);
	DDX_Control(pDX, IDC_WORKMODE_CAP, m_modeCap);
	if(!::IsWindow(m_cmbWorkMode.m_hWnd)) return ;
	if(m_cmbWorkMode.GetCount() <= 0)
	{
		CString strMode;
		strMode.LoadString(IDS_COLLECTION_MODE);//"Collection Mode"
		m_cmbWorkMode.InsertString(0,strMode);
		strMode.LoadString(IDS_AUTO_MODE);//_T("Auto Mode")
		m_cmbWorkMode.InsertString(1,strMode);
		
		pSYSTEM_CONFIG  pSysCf = ConfigManager::Instance()->GetSystemConfig();
		TRACE("show1 :%d\n",pSysCf->dwWorkMode);
		m_cmbWorkMode.SetCurSel(pSysCf->dwWorkMode);
	}
	
}


BEGIN_MESSAGE_MAP(CWorkModeDlg, CDialog)
ON_CBN_SELCHANGE(IDC_WORK_MODE_COMBO, OnCbnSelchangeWorkModeCombo)
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(CWorkModeDlg::CStaticEx, CStatic)
ON_WM_ERASEBKGND()
ON_WM_PAINT()
END_MESSAGE_MAP()

void HookToWeb( CIEComCtrlSink * pSink)
{
	CMfcieView *pView = (CMfcieView*)((CMainFrame *)theApp.GetMainWnd())->GetActiveViewWnd();
	pView->InstallSink(pSink);
}

void CWorkModeDlg::OnCbnSelchangeWorkModeCombo()
{
	// TODO: Add your control notification handler code here
	pSYSTEM_CONFIG  pSysCf = ConfigManager::Instance()->GetSystemConfig();
	pSysCf->dwWorkMode = m_cmbWorkMode.GetCurSel();

	CAnchorsBar::Instance()->ShowPane( pSysCf->dwWorkMode == ANCHOR_MODE,0,0);
	CIEComCtrlSink *pSink = TIESinkManager::CreateSinkInstance();//zxy error....

	CMfcieView *pView = (CMfcieView*)((CMainFrame *)theApp.GetMainWnd())->GetActiveViewWnd();
	if(pSink)
	{
		HookToWeb(pSink);
		pView->SetFocus();
		//TCatalogBuilder::m_bOK = TRUE;

	}else
	{
		//CMfcieView *pView = (CMfcieView*)((CMainFrame *)theApp.GetMainWnd())->GetActiveViewWnd();
		pView->m_pEditSite->RemoveBehavior();
		pView->SetFocus();
		//TCatalogBuilder::m_bOK = FALSE;

	}

}
BOOL CWorkModeDlg::EnableWindow(BOOL bEnable )
{
	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd)
	{
		pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetNextWindow();
	}
	return S_OK;
}
BOOL CWorkModeDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;// CDialog::OnEraseBkgnd(pDC);
}

BOOL CWorkModeDlg::CStaticEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	
	return 1;// CDialog::OnEraseBkgnd(pDC);
}



void CWorkModeDlg::CStaticEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CString  str;
	CRect cliRect;

	this->GetWindowText(str);
	this->GetClientRect(&cliRect);
	dc.SaveDC();
	dc.SelectObject(this->GetFont());
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(str,str.GetLength(),&cliRect,DT_VCENTER);
	dc.RestoreDC(-1);
}

void CWorkModeDlg::SetMode(int nMode)
{
	m_cmbWorkMode.SetCurSel(nMode);
	OnCbnSelchangeWorkModeCombo();
}
