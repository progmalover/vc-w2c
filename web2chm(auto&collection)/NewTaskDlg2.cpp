// NewTaskDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "NewTaskDlg2.h"
#include ".\tanchormanager.h"
#include "tcatalogbuilder.h"
#include "httpapi.h"
#include "shlwapi.h"
#include "CProgressList.h"
#include "Defs.h"
#include "Simiebar.h"
#include "ui.h"
using namespace ::ui;

// CNewTaskDlg2 dialog

IMPLEMENT_DYNAMIC(CNewTaskDlg2, CDialog)
CNewTaskDlg2::CNewTaskDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTaskDlg2::IDD, pParent)
{
}

CNewTaskDlg2::~CNewTaskDlg2()
{
}

void CNewTaskDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCATIONEDIT2, m_TargetDir);
}


BEGIN_MESSAGE_MAP(CNewTaskDlg2, CDialog)
	ON_BN_CLICKED(IDC_TASKSTART2, OnBnClickedTaskstart2)
	ON_BN_CLICKED(IDC_TASKCONF2, OnBnClickedTaskconf2)
	ON_BN_CLICKED(IDC_EXPLORE, OnBnClickedExplore)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ANCHOR_HELP, OnBnClickedButtonAnchorHelp)
END_MESSAGE_MAP()


// CNewTaskDlg2 message handlers
BOOL CNewTaskDlg2::CheckPathname(CString strPathName)
{
	if(!strPathName.IsEmpty())
	{
		CString strP =  FileStripFileName(strPathName),
			strN =  FileStripPath(strPathName);
		if(::IsValidFileName(strN) &&::IsValidPath(strPathName))
		{
			if(PathFileExists(strPathName))
				DeleteFile(strPathName);
			return TRUE;
		}
	}
	AfxMessageBox(IDS_E_SELECT_DIRECTORY);
	GotoDlgCtrl(GetDlgItem(IDC_LOCATIONEDIT2));
	return FALSE;
}

void CNewTaskDlg2::OnBnClickedTaskstart2()
{
	/*Test Optin*/
	pPRIVATE_CONFIG  pPrvCf;
	pPrvCf = ConfigManager::Instance()->GetDefaultSessionConf();
	if(pPrvCf && pPrvCf->bEnableLimit)
	{
		if(pPrvCf->dwMaxFileNum <= 0
			||pPrvCf->dwMaxFileSize <= 0
			||pPrvCf->dwMaxSize <= 0)
		{
			AfxMessageBox(IDS_LIMIT_ERROR);
			return ;
		}
	}
	// TODO: Add your control notification handler code here
	//1.build anchor pages....
    //a.create file in disk,and then,load page ,and make ref url....
	//2.put anchor urls to  url bufer,and set the lay val 1....
	//3.start run session.....

	CString	strPath;
	GetDlgItemText(IDC_LOCATIONEDIT2,strPath);
	strPath.Trim();
	if(!CheckPathname(strPath))//验证是否有效的文件路径
		return;


 	if(GP_CurrentSession)
		GP_CurrentSession->UnActiveTaskUI();

	//FreeAllTask();
/* make path name .... */
	CString tskName = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\')-1);
	CString tskDirName;
	tskName = tskName.Left(tskName.Find('.'));
	tskDirName.Format("\\%s_WEB_SRC\\",tskName);
	tskName +=  tskDirName;
    strPath.Insert(strPath.ReverseFind('\\')+1,tskName);
 
/*create anchor start page*/
	if(TAnchorManager::Instance()->GetPageCount() <= 0)
	{
		AfxMessageBox(IDS_E_NO_ANCHOR);
		return ;
	}

	/**/
	CString strDir = strPath.Left(strPath.ReverseFind('\\')+1);
	TCatalogBuilder::BuilderIndex(strDir);
	TCatalogBuilder::BuilderStartPages(strDir);
	
	/*return ;*/
/*得到系统配置数据，创建对应的task*//*put all the urls to downfilebuffer*/
	pSYSTEM_CONFIG  pSysCf;
	pSysCf = ConfigManager::Instance()->GetSystemConfig();

	 
	/*ui pregress proprepare..*/
	GP_TskPrgList->Lock();
	GP_TskPrgList->ClearAllCellInf();
	GP_TskPrgList->UnLock();
	
	/*建立新的session*/

	FreeAllTask();

	
	TWebSession *tpSession = new TWebSession() ;

	int pageCount = TAnchorManager::Instance()->GetPageCount(); 
	tpSession->SetRootUrl(CString());//初始化 
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
	tpSession->Create(CString(),strPath,GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);

	tpSession->ActiveTaskUI();
	tpSession->TASK_Start();

	AddToSessionTree(GP_CurrentSession = tpSession);
/*Add To Session List, By  Gloable List Option Funtions*/
 
	OnOK();
	/*m_TargetDir.AddCurrentText();
	m_TargetDir.Save();*/
}

void CNewTaskDlg2::OnBnClickedTaskconf2()
{

	//CTaskConfig   dlg;
	//dlg.DoModal();
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	CMenu *pSub = pMenu->GetSubMenu(1);
	
	CRect rc;
	GetDlgItem(IDC_TASKCONF2)->GetWindowRect(rc);
	pSub->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rc.left,
		rc.bottom,  AfxGetMainWnd());
}

void CNewTaskDlg2::OnBnClickedExplore()
{
	CFileDialog   cDlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Job Files (*.job)|*.job||");
	int ret = cDlg.DoModal();

	if(ret == IDOK)
	{
		CString dir = cDlg.GetPathName();
		CString ext = PathFindExtension(dir);
	 
		if(ext.CompareNoCase(".job") !=0)
			dir += ".job";
			
		 
		SetDlgItemText( IDC_LOCATIONEDIT2, dir);
	}
}

void CNewTaskDlg2::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CNewTaskDlg2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	TCatalogBuilder::InitBuilder();

//	m_TargetDir.Init("Select Output Directory");
//	m_TargetDir.Load();
//	if(m_TargetDir.GetCount())
//	{
//		m_TargetDir.SetCurSel(0);
//	}
//	else
//	{
////
//	}
	int pages = TAnchorManager::Instance()->GetPageCount();
	CString info;
	if(pages>0)
		info.Format(IDS_ANCHOR_COUNT,pages);
	else 
		info.LoadString(IDS_ANCHOR_ZERO);
	SetDlgItemText(IDC_STATIC_STATE,info);

	return TRUE;  
}

void CNewTaskDlg2::OnBnClickedButtonAnchorHelp()
{
	AfxMessageBox("Show how to use anchor mode!");
}
