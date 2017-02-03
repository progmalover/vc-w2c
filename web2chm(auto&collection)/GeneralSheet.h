#pragma once


#include "generalpage.h"
// CGeneralSheet

class CGeneralSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CGeneralSheet)

public:
	CGeneralSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CGeneralSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CGeneralSheet();

	DECLARE_MESSAGE_MAP()
public:
	CGeneralPage m_General;
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual BOOL OnInitDialog();
	void ShowHelp(BOOL bShow=TRUE);
	afx_msg void OnDestroy();
};


