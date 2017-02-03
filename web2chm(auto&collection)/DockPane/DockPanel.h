#pragma once
/*
AUTHOR:: ZHANGXY 
2007.12  DESIGNED FOR SOTHINK COMPANY
*/
//#include "DockContextex.h"
#include "SilibingPanelWnd.h" 
#include  <vector>
using namespace::std;
// CDockPanel
typedef   vector<CTabPanelWnd *>  ARRAY_PANELS;
class CDockPanel : public CWnd
{
	DECLARE_DYNAMIC(CDockPanel)

public:
	CDockPanel();
	CDockPanel(CWnd *pParent);
	virtual ~CDockPanel();
	BOOL Create(CWnd* pParent);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void     RecalcLayout(void);
	virtual BOOL  AssignChildrenTo(CWnd *pNewParent);	
	virtual BOOL  CreateChildren(void);
private:
	//CDockContextEx *m_pDockContext;//这里对CPanelCtrlBar的m_pDockContext 作一个引用
	//CRect  m_storeRect;
	CSize m_storeSize;
public:
	ARRAY_PANELS  m_arrPanel;
	BOOL				   m_bOrder; /*0 : up to down,1 :down to up*/
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CTabPanelWnd * AddTabPanelWnd(void);
protected:
	virtual void PostNcDestroy();
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
 
	void SetItemExtend(CTabPanelWnd* pItem, BOOL bEnable);
	void AddTabPanelItem(CTabPanelWnd * pItem);
	void StoreSpace(CRect  &Rect);
	void RestoreSpace(CRect & Rect);
	void MakeChildrenFit(CRect cltRect);
	void PlayMessage(UINT msg,WPARAM,LPARAM);
};


