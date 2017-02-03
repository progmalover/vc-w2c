#pragma once


// CAppDockablePane

class CAppDockablePane : public CDockablePane
{
	DECLARE_DYNAMIC(CAppDockablePane)

public:
	CAppDockablePane();
	virtual ~CAppDockablePane();
	virtual BOOL	Create(CWnd *pParent,LPCTSTR  lpCaption, CSize &size,UINT dwStyle,UINT nID);
	
	virtual void	GetTitleRect(CRect *pRect);
	virtual	void	GetClientRectExcludeTitle(CRect *pRect);
	virtual	void	GetClientRectExcludeBorder(CRect *pRect);
	virtual void	PaintClient(CDC & dc){};
	virtual	BOOL	IsHorzDocked();
	virtual BOOL	IsTabDocked();
	virtual void	SetCaptionButtons();
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnNeedTipText(UINT id, NMHDR* pNMH, LRESULT* pResult);
};


