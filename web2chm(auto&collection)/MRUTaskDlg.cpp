// MRUTaskDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "web2chm.h"
#include "MRUTaskDlg.h"
#include ".\mrutaskdlg.h"
#include "historylist.h"
#include "afxpriv.h"
#include "defs.h"
#include "httpapi.h"
#include "cprogresslist.h"
#include "ui.h"
#include "jobfilereader.h"
using namespace ui;
#define   CAPTION_SECTIN 3
static char *szCaption[]=
{
	"URL",
	"Save To",
	"Project Name"
};

static  int Width[]={270,80,80};
// CMRUTaskDlg 对话框

IMPLEMENT_DYNCREATE(CMRUTaskDlg, CDialog)

CMRUTaskDlg::CMRUTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMRUTaskDlg::IDD)
{
}

CMRUTaskDlg::~CMRUTaskDlg()
{
}

void CMRUTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASKLIST, m_TaskList);
}

BOOL CMRUTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	LV_COLUMN  lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	m_TaskList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_TaskList.ModifyStyle(0,WS_CLIPSIBLINGS | WS_CLIPCHILDREN,0);

	for(int i = 0; i<CAPTION_SECTIN; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = szCaption[i];
		lvc.cx = Width[i];
		lvc.fmt = LVCFMT_LEFT;
		m_TaskList.InsertColumn(i,&lvc);
	}

	SESSION_INF_LIST & list = CHistoryList::Instance()->GetDataList();
	if(list.size())
	{
		int i= 0;
		for(SESSION_INF_LIST::iterator it = list.begin();it != list.end();it++,i++)
		{
			 SessionInf &inf = *it;
	
			 m_TaskList.InsertItem(i,inf.m_rootUrl);
			 m_TaskList.SetItemText(i,1,inf.m_TaskFile);
			 m_TaskList.SetItemText(i,2,inf.m_TaskName);	 
			 m_TaskList.SetItemData(i,(DWORD_PTR)&inf);

		}
	}
	OnIdleUpDataUI(0,0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CMRUTaskDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    //ON_UPDATE_COMMAND_UI(IDC_BUTTON1,OnDeleteUpdata)
	//ON_WM_UPDATEUISTATE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpDataUI)
	ON_NOTIFY(NM_DBLCLK, IDC_TASKLIST, OnNMDblclkTasklist)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LOADMISSION, &CMRUTaskDlg::OnLoadmission)
	ON_COMMAND(ID_OPENDIRECTORY, &CMRUTaskDlg::OnOpendirectory)
	
END_MESSAGE_MAP()
 
LRESULT CMRUTaskDlg::OnIdleUpDataUI(WPARAM ,LPARAM)
{
	 
	int  curItem =  m_TaskList.GetNextItem(-1,LVNI_FOCUSED);

	if(curItem >= 0)
	{
		CWnd *pwnd = GetDlgItem(IDC_BUTTON1);
		pwnd->EnableWindow(TRUE);
	}else
	{
		CWnd *pwnd = GetDlgItem(IDC_BUTTON1);
		pwnd->EnableWindow(FALSE);
	}
	
	return S_OK;
}

//int iitem,isubitem;
void CMRUTaskDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CMRUTaskDlg::OnBnClickedButton1()
{
	//TODO: Add your control notification handler code here

	SESSION_INF_LIST & list = CHistoryList::Instance()->GetDataList();
	int  curItem =  m_TaskList.GetNextItem(-1,LVIS_SELECTED);
	//int curItem = m_TaskList.GetHotItem();
	if(curItem >= 0)
	{
		SessionInf inf = *(SessionInf *)m_TaskList.GetItemData(curItem);
		list.remove(inf);
		m_TaskList.DeleteItem(curItem);
	}
	curItem =  m_TaskList.GetNextItem(-1,LVNI_FOCUSED);
	if(curItem < 0) curItem = 0;
    if(m_TaskList.GetItemCount())
	m_TaskList.SetItemState(curItem, LVIS_SELECTED, LVIS_SELECTED);

	OnIdleUpDataUI(0,0);
}


void CMRUTaskDlg::OnNMDblclkTasklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int  curItem =  m_TaskList.GetNextItem(-1,LVIS_SELECTED);
	//int curItem = m_TaskList.GetHotItem();
	if(curItem >= 0)
	{
		SessionInf inf = *(SessionInf *)m_TaskList.GetItemData(curItem);
		
		CString fileName = inf.m_TaskFile ;
		LPSTR lpStr = fileName.GetBuffer();
		fileName.ReleaseBuffer();
		
		::PathRemoveFileSpec(lpStr);
		::ShellExecute(m_hWnd,"explore",NULL,NULL,lpStr,SW_SHOWDEFAULT);
	}
	*pResult = 0;
}

void CMRUTaskDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint pt)
{
	// TODO: Add your message handler code here
	int  curItem =  m_TaskList.GetNextItem(-1,LVIS_SELECTED);
	if(curItem >= 0)
	{
	//	SessionInf inf = *(SessionInf *)m_TaskList.GetItemData(curItem);
		CMenu  menu;
		menu.LoadMenu(IDR_OPEN_LOAD_TASK);
		CMenu *pMenu = menu.GetSubMenu(0);

		if(GP_CurrentSession != NULL)
			pMenu->EnableMenuItem(ID_LOADMISSION,0);
		pMenu->TrackPopupMenu( TPM_LEFTALIGN  ,pt.x,pt.y,this);

	}

	

}

void CMRUTaskDlg::OnLoadmission()
{
	// TODO: Add your command handler code here
	int  curItem =  m_TaskList.GetNextItem(-1,LVIS_SELECTED);
	if(curItem >= 0)
	{
		CWaitCursor  wt;

		SessionInf inf = *(SessionInf *)m_TaskList.GetItemData(curItem);
		if(GP_CurrentSession!=NULL)
				GP_CurrentSession->UnActiveTaskUI();
		GP_TskPrgList->Lock();
		GP_TskPrgList->ClearAllCellInf();
		GP_TskPrgList->UnLock();
		FreeAllTask();	

		CJobFileReader job;
		if(!job.LoadFile(inf.m_TaskFile))
		{
			CString prmpt;
			prmpt.LoadString(IDS_LOAD_FAILURED);
			AfxMessageBox(prmpt);
			return ;
		}

		if( job.m_downStatus  == SESSION_COMPLETE)
		{
			CString prmpt;
			prmpt.LoadString(IDS_LOAD_JOB_COMPLETE);
			AfxMessageBox(prmpt);
			return;
		}
		
		pSYSTEM_CONFIG pSyscf = ConfigManager::Instance()->GetSystemConfig();
		TWebSession *tpSession = new TWebSession() ;
	
		pSYSTEM_CONFIG  pSysCf = ConfigManager::Instance()->GetSystemConfig();
		//pSysCf->dwMode = job.m_downloadMode;

		tpSession->Create(job.m_rootURL,inf.m_TaskFile,GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);
		tpSession->FillUrlsBuffer(job.m_listDowned);
		tpSession->FillUrlsBuffer(job.m_listPageURL);
		tpSession->FillResourcUrlsBuffer(job.m_listSourceURL);
		
		tpSession->ActiveTaskUI();
		tpSession->TASK_Start();
		AddToSessionTree(GP_CurrentSession = tpSession);
	
		this->OnOK();
	}
}

void CMRUTaskDlg::OnOpendirectory()
{
	// TODO: Add your command handler code here
	int  curItem =  m_TaskList.GetNextItem(-1,LVIS_SELECTED);
	if(curItem >= 0)
	{
		SessionInf inf = *(SessionInf *)m_TaskList.GetItemData(curItem);
		
		static CString fileName = inf.m_TaskFile ;
		LPSTR lpStr = fileName.GetBuffer();
		fileName.ReleaseBuffer();
		
		::PathRemoveFileSpec(lpStr);

		fileName.ReleaseBuffer();
		fileName += _T("\\");
		::ShellExecute(m_hWnd,_T("open"),NULL,NULL,fileName,SW_SHOWDEFAULT);
	}

}

