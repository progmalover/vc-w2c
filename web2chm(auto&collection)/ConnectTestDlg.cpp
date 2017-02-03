// ConnectTestDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ConnectTestDlg.h"
#include ".\connecttestdlg.h"
#include "afxinet.h"
// CConnectTestDlg 对话框

IMPLEMENT_DYNAMIC(CConnectTestDlg, CDialog)
CConnectTestDlg::CConnectTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectTestDlg::IDD, pParent)
{
}

CConnectTestDlg::~CConnectTestDlg()
{
}

void CConnectTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFDISP, m_DspList);
}


BEGIN_MESSAGE_MAP(CConnectTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConnectTestDlg 消息处理程序

void CConnectTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CConnectTestDlg::SetConnectInf(CONNECT_INF  * pCInf)
{
   m_pinf = pCInf;
}

/*
DWORD bitRat;
DWORD respondTime; 
DWORD serverType;
DWORD downSize;  
DWORD useTime;
INTERNET_PORT port;

CString nameServer;
CString nameObject;
*/
BOOL CConnectTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    CRect rect ;
	GetClientRect(&rect);
	m_DspList.InsertColumn(0,"Property",LVCFMT_LEFT,rect.Width()/2-10);
	m_DspList.InsertColumn(1,"Content",LVCFMT_LEFT,rect.Width()/2-11);

    CString  strText;


	//m_DspList.SetItemText(0,0,"Server Name");
	m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 0, "Server Name",
				0, LVIS_SELECTED,0, 0);

	m_DspList.SetItemText(0,1,m_pinf->nameServer);	
	
	//m_DspList.SetItemText(1,0,"Server Name");
	m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 1, "Server Type",
		0, LVIS_SELECTED,0, 0);
	switch(m_pinf->serverType)
	{
		
		case AFX_INET_SERVICE_HTTP: 
		case AFX_INET_SERVICE_HTTPS:
		
		    m_DspList.SetItemText(1,1,"Http Server");  
			break;
		default:
		
			m_DspList.SetItemText(1,1,"Unknow Server");
			break;
	}
	// m_DspList.SetItemText(2,0,"Server Port");  
	
	m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 2, "Server Port",
		0, LVIS_SELECTED,0, 0);
	 strText.Format("%d",m_pinf->port);
	 m_DspList.SetItemText(2,1,strText);  
	
	 m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 3, "Responed Time",
		 0, LVIS_SELECTED,0, 0);
	 strText.Format("%d ms",m_pinf->respondTime);
	 m_DspList.SetItemText(3,1,strText);

	 m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 4, "Download Size",
		 0, LVIS_SELECTED,0, 0);
	 strText.Format("%d bytes",m_pinf->downSize);
	 m_DspList.SetItemText(4,1,strText);

	// m_DspList.SetItemText(3,0,"Bit Rate");
	 m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 5, "Used Time",
		 0, LVIS_SELECTED,0, 0);
	 strText.Format("%d ms",m_pinf->useTime);
	 m_DspList.SetItemText(5,1,strText);
	
	// m_DspList.SetItemText(4,0,"Use Time");
	 m_DspList.InsertItem(LVIF_TEXT|LVIF_STATE, 6, "bits Rate",
		 0, LVIS_SELECTED,0, 0);
	 strText.Format("%d bits/s",m_pinf->bitRat);
	 m_DspList.SetItemText(6,1,strText);
	 

	return TRUE;  
}
