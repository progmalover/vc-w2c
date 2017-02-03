// tabdoc.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "tabdoc.h"
#include "MainFrm.h"
#include ".\tabdoc.h"
#include "Defs.h"
#include "HttpApi.h"
#include "Dplayer.h"
#include "ui.h"
#include "historylist.h"
#include "Splash.h"
#include ".\tanchormanager.h"
#include ".\defnamemanager.h"

//#ifndef _DEBUG
#include "Register.h"
//#endif
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "ComCtl32.lib") 

// CtabdocApp

BEGIN_MESSAGE_MAP(CtabdocApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_TUTORIAL, OnTutorial)
	ON_COMMAND(ID_HELP_CHECKFORUPDATE, OnUpdate)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_HELP_TOPICS, OnHelpTopics)
END_MESSAGE_MAP()


// CtabdocApp 构造

CtabdocApp::CtabdocApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CtabdocApp 对象

CtabdocApp theApp;

// CtabdocApp 初始化
CMainFrame  *pMainFrame;

BOOL CtabdocApp::InitInstance()
{

    
   
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();
  
	SetRegistryKey("W2EBOOK");
	//m_pszProfileName =_tcsdup("W2EBOOK");
		
	ConfigManager::Instance()->LoadSessionConfig();
	ConfigManager::Instance()->LoadSysConfig();
   
	//CMenuXP::InitializeHook();

	InitContextMenuManager();
	InitKeyboardManager();

	// TODO: Remove this if you don't want extended tooltips:
	InitTooltipManager();

	CMFCToolTipInfo params;
	params.m_bVislManagerTheme = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl),
		&params);

#if _MSC_VER < 1300
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif
	
    AfxInitRichEdit2();

	CWinAppEx::InitInstance();


	CHistoryList::Instance()->Load();
	
	AfxEnableControlContainer();
	
#ifndef _DEBUG
	CheckKeyState(638,1);
	//IsKeyRegisted();
#endif 
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//CMainFrame  *pMainFrame;
    pMainFrame = new CMainFrame();
	pMainFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	m_pMainWnd = pMainFrame;

   
	//m_pMainWnd->CenterWindow();
	pMainFrame->SetCenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
#ifdef _USE_SPLASH
	CSplashWnd::HideSplashScreen();
#endif
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

#include "HyperlinkStatic.h"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
private:
	CHyperlinkStatic	m_WebsiteStatic;
	CHyperlinkStatic	m_EmailStatic;
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CtabdocApp::OnHelp()
{

	
//	::AfxGetApp()-
	//::ShellExecute(NULL ,"open",TxtCompiler::GetTxtDirPath(m_TaskDirPath) , NULL, NULL, SW_SHOWNORMAL);
}

void CtabdocApp::OnTutorial()
{
   CDPlayer  fplay;
   fplay.DoModal();
}

void CtabdocApp::OnUpdate()
{
	CString strHomePage;
	strHomePage.LoadString(IDS_HOMEPAGE);
	::ShellExecute(NULL, "open", strHomePage, NULL, NULL, SW_SHOW);
}

void CtabdocApp::OnAppAbout()
{
//	((test *)_ptr)->test1();

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();	
}


// CtabdocApp 消息处理程序
CFrameWnd * CtabdocApp::AppGetMainForame(void)
{
	return (CFrameWnd *)m_pMainWnd;
}


void CAboutDlg::OnBnClickedOk()
{
	OnOK();
}

int CtabdocApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	//this->SaveStdProfileSettings();
	////this->AddToRecentFileList();
	CHistoryList::Instance()->Save();
	CHistoryList::Instance()->ReleaseInstance();
  
	ConfigManager::Instance()->SaveSessionConfig();
    ConfigManager::Instance()->SaveSysConfig();//退出时保留当前设置。

	ConfigManager::Instance()->ReleaseInstance();
	CDefnameManager::ReleaseInstance();
	TAnchorManager::ReleaseInstance();
	//CMenuXP::UninitializeHook();
	return CWinAppEx::ExitInstance();
}

CRecentFileList & CtabdocApp::GetRecentList(void)
{
	//TODO: return statement
	return  *this->m_pRecentFileList;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTemp;
	strTemp.LoadString(IDS_HOMEPAGE);
	m_WebsiteStatic.SubclassDlgItem(IDC_STATIC_WEBSITE,this);
	m_WebsiteStatic.SetCaption(strTemp);
	m_WebsiteStatic.SetHyperlink(strTemp);

	strTemp.LoadString(IDS_SUPPORT_SHOW);
	m_EmailStatic.SubclassDlgItem(IDC_STATIC_SPPORT,this);
	m_EmailStatic.SetCaption(strTemp);
	strTemp.LoadString(IDS_SUPPORT_MAIL);
	m_EmailStatic.SetHyperlink(strTemp);

	return TRUE;  
}

void CtabdocApp::OnHelpTopics()
{
	//// TODO: Add your command handler code here
	CString strPath;
	::GetModuleFileName(AfxGetInstanceHandle(),strPath.GetBufferSetLength(_MAX_PATH + 1),_MAX_PATH);
	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	strPath += "topics.chm";
	/*if(!::PathFileExists(strPath))
	{
		AfxMessageBox();
		return;
	}*/
	::ShellExecute(NULL, "open",strPath, NULL, NULL, SW_SHOWNORMAL);
}
