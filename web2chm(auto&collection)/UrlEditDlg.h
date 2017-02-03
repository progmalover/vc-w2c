#pragma once
#include "afxcmn.h"
#include "defs.h"
#include "exlistbox.h"

class CUrlEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CUrlEditDlg)

public:
	CUrlEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUrlEditDlg();

// Dialog Data
	enum { IDD = IDD_URL_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CExListBox m_UrlList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Recalculate(void);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	virtual BOOL OnInitDialog();
	 
	afx_msg void OnBnClickedUrlSelectAll();
	afx_msg void OnBnClickedUndoSelect();
	afx_msg void OnBnClickedUrlDeleteAll();
	afx_msg void OnBnClickedUrlDelete();
public:
	URL_LIST  *m_pList;
	afx_msg void OnBnClickedUrlExit();

	
	//afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
};
