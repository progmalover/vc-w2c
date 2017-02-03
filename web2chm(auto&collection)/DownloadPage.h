#pragma once

#include "resource.h"
#include "afxwin.h"
#include "Defs.h"
#include "ConfigManager.h"
#include "w2ebook_utils.h"

class CDownloadPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDownloadPage)

public:
	CDownloadPage();
	virtual ~CDownloadPage();
	// Dialog Data
	enum { IDD = IDD_OPTION_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedLimit();

public:
	void OnUpDateUI();
	DECLARE_MESSAGE_MAP()
	virtual BOOL    OnInitDialog();
	virtual BOOL    OnApply();
	virtual LRESULT OnWizardNext();
	virtual BOOL    OnSetActive();
private:
	CComboBox m_TotalSizeBox;
	CComboBox m_FileSizeBox;
private:
	void  InitMode(int WorkMode,int DownloadMode);
	void  EnableDeepth(BOOL bEnable = TRUE);
	void  EnableLimit(BOOL bEnable =TRUE);
	void  EnablePathInclude(BOOL bEnabl = TRUE);
private:
    pPRIVATE_CONFIG  m_pConf;
	pSYSTEM_CONFIG   m_pSysConf;
	int              m_MaxFileNum;
	BOOL             CheckData();
	RATIO_CHECK;
public:
	CString m_incStrPath;
};
