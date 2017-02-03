#pragma once
#include ".\w2cbar.h"
#include "staticinstance.h"
#include ".\DockPane\DockPanel.h"
#include ".\AppDockablePane.h"
class CAnchorsBar :
	public CAppDockablePane,public StaticInst<CAnchorsBar> 
{
	DECLARE_DYNAMIC(CAnchorsBar)
	DECLARE_MESSAGE_MAP()
public:
	CAnchorsBar(void);
	~CAnchorsBar(void);
	CDockPanel * GetPanel();
	virtual void RecalcLayout();
private:
	CDockPanel *m_pDockPanel;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL   OnAddDockPanel(void);
	afx_msg void   OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
