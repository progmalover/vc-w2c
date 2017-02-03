#pragma once

#include "Defs.h"
// CDownTaskList

class CDownTaskList : public CListCtrl
{
	DECLARE_DYNAMIC(CDownTaskList)

public:
	CDownTaskList();
	virtual ~CDownTaskList();

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg HRESULT OnDownProcess(WPARAM wparam ,LPARAM lparam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int FindInItems(_URL_TASK * pTask);
};


