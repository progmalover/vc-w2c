#pragma once


// CSimToolBar

class CSimToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CSimToolBar)

public:
	CSimToolBar();
	virtual ~CSimToolBar();
   	virtual BOOL SetStatusText(INT_PTR nHit);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


