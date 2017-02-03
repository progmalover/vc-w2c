// ConverDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ConverDlg.h"
#include ".\converdlg.h"
#include "TCompilerTask.h"
#include "hmcompiler.h"
// CConverDlg dialog

IMPLEMENT_DYNAMIC(CConverDlg, CDialog)
CConverDlg::CConverDlg(int nConver,CWnd* pParent /*=NULL*/)
	: CDialog(CConverDlg::IDD, pParent)
{
	//m_TaskDir = workDir ;
	m_nConver = nConver;
}

CConverDlg::~CConverDlg()
{
}

void CConverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_InfoList);
}


BEGIN_MESSAGE_MAP(CConverDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_CHM_PRGRESS ,OnProgress)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConverDlg message handlers

void CConverDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	 TCompilerTask  CT(this,m_TaskDir);
	 CT.Create(m_nConver,NULL);
//	CT.Start();
	// TODO: Add your message handler code here
}

LRESULT CConverDlg::OnProgress(WPARAM wparam, LPARAM lparam)
{
	CString str((LPSTR)wparam);
	CString strFileName = ::PathFindFileName(str);
	m_InfoList.SetCurSel(m_InfoList.AddString(strFileName));
	
	return S_OK;
}

void CConverDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	/*if(ChmCompiler::b_Exit == FALSE)
	{
		ChmCompiler::b_Exit = TRUE;
		return;
	}*/

 	if(ChmCompiler::b_Exit)
		CDialog::OnCancel();
	 if(TxtCompiler::b_Exit)
		CDialog::OnCancel();


	  //CDialog::OnClose();
	 
}

void CConverDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}

void CConverDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnCancel();
}
