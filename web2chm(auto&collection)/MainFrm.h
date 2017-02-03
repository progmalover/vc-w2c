// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "TabDocTemp.h";
#include "WorkSpace.h"
#include "PadFrame.h"
#include "LookPanel.h"
#include "AddPanel.h"
#include "OutBar.h"
//#include "ToolBarEx.h"
#include "ReBarEx.h"
#include "IEbar.h"
#include "w2cribbonbar.h"
class CMainFrame : public CFrameWndEx
{
	friend class CPadFrame;
protected: // 仅从序列化创建

	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MENUXP()   
// 属性


// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:	
	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCStatusBar		m_wndStatusBar;
	//CMFCToolBar 		m_wndToolBar;
	CW2CRibbonBar		m_wndRibbonBar;
	
	CImageList			m_ImageList;  
	COutBar				m_CtrlBar;
	CWorkSpace			m_WorkSpace;
	
	 
public:
   
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext); 
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnClose();
	afx_msg void Ontabdoctype();
	afx_msg void OnUpdatetabdoctype(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConfigTaskconfig();
	afx_msg void OnConfigSysconfig();
	afx_msg void OnFileMruFile1();
	afx_msg void OnFileMruFilelist();
	afx_msg void OnUpdateViewPropertypanel(CCmdUI *pCmdUI);
	afx_msg void OnViewPropertypanel();
	afx_msg void OnFileNew();
	afx_msg void OnAppHelp();
	afx_msg void OnUpdateAppHelp(CCmdUI *pCmdUI);
	afx_msg void OnTaskRefresh();
	afx_msg HRESULT OnEditUrls(WPARAM wparam, LPARAM lparam);
	afx_msg HRESULT StartConvert(WPARAM wparam,LPARAM lparam);
	afx_msg void OnAbandonJob(void);
	afx_msg void OnPauseJob(void);
	afx_msg void OnResumeJob(void);
	afx_msg void OnSuperKey(void);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewRefresh(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAnchorpanel(CCmdUI *pCmdUI);
	afx_msg void OnViewAnchorpanel();
	afx_msg void OnCut();
	afx_msg void OnHelpRegister();
	afx_msg void OnUpdateHelpRegister(CCmdUI *pCmdUI);
	afx_msg void OnAnchorListMenuUpdate(CCmdUI *pUI);
	afx_msg void OnBtnKillUpdate(CCmdUI*pUI);
	afx_msg void OnBtnSuspendUpdate(CCmdUI*pUI);
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonKill();
	afx_msg void OnBnClickedCatchTool();
	afx_msg void OnBnCatchToolUpdate(CCmdUI*pUI);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	void CacalLayout(void);
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PostNcDestroy();
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | 
							FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	void SetCenterWindow(void);
	void OnUpDateAbandonJobUI(CCmdUI* pUI);
	void OnUpDateResumeJobUI(CCmdUI* pUI);
	void OnUpDatePauseJobUI(CCmdUI* pUI);
	void OnApplicationLook();
	BOOL CreateRibbonBar();
	CWnd * GetActiveViewWnd(void);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


