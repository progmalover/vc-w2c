#pragma once
#include "afxwin.h"

#include "resource.h"
// CWorkModeDlg dialog

class CWorkModeDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorkModeDlg)

public:
	CWorkModeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkModeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_WORKMODE };
public:
	BOOL EnableWindow(BOOL bEnable =1);
	class CStaticEx:public CStatic
	{
		DECLARE_MESSAGE_MAP()
	public:
		CStaticEx(){};
		~CStaticEx(){};
	public:
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
	};
	CStaticEx m_modeCap;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbWorkMode;
	afx_msg void OnCbnSelchangeWorkModeCombo();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	void SetMode(int nMode);
};
//extern CWorkModeDlg GO_WorkModeDlg;