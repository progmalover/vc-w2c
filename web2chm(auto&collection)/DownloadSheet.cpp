// DownloadSheet.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "DownloadSheet.h"
#include "afxpriv.h"
#include "shlwapi.h"
#include "ConfigManager.h"
#include ".\downloadsheet.h"

int CDownloadSheet::m_nSelectedPage = 0;

IMPLEMENT_DYNAMIC(CDownloadSheet, CPropertySheet)
CDownloadSheet::CDownloadSheet(UINT nIDCaption,BOOL bShowDownloadPage, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPages(bShowDownloadPage);
	m_psh.nStartPage = iSelectPage;
}

CDownloadSheet::CDownloadSheet(LPCTSTR pszCaption, BOOL bShowDownloadPage,CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPages(bShowDownloadPage);
	m_psh.nStartPage = iSelectPage;
}

void CDownloadSheet::AddPages(BOOL bShowDownloadPage)
{
	if(bShowDownloadPage)
	{
		AddPage(&m_Download);
		AddPage(&m_FileTypes);
	}
	AddPage(&m_Proxy);
	AddPage(&m_Modify);
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
	//m_psh.dwFlags |= PSH_HASHELP;
}

CDownloadSheet::~CDownloadSheet()
{

}


BEGIN_MESSAGE_MAP(CDownloadSheet, CPropertySheet)
     ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTaskConfig 消息处理程序

BOOL CDownloadSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	return TRUE;  
}

void CDownloadSheet::OnDestroy()
{
	CPropertySheet::OnDestroy();
	m_nSelectedPage = GetTabControl()->GetCurSel();
}