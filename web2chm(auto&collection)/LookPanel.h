#pragma once
#include "afxcmn.h"
//#include "HitsListCtrl.h"
#include "DownTaskList.h"
// CLookPanel 窗体视图

class CLookPanel : public CFormView
{
	DECLARE_DYNCREATE(CLookPanel)

public:
	CLookPanel();           // 动态创建所使用的受保护的构造函数
	virtual ~CLookPanel();

public:
//	enum { IDD = IDD_LOOKPANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	 
	bool InitHelper(void);

protected:
//	virtual void PreSubclassWindow();
public:
	virtual void OnInitialUpdate();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CDownTaskList m_DownTaskList;
};

extern CListCtrl * gp_DownTaskList;
