#pragma once

#include "BtnDlg.h"
#include "TaskTree.h"
#include "AppDockablePane.h"
class CWorkSpace :
	public CAppDockablePane
{

    DECLARE_DYNAMIC(CWorkSpace)
public:
	CWorkSpace(void);
	~CWorkSpace(void);

public:
	 CTaskTree    m_TypeTree;
	// CBtnDlg      m_TskToolDlg;/*a plate tool bar*/
protected: 
	DECLARE_MESSAGE_MAP()
	 afx_msg void OnFileNew();
public:
	 virtual BOOL  Create(CWnd* pParentWnd, UINT nID, LPCTSTR lpszWindowName, CSize sizeDefault , DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBRS_TOP);
	 afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);
	 afx_msg void  OnTvToolTip( NMHDR * pNotifyStruct, LRESULT * result );

	
	 void InitTaskTree(void);
	

	 virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	 afx_msg void OnSetFocus(CWnd* pOldWnd);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
};
  