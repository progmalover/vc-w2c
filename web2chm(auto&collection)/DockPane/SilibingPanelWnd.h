#pragma once

#include "TopTitleBarWnd.h" 
/////////////////////////////////////////////////////////////
//
//zxy 2007.12
//
/////////////////////////////////////////////////////////////////

class CTabPanelWnd : public CWnd
{
	DECLARE_DYNAMIC(CTabPanelWnd)

public:
	CTabPanelWnd();
	CTabPanelWnd(CWnd *pParent);
	virtual ~CTabPanelWnd();
public:
	//CWnd *m_pSilibing;
	CTabPanelTitleWnd  *m_pTabPanelTitle ;
	BOOL  m_bShowTitle;
	BOOL  m_bExtend;

	CSize  m_Size;
	CSize  m_ConstrictSize;
	CSize  m_defaultMaxSize;
	CSize  m_defaultMiniSize;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg  int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	 BOOL Create(CWnd * pParent);
	
	BOOL CreateTitleWnd(void);
	void	ShowTitle(BOOL bShow);
	 
	virtual void RecalcLayout(void);	
	
//	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void PostNcDestroy();
public:
 
	void Constrict(void);
	void Extend(void);

	void HideChildrenExcludeTitle(void);
	void ShowChildren(void);
     
public:
	//void SetMiniSize(CSize& size);

	afx_msg LRESULT OnSizeParent(WPARAM WParam, LPARAM lParam);
	void SetPanelSize(int cx,int cy);
	void GetClientRectExcludeTitle(CRect &cltRect);
 
	void SetTitleText(LPCTSTR strName);

	void SetMaxDefaultSize(CSize maxSize);
	void SetMiniDefaultSize(CSize miniSize);
	CTabPanelWnd * GetLastBuddy(void);
	virtual void SaveState(LPCTSTR lpszProfileName,int nIndex);
	virtual void LoadState(LPCTSTR lpszProfileName, int nIndex);
};


