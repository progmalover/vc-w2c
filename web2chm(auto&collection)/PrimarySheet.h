#pragma once
#include "NewDownloadPage.h"
#include "GeneralPage.h"
#include "DownloadPage.h"
#include "FinishPage.h"
#include "FileTypesPage.h"

class CPrimarySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPrimarySheet)
public:
	CPrimarySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPrimarySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPrimarySheet();

protected:
	DECLARE_MESSAGE_MAP()
private:
	HICON			 m_hIcon;
	CGeneralPage     m_Generalpage;
	CDownloadPage    m_DownPage;
    //CFinishPage    m_Finishpage;
	CFileTypesPage   m_Finishpage;
private:
	void   AddPages();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	BOOL         PreTranslateMessage(MSG* pMsg);
public:
	CNewDownloadPage m_NewPage;
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


