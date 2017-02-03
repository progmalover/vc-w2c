#pragma once
#include "HistoryComboBox.h"
#include "filepath.h"
// CNewTaskDlg2 dialog

class CNewTaskDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CNewTaskDlg2)

public:
	CNewTaskDlg2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewTaskDlg2();

// Dialog Data
	enum { IDD = IDD_NEWTASK_ANCHOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//CHistoryComboBox m_TargetDir;
	CEdit	m_TargetDir;
public:
	afx_msg void OnBnClickedTaskstart2();
	afx_msg void OnBnClickedTaskconf2();
	afx_msg void OnBnClickedExplore();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAnchorHelp();
private:
	BOOL    CheckPathname(CString strPathName);
};
