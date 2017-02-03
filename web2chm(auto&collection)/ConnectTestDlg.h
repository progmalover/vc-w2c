#pragma once

#include "resource.h"
#include "HttpApi.h"
#include "afxcmn.h"
// CConnectTestDlg �Ի���

class CConnectTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectTestDlg)

public:
	CConnectTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectTestDlg();

// �Ի�������
	enum { IDD = IDD_CONNECTTESTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetConnectInf(CONNECT_INF* CInf);
private:
	CONNECT_INF  *m_pinf;
public:
	virtual BOOL OnInitDialog();
protected:
	CListCtrl m_DspList;
};
