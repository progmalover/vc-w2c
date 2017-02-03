// ModifyPage.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ModifyPage.h"
#include ".\modifypage.h"


// CModifyPage dialog

IMPLEMENT_DYNAMIC(CModifyPage, CPropertyPage)
CModifyPage::CModifyPage()
	: CPropertyPage(CModifyPage::IDD)
	,m_pConf(NULL)
{
}

CModifyPage::~CModifyPage()
{
}

void CModifyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModifyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_USEDEFAULT, OnBnClickedCheckUsedefault)
END_MESSAGE_MAP()

 
	 
void CModifyPage::OnBnClickedCheckUsedefault()
{
	BOOL bEnable = !GetCheck(IDC_CHECK_USEDEFAULT);
	if(!bEnable)
	{
		SetCheck(IDC_CHECK_SCRIPT,TRUE);
		SetCheck(IDC_CHECK_NOSCRIPT,TRUE);
		SetCheck(IDC_CHECK_LOAD,TRUE);
		SetCheck(IDC_CHECK_UNLOAD,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_CLICK,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_ENTER,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_LEAVE,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_OUT,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_OVER,TRUE);
		SetCheck(IDC_CHECK_MOUSEEVENT_DOWN,TRUE);
	}
	Enable( IDC_CHECK_SCRIPT, bEnable);
	Enable( IDC_CHECK_NOSCRIPT, bEnable);
	Enable(IDC_CHECK_LOAD , bEnable);
	Enable(IDC_CHECK_UNLOAD , bEnable);
	Enable( IDC_CHECK_MOUSEEVENT_CLICK, bEnable);
	Enable( IDC_CHECK_MOUSEEVENT_ENTER, bEnable);
	Enable(IDC_CHECK_MOUSEEVENT_LEAVE , bEnable);
	Enable( IDC_CHECK_MOUSEEVENT_OUT, bEnable);
	Enable( IDC_CHECK_MOUSEEVENT_OVER, bEnable);
	Enable(IDC_CHECK_MOUSEEVENT_DOWN, bEnable); 
}

// CModifyPage message handlers

BOOL CModifyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
    m_pConf = ConfigManager::Instance()->GetDefaultSessionConf();
	CButton*pbtn= (CButton*)GetDlgItem(IDC_CHECK_USEDEFAULT);
	SetCheck(IDC_CHECK_USEDEFAULT,m_pConf->bModifyDef);
    InitDefault(!m_pConf->bModifyDef);
	return TRUE; 
}

 void  CModifyPage::InitDefault(BOOL bEnable)
 {
	 SetCheckEx( IDC_CHECK_SCRIPT,m_pConf->bCScript ,bEnable);
	 SetCheckEx( IDC_CHECK_NOSCRIPT, m_pConf->bCNoScript , bEnable);
	 SetCheckEx( IDC_CHECK_LOAD,m_pConf->bCLoad , bEnable);
	 SetCheckEx( IDC_CHECK_UNLOAD, m_pConf->bCUnload ,bEnable );
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_CLICK, m_pConf->bCMouseClick , bEnable);
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_ENTER ,m_pConf->bCMouseEnter , bEnable);
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_LEAVE, m_pConf->bCMouseLeave, bEnable);
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_OUT, m_pConf->bCMouseOut, bEnable);
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_OVER, m_pConf->bCMouseOver ,bEnable );
	 SetCheckEx( IDC_CHECK_MOUSEEVENT_DOWN,m_pConf->bCMouseDown , bEnable);
 }

 BOOL CModifyPage::OnApply()
 {
	 // TODO: Add your specialized code here and/or call the base class
	 m_pConf->bModifyDef = GetCheck(IDC_CHECK_USEDEFAULT);
	 m_pConf->bCScript = GetCheck(IDC_CHECK_SCRIPT);
	 m_pConf->bCNoScript = GetCheck(IDC_CHECK_NOSCRIPT);
	 m_pConf->bCLoad =  GetCheck(IDC_CHECK_LOAD);
	 m_pConf->bCUnload =  GetCheck(IDC_CHECK_UNLOAD);
	 m_pConf->bCMouseClick =  GetCheck(IDC_CHECK_MOUSEEVENT_CLICK);
	 m_pConf->bCMouseEnter =  GetCheck(IDC_CHECK_MOUSEEVENT_ENTER);
	 m_pConf->bCMouseLeave = GetCheck(IDC_CHECK_MOUSEEVENT_LEAVE);
	 m_pConf->bCMouseOut = GetCheck(IDC_CHECK_MOUSEEVENT_OUT);
	 m_pConf->bCMouseOver = GetCheck(IDC_CHECK_MOUSEEVENT_OVER);
	 m_pConf->bCMouseDown = GetCheck(IDC_CHECK_MOUSEEVENT_DOWN);
	 return CPropertyPage::OnApply();
 }
