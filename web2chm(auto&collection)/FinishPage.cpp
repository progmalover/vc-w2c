// FinishPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FinishPage.h"
#include "PrimarySheet.h"
// CFinishPage 对话框

static CPrimarySheet *pSheet=NULL;
IMPLEMENT_DYNAMIC(CFinishPage, CPropertyPage)
CFinishPage::CFinishPage()
: CPropertyPage(CFinishPage::IDD)
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle  = "Finish";
	m_psp.pszHeaderSubTitle = "Select work mode";
	//m_psp.dwFlags |= PSP_HIDEHEADER;
}

CFinishPage::~CFinishPage()
{
}

void CFinishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFinishPage, CPropertyPage)
END_MESSAGE_MAP()


// CFinishPage 消息处理程序
BOOL CFinishPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	pSheet = (CPrimarySheet*)GetParent();
	return TRUE;  
}

BOOL CFinishPage::OnSetActive()
{
	if(pSheet!=NULL)
		pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);
	return CPropertyPage::OnSetActive();
}


BOOL CFinishPage::OnWizardFinish()
{
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
	return CPropertyPage::OnWizardFinish();
}

void    CFinishPage::CreateAutoTask(TWebSession *tpSession,int dwThrdNum)
{
	CString strURL  = pSheet->m_NewPage.m_URL,
		strPath = TaskName(pSheet->m_NewPage.m_strPath);
	strURL.MakeLower();
	FixUrlStr(strURL); //Add "http:"
	tpSession->Create(strURL,strPath,GA_dwConnectTrdsArray[dwThrdNum]);
}

void    CFinishPage::CreateAnchorTask(TWebSession *tpSession,int dwThrdNum)
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
	TAnchorManager::Instance()->GetAllUrls(list);	//pages 中包含的 url...
	tpSession->PutToFileBuffer( list ,ALL_REF_IN_PAGE,TAG_SRC | TAG_URL);
	/*if(list.size())
	tpSession->Create(*list.begin(),strPath,GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);
	else*/
	tpSession->Create(CString(),strPath,GA_dwConnectTrdsArray[dwThrdNum]);
}
