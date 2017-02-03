#pragma once
#include "afxcmn.h"
#include "resource.h"
class CWaitDlg : public CDialog
{
public:
	CWaitDlg(CString Msg,int TimeCount,CWnd *pParent=NULL);
	enum { IDD = IDD_DLG_WAITING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	CString m_Msg;
	int m_TimeCount;
	UINT_PTR m_Timerhd;
	CProgressCtrl m_ElapseProgress;

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	void ShowProgress(void);
public:
	afx_msg void OnBnClickedOk();
};
