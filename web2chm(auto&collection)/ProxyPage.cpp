// ProxyPage.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ProxyPage.h"
#include ".\proxypage.h"


// CProxyPage dialog

IMPLEMENT_DYNAMIC(CProxyPage, CPropertyPage)
CProxyPage::CProxyPage()
	: CPropertyPage(CProxyPage::IDD)
	,m_pConf(NULL)
{

}

CProxyPage::~CProxyPage()
{

}

void CProxyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProxyPage, CPropertyPage)
	ON_BN_CLICKED(IDC_USE_PROXY, OnBnClickedUseProxy)
END_MESSAGE_MAP()
 
void CProxyPage::OnBnClickedUseProxy()
{
	BOOL bEnable = ((CButton *)GetDlgItem(IDC_USE_PROXY))->GetCheck();
	EnableProxy(bEnable);

}
// CProxyPage message handlers
BOOL CProxyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_pConf = ConfigManager::Instance()->GetDefaultSessionConf();
	EnableProxy(m_pConf->bEnableProxy);
	return TRUE;
}

 void CProxyPage::EnableProxy(BOOL bEnable)
 {
	 GetDlgItem(IDC_SERVER_NAME)->EnableWindow(bEnable);
	 GetDlgItem(IDC_USER_NAME)->EnableWindow(bEnable);
	 GetDlgItem(IDC_PASSWORD)->EnableWindow(bEnable);
	 if(bEnable)
	 {
		 GetDlgItem(IDC_SERVER_NAME)->SetWindowText(m_pConf->strProxyName);
		 GetDlgItem(IDC_USER_NAME)->SetWindowText(m_pConf->strUserName);
		 GetDlgItem(IDC_PASSWORD)->SetWindowText(m_pConf->strPassWord);
	 }
	 GetDlgItem(IDC_STATIC_S)->EnableWindow(bEnable);
	 GetDlgItem(IDC_STATIC_U)->EnableWindow(bEnable);
	 GetDlgItem(IDC_STATIC_P)->EnableWindow(bEnable);
 }

 BOOL CProxyPage::OnApply()
 {
	 // TODO: Add your specialized code here and/or call the base class
	 m_pConf->bEnableProxy = GetCheck(IDC_USE_PROXY);
	 if(m_pConf->bEnableProxy)
	 {
		 CEdit  *pedit = (CEdit *)GetDlgItem(IDC_SERVER_NAME);
		 pedit->GetWindowText(m_pConf->strProxyName);

		 pedit = (CEdit *)GetDlgItem(IDC_USER_NAME);
		 pedit->GetWindowText(m_pConf->strUserName);

		 pedit = (CEdit *)GetDlgItem(IDC_PASSWORD);
		 pedit->GetWindowText(m_pConf->strPassWord);

	 }
	 return CPropertyPage::OnApply();
 }
