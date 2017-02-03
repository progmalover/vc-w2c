#pragma once
#include "Defs.h"
#include "resource.h"
#include "ConfigManager.h"
#include "w2ebook_utils.h"

class CFileTypesPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFileTypesPage)

public:
	CFileTypesPage(BOOL bShowAnvance=FALSE);
	virtual ~CFileTypesPage();

	// Dialog Data
	enum { IDD = IDD_OPTION_FILETYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	afx_msg void OnBnClickedButtonAdvance();
private:
	pPRIVATE_CONFIG  m_pConf;
	BOOL  InitModify();
	void  GetModify();
	/////////////for wizard mode
	void    CreateAutoTask(TWebSession *tpSession,int dwThrdNum);
	void    CreateAnchorTask(TWebSession *tpSession,int dwThrdNum);
	RATIO_CHECK;
};
