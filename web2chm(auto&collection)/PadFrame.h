#pragma once


// CPadFrame 框架

#include "HttpApi.h"
#include "SimIEBar.h"
#include "mfcieVw.h"

class CPadFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPadFrame)
public:
	CPadFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CPadFrame();
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	CMenu				m_FavMenu;
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void DoNothing();
	afx_msg void OnNewAddressEnter();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual CWnd* GetMessageBar();
public:
	bool CreateIEBar(void);
	
	virtual void RecalcLayout(BOOL bNotify = TRUE);
 
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	int BuildFavMenu(LPCTSTR pszPath, int nStartPos , CMenu * pMenu);
	afx_msg void OnDestroy();
 
};


