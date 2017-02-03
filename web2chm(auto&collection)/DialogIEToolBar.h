#pragma once
#include "resource.h"
#include "afxwin.h"
#include "StaticInstance.h"
#include "HistoryComboBox.h"
#include "visualcontrols.h"
// CDialogIEToolBar dialog
using namespace  VIRSUAL_CONTROLS;
class CDialogIEToolBar : public CDialogEx,public  StaticInst<CDialogIEToolBar>
{
	DECLARE_DYNAMIC(CDialogIEToolBar)
	 
public:
	CDialogIEToolBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogIEToolBar();
	DEC_VISUAL_MANAGER()
// Dialog Data
	enum { IDD = IDD_DIALOG_IEBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void  OnCbnSelchangeLocationedit();
	 
	DECLARE_MESSAGE_MAP()
public:
	CHistoryComboBox m_URLAdressComboBox;
	CMFCButton m_PreBtn;
	CMFCButton m_NextBtn;
	CMFCButton m_ReloadBtn;
	CMFCButton m_CancelBtn;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();

	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoSearchTheWeb();
	afx_msg void OnGoStartPage();
	afx_msg void OnViewStop();
	afx_msg void OnViewRefresh();
};
