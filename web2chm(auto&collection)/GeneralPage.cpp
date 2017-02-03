// GeneralPage.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "GeneralPage.h"
#include "Defs.h"
#include "ConfigManager.h"
#include "CProgressList.h"
#include "ShlWapi.h"
#include ".\generalpage.h"

// CGeneralPage dialog

IMPLEMENT_DYNAMIC(CGeneralPage, CPropertyPage)
CGeneralPage::CGeneralPage()
	: CPropertyPage(CGeneralPage::IDD)
{
	m_psp.dwFlags |= PSP_DEFAULT|PSP_USEHEADERTITLE|PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle  = "Second";
	m_psp.pszHeaderSubTitle = "Set general option";
	m_pSysConfig= NULL;
}

CGeneralPage::~CGeneralPage()
{
}

BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_WARZARD, OnBnClickedCheckWarzard)
	ON_BN_CLICKED(IDC_CHECK_ASK_BEFORE, OnBnClickedCheckAskBefore)
END_MESSAGE_MAP()

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOTRDNUMBER, m_ComboTrdNumber);
	DDX_Control(pDX, IDC_COMBOWAITTIME, m_ComboWaitTime);
	DDX_Control(pDX, IDC_COMBOCONNECTTRY, m_ComboConnectNum);
	DDX_Control(pDX, IDC_MRUNUMEDIT, m_RecordNum);
}


// CSysConfig 消息处理程序
BOOL CGeneralPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pSysConfig = ConfigManager::Instance()->GetSystemConfig();
	m_toolTip.Create(this);

    CString tipStr;
	tipStr.LoadString(IDS_TRDNUMBER);
	CWnd *pWnd = GetDlgItem(IDC_COMBOTRDNUMBER);
	m_toolTip.AddTool(pWnd,tipStr);

	if(NULL == GP_CurrentSession )
	{
		pWnd->EnableWindow();
		pWnd = GetDlgItem(IDC_TRDNUMBER);
		pWnd->EnableWindow();
	}
	else
	{
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_TRDNUMBER);
		pWnd->EnableWindow(FALSE);
	}

	tipStr.LoadString(IDS_MAXWAITTIME);
	pWnd = GetDlgItem(IDC_COMBOWAITTIME);
	m_toolTip.AddTool(pWnd,tipStr);

	tipStr.LoadString(IDS_CONNECTTRY);
	pWnd = GetDlgItem(IDC_COMBOCONNECTTRY);
	m_toolTip.AddTool(pWnd,tipStr);
	m_toolTip.Activate(TRUE);

	CString txtNumber;
	for(int i = 0;i < CONNECT_TRD_NUMBER;i++)
	{
		txtNumber.Format("%d",GA_dwConnectTrdsArray[i]);
		m_ComboTrdNumber.InsertString(i,txtNumber);
	}
	m_ComboTrdNumber.SetCurSel(m_pSysConfig->dwThrdNum);

	for(int i = 0;i < CONNECT_WAIT_TIME;i++)
	{
		txtNumber.Format("%d",GA_dwWaitTimeArray[i]);
		m_ComboWaitTime.InsertString(i,txtNumber);
	}
    m_ComboWaitTime.SetCurSel(m_pSysConfig->dwWaitTime); //default 2 second

	for(int i = 0;i < CONNECT_NUMS_MAX;i++)
	{
		txtNumber.Format("%d",GA_dwConnectRepondsArray[i]);
		m_ComboConnectNum.InsertString(i,txtNumber);
	}
	m_ComboConnectNum.SetCurSel(m_pSysConfig->dwConnectTimes);

	CString  strRecNum ;
	strRecNum.Format("%d",m_pSysConfig->dwRecordNum);
	m_RecordNum.SetWindowText(strRecNum);

	CButton *pBut;
	if(m_pSysConfig->dwMode & 1)
		SetRatioCheck(IDC_DOWNWEB2CHM,TRUE);

	if(m_pSysConfig->dwMode & 2)
		SetRatioCheck(IDC_DOWNWEB,TRUE);
	SetRatioCheck(IDC_CHECK_SHUTDOWN,m_pSysConfig->bShutDown);
	SetRatioCheck(IDC_CHECK_WARZARD,m_pSysConfig->bWarzard);
	SetRatioCheck(IDC_CHECK_ASK_BEFORE,m_pSysConfig->bAskBeforclose);
	GetDlgItem(IDC_CHECK_ASK_BEFORE)->EnableWindow(m_pSysConfig->bWarzard);


	if(m_pSysConfig->dwWorkMode == AUTO_MODE)
	{
		GetDlgItem(IDC_DOWNWEB2CHM)->EnableWindow(FALSE);
		GetDlgItem(IDC_DOWNWEB)->EnableWindow(FALSE);
	}else
	{
		GetDlgItem(IDC_DOWNWEB2CHM)->EnableWindow(TRUE);
		GetDlgItem(IDC_DOWNWEB)->EnableWindow(TRUE);
	}

	return TRUE;  
}

BOOL CGeneralPage::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE)
	m_toolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

BOOL   CGeneralPage::CheckData()
{
	DWORD  trdNumber = m_pSysConfig->dwThrdNum;
	m_pSysConfig->dwThrdNum = m_ComboTrdNumber.GetCurSel();//线程数量
	m_pSysConfig->dwWaitTime = m_ComboWaitTime.GetCurSel();//等待时间
	m_pSysConfig->dwConnectTimes = m_ComboConnectNum.GetCurSel();//连接次数

	m_pSysConfig->dwMode  =  GetRatioCheck(IDC_DOWNWEB2CHM)?1:0;
	m_pSysConfig->dwMode |=  GetRatioCheck(IDC_DOWNWEB)?2:0;
	m_pSysConfig->bShutDown =GetRatioCheck(IDC_CHECK_SHUTDOWN);
	m_pSysConfig->bWarzard = GetRatioCheck(IDC_CHECK_WARZARD);
	//m_pSysConfig->bAskBeforclose = GetRatioCheck(IDC_CHECK_ASK_BEFORE);
	//maybe append pdf format....
	CString  strRecNum;
	m_RecordNum.GetWindowText(strRecNum);
	m_pSysConfig->dwRecordNum = StrToInt(strRecNum);
	//m_pSysConfig->dwWorkMode = m_pSysConfig->dwWorkMode;
	//ConfigManager::Instance()->SetSysConfig(&sysConfig);
	if(NULL == GP_CurrentSession && trdNumber !=  m_pSysConfig->dwThrdNum )
	{
		GP_TskPrgList->SetProgressSells(GA_dwConnectTrdsArray[ m_pSysConfig->dwThrdNum]);
	}
	return TRUE;

}

BOOL CGeneralPage::OnApply()
{
	CheckData();
	return CPropertyPage::OnApply();
}

LRESULT CGeneralPage::OnWizardNext()
{
	CheckData();
	return CPropertyPage::OnWizardNext();
}

void CGeneralPage::OnBnClickedCheckWarzard()
{
	BOOL bWarzard = GetRatioCheck(IDC_CHECK_WARZARD);
	GetDlgItem(IDC_CHECK_ASK_BEFORE)->EnableWindow(bWarzard);
}

void CGeneralPage::OnBnClickedCheckAskBefore()
{
	m_pSysConfig->bAskBeforclose = GetRatioCheck(IDC_CHECK_ASK_BEFORE);
}
