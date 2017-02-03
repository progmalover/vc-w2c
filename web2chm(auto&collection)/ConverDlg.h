#pragma once


// CConverDlg dialog
#include "resource.h"
#include "afxwin.h"
class CConverDlg : public CDialog
{
	DECLARE_DYNAMIC(CConverDlg)

public:
	CConverDlg(int nConver,CWnd* pParent = NULL);   // standard constructor
	virtual ~CConverDlg();

// Dialog Data
	enum { IDD = IDD_CONVER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnProgress(WPARAM wparam, LPARAM lparam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CString m_TaskDir;
	
	afx_msg void OnClose();
	CListBox m_InfoList;
protected:
	virtual void OnOK();
	virtual void OnCancel();
	int		m_nConver;
};
