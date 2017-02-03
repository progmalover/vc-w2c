#pragma once
#include "resource.h"
#include "Defs.h"
#include "ConfigManager.h"

class CProxyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CProxyPage)

public:
	CProxyPage();
	virtual ~CProxyPage();

// Dialog Data
	enum { IDD = IDD_OPTION_PROXY };
	afx_msg void OnBnClickedUseProxy();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
private:
	 pPRIVATE_CONFIG  m_pConf;
	 void EnableProxy(BOOL bEnable=TRUE);
	 BOOL  GetCheck(int ID)
	 {
		 CButton *pbtn = (CButton*)GetDlgItem(ID);
		 return pbtn->GetCheck();
	 };
};
