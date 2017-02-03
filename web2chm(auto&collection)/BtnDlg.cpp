// BtnDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "web2chm.h"
#include "BtnDlg.h"
#include ".\btndlg.h"

#include "httpapi.h"
#include "ui.h"
#include "CProgressList.h"
#include "Defs.h"
#include "TCompilerTask.h"
// CBtnDlg 对话框

IMPLEMENT_DYNAMIC(CBtnDlg, CDialog)
CBtnDlg::CBtnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBtnDlg::IDD, pParent)
{
}

CBtnDlg::~CBtnDlg()
{
}

void CBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_START ,m_btnStart);
	DDX_Control(pDX,IDC_BUTTON_STOP ,m_btnStop);
	DDX_Control(pDX,IDC_BUTTON_KILL ,m_btnKill);
}


BEGIN_MESSAGE_MAP(CBtnDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_START,OnBtnStartUpdate)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_STOP,OnBtnSuspendUpdate)
    ON_UPDATE_COMMAND_UI(IDC_BUTTON_KILL,OnBtnKillUpdate)
    ON_WM_LBUTTONDOWN()
    ON_WM_NCPAINT()
    ON_BN_CLICKED(IDC_BUTTON_START, OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_KILL, OnBnClickedButtonKill)
END_MESSAGE_MAP()


// CBtnDlg 消息处理程序
void CBtnDlg::OnBtnSuspendUpdate(CCmdUI*pUI)
{
	if(!GP_CurrentSession)
	{
		m_btnStop.SetCheck(TRUE);
		return;
	}
	if(GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_SUSPEND)
		m_btnStop.SetCheck(TRUE);
	else
		m_btnStop.SetCheck(FALSE);
	
}

void CBtnDlg::OnBtnKillUpdate(CCmdUI*pUI)
{
	if(!GP_CurrentSession)
		m_btnKill.SetCheck(TRUE);
	else
		m_btnKill.SetCheck(FALSE);

}

void CBtnDlg::OnBtnStartUpdate(CCmdUI*pUI)
{
	if(!GP_CurrentSession)
	{
		m_btnStart.SetCheck(TRUE);
		return ;
	}
	if(GP_CurrentSession->GetTaskStatus() == _URL_TASK::THREAD_COMPLETE)
		m_btnStart.SetCheck(TRUE);

	if(GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_SUSPEND)
		m_btnStart.SetCheck(FALSE);

	if(GP_CurrentSession->GetTaskStatus() == _URL_TASK::THREAD_RUNING)
		m_btnStart.SetCheck(TRUE);

}

int CBtnDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}

BOOL CBtnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_btnStart.SetBmpIDUP(IDB_START);
	m_btnStart.SetBmpIDDOWN(IDB_STARTDOWN);
	
	m_btnStop.SetBmpIDUP(IDB_PAUSE);
	m_btnStop.SetBmpIDDOWN(IDB_PAUSEDOWN);

	m_btnKill.SetBmpIDUP(IDB_STOP);
	m_btnKill.SetBmpIDDOWN(IDB_STOPDOWN);

	EnableToolTips(TRUE);
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(&m_btnStart,"Resume download");
	m_ToolTip.AddTool(&m_btnStop,"Suspend download");
	m_ToolTip.AddTool(&m_btnKill,"Cancel download");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

 void CBtnDlg::OnTaskStart()
 {
		m_btnStart.SetCheck(1);
 }

 void CBtnDlg::OnPaint()
 {
	 CPaintDC dc(this); // device context for painting
	 
	 CRect rct;
	 CPen  pen;
	 
	 GetClientRect(&rct);
	
	 dc.FillSolidRect(&rct,GetSysColor(COLOR_3DSHADOW));
	 rct.DeflateRect(1,1,1,0);
	 
	 dc.FillSolidRect(&rct,GetSysColor(COLOR_3DFACE));
 }

 void CBtnDlg::OnLButtonDown(UINT nFlags, CPoint point)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值
	 CWnd *pOwner = GetOwner();
	 pOwner->SetFocus();
	 CDialog::OnLButtonDown(nFlags, point);
 }

  void CBtnDlg::OnNcPaint()
 {
	
 }

 BOOL CBtnDlg::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: 在此添加专用代码和/或调用基类
	 m_ToolTip.RelayEvent(pMsg);
	 if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	 {
		 CWnd* pOwner = GetOwner();
		 while (pOwner != NULL)
		 {
			 // allow owner & frames to translate before IsDialogMessage does
			 if (pOwner->PreTranslateMessage(pMsg))
				 return TRUE;

			 // try parent frames until there are no parent frames
			 pOwner = pOwner->GetParentFrame();
		 }
	 }
	 return CDialog::PreTranslateMessage(pMsg);
 }

 void CBtnDlg::OnBnClickedButtonStart()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(GP_CurrentSession && GP_CurrentSession->GetStatus() == _URL_TASK::THREAD_SUSPEND )
		 GP_CurrentSession->TASK_Start();
 }

 void CBtnDlg::OnBnClickedButtonStop()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(GP_CurrentSession && (GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_RUNING
		||GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_FREE))
	 {
		 GP_CurrentSession->TASK_Stop();
	 }
 }
//TCompilerTask  compilerTask;
 void CBtnDlg::OnBnClickedButtonKill()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(GP_CurrentSession)
	 {

		pSYSTEM_CONFIG pSysConf;
		int ret = AfxMessageBox(IDS_QUERY_DOWNLOD,MB_OKCANCEL|MB_ICONQUESTION );

		if(ret == IDCANCEL)
			return ;

		pSysConf = ConfigManager::Instance()->GetSystemConfig();

		
		GP_CurrentSession->Task_ForceStop();
		//void SaveToHistory(void);
		GP_CurrentSession->UninitTaskList();

		GP_CurrentSession->CreateRootPageQuickIcon();
		GP_CurrentSession->ForceDestroy();
		GP_CurrentSession->SetTaskStatus(_URL_TASK::THREAD_EXIT);
		GP_CurrentSession->SaveToTaskFile();
		GP_CurrentSession->SaveToHistory();
		
		 
		ui::RemoveFromSessionTree(GP_CurrentSession); 

		GP_CurrentSession = NULL;
	   
		GP_TskPrgList->Lock(); 
		GP_TskPrgList->ClearAllCellInf();
		GP_TskPrgList->Invalidate();
        GP_TskPrgList->ClearInfMessage();
		GP_TskPrgList->UnLock();		


	 }
 }
