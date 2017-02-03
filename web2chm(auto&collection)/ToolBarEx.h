#pragma once
#include "afxext.h"

class CToolBarEx :
	public CToolBar
{
public:
	CToolBarEx(void);
	~CToolBarEx(void);
	virtual void DrawBorders(CDC* pDC, CRect& rect);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
