#pragma once
#include ".\dockpane\silibingpanelwnd.h"

class CAnchorPanelWnd :
	public CTabPanelWnd
{
public:
	CAnchorPanelWnd(CWnd *pParent);
	~CAnchorPanelWnd(void);

	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTreeEndLabelEdit(NMHDR* pHdr,LRESULT *lpResult);
	afx_msg void OnCut(void);
	afx_msg void OnCutAll(void);

private:
	CImageList m_ImagList; 
	 
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnUpdateCut(CCmdUI *pCmdUI);

	afx_msg void OnUpdateCutAll(CCmdUI* pCmdUI);
	afx_msg void OnEndTreeDrag(NMHDR * phdr, LRESULT * pResult);
	afx_msg void OnBeginTreeDrag(NMHDR * phdr, LRESULT * pResult);
	DECLARE_MESSAGE_MAP()
	
	
};
