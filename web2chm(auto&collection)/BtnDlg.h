#pragma once
#include "resource.h"
#include "DlgBtn.h"
// CBtnDlg 对话框

class CBtnDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtnDlg)

public:
	CBtnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBtnDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
private:
	CDlgBtn  m_btnStart;
	CDlgBtn  m_btnStop;
	CDlgBtn  m_btnKill;
    CToolTipCtrl m_ToolTip;
    CImageList	 m_Imagelist;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTaskStart();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	void OnBtnStartUpdate(CCmdUI*pUI);
	void OnBtnSuspendUpdate(CCmdUI *pUI);
	void OnBtnKillUpdate(CCmdUI *pUI);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonKill();
};
