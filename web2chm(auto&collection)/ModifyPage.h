#pragma once
#include "Defs.h"
#include "resource.h"
#include "ConfigManager.h"

class CModifyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CModifyPage)

public:
	CModifyPage();
	virtual ~CModifyPage();

// Dialog Data
	enum { IDD = IDD_OPTION_MODIFY };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckUsedefault();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
private:
	 pPRIVATE_CONFIG  m_pConf;
	 void  Enable(int ID,BOOL bEnable)
	 {
		 GetDlgItem(ID)->EnableWindow(bEnable);
	 }

	 void  SetCheck(int ID,BOOL bCheck)
	 {
		 CButton *pbtn = (CButton*)GetDlgItem(ID);
		 pbtn->SetCheck(bCheck);
	 };

	 void SetCheckEx(int ID,BOOL bCheck,BOOL bEnable)
	 {
		 SetCheck(ID,bCheck);
		 Enable(ID,bEnable);
	 }

	 BOOL GetCheck(int ID)
	 {
		 CButton *pbtn = (CButton*)GetDlgItem(ID);
		 return pbtn->GetCheck();
	 };

	 void InitDefault(BOOL bEnable);	
};
