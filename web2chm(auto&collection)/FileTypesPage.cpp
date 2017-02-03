// FileTypesPage.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "FileTypesPage.h"
#include ".\filetypespage.h"
#include "DownloadSheet.h"
#include "PrimarySheet.h"
#include "ui.h"

using namespace ui;
// CFileTypesPage dialog
static CPrimarySheet *pSheet=NULL;
IMPLEMENT_DYNAMIC(CFileTypesPage, CPropertyPage)
CFileTypesPage::CFileTypesPage(BOOL bShowAnvance)
: CPropertyPage(CFileTypesPage::IDD)
,m_pConf(NULL)
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle  = "Finish";
	m_psp.pszHeaderSubTitle = "Select download file types";
	//m_psp.dwFlags |= PSP_HIDEHEADER;
}

CFileTypesPage::~CFileTypesPage()
{
}

void CFileTypesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileTypesPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnBnClickedButtonAdvance)
END_MESSAGE_MAP()


// CFileTypesPage message handlers

BOOL CFileTypesPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pConf = ConfigManager::Instance()->GetDefaultSessionConf();
	GetDlgItem(IDC_HTMLTEXT)->EnableWindow(FALSE);
	SetRatioCheck(IDC_HTMLTEXT,TRUE); //force to be true...
	InitModify();
	
	CWnd *pParent = GetParent();
	if(pParent->IsKindOf(RUNTIME_CLASS(CPrimarySheet)))
		pSheet = (CPrimarySheet *)pParent;
	else
		pSheet = NULL;
	GetDlgItem(IDC_BUTTON_ADVANCE)->ShowWindow(pSheet!=NULL);
	return TRUE; 
}

BOOL  CFileTypesPage::InitModify()
{
	SetRatioCheck(IDC_IMAGES,m_pConf->enableTypeArray[1]);
	SetRatioCheck(IDC_ZIP,m_pConf->enableTypeArray[2]);
	SetRatioCheck(IDC_MUSIC,m_pConf->enableTypeArray[3]);
	SetRatioCheck(IDC_SWF,m_pConf->enableTypeArray[4]);
	SetRatioCheck(IDC_PDF,m_pConf->enableTypeArray[5]);
	SetRatioCheck(IDC_CHM,m_pConf->enableTypeArray[6]);
	return TRUE;
}

void  CFileTypesPage::GetModify()
{
	m_pConf->enableTypeArray[0] = 1;
	m_pConf->enableTypeArray[1] =  GetRatioCheck(IDC_IMAGES);
	m_pConf->enableTypeArray[2] =  GetRatioCheck(IDC_ZIP);
	m_pConf->enableTypeArray[3] =  GetRatioCheck(IDC_MUSIC);
	m_pConf->enableTypeArray[4] =  GetRatioCheck(IDC_SWF);
	m_pConf->enableTypeArray[5] =  GetRatioCheck(IDC_PDF);
	m_pConf->enableTypeArray[6] =  GetRatioCheck(IDC_CHM);
}

BOOL CFileTypesPage::OnApply()
{
	GetModify();
	return CPropertyPage::OnApply();
}

void CFileTypesPage::OnBnClickedButtonAdvance()
{
	CDownloadSheet sheet("Advance Options",FALSE);
	sheet.DoModal();
}

//
BOOL CFileTypesPage::OnSetActive()
{
	if(pSheet!=NULL)
		pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	return CPropertyPage::OnSetActive();
}

BOOL CFileTypesPage::OnWizardFinish()
{
	if(pSheet!=NULL)
	{
		GetModify();
		if(GP_CurrentSession!=NULL)
			GP_CurrentSession->UnActiveTaskUI();
		GP_TskPrgList->Lock();
		GP_TskPrgList->ClearAllCellInf();
		GP_TskPrgList->UnLock();
		FreeAllTask();
		pSYSTEM_CONFIG pSyscf = ConfigManager::Instance()->GetSystemConfig();
		TWebSession *tpSession = new TWebSession() ;
		if(pSyscf->dwWorkMode == AUTO_MODE)
			CreateAutoTask(tpSession,pSyscf->dwThrdNum);
		else
			CreateAnchorTask(tpSession,pSyscf->dwThrdNum);
		tpSession->ActiveTaskUI();
		tpSession->TASK_Start();
		AddToSessionTree(GP_CurrentSession = tpSession);
	}
	return CPropertyPage::OnWizardFinish();
}

void    CFileTypesPage::CreateAutoTask(TWebSession *tpSession,int dwThrdNum)
{
	CString strURL  = pSheet->m_NewPage.m_URL,
		strPath = TaskName(pSheet->m_NewPage.m_strPath);
	strURL.MakeLower();
	FixUrlStr(strURL); //Add "http:"
	tpSession->Create(strURL,strPath,GA_dwConnectTrdsArray[dwThrdNum]);
}

void    CFileTypesPage::CreateAnchorTask(TWebSession *tpSession,int dwThrdNum)
{
	CString strPath = TaskName(pSheet->m_NewPage.m_strPath);
	CString strDir = strPath.Left(strPath.ReverseFind('\\')+1);
	TCatalogBuilder::BuilderIndex(strDir);
	TCatalogBuilder::BuilderStartPages(strDir);
	int pageCount = TAnchorManager::Instance()->GetPageCount(); 
	tpSession->SetRootUrl(CString());
	if(pageCount)
	{
		URL_LIST pageList;
		TAnchorManager::Instance()->GetPageUrls(pageList);	
		tpSession->FillUrlsBuffer(pageList);
	}

	URL_LIST list;
	TAnchorManager::Instance()->GetAllUrls(list);	//pages ÖÐ°üº¬µÄ url...
	tpSession->PutToFileBuffer( list ,ALL_REF_IN_PAGE,TAG_SRC | TAG_URL);
	/*if(list.size())
	tpSession->Create(*list.begin(),strPath,GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);
	else*/
	tpSession->Create(CString(),strPath,GA_dwConnectTrdsArray[dwThrdNum]);
}
