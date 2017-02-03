// NewTaskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "shlwapi.h"
#include "NewTaskDlg.h"
#include ".\newtaskdlg.h"

#include "ConnectTestDlg.h"
#include "HttpApi.h"
#include "CProgressList.h"
#include "Defs.h"
#include "Simiebar.h"
#include "TCatalogBuilder.h"
#include "ui.h"

using namespace ::ui;
IMPLEMENT_DYNAMIC(CNewTaskDlg, CDialog)
CNewTaskDlg::CNewTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTaskDlg::IDD, pParent)
	, m_URL(_T(""))
{
}

CNewTaskDlg::~CNewTaskDlg()
{
}

void CNewTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TESTPROGRESS, m_ConnectProgress);
	DDX_Control(pDX, IDC_LOCATIONEDIT, m_TargetDir);
	DDX_Control(pDX, IDC_COMBO_URL, m_URLBox);
}


BEGIN_MESSAGE_MAP(CNewTaskDlg, CDialog)
	ON_BN_CLICKED(IDC_CONNCTTEST, OnBnClickedConncttest)
	ON_BN_CLICKED(IDC_TASKCONF, OnBnClickedTaskconf)
	ON_BN_CLICKED(IDC_EXPLORE, OnBnClickedExplore)
	ON_MESSAGE(WM_DOWNPROGRESS,OnDownLoadProgress)
	ON_BN_CLICKED(IDC_TASKSTART, OnBnClickedTaskstart)
END_MESSAGE_MAP()




void CNewTaskDlg::OnBnClickedConncttest()
{
	if(!CheckURL())
		return ;
	SpreadDlg();
	CWnd *pbt = GetDlgItem(IDC_CONNCTTEST);
	pbt->EnableWindow(FALSE);
	
	HRESULT  hRet = ConnectTest(this,m_URL,m_ConnectInf);
    CString  strCaption;

	switch(hRet)
	{
	case  URL_OPEN_FAILURE:
		 strCaption.LoadString(IDS_URL_OPEN_FAILURE);
		break;
	case  INET_INIT_FAILURE:
		 strCaption.LoadString(IDS_INET_INIT_FAILURE);
		break;
	case  BUFF_FILE_FAILURE:
		strCaption.LoadString(IDS_BUFF_FILE_FAILURE);
		break;
	case  S_OK:
		{
			CConnectTestDlg  dlg;
			dlg.SetConnectInf( &m_ConnectInf);
			dlg.DoModal();
		}
		break;
	}
	if(hRet != S_OK)
	{
		AfxMessageBox(strCaption);
		CWnd *pwnd = GetDlgItem(IDC_TASKSTART);
		pwnd->EnableWindow(FALSE);
	}else
	{
		CWnd *pwnd = GetDlgItem(IDC_TASKSTART);
		pwnd->EnableWindow(TRUE);
	}

	SubDlg();
	pbt->EnableWindow(TRUE);
}

BOOL CNewTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//收紧dlg
	SubDlg();
	TCatalogBuilder::InitBuilder();

	m_URLBox.Init("Select Output URL");
	m_URLBox.Load();
	CString		strAddress;
	CSimIEBar::Instance()->m_wndAddress.GetWindowText(strAddress);
	strAddress.Trim();
	if(strAddress.IsEmpty())
	{
		if(m_URLBox.GetCount())
			m_URLBox.SetCurSel(0);
	}
	else
		m_URLBox.SetWindowText(strAddress);
	return TRUE;  
}

void CNewTaskDlg::SaveHistoryBox()
{
//	m_TargetDir.AddCurrentText();
//	m_TargetDir.Save();
	m_URLBox.AddCurrentText();
	m_URLBox.Save();
}

//展开 dlg
BOOL CNewTaskDlg::SpreadDlg(void)
{
	CRect   dlgRect, prsRect;
	if(!IsWindowEnabled())
		return FALSE;

	if(!m_ConnectProgress.IsWindowEnabled())
		return FALSE;

	GetWindowRect(&dlgRect);

	m_ConnectProgress.GetWindowRect(&prsRect);
	dlgRect.bottom += prsRect.Height();
	MoveWindow(&dlgRect,TRUE);

	m_ConnectProgress.SetRange(0,100);
	m_ConnectProgress.SetPos(0);
	
	m_ConnectProgress.RedrawWindow();
	return TRUE;
}
//收缩 dlg
BOOL CNewTaskDlg::SubDlg(void)
{
	CRect   dlgRect, prsRect;
	if(!IsWindowEnabled())
		return FALSE;

	if(!m_ConnectProgress.IsWindowEnabled())
		return FALSE;

	GetWindowRect(&dlgRect);
	m_ConnectProgress.GetWindowRect(&prsRect);
	dlgRect.bottom -= prsRect.Height();

	MoveWindow(&dlgRect,TRUE);
	return TRUE;
}

void CNewTaskDlg::OnBnClickedTaskconf()
{
	CMenu *pMenu = AfxGetMainWnd()->GetMenu();
	CMenu *pSub = pMenu->GetSubMenu(1);
	
	CRect rc;
	GetDlgItem(IDC_TASKCONF)->GetWindowRect(rc);
	pSub->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, rc.left,
		rc.bottom,  AfxGetMainWnd());
}

 

void CNewTaskDlg::OnBnClickedExplore()
{
	CFileDialog   cDlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Job Files (*.job)|*.job||");

	if(cDlg.DoModal() == IDOK)  
	{
		CString dir = cDlg.GetPathName();
		CString ext = PathFindExtension(dir);
	 
		if(ext.CompareNoCase(".job") !=0)
			dir += ".job";
			
		SetDlgItemText( IDC_LOCATIONEDIT, dir);
	}
}

HRESULT CNewTaskDlg::OnDownLoadProgress(WPARAM mparam,LPARAM lparam)
{
	m_ConnectProgress.SetRange(0,mparam);
	m_ConnectProgress.SetPos(lparam);
	return S_OK;
}

void CNewTaskDlg::OnOK()
{
	CDialog::OnOK();
}



BOOL  CNewTaskDlg::CheckURL()
{
	m_URLBox.GetWindowText(m_URL);
	m_URL.Trim();
	if(m_URL.IsEmpty()||(!IsValidURL(m_URL)))
	{
		AfxMessageBox(IDS_E_URL);
		//m_URLBox.SetWindowText("");
		GotoDlgCtrl(GetDlgItem(IDC_COMBO_URL));
		return FALSE;
	}
	m_URL.MakeLower();
	FixUrlStr(m_URL); //Add "http:"
	return TRUE;
}

BOOL CNewTaskDlg::CheckPathname(CString strPathName)
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
	GotoDlgCtrl(GetDlgItem(IDC_LOCATIONEDIT));
	return FALSE;
}

TWebSession   *GP_CurrentSession = NULL;
void CNewTaskDlg::OnBnClickedTaskstart()
{
	if(!CheckURL()) //检测URL的有效性
		return;
	CString		strPath;
	GetDlgItemText(IDC_LOCATIONEDIT,strPath);
	strPath.Trim();
    if(!CheckPathname(strPath))//检测文件路径以及文件的有效性
		return;
/* make path name .... */
	CString strPathEx = TaskName(strPath);
/*得到系统配置数据，创建对应的task*/
	pSYSTEM_CONFIG  pSysCf;
	pSysCf = ConfigManager::Instance()->GetSystemConfig();
	if(GP_CurrentSession)
		GP_CurrentSession->UnActiveTaskUI();
	/*ui pregress proprepare..*/
	GP_TskPrgList->Lock();
	GP_TskPrgList->ClearAllCellInf();
	GP_TskPrgList->UnLock();
	
	/*建立新的session*/
	FreeAllTask();

	TWebSession *tpSession = new TWebSession() ;
	tpSession->Create(m_URL,strPathEx,GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);
	tpSession->ActiveTaskUI();
	tpSession->TASK_Start();

	AddToSessionTree(GP_CurrentSession = tpSession);
	SaveHistoryBox();
	OnOK();
}


