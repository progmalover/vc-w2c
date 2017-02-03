#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "wininet.h"
#include "HttpApi.h"
#include "afxwin.h"
#include "HistoryComboBox.h"
#include "filepath.h"

class CNewTaskDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewTaskDlg)

public:
	CNewTaskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewTaskDlg();

// 对话框数据
	enum { IDD = IDD_NEWTASK_AUTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CProgressCtrl m_ConnectProgress;
	CString m_URL;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTaskconf();
	afx_msg  HRESULT OnDownLoadProgress(WPARAM mparam,LPARAM lparam);
	afx_msg void OnBnClickedExplore();
	afx_msg void OnBnClickedTaskstart();
	afx_msg void OnBnClickedConncttest();
private:
	CONNECT_INF  m_ConnectInf;
	BOOL         SpreadDlg(void);
	BOOL         SubDlg(void);
	//CHistoryComboBox m_TargetDir;
	CEdit	     m_TargetDir;
	CHistoryComboBox m_URLBox;
	void         SaveHistoryBox();
	BOOL         CheckURL();
	BOOL         CheckPathname(CString strPathName);
protected:
	void         OnOK();
 
};
