#pragma once

#include "defs.h"
#include "downloadpage.h"
#include "Proxypage.h"
#include "FileTypespage.h"
#include "modifypage.h"
// CDownloadSheet

class CDownloadSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDownloadSheet)

public:
	CDownloadSheet(UINT nIDCaption, BOOL bShowDownloadPage=TRUE,CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDownloadSheet(LPCTSTR pszCaption, BOOL bShowDownloadPage=TRUE,CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDownloadSheet();
	BOOL OnInitDialog();
	void SetProperPage(void);
	afx_msg void OnDestroy();
public:
	static int m_nSelectedPage;
	CDownloadPage  m_Download;
	CModifyPage    m_Modify;
	CProxyPage	   m_Proxy;
	CFileTypesPage m_FileTypes;
protected:
	DECLARE_MESSAGE_MAP()
private:
	void AddPages(BOOL bShowDownloadPage);
	 
};


