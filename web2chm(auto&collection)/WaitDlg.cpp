// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "WaitDlg.h"
#include ".\waitdlg.h"


CWaitDlg::CWaitDlg(CString Msg,int TimeCount,CWnd *pParent)
: CDialog(CWaitDlg::IDD, pParent)
{
	m_Msg=Msg;
	m_TimeCount=TimeCount*10;
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ElapseProgress);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CWaitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_WAITMSG)->SetWindowText(m_Msg);
	m_ElapseProgress.SetRange32(0,m_TimeCount);
	ShowProgress();
	m_Timerhd=SetTimer(911,100,NULL);

	return TRUE;
}

void CWaitDlg::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	ShowProgress();
}

void CWaitDlg::ShowProgress(void)
{
	if(m_TimeCount--)
	{
		CString ts;
		ts.Format("%d s",m_TimeCount/10);
		GetDlgItem(IDC_TIMEREMAIN)->SetWindowText(ts);
		m_ElapseProgress.SetPos(m_TimeCount);
	}
	else
	{
		KillTimer(m_Timerhd);
		EndDialog(IDOK);
	}
}


void CWaitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
