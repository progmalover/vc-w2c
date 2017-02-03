// ElementProperSheet.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ElementProperSheet.h"
#include ".\elementpropersheet.h"


// CElementPropertySheet
IHTMLElement*  CElementPropertySheet:: m_pCurElement;
IMPLEMENT_DYNAMIC(CElementPropertySheet, CPropertySheet)
CElementPropertySheet::CElementPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	 
}

CElementPropertySheet::CElementPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_psh.dwFlags =  PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP|PSH_PROPSHEETPAGE;
	m_psh.dwFlags &= ~PSH_HASHELP;
	
	m_ElementPage.m_psp.dwFlags &= ~PSH_HASHELP;
	AddPage(&m_ElementPage);
}

CElementPropertySheet::~CElementPropertySheet()
{
}

BEGIN_MESSAGE_MAP(CElementPropertySheet, CPropertySheet)
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CElementPropertySheet message handlers

//void CElementPropertySheet::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CPropertySheet::OnShowWindow(bShow, nStatus);
//
//}
