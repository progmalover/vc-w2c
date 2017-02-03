// NewDownloadPage.cpp : 实现文件
//

#include "stdafx.h"
#include "NewDownloadPage.h"
#include "PrimarySheet.h"
#include <afxpriv.h>
#include "ConnectTestDlg.h"
#include "CProgressList.h"
#include "Defs.h"
#include "Simiebar.h"
#include "TCatalogBuilder.h"
#include "ui.h"
#include ".\tanchormanager.h"
#include "WorkModeDlg.h"
#include "Dplayer.h"
#include "DialogIEToolBar.h"
using namespace ::ui;
#include "filepath.h"

// CNewDownloadPage 对话框
static CPrimarySheet *pSheet=NULL;

IMPLEMENT_DYNAMIC(CNewDownloadPage, CPropertyPage)
CNewDownloadPage::CNewDownloadPage()
	: CPropertyPage(CNewDownloadPage::IDD)
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle  = "Start";
	m_psp.pszHeaderSubTitle = "Select work mode";
	//m_psp.dwFlags |= PSP_HIDEHEADER;
	m_pSyscf = NULL;
}

CNewDownloadPage::~CNewDownloadPage()
{
}

void CNewDownloadPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WORKMODE, m_WorkModeBox);
	DDX_Control(pDX, IDC_TESTPROGRESS, m_ConnectProgress);
	DDX_Control(pDX, IDC_LOCATIONEDIT, m_TargetDir);
	DDX_Control(pDX, IDC_COMBO_URL, m_URLBox);
	DDX_Control(pDX, IDC_LIST_URL, m_URLList);
	DDX_Control(pDX, IDC_STATIC_INFO, m_ModeStatic);
}


BEGIN_MESSAGE_MAP(CNewDownloadPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_WORKMODE, OnSelchangeWorkmode)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_CONNCTTEST, OnBnClickedConncttest)
	ON_BN_CLICKED(IDC_EXPLORE, OnBnClickedExplore)
	ON_MESSAGE(WM_DOWNPROGRESS,OnDownLoadProgress)
END_MESSAGE_MAP()


// CNewDownloadPage 消息处理程序

BOOL CNewDownloadPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
    pSheet = (CPrimarySheet*)GetParent();

	CString strMode;
	strMode.LoadString(IDS_COLLECTION_MODE);
	m_WorkModeBox.InsertString(0,strMode);
	strMode.LoadString(IDS_AUTO_MODE);
	m_WorkModeBox.InsertString(1,strMode);

	m_pSyscf = ConfigManager::Instance()->GetSystemConfig();
	m_WorkModeBox.SetCurSel(m_pSyscf->dwWorkMode);
	if(m_pSyscf->dwWorkMode == ANCHOR_MODE)
		AddURL2List();
	ShowMode(m_pSyscf->dwWorkMode);
	
	TCatalogBuilder::InitBuilder();

	/*m_TargetDir.Init("Select Output Directory");
	m_TargetDir.Load();
	if(m_TargetDir.GetCount())
		m_TargetDir.SetCurSel(0);*/

	m_URLBox.Init("Select Output URL");
	m_URLBox.Load();

	CString		strAddress;
	//CSimIEBar::Instance()->m_wndAddress.GetWindowText(strAddress);
	CDialogIEToolBar::Instance()->m_URLAdressComboBox.GetWindowText(strAddress);
	strAddress.Trim();
	if(strAddress.IsEmpty())
	{
		if(m_URLBox.GetCount())
			m_URLBox.SetCurSel(0);
	}
	else
		m_URLBox.SetWindowText(strAddress);
	m_ModeStatic.SetFontSize(10);
	return TRUE; 
}


BOOL CNewDownloadPage::OnSetActive()
{
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

//判断文件路径以及文件名是否有效
BOOL CNewDownloadPage::CheckPathname(CString strPathName)
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

LRESULT CNewDownloadPage::OnWizardNext()
{
	m_URLBox.GetWindowText(m_URL);
	m_URL.Trim();
	m_URL.MakeLower();
	if(m_WorkMode == AUTO_MODE)
	{
		if(!CheckAutoMode())
			return -1;
	}
	else
	{
		if(!CheckAnchorMode())
			return -1;
	}
    
	CString  strPathName;
	GetDlgItemText(IDC_LOCATIONEDIT,strPathName);
	strPathName.Trim();
	if (!CheckPathname(strPathName))
		return -1;
	m_strPath = strPathName;
	SaveHistoryBox();
	m_pSyscf->dwWorkMode = m_WorkMode;
	pSheet->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);
	return CPropertyPage::OnWizardNext();
}

void   CNewDownloadPage::SaveHistoryBox()
{
	/*m_TargetDir.AddCurrentText();
	m_TargetDir.Save();*/
	m_URLBox.AddCurrentText();
	m_URLBox.Save();
}

void   CNewDownloadPage::AddURL2List()
{
	m_URLList.ResetContent();
	int pages = TAnchorManager::Instance()->GetPageCount();
	if(pages>0)
	{
		URL_LIST URLList;
		TAnchorManager::Instance()->GetPageUrls(URLList);
		TAnchorManager::Instance()->GetAllUrls(URLList);
	
		for(URL_LIST::iterator it = URLList.begin();it != URLList.end();it++)
		{
			CString &str = *it;
			_URL obj(str);
			m_URLList.AddString(*obj.GetURL());
		}

	}
}

//IDC_STATIC_URL  IDC_URLEDIT  IDC_CONNCTTEST
#define NORMAL_STATIC_HEIGHT   80
#define SMALL_STATIC_HEIGHT    16
void   CNewDownloadPage::ChangeStatic(COLORREF crText,CString strText,BOOL bEnlarge)
{
	CRect rtStatic,rtParent;
    GetWindowRect(&rtParent);
	m_ModeStatic.GetWindowRect(&rtStatic);
	CPoint ptStatic(rtStatic.left - rtParent.left,rtStatic.top - rtParent.top);
    m_ModeStatic.GetClientRect(&rtStatic);
	rtStatic.bottom = rtStatic.top+(bEnlarge?NORMAL_STATIC_HEIGHT:SMALL_STATIC_HEIGHT);
	m_ModeStatic.SetTextColor(crText);
	m_ModeStatic.SetWindowText(strText);
	rtStatic.MoveToXY(ptStatic);
	m_ModeStatic.MoveWindow(&rtStatic);
}

void   CNewDownloadPage::ShowMode(int Workmode)
{
	m_WorkMode = Workmode;
	GetDlgItem(IDC_STATIC_URL)->ShowWindow(Workmode ==AUTO_MODE? SW_SHOW:SW_HIDE);
	m_URLBox.ShowWindow(Workmode ==AUTO_MODE? SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_CONNCTTEST)->ShowWindow(Workmode ==AUTO_MODE? SW_SHOW:SW_HIDE);
	BOOL bShowStatic = FALSE,
		 bShowList = FALSE,
		 bEnlarge = TRUE;
	CString strInfo;
	COLORREF crText=BLACK;
	if(m_WorkMode == AUTO_MODE)
		strInfo.LoadString(IDS_AUTO_MODE_USE);
	else
	{
		if(m_URLList.GetCount()>0)
		{
			bShowList = TRUE;
			strInfo.Format(IDS_ANCHOR_COUNT,m_URLList.GetCount());
			bEnlarge = FALSE;
		}
		else
		{
			strInfo.LoadString(IDS_E_NO_ANCHOR);
			crText = RED;
		}
	}
	ChangeStatic(crText,strInfo,bEnlarge);
	m_URLList.ShowWindow(bShowList? SW_SHOW:SW_HIDE);
}

void CNewDownloadPage::OnSelchangeWorkmode()
{
	int  WorkMode = m_WorkModeBox.GetCurSel();
	if(WorkMode != m_WorkMode)
	{
		//GO_WorkModeDlg.SetMode(WorkMode);
		ShowMode(WorkMode);
	}
}

void CNewDownloadPage::OnBnClickedButtonHelp()
{
	CDPlayer  fplay(m_WorkMode);

	fplay.DoModal();
}

void CNewDownloadPage::OnBnClickedConncttest()
{
	m_URLBox.GetWindowText(m_URL);
	m_URL.Trim();
	m_URL.MakeLower();
	if(m_URL.IsEmpty()||(!IsValidURL(m_URL)))
	{
		AfxMessageBox(IDS_E_URL);
		GotoDlgCtrl(GetDlgItem(IDC_COMBO_URL));
		return;
	}
	FixUrlStr(m_URL); //Add "http:"
	
	GetDlgItem(IDC_CONNCTTEST)->EnableWindow(FALSE);
	m_ConnectProgress.ShowWindow(SW_SHOW);
	m_ConnectProgress.RedrawWindow();

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
		AfxMessageBox(strCaption);
	GetDlgItem(IDC_CONNCTTEST)->EnableWindow(TRUE);
	m_ConnectProgress.ShowWindow(SW_HIDE);
}

void CNewDownloadPage::OnBnClickedExplore()
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
		SetDlgItemText( IDC_LOCATIONEDIT, dir);
	}
}

void CNewDownloadPage::OnEnChangeUrledit()
{
	//CWnd *pwnd = GetDlgItem(IDC_TASKSTART);
	//pwnd->EnableWindow(TRUE);
}

HRESULT CNewDownloadPage::OnDownLoadProgress(WPARAM mparam,LPARAM lparam)
{
	m_ConnectProgress.SetRange(0,mparam);
	m_ConnectProgress.SetPos(lparam);
	return S_OK;
}

//判断URL是否有效
BOOL  CNewDownloadPage::CheckAutoMode()
{
	if(m_URL.IsEmpty()||(!IsValidURL(m_URL)))
	{
		AfxMessageBox(IDS_E_URL);
		//m_URLBox.SetWindowText("");
		GotoDlgCtrl(GetDlgItem(IDC_COMBO_URL));
		return FALSE;
	}
	return TRUE;
}

BOOL  CNewDownloadPage::CheckAnchorMode()
{
	if(TAnchorManager::Instance()->GetPageCount() <= 0)
	{
		AfxMessageBox(IDS_E_NO_ANCHOR);
		return FALSE;
	}
	return TRUE;
}

