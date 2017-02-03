// tabdoc.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_HELP_TOPICS, OnHelpTopics)
END_MESSAGE_MAP()


// CtabdocApp ����

CtabdocApp::CtabdocApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CtabdocApp ����

CtabdocApp theApp;

// CtabdocApp ��ʼ��
CMainFrame  *pMainFrame;

BOOL CtabdocApp::InitInstance()
{

    
   
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
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
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

#include "HyperlinkStatic.h"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
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


// CtabdocApp ��Ϣ�������
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
	// TODO: �ڴ����ר�ô����/����û���
	//this->SaveStdProfileSettings();
	////this->AddToRecentFileList();
	CHistoryList::Instance()->Save();
	CHistoryList::Instance()->ReleaseInstance();
  
	ConfigManager::Instance()->SaveSessionConfig();
    ConfigManager::Instance()->SaveSysConfig();//�˳�ʱ������ǰ���á�

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
