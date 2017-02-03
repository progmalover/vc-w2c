// GeneralSheet.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "GeneralSheet.h"
#include "ConfigManager.h"
#include "CProgressList.h"
#include ".\generalsheet.h"

// CGeneralSheet
IMPLEMENT_DYNAMIC(CGeneralSheet, CPropertySheet)
CGeneralSheet::CGeneralSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_General);
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
	m_psh.nStartPage = iSelectPage;
}

CGeneralSheet::CGeneralSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_General);
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
	m_psh.nStartPage = iSelectPage;
}

CGeneralSheet::~CGeneralSheet()
{

}


BEGIN_MESSAGE_MAP(CGeneralSheet, CPropertySheet)
END_MESSAGE_MAP()


void CGeneralSheet::WinHelp(DWORD dwData, UINT nCmd)
{
	CPropertySheet::WinHelp(dwData, nCmd);
}

void   CGeneralSheet::ShowHelp(BOOL bShow)
{
	CWnd* pButton = GetDlgItem (IDHELP);
	ASSERT (pButton);
	pButton->ShowWindow (bShow?SW_SHOW:SW_HIDE);
}
BOOL CGeneralSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	//ShowHelp();
	return bResult;
}
