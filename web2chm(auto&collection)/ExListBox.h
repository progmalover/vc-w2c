#if !defined(AFX_EXLISTBOX_H__202DD0DE_98E2_4300_A841_C20B49F46367__INCLUDED_)
#define AFX_EXLISTBOX_H__202DD0DE_98E2_4300_A841_C20B49F46367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExListBox.h : header file
//
#include "OwnerDrawListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CExListBox window
#define WM_REFRESHCAPTION   WM_USER + 88
class CExListBox : public COwnerDrawListBox
{
// Construction
	

public:
	CExListBox();
    DECLARE_DYNCREATE(CExListBox)
// Attributes
public:
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	UINT ItemFromPoint2(CPoint pt, BOOL& bOutside)  ;
//	void PreSubclassWindow() ;
	
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	BOOL SetEnableFullPath(BOOL bEnale = FALSE);
	BOOL GetCheck(int){return FALSE;}
	void SetCheck(int,int){};
	virtual void OnDeleteString(LPCTSTR lpStr);
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExListBox();

	// Generated message map functions
	int m_dxItem;
protected:
	//{{AFX_MSG(CExListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void SetNoSel(void);
	//int  AddString(LPCTSTR lpszItem);
	void GetText(int iItem,CString &str);
	void SetRawWidth(int nWidth);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXLISTBOX_H__202DD0DE_98E2_4300_A841_C20B49F46367__INCLUDED_)
