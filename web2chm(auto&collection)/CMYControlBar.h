// CMYControlBar.h : header file
/////////////////////////////////////////////////////////////////////////////

#if !defined(CMYCONTROLBAR_H_INCLUDED)
#define CMYCONTROLBAR_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DockBtn.h"

/////////////////////////////////////////////////////////////////////////////
// CMyControlBar class

class CMyControlBar : public CControlBar
{
	DECLARE_DYNAMIC(CMyControlBar)

// Construction
public:
	CMyControlBar();

// Attributes
protected:
	BOOL		 m_bInRecalcNC;
    BOOL         m_bTracking;
	BOOL		 m_bGripper;
	BOOL		 m_bButtons;
    UINT         m_cxEdge;
	BOOL		 m_barFocus;
	UINT		 m_menuID;
    UINT         m_nDockBarID;
    CSize        m_sizeMin;
    CSize        m_sizeHorz;
    CSize        m_sizeVert;
    CSize        m_sizeFloat;
    CRect        m_rectBorder;
    CRect        m_rectTracker;
    CPoint       m_ptOld;
	CDockBtn    m_btnClose;
	CDockBtn    m_btnMinim;
	CImageList*  m_ImageList;
	CToolTipCtrl m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyControlBar)
	public:
	virtual void	OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL	Create(CWnd* pParentWnd, UINT nID, LPCTSTR lpszWindowName = NULL, CSize sizeDefault = CSize(200,200), DWORD dwStyle = WS_CHILD|WS_VISIBLE|CBRS_TOP);
    virtual CSize	CalcFixedLayout( BOOL bStretch, BOOL bHorz );
    virtual CSize	CalcDynamicLayout( int nLength, DWORD dwMode );
	virtual BOOL	IsFloating();
//	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsHorzDocked() const;
	BOOL IsVertDocked() const;
	void ShowBar( BOOL bEnable);
	CWnd * ShowFloatFrame(BOOL enable);
	void ShowFrameStuff(BOOL bGripper = TRUE, BOOL bButtons = TRUE) { 
		m_bGripper = bGripper;
		m_bButtons = bButtons;
	}

	virtual void DrawGripper(CDC* pDC);
	virtual CRect GetGripperRect();
	virtual CRect GetButtonRect();
	UINT GetMenuID();
	void SetMenuID(UINT nID);
	CImageList* SetBtnImageList(CImageList *pImageList);
	void OnInvertTracker(const CRect& rect);
	void StopTracking(BOOL bAccept);
	void StartTracking();
	CPoint& ClientToWnd(CPoint& point);
	virtual ~CMyControlBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CMyControlBar)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonClose();
	afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
	afx_msg void OnButtonMinimize();
	afx_msg void OnUpdateButtonMinimize(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	void RecaclMainFrame(void);
	CWnd * GetMainFrame(void);
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GetFocusStatus(void);
protected:
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	void SetBarSize(int cx , int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(CJCONTROLBAR_H_INCLUDED)

