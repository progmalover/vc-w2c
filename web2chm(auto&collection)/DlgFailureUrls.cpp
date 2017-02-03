// DlgFailureUrls.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFailureUrls.h"

// CDlgFailureUrls dialog
URL_OBJ_LIST   g_FailureUrlsList;
IMPLEMENT_DYNAMIC(CDlgFailureUrls, CDialog)

CDlgFailureUrls::CDlgFailureUrls(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFailureUrls::IDD, pParent)
{

}

CDlgFailureUrls::~CDlgFailureUrls()
{
}

void CDlgFailureUrls::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URL_LIST, m_UrlsList);
}


BEGIN_MESSAGE_MAP(CDlgFailureUrls, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgFailureUrls::OnBnClickedOk)
	ON_BN_CLICKED(ID_ABOND, &CDlgFailureUrls::OnBnClickedAbond)
	ON_BN_CLICKED(ID_RETRY, &CDlgFailureUrls::OnBnClickedRetry)
	ON_LBN_SELCHANGE(IDC_URL_LIST, &CDlgFailureUrls::OnLbnSelchangeUrlList)
	ON_BN_CLICKED(IDC_DEL_URLS, &CDlgFailureUrls::OnBnClickedDelUrls)
END_MESSAGE_MAP()


// CDlgFailureUrls message handlers
 URL_OBJ_LIST CDlgFailureUrls::m_FailureUrlsList;

void CDlgFailureUrls::ReBkFailureUrl( _URL &obj)
{
	m_FailureUrlsList.push_back(obj);
}

void CDlgFailureUrls::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDlgFailureUrls::OnBnClickedAbond()
{
	// TODO: Add your control notification handler code here
	m_dwStatus = 0;

	OnOK();
}

void CDlgFailureUrls::OnBnClickedRetry()
{
	// TODO: Add your control notification handler code here
	m_dwStatus = 1;
	OnOK();
}

void CDlgFailureUrls::OnLbnSelchangeUrlList()
{
	// TODO: Add your control notification handler code here
}

BOOL CDlgFailureUrls::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_FailureUrlsList.size())
		g_FailureUrlsList.assign(m_FailureUrlsList.begin(),m_FailureUrlsList.end());

	m_FailureUrlsList.clear();

	URL_OBJ_LIST::iterator _it = g_FailureUrlsList.begin();
	URL_OBJ_LIST::iterator _end = g_FailureUrlsList.end();

	for(;_it != _end;_it++)
	{
		_URL &obj = *_it;
		int iItem = m_UrlsList.AddString(*obj.GetURL());
		m_UrlsList.SetItemData(iItem, (DWORD_PTR)&obj);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

extern void FreeListBoxStrings(CExListBox *pListBox);
void CDlgFailureUrls::OnBnClickedDelUrls()
{
	// TODO: Add your control notification handler code here
	if(m_UrlsList.GetCount() <= 0) return;
	m_UrlsList.LockWindowUpdate();

	BOOL bChecked = FALSE;
	URL_OBJ_LIST list;
	for(int i = 0 ;i < m_UrlsList.GetCount();i++)
	{
		_URL * lp ;
		if(m_UrlsList.GetCheck(i))
		{
			lp  = (_URL * )m_UrlsList.GetItemData(i);
			bChecked = TRUE;
		}else
			list.push_back(*lp);
	}

	if(bChecked == FALSE ) return ;
	g_FailureUrlsList.clear();
	g_FailureUrlsList.assign(list.begin(),list.end());
	

	FreeListBoxStrings(&m_UrlsList);
	for(URL_OBJ_LIST::iterator it = g_FailureUrlsList.begin();it != g_FailureUrlsList.end();it++)
	{
		_URL &obj = *it;
		int iItem = m_UrlsList.AddString(*obj.GetURL());
		m_UrlsList.SetItemData(iItem, (DWORD_PTR)&obj);
	}
	m_UrlsList.UnlockWindowUpdate();
}
