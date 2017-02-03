#pragma once


// CTabDocTemp
#include "afxtempl.h"

#include "LookPanel.h"
#include "AddPanel.h"
 
class CTabDocTemp : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabDocTemp)

public:
	CTabDocTemp();
	virtual ~CTabDocTemp();
protected:
		CObArray  m_DocArray;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);


};


