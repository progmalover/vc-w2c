#pragma once
#include "afxwin.h"
#include "resource.h"
#include "exlistbox.h"
#include "httpApi.h"
#include "defs.h"
// CDlgFailureUrls dialog
#include "ExListBox.h"
class CDlgFailureUrls : public CDialog
{
	DECLARE_DYNAMIC(CDlgFailureUrls)

public:
	CDlgFailureUrls(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFailureUrls();

// Dialog Data
	enum { IDD = IDD_DOWNLOAD_FAILURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExListBox m_UrlsList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAbond();
	afx_msg void OnBnClickedRetry();

	static  void ReBkFailureUrl( _URL &);
	static  URL_OBJ_LIST m_FailureUrlsList;
public:
	DWORD  m_dwStatus;
	afx_msg void OnLbnSelchangeUrlList();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelUrls();
};
extern  URL_OBJ_LIST   g_FailureUrlsList;