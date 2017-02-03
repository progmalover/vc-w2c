#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "wininet.h"
#include "HttpApi.h"
#include "afxwin.h"
#include "HistoryComboBox.h"
#include "StaticText.h"
#define   RED   RGB(255,0,0)
#define   BLACK RGB(0,0,0)
// CNewDownloadPage 对话框

class CNewDownloadPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNewDownloadPage)

public:
	CNewDownloadPage();
	virtual ~CNewDownloadPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_NEWDOWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL    OnInitDialog();
	virtual LRESULT OnWizardNext();
	virtual BOOL    OnSetActive();
	
private:
	CComboBox        m_WorkModeBox;
	int              m_WorkMode;
	CONNECT_INF      m_ConnectInf;
	CEdit			 m_TargetDir;
	CProgressCtrl    m_ConnectProgress;
	pSYSTEM_CONFIG   m_pSyscf;
	CHistoryComboBox m_URLBox;
	void             ShowMode(int Workmode);
	BOOL             CheckAutoMode();
	BOOL             CheckAnchorMode();
	void             SaveHistoryBox();
	CListBox         m_URLList;
	void             AddURL2List();
	void             ChangeStatic(COLORREF crText,CString strText,BOOL bEnlarge=TRUE);
	CStaticText      m_ModeStatic;
	BOOL             CheckPathname(CString strPathName);
public:
	afx_msg void OnSelchangeWorkmode();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg  HRESULT OnDownLoadProgress(WPARAM mparam,LPARAM lparam);
	afx_msg void OnEnChangeUrledit();
	afx_msg void OnBnClickedConncttest();
	afx_msg void OnBnClickedExplore();
public:
	//Use Finish page
	CString    m_URL;
    CString    m_strPath;
	
}; 
