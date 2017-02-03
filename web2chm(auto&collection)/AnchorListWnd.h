#pragma once
#include ".\dockpane\silibingpanelwnd.h"
#include "DlgBtn.h"
class CAnchorListWnd :
	public CTabPanelWnd
{
public:
	CAnchorListWnd(CWnd *pParent);
	~CAnchorListWnd(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	 
	
protected:
	//CDlgBtn m_DeleteBtn;
	 

	//CDlgBtn m_SelectAllBtn;
	//CDlgBtn m_UnSelectBtn;
	CToolTipCtrl  m_ToolTip;
public:
	void OnDelectSelectionClick(void);
	void OnDelectAllClick(void);
	void OnSelectAllClick(void);
	 
	afx_msg void OnPaint();
	afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	
};
