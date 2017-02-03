#pragma once
#include "TabTrack.h"

/////////////////////////////////////////////////////////////
//
//zxy   ....2007.12
//
/////////////////////////////////////////////////////////////////


#define  TAB_TITLE_HTIGHT  18
class CTabPanelTitleWnd : public CWnd 
{
	DECLARE_DYNAMIC(CTabPanelTitleWnd)

public:
	CTabPanelTitleWnd();
	CTabPanelTitleWnd(CWnd *pParent);
	virtual ~CTabPanelTitleWnd();
public:
	BOOL Create(CWnd *pWnd);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void	ConstrictParent(void);
	virtual void	ExtendParent(void);
	virtual void   RecalcLayout();

protected:
	CSize			m_storeSize;
	CRect			m_Handler;

	BOOL			m_bPtInHandler;
	BOOL			m_bDraging;
	BOOL			m_bActive;

	CTabTrack	m_Track;

	CBitmap    m_bmpArry[6];//title bar bmp.....
	CSize		  m_bmpSizeArry[6];
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	 
protected:
	virtual void PostNcDestroy();

public:
	CWnd * GetDockedPanel(void);
	void GetTitleBarWndSize(CSize &size);
	void DrawFace(CDC * pDC);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HRESULT OnCaptureChanged(WPARAM wparam,LPARAM lparam);
	//void Track(void);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	BOOL ImFirstOne(void);
	BOOL CanDrag(void);
	BOOL PreItemExtend(void);
 
	void InitTitleResource(void);
	HRESULT OnUpdateUI(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


