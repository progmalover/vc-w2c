#pragma once

#include "resource.h"
#include "afxcmn.h"
// CMRUTaskDlg �Ի���

class CMRUTaskDlg : public CDialog
{
	DECLARE_DYNCREATE(CMRUTaskDlg)

public:
	CMRUTaskDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMRUTaskDlg();
// ��д
	 

// �Ի�������
	enum { IDD = IDD_RECENTTASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_TaskList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	
	//afx_msg	void OnDeleteUpdata(CCmdUI *pui);
	//afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
	afx_msg LRESULT OnIdleUpDataUI(WPARAM , LPARAM);
 
	afx_msg void OnNMDblclkTasklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLoadmission();
	afx_msg void OnOpendirectory();
	
};
