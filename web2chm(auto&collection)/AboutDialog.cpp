// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "CodeStore.h"
#include "AboutDialog.h"


// CAboutDialog dialog

IMPLEMENT_DYNAMIC(CAboutDialog, CDialog)
CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDialog::IDD, pParent)
{
}

CAboutDialog::~CAboutDialog()
{
}

void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAboutDialog message handlers

void CAboutDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
