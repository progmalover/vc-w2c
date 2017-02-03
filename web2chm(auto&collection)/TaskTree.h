#pragma once
#include "afxcmn.h"

class CTaskTree :
	public CTreeCtrl
{
public:
	CTaskTree(void);
	~CTaskTree(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void InitImageList(void);
private:	
	CImageList  m_ImageList;
};
