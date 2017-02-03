#pragma once
#include "resource.h"
#include "defs.h"
#include "configmanager.h"
#include "w2ebook_utils.h"

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralPage)

public:
	CGeneralPage();
	virtual ~CGeneralPage();

// Dialog Data
	enum { IDD = IDD_OPTION_SYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

	DECLARE_MESSAGE_MAP()
protected :
	CToolTipCtrl  m_toolTip;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox	m_ComboTrdNumber;
	CComboBox	m_ComboWaitTime;
	CComboBox	m_ComboConnectNum;
	CEdit		m_RecordNum;
private:
	pSYSTEM_CONFIG m_pSysConfig;
	BOOL           CheckData();
	RATIO_CHECK;
public:
	virtual BOOL    OnInitDialog();
	virtual BOOL    OnApply();
	virtual LRESULT OnWizardNext();
	afx_msg void OnBnClickedCheckWarzard();
	afx_msg void OnBnClickedCheckAskBefore();
};
