// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "shlwapi.h"
#include "tabdoc.h"
#include "MainFrm.h"
 
#include "afxpriv.h"

#include "NewTaskDlg.h"
#include "NewTaskDlg2.h"

#include "MRUTaskDlg.h"
#include "Defs.h"
#include "httpapi.h"
#include "ui.h"
#include "historylist.h"
#include "Splash.h"
 
#include ".\urleditdlg.h"
#include "TWebTask.h"
#include "anchorsbar.h"
#include "anchorlistwnd.h"
#include "anchorpanelwnd.h"
#include "workmodedlg.h"
#include "GeneralSheet.h"
#include "DownloadSheet.h"
#include "primarysheet.h"
#include "ConverDlg.h"
#include "mfcievw.h"
#include "AnchorUrlsList.h"
#include "DialogIEToolBar.h"
#include "resource.h"
#ifndef  _DEBUG
#include "Register.h"
#endif
#ifdef _DEBUG

#define new DEBUG_NEW
#endif

#define _AFX_NO_OLE_SUPPORT
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)

	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()

	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTYPANEL, OnUpdateViewPropertypanel)
	ON_UPDATE_COMMAND_UI(IDR_tabdocTYPE, OnUpdatetabdoctype)
	ON_UPDATE_COMMAND_UI(ID_TASK_REFRESH, OnUpdateViewRefresh)
    ON_UPDATE_COMMAND_UI(ID_APP_HELP,OnUpdateAppHelp)

	ON_COMMAND(IDR_tabdocTYPE, Ontabdoctype)
	ON_COMMAND(ID_VIEW_ANCHORPANEL, OnViewAnchorpanel)
	ON_COMMAND(ID_CONFIG_TASKCONFIG, OnConfigTaskconfig)
	ON_COMMAND(ID_CONFIG_SYSCONFIG, OnConfigSysconfig)
	ON_COMMAND(ID_FILE_MRU_FILE1, OnFileMruFile1)
	ON_COMMAND(ID_FILE_MRU_FILELIST, OnFileMruFilelist)
	ON_COMMAND(ID_TASK_REFRESH,OnTaskRefresh)
	ON_COMMAND(ID_APP_HELP,OnAppHelp)
	ON_COMMAND(ID_VIEW_PROPERTYPANEL, OnViewPropertypanel)

	ON_COMMAND(ID__ABANDONJOB,OnAbandonJob)
	ON_COMMAND(ID__RESUMEJOB,OnResumeJob)
	ON_COMMAND(ID__PAUSEJOB,OnPauseJob)
	ON_COMMAND( ID_SUPER_KEY,OnSuperKey) 
	ON_UPDATE_COMMAND_UI(ID__ABANDONJOB,OnUpDateAbandonJobUI)
	ON_UPDATE_COMMAND_UI(ID__RESUMEJOB,OnUpDateResumeJobUI)
	ON_UPDATE_COMMAND_UI(ID__PAUSEJOB,OnUpDatePauseJobUI)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANCHORPANEL, OnUpdateViewAnchorpanel)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_REGISTER, OnUpdateHelpRegister)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER + 1,WM_USER + 3,OnAnchorListMenuUpdate)
	
	ON_MESSAGE(WM_URLS_EDIT,OnEditUrls)
	ON_COMMAND(IDM_CUT, OnCut)
	ON_COMMAND(ID_REGISTER, OnHelpRegister)
	ON_MESSAGE(WM_START_CONVERT,StartConvert)
	ON_WM_SHOWWINDOW()

	ON_COMMAND(IDC_BUTTON_KILL,OnBnClickedButtonKill)
	ON_COMMAND(IDC_BUTTON_STOP ,OnBnClickedButtonStop)
	ON_COMMAND(IDC_BUTTON_RESUME ,OnBnClickedButtonStop)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_STOP,OnBtnSuspendUpdate)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_RESUME,OnBtnSuspendUpdate)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_KILL,OnBtnKillUpdate)

	ON_COMMAND(ID_CATCH_TOOL,OnBnClickedCatchTool)
	ON_UPDATE_COMMAND_UI(ID_CATCH_TOOL,OnBnCatchToolUpdate)
	
END_MESSAGE_MAP()

IMPLEMENT_MENUXP(CMainFrame, CFrameWndEx);
static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	//pPadFrame = NULL;
}

CMainFrame::~CMainFrame()
{
	
}

CMFCRibbonButton *suspendTaskBtn = NULL;
CMFCRibbonButton *validateKeyBtn = NULL;
BOOL CMainFrame::CreateRibbonBar ()
{
	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}
#if 0
	m_wndToolBar.SetButtonInfo(0, ID_FILE_NEW, TBSTYLE_BUTTON, 0);
	m_wndToolBar.SetButtonInfo(1, ID_FILE_MRU_FILELIST, TBSTYLE_BUTTON, 1);

	m_wndToolBar.SetButtonInfo(2, ID_CONFIG_SYSCONFIG, TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(3, ID_CONFIG_TASKCONFIG, TBSTYLE_BUTTON, 3);
	
    m_wndToolBar.SetButtonInfo(4, ID_APP_HELP, TBSTYLE_BUTTON, 4);
	m_wndToolBar.SetButtonInfo(5, ID_TASK_REFRESH, TBSTYLE_BUTTON, 5);
#endif

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("Main Pane"),0, 0);
	 
	//task pane
	CMFCRibbonPanel* pPanel1 = pCategory->AddPanel(_T("Download Task"));
	std::auto_ptr<CMFCRibbonButton> newTaskBtn(new CMFCRibbonButton(ID_FILE_NEW, _T("New Task"), 0, 0));
	newTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(newTaskBtn.release());

	/*std::auto_ptr<CMFCRibbonButton> startTaskBtn(new CMFCRibbonButton(IDC_BUTTON_START, _T("Start Task"), 0, 0));
	startTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(startTaskBtn.release());
*/
	suspendTaskBtn = new CMFCRibbonButton(IDC_BUTTON_STOP, _T("Suspend Task"), 0, 0);
	suspendTaskBtn->SetDescription(_T("Suspend Current Download Task"));
	suspendTaskBtn->SetToolTipText(_T("Suspend Task"));
	suspendTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(suspendTaskBtn);

	std::auto_ptr<CMFCRibbonButton> stopTaskBtn(new CMFCRibbonButton(IDC_BUTTON_KILL, _T("Kill Task"), 0, 0));
	stopTaskBtn->SetDescription(_T("Kill Current Download Task,And Remove It"));
	stopTaskBtn->SetToolTipText(_T("Kill Task"));
	stopTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(stopTaskBtn.release());

	/*std::auto_ptr<CMFCRibbonButton> refreshTaskBtn(new CMFCRibbonButton(ID_TASK_REFRESH, _T("Refresh Task"), 0, 0));
	refreshTaskBtn->SetDescription(_T("Show/Hide Task Bar"));
	refreshTaskBtn->SetToolTipText(_T("Refresh Task"));
	refreshTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(refreshTaskBtn.release());*/
	 
	std::auto_ptr<CMFCRibbonButton> historyTaskBtn(new CMFCRibbonButton(ID_FILE_MRU_FILELIST, _T("Task History"), 0, 0));
	historyTaskBtn->SetAlwaysLargeImage();
	pPanel1->Add(historyTaskBtn.release());
	 
	 
	//config pane
	CMFCRibbonPanel* pPane2 = pCategory->AddPanel(_T("Configuration"));
	std::auto_ptr<CMFCRibbonButton> sysConfigBtn(new CMFCRibbonButton(ID_CONFIG_SYSCONFIG, _T("General Options"), 0, 0));
	sysConfigBtn->SetAlwaysLargeImage();
	pPane2->Add(sysConfigBtn.release());

	std::auto_ptr<CMFCRibbonButton> taskConfigBtn(new CMFCRibbonButton(ID_CONFIG_TASKCONFIG, _T("Task Options"), 0, 0));
	taskConfigBtn->SetAlwaysLargeImage();
	pPane2->Add(taskConfigBtn.release());
	//catch tool pane
	CMFCRibbonPanel* pPane3 = pCategory->AddPanel(_T("IECatcher"));
	std::auto_ptr<CMFCRibbonButton> catchToolBtn(new CMFCRibbonButton(ID_CATCH_TOOL, _T("Enable Catcher"), 0, 0));
	catchToolBtn->SetAlwaysLargeImage();
	pPane3->Add(catchToolBtn.release());
	//view 
	CMFCRibbonPanel* pPane4 = pCategory->AddPanel(_T("View"));
	std::auto_ptr<CMFCRibbonButton> taskBarBtn(new CMFCRibbonButton(IDR_tabdocTYPE, _T("Task Bar"), 0, 0));
	taskBarBtn->SetDescription(_T("Show/Hide Task Bar"));
	taskBarBtn->SetToolTipText(_T("Task Bar"));
	taskBarBtn->SetAlwaysLargeImage();
	pPane4->Add(taskBarBtn.release());
	 
	std::auto_ptr<CMFCRibbonButton> outputBarBtn(new CMFCRibbonButton(ID_VIEW_PROPERTYPANEL, _T("Download Bar"), 0, 0));
	outputBarBtn->SetAlwaysLargeImage();
	outputBarBtn->SetDescription(_T("Show/Hide Download Bar"));
	outputBarBtn->SetToolTipText(_T("Download Bar"));
	pPane4->Add(outputBarBtn.release());
	 
	std::auto_ptr<CMFCRibbonButton> anchorBarBtn(new CMFCRibbonButton(ID_VIEW_ANCHORPANEL, _T("Anchor Bar"), 0, 0));
	anchorBarBtn->SetAlwaysLargeImage();
	anchorBarBtn->SetDescription(_T("Show/Hide Anchor Bar"));
	anchorBarBtn->SetToolTipText(_T("Anchor Bar"));
	pPane4->Add(anchorBarBtn.release());
	 
	//help pane
	CMFCRibbonPanel* pPane5 = pCategory->AddPanel(_T("Help"));
	std::auto_ptr<CMFCRibbonButton> helpBtn(new CMFCRibbonButton(ID_APP_HELP, _T("Help"), 0, 0));
	helpBtn->SetAlwaysLargeImage();
	pPane5->Add(helpBtn.release());
	
	std::auto_ptr<CMFCRibbonButton> aboutBtn(new CMFCRibbonButton(ID_APP_ABOUT, _T("About"), 0, 0));
	aboutBtn->SetAlwaysLargeImage();
	pPane5->Add(aboutBtn.release());

#ifndef _DEBUG
	if(!IsKeyRegisted())
	{
		validateKeyBtn = new CMFCRibbonButton(ID_REGISTER, _T("Regist Software"), 0, 0);
		validateKeyBtn->SetToolTipText(_T("Regist Software"));
		validateKeyBtn->SetDescription(_T("Click To Regist Software."));
		validateKeyBtn->SetAlwaysLargeImage();
		pPane5->Add(validateKeyBtn);
	}
	else
#endif
	{
		validateKeyBtn = new CMFCRibbonButton(ID_HELP_CHECKFORUPDATE, _T("Check for Update"), 0, 0);
		validateKeyBtn->SetToolTipText(_T("Click It To Update Software."));
		validateKeyBtn->SetAlwaysLargeImage();
		pPane5->Add(validateKeyBtn);
	}
	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}

#ifdef _USE_SPLASH
	CSplashWnd::EnableSplashScreen(TRUE);
	CSplashWnd::ShowSplashScreen(this);
#endif
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	
	m_wndStatusBar.EnableToolTips();

	if (!m_WorkSpace.Create(this, CG_ID_VIEW_WORKSPACE ,
		_T("Project"), CSize(150,200)))
	{
		TRACE0("Failed to create dialog bar m_wndWorkspace\n");
		return -1;		// fail to create
	}
	m_WorkSpace.ModifyStyle(0,WS_CLIPCHILDREN);
	m_WorkSpace.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	this->DockPane(&m_WorkSpace,  AFX_IDW_DOCKBAR_LEFT);
	m_WorkSpace.SetMinSize(120);

	CAnchorsBar::Instance()->Create(this, 
		_T("Page-collection"),CSize(250,400),WS_VISIBLE|WS_CHILD,CG_ID_VIEW_ANCHORS);;
	CAnchorsBar::Instance()->ModifyStyle(0,WS_CLIPCHILDREN);
	CAnchorsBar::Instance()->SetMinSize(120);
	CDockPanel *pPane = CAnchorsBar::Instance()->GetPanel();
	
	CTabPanelWnd *pPanelWnd = (CTabPanelWnd *)new CAnchorPanelWnd(pPane);
	
	pPanelWnd->SetTitleText("Web Resource-selection");
	pPanelWnd->SetMiniDefaultSize(CSize(100,200));
	pPane->AddTabPanelItem(pPanelWnd);
	
	pPanelWnd = new CAnchorListWnd(pPane);
	pPanelWnd->SetTitleText("URL List");
	pPanelWnd->SetMiniDefaultSize(CSize(100,15));
	

	pPane->AddTabPanelItem(pPanelWnd);

   	CAnchorsBar::Instance()->EnableDocking(CBRS_ORIENT_VERT);
	EnableDocking(CBRS_ALIGN_LEFT |CBRS_ALIGN_RIGHT );
	DockPane(CAnchorsBar::Instance(), AFX_IDW_DOCKBAR_RIGHT);
	 
	CAnchorsBar::Instance()->ShowPane(ConfigManager::Instance()->GetSystemConfig()->dwWorkMode == ANCHOR_MODE,0,0);
	 
	if (!m_CtrlBar.Create(this,
		_T("Download"), CSize(120,100),WS_CHILD|WS_VISIBLE, CG_ID_VIEW_WORKSPACE +1) )
	{
		TRACE0("Failed to create dialog bar m_wndWorkspace\n");
		return -1;		// fail to create
	}

	m_CtrlBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	this->DockPane(&m_CtrlBar, AFX_IDW_DOCKBAR_BOTTOM);

	//this->LoadBarState(NULL);
	OnApplicationLook();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_CLIPCHILDREN;// ~WS_THICKFRAME;
	cs.style |= WS_CLIPSIBLINGS;
	if( !__super::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
  
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	__super::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	//pPadFrame = new CPadFrame;
	/*pPadFrame.Create(NULL,"PadFrame",WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this);
	pPadFrame.SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pPadFrame.ShowWindow(SW_SHOW
	*/
	CIEPane::Instance()->SetControlBarStyle(0);
	CIEPane::Instance()->Create(this,_T("Web Browser"),CSize(0,0),WS_CHILD|WS_VISIBLE,AFX_IDW_PANE_FIRST);
	
	return __super::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	
	__super::OnSize(nType, cx, cy);
	CacalLayout();
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	__super::RecalcLayout(bNotify); 
	
	
}


void CMainFrame::CacalLayout(void)
{

}

void CMainFrame::Ontabdoctype()
{
	if(m_WorkSpace.IsWindowVisible())
		 m_WorkSpace.ShowPane(FALSE,0,0);
	else
		 m_WorkSpace.ShowPane(TRUE,0,0);
	
}

void CMainFrame::OnUpdatetabdoctype(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here

	pCmdUI->SetCheck(m_WorkSpace.IsWindowVisible());
		
	pCmdUI->ContinueRouting();
}




BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;//CFrameWnd::O
}

inline  BOOL AllSessionIsExit(void)
{
	for(TASK_LIST::iterator it = GP_SessionList.begin();
		it != GP_SessionList.end();it++)
	{
		TWebSession *p = (TWebSession *)*it;
		if(p->GetStatus() != _URL_TASK::THREAD_EXIT)
			return FALSE;
	}

	return TRUE;
}

void CMainFrame::OnClose()
{
	//this->SaveBarState(NU);
	DWORD nCount = GP_SessionList.size();
	if(nCount)
	{
		DWORD ret = 0 ;
		if(!AllSessionIsExit())
		ret = AfxMessageBox(IDS_QUERY_DOWNLOD,MB_OKCANCEL|MB_ICONQUESTION );

		if(ret == IDCANCEL)
			return ;

		for(TASK_LIST::iterator it = GP_SessionList.begin();
			it != GP_SessionList.end();it++)
		{
			TWebSession *p = (TWebSession *)*it;

			if(PathFileExistsA(p->GetTaskPathName()) == FALSE)
					p->SaveToTaskFile();

			p->SaveToHistory();
			p->Destroy();
			p->UnPrepare();
			//delete p;
		}
//save to  history list here.....


	}


	CFrameWndEx::OnClose();
}

void CMainFrame::OnConfigTaskconfig()
{
	// TODO: 在此添加命令处理程序代码
	CDownloadSheet  Ctg("Task Options",TRUE,this,CDownloadSheet::m_nSelectedPage);
	Ctg.DoModal();
}

void CMainFrame::OnConfigSysconfig()
{
	// TODO: 在此添加命令处理程序代码
	CGeneralSheet  Csg("General Options");
	Csg.DoModal();

}

void CMainFrame::OnFileMruFile1()
{
		
}

void CMainFrame::OnFileMruFilelist()
{
	// TODO: 在此添加命令处理程序代码
	CMRUTaskDlg  dlg;
	dlg.DoModal();
}


void CMainFrame::OnUpdateViewPropertypanel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_CtrlBar.IsWindowVisible());
	pCmdUI->ContinueRouting();
}

void CMainFrame::OnViewPropertypanel()
{
	if(m_CtrlBar.IsWindowVisible())
		m_CtrlBar.ShowPane(FALSE,0,0);
	else
		m_CtrlBar.ShowPane(TRUE,0,0);
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRet = FALSE;
 
	//bRet = CMfcieView::Instance()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);	
	if(!bRet)
	bRet =  CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);	
	if(!bRet)
	{
		bRet = CAnchorsBar::Instance()->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo);
		
	}
	return bRet;
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return __super::OnWndMsg(message, wParam, lParam, pResult);
}

void CMainFrame::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	pSYSTEM_CONFIG pSyscf = ConfigManager::Instance()->GetSystemConfig();
	pSyscf->dwWorkMode = ANCHOR_MODE;
	int pages = TAnchorManager::Instance()->GetPageCount();
	//if(pages > 0)
	//	pSyscf->dwWorkMode = ANCHOR_MODE;
	
	CPrimarySheet sheet("W2EBook",this);
	sheet.DoModal();

#if 0	
	if(pSyscf->bWarzard)
	{
		CPrimarySheet sheet("W2EBook",this);
		sheet.DoModal();
	}
	else
	{
	 

		if(GO_WorkModeDlg.m_cmbWorkMode.GetCurSel() == 1)
		{
			CNewTaskDlg  dlg;
			pSyscf->dwWorkMode = AUTO_MODE;
			dlg.DoModal();
		}else
		{
			CNewTaskDlg2 dlg;
			pSyscf->dwWorkMode = ANCHOR_MODE;
			dlg.DoModal();
		}

	}
#endif
}




void CMainFrame::OnUpdateViewRefresh(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GP_CurrentSession != NULL);
}
void CMainFrame::OnTaskRefresh()
{
	if(GP_CurrentSession !=NULL)
	{
		GP_CurrentSession->TASK_Stop();
		Sleep(200);
		GP_CurrentSession->TASK_Start();
	}
}
void CMainFrame::PostNcDestroy()
{
	CDialogIEToolBar::Instance()->ReleaseInstance();
	CMfcieView::Instance()->ReleaseInstance();
	CIEPane::Instance()->ReleaseInstance();
	// TODO: 在此添加专用代码和/或调用基类
    DWORD nCount = GP_SessionList.size();
	if(nCount)
	{
		
		for(TASK_LIST::iterator it = GP_SessionList.begin();
			it != GP_SessionList.end();it++)
		{
			TWebSession *p = (TWebSession *)*it;
			p->TASK_Destroy();
			delete p;
		}
		GP_SessionList.clear();
		
	}
	
	//pPadFrame=NULL;
   // delete pPadFrame;
	CAnchorsBar::ReleaseInstance();
	CFrameWndEx::PostNcDestroy();
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	if ( !CFrameWndEx::LoadFrame (nIDResource, dwDefaultStyle, pParentWnd, pContext) )
    {
        return false;
    }
    return true;
}

void CMainFrame::SetCenterWindow(void)
{
	CRect dskRect;
	HWND  hwnd = ::GetDesktopWindow();
	::GetClientRect(hwnd,dskRect);
	float  xLeft = (float)dskRect.Width() ;
	float  xTop  = (float)dskRect.Height();
   
	double  btt = xLeft / xTop;


	if(btt == (double)1.600000) //8:5的尺寸。
		CenterWindow();
	else
	{
		 float cx = 800.00;
		 float cy = 500.00;

		 xTop = ((float)dskRect.Height() - cy) / 2;
		 xLeft =((float)dskRect.Width() - cx) / 2;
		 xTop = max(0,xTop );
		 xLeft = max(0,xLeft);

		 MoveWindow(xLeft,xTop,cx,cy);
	}
	
	///*SetWindowPos(NULL, xLeft, xTop,xLeft *2, xTop * 2,
	//	SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);*/
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	 
	 pCmdUI->Enable(GP_CurrentSession == NULL);
	// GO_WorkModeDlg.EnableWindow(GP_CurrentSession == NULL);

}

void CMainFrame::OnAppHelp()
{
	/*#include "UrlEditDlg.h"
	CUrlEditDlg dlg;
	dlg.DoModal();*/
	/*http://www.ten-dollarsoft.com/w2ebook_tutorial.htm*/
	CMfcieView::Instance()->Navigate(_T("http://www.ten-dollarsoft.com/w2ebook_tutorial.htm"));
	
}

void CMainFrame:: OnUpdateAppHelp(CCmdUI *pCmdUI)
{
	//find pages  in the help directory....
	pCmdUI->Enable(TRUE);
	
}

HRESULT CMainFrame::OnEditUrls(WPARAM wparam, LPARAM lparam)
{
	URL_LIST  *pList = (URL_LIST *)wparam;
	if(pList->size())
	{
		CUrlEditDlg  dlg;
		dlg.m_pList = pList;
		dlg.DoModal();
	}
	return S_OK;
}

void CMainFrame::OnAbandonJob(void)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	
	pTask->Destroy();
	pTask->UnPrepare();
	pTask->m_prgCtrl.UnPrepareProgress();

	pTask->Create((_URL_TASK *)GP_CurrentSession);
	//int status = pTask->GetStatus();

}

void CMainFrame::OnPauseJob(void)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	pTask->Stop();

}

void CMainFrame::OnResumeJob(void)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	pTask->Start();
}

void CMainFrame::OnUpDateAbandonJobUI(CCmdUI* pUI)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	pUI->Enable( pTask && ( pTask->GetStatus() == _URL_TASK::THREAD_RUNING));
}

void CMainFrame::OnUpDateResumeJobUI(CCmdUI* pUI)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	pUI->Enable(pTask && (pTask->GetStatus() == _URL_TASK::THREAD_SUSPEND));
}

void CMainFrame::OnUpDatePauseJobUI(CCmdUI* pUI)
{
	long tt = GP_TskPrgList->GetCurrentRow();
	TWebTask *pTask = GP_TskPrgList->GetRowTask(tt);
	pUI->Enable(pTask && (pTask->GetStatus() == _URL_TASK::THREAD_RUNING));
}

CWnd * CMainFrame::GetActiveViewWnd(void)
{
	return (CWnd *) CMfcieView::Instance();
}

void CMainFrame::OnUpdateViewAnchorpanel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	 
	pCmdUI->SetCheck(CAnchorsBar::Instance()->IsVisible());
	//pCmdUI->Enable(ConfigManager::Instance()->GetSystemConfig()->dwWorkMode == ANCHOR_MODE);
}

void CMainFrame::OnViewAnchorpanel()
{
	// TODO: Add your command handler code here
	BOOL bVisible = CAnchorsBar::Instance()->IsVisible();
	CAnchorsBar::Instance()->ShowPane(!bVisible,0,0);
}

void CMainFrame::OnCut()
{
	// TODO: Add your command handler code here
	 CDockPanel *pPane = CAnchorsBar::Instance()->GetPanel();
	 CAnchorPanelWnd *pAnchorWnd = ( CAnchorPanelWnd *)pPane->m_arrPanel[0];
	 pAnchorWnd->OnCut();
}

void CMainFrame::OnHelpRegister()
{
	// TODO: Add your command handler code here
	
#ifndef _DEBUG
	//CheckKeyState();
	if(!IsKeyRegisted())
	{
		::CheckKeyState(638,2);
	 
		if(IsKeyRegisted())
		{
			validateKeyBtn->SetID(ID_HELP_CHECKFORUPDATE);
			validateKeyBtn->SetText(_T("Check for Update"));
		}
	}
#endif 
}

void CMainFrame::OnUpdateHelpRegister(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
#ifndef _DEBUG
	BOOL bEnable = IsKeyRegisted();
	pCmdUI->Enable(!bEnable);
#endif
}
LRESULT CMainFrame::StartConvert(WPARAM wparam,LPARAM lparam)
{
	CString  workDir((LPSTR)wparam);
	CConverDlg  cdlg(lparam);
	cdlg.m_TaskDir = workDir;
	cdlg.DoModal();

	GP_TskPrgList->Lock(); 
	GP_TskPrgList->ClearAllCellInf();
	GP_TskPrgList->Invalidate();
    GP_TskPrgList->ClearInfMessage();
	GP_TskPrgList->UnLock();	
	return S_OK;
}

void CMainFrame:: OnAnchorListMenuUpdate(CCmdUI *pUI)
{
	if(GO_AnchorUrlsList.GetCount() <= 0)
	{
		pUI->Enable(0);
		return;
	}

	if(GO_AnchorUrlsList.GetSelCount() <= 0)
	{
		if(pUI->m_nID == WM_USER + 3)
			pUI->Enable(0);
		else
			pUI->Enable(1);

		return;
	}

	pUI->Enable(1);
}

void  CMainFrame::OnSuperKey()
{
	//AfxGetApp()->ShowAppMessageBox(AfxGetApp(),_T("test"),0,0);
	CDialog dlg(MAKEINTRESOURCE(IDD_SUPER_DIALOG));
	int nRet = dlg.DoModal();
	if(nRet == IDOK)
	{
		//dlg.getdlg
	}

}

void CMainFrame::OnApplicationLook()
{	
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	CDockingManager::SetDockingMode(DT_SMART);
	
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

} 


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWndEx::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
}

 void CMainFrame::OnBtnSuspendUpdate(CCmdUI*pUI)
{
	if(!GP_CurrentSession)
	{
		pUI->Enable(FALSE);
		return;
	}
	
	pUI->Enable(TRUE);
	
}

void CMainFrame::OnBtnKillUpdate(CCmdUI*pUI)
{
	if(!GP_CurrentSession)
		pUI->Enable(FALSE);
	else
		pUI->Enable(TRUE);

}

 void CMainFrame::OnBnClickedButtonStop()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(GP_CurrentSession && (GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_RUNING
		||GP_CurrentSession->GetTaskStatus()== _URL_TASK::THREAD_FREE))
	 {
		 GP_CurrentSession->TASK_Stop();
		 if(suspendTaskBtn)
		 {
			 suspendTaskBtn->SetID(IDC_BUTTON_RESUME);
			 suspendTaskBtn->SetText(_T("Resume Task"));
			 suspendTaskBtn->SetToolTipText("Resume task to continue download.");
		 }
	 }
	 else
	 {
		 if(GP_CurrentSession && GP_CurrentSession->GetStatus() == _URL_TASK::THREAD_SUSPEND )
			GP_CurrentSession->TASK_Start();
		 if(suspendTaskBtn)
		 {
			 suspendTaskBtn->SetID(IDC_BUTTON_STOP);
			 suspendTaskBtn->SetText(_T("Suspend Task"));
			 suspendTaskBtn->SetToolTipText("Suspend task to stop download.");
		 }
	 }
 }

//TCompilerTask  compilerTask;
 void CMainFrame::OnBnClickedButtonKill()
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

void CMainFrame::OnBnClickedCatchTool()
{
	CMfcieView::Instance()->EnableCatch((CMfcieView::s_bIsEnableSink?FALSE:TRUE));
}

void CMainFrame::OnBnCatchToolUpdate(CCmdUI*pUI)
{
	pUI->SetCheck(CMfcieView::s_bIsEnableSink);
}