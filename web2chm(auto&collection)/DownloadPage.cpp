// DownloadPage.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "DownloadPage.h"
#include "defs.h"
#include "afxpriv.h"
#include "configmanager.h"
#include ".\downloadpage.h"
#include "DownloadSheet.h"
#include "w2ebook_utils.h"
#include "PrimarySheet.h"
#ifndef _DEBUG
#include "Register.h"
#endif
// CDownloadPage dialog
static CPrimarySheet *pSheet=NULL;
IMPLEMENT_DYNAMIC(CDownloadPage, CPropertyPage)
CDownloadPage::CDownloadPage()
	: CPropertyPage(CDownloadPage::IDD)
	, m_MaxFileNum(0)
	, m_incStrPath(_T(""))
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle  = "Third";
	m_psp.pszHeaderSubTitle = "Set download  option";
	m_pConf =NULL;
	m_pSysConf = NULL;
}

CDownloadPage::~CDownloadPage()
{
	//pSheet = NULL;
}

void CDownloadPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAX_TOTLE_SIZE, m_TotalSizeBox);
	DDX_Control(pDX, IDC_MAX_FILE_SIZE, m_FileSizeBox);
	DDX_Text(pDX, IDC_MAX_FILE_NUMBER, m_MaxFileNum);
	DDV_MinMaxInt(pDX, m_MaxFileNum, 1, 1000000);
	DDX_Text(pDX, IDC_PATH_STRING_PART, m_incStrPath);
}


BEGIN_MESSAGE_MAP(CDownloadPage, CPropertyPage)
	ON_BN_CLICKED(IDC_LIMIT, OnBnClickedLimit)
	ON_BN_CLICKED(IDC_DOWNLOADPAGES,OnUpDateUI) 
	ON_BN_CLICKED(IDC_DOWNLOADWEBSIT2,OnUpDateUI) 
	ON_BN_CLICKED(IDC_DOWNLOADWEBHOST2,OnUpDateUI) 
	ON_BN_CLICKED(IDC_INCLUDE_STRING,OnUpDateUI) 
END_MESSAGE_MAP()

void CDownloadPage::OnBnClickedLimit()
{
	BOOL bEnable = GetRatioCheck(IDC_LIMIT);
	GetDlgItem(IDC_MAX_TOTLE_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MAX_FILE_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MAX_FILE_NUMBER)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TOTAL_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_FILE_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MAX_FILES)->EnableWindow(bEnable);
}
void CDownloadPage::EnablePathInclude(BOOL bEnable)
{
	GetDlgItem(IDC_PATH_STRING_PART)->EnableWindow(bEnable);
}
void CDownloadPage::OnUpDateUI()
{
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl  *pSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_DEEP);
	pSpin->SetRange(FIRST_LAY,END_LAY);
	
	CButton *pBt = (CButton *)GetDlgItem(IDC_DOWNLOADPAGES);
	EnableDeepth(pBt->GetCheck());

	pBt = (CButton *)GetDlgItem(IDC_INCLUDE_STRING);
	EnablePathInclude(pBt->GetCheck());

}
// CDownloadPage message handlers

BOOL CDownloadPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CWnd *pWnd=NULL;
	pSheet = NULL;
	pWnd  =  GetParent();
	if(pWnd->IsKindOf(RUNTIME_CLASS(CPropertySheet)))
		pSheet = (CPrimarySheet *)pWnd ;

	m_pConf = ConfigManager::Instance()->GetDefaultSessionConf();
	m_pSysConf = ConfigManager::Instance()->GetSystemConfig();
	InitMode(m_pSysConf->dwWorkMode,m_pConf->dwDownMode);

	OnUpDateUI();
	CSpinButtonCtrl  *pSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_DEEP);
	pSpin->SetPos(m_pConf->dwDownLay);
	CButton *pBt = (CButton *)GetDlgItem(IDC_CHECK_EDIT);
	pBt->SetCheck(m_pConf->bEnableEdit);

	pBt = (CButton *)GetDlgItem(IDC_INCLUDE_STRING);
	if(pBt->GetCheck())
	{
		CString url;
		if(m_pSysConf->bWarzard && pSheet && ::IsWindow(pSheet->m_NewPage.m_hWnd))
		{
			
			if(m_pSysConf->dwWorkMode == AUTO_MODE)
				pSheet->m_NewPage.GetDlgItemText(IDC_COMBO_URL ,url);
			else
				url = TAnchorManager::Instance()->GetFirstUrl();
			 
			
		}else
			url = m_pConf->strIncPath;
		
		SetDlgItemText(IDC_PATH_STRING_PART,url);
	}
#ifndef _DEBUG
	if( 0 /*HasNoRegKey()*/)
	{
		EnableLimit(FALSE);
		GetDlgItem(IDC_MAX_FILE_NUMBER)->SetWindowText("100");
		GetDlgItem(IDC_LIMIT)->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_LIMIT))->SetCheck(TRUE);
		m_TotalSizeBox.SetWindowText("10");
		m_FileSizeBox.SetWindowText("1");

		m_pConf->dwMaxSize = 10;
		m_pConf->dwMaxFileNum = 100;
		m_pConf->dwMaxFileSize = 1;
		m_pConf->bEnableLimit = FALSE;
		return 0;
	}else
	{
		EnableLimit(m_pConf->bEnableLimit);
	}
#else
	EnableLimit(m_pConf->bEnableLimit);
#endif

	//Max Total Size
	ModifyCmbEdit(&m_TotalSizeBox,ES_NUMBER);
	AddComboxString(&m_TotalSizeBox,IDS_MAX_SIZE);
	CString strVal;
	strVal.Format("%d",(int)m_pConf->dwMaxSize);
	m_TotalSizeBox.SetWindowText(strVal);
	m_TotalSizeBox.SetEditSel(-1,0);
	//Max File Size
	ModifyCmbEdit(&m_FileSizeBox,ES_NUMBER);
	AddComboxString(&m_FileSizeBox,IDS_MAX_FILESIZE);
	strVal.Format("%d",(int)m_pConf->dwMaxFileSize);
	m_FileSizeBox.SetWindowText(strVal);
	m_FileSizeBox.SetEditSel(-1,0);
    //Max File Number
	strVal.Format("%d",(int)m_pConf->dwMaxFileNum);
	GetDlgItem(IDC_MAX_FILE_NUMBER)->SetWindowText(strVal);
	return TRUE; 
}

void CDownloadPage::InitMode(int WorkMode,int DownloadMode)
{
	if(WorkMode == 0)
	{
		CButton *pBnt = (CButton *)GetDlgItem(IDC_DOWNLOADPAGES);
		pBnt->SetCheck(TRUE); 
		GetDlgItem(IDC_DOWNLOADWEBHOST2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWNLOADWEBSIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_INCLUDE_STRING)->EnableWindow(FALSE);
		m_pConf->dwDownMode = 1;
	}
	else
	{
		CButton *pBnt;
		switch(DownloadMode)
		{
		case 1:
			pBnt = (CButton *)GetDlgItem(IDC_DOWNLOADPAGES);
			pBnt->SetCheck(TRUE);
			break;
		case 2:
			pBnt = (CButton *)GetDlgItem(IDC_DOWNLOADWEBHOST2);
			pBnt->SetCheck(TRUE);
			break;
		case 3:
			pBnt = (CButton *)GetDlgItem(IDC_DOWNLOADWEBSIT2);
			pBnt->SetCheck(TRUE);
			break;
		case 4:
			pBnt = (CButton *)GetDlgItem(IDC_INCLUDE_STRING);
			pBnt->SetCheck(TRUE);
			break;
		}
	}
}

void CDownloadPage::EnableDeepth(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_DEEP)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_DEEP)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_DEEP)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EDIT)->EnableWindow(bEnable);
}

void CDownloadPage::EnableLimit(BOOL bEnable )
{
	CButton *pbtn = (CButton *)GetDlgItem(IDC_LIMIT);
	pbtn->SetCheck(bEnable);
	GetDlgItem(IDC_MAX_TOTLE_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MAX_FILE_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MAX_FILE_NUMBER)->EnableWindow(bEnable);
}

BOOL   CDownloadPage::CheckData()
{
	m_pConf->dwDownLay = 1;
	m_pConf->bEnableEdit = 0;
	if(GetRatioCheck(IDC_DOWNLOADWEBHOST2))
		m_pConf->dwDownMode = 2;
	else if(GetRatioCheck(IDC_DOWNLOADWEBSIT2))
		m_pConf->dwDownMode = 3;
	else if(GetRatioCheck(IDC_INCLUDE_STRING))
	{
		m_pConf->dwDownMode  =4;
		this->GetDlgItemText( IDC_PATH_STRING_PART,m_pConf->strIncPath);
		if(m_pConf->strIncPath.IsEmpty())
		{
			AfxMessageBox("In this download mode,string can't be emply ");
			return FALSE;
		}

	}
	else
	{
		m_pConf->dwDownMode = 1;
		CSpinButtonCtrl  *pSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_DEEP);
		m_pConf->dwDownLay = pSpin->GetPos() ;
		m_pConf->bEnableEdit = GetRatioCheck(IDC_CHECK_EDIT);
	}

	m_pConf->bEnableLimit = GetRatioCheck(IDC_LIMIT);
	if(m_pConf->bEnableLimit)
	{
		CString strVal;
		m_TotalSizeBox.GetWindowText(strVal);
		m_pConf->dwMaxSize =(DWORD) StrToInt(strVal);

		m_FileSizeBox.GetWindowText(strVal);
		m_pConf->dwMaxFileSize =(DWORD) StrToInt(strVal);

		m_pConf->dwMaxFileNum =GetDlgItemInt(IDC_MAX_FILE_NUMBER);
	}
	return TRUE;
}


BOOL CDownloadPage::OnApply()
{
	if(!CheckData())
	  return FALSE;
	return CPropertyPage::OnApply();
}

LRESULT CDownloadPage::OnWizardNext()
{
	 if(!CheckData())
		return 1;
	return CPropertyPage::OnWizardNext();
}

BOOL CDownloadPage::OnSetActive()
{
	CWnd *pParent = GetParent();
	if(pParent->IsKindOf(RUNTIME_CLASS(CPrimarySheet)))
	{
		CPrimarySheet *pSheet=(CPrimarySheet *)pParent;
		if(pSheet!=NULL)
			pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);
	}
	return CPropertyPage::OnSetActive();
}
