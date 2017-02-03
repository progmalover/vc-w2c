#pragma once
#include "appdockablepane.h"
#include "StaticInstance.h"
#include "HistoryComboBox.h"
class CIEPane :
	public CAppDockablePane,public StaticInst<CIEPane>
{
	DECLARE_DYNAMIC(CIEPane)
public:
	CIEPane(void);
	~CIEPane(void);

protected:
	DECLARE_MESSAGE_MAP()

	int InitlizeLayoutCtrls();
public:
	 

	BOOL				SaveAllURL();
	BOOL				Check(CString strText);// «∑Ò÷ÿ∏¥
	BOOL				GetCurURL(CString &strURL);
	void				AddCurURL(BOOL bSave=TRUE);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	HRESULT OnUpdateUI(WPARAM wparam, LPARAM lparam);
};

