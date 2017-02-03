#pragma once

#include "resource.h"
#include "afxwin.h"
// CElementProperty dialog

class CElementProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CElementProperty)

public:
	CElementProperty();
	virtual ~CElementProperty();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ELEMENT_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_List;
};
