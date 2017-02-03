#pragma once


// CHitsListCtrl

class CHitsListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CHitsListCtrl)

public:
	CHitsListCtrl();
	virtual ~CHitsListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


