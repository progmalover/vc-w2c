#pragma once
#include "SimToolBar.h"
#include "staticinstance.h"
#include "HistoryComboBox.h"
#include <AFXINET.H>
#include "HttpApi.h"
#include "w2ebook_utils.h"

//�޸�˵��:��ԭ�л�����������ʷ��¼����
//���ڣ�08.07.18
//Ӱ�죺
//(1)CSimIEBar::Instance()->m_wndAddress.GetWindowText(strAddress)-->CSimIEBar::Instance()->GetCurURL(strAddress);
//
//(2)void CPadFrame:: OnNewAddressEnter()
//{
//	CString str;
//	if(CSimIEBar::Instance()->GetCurURL(str))
//	{
//		((CMfcieView*)GetActiveView())->Navigate2(str, 0, NULL);
//		CSimIEBar::Instance()->AddCurURL();
//	}
//}


class CSimIEBar : public CMFCReBar,public StaticInst<CSimIEBar>
{
	DECLARE_DYNAMIC(CSimIEBar)

public:
	CSimIEBar();
	virtual ~CSimIEBar();
	BOOL    Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS, DWORD dwStyle = WS_CHILD | TBSTYLE_FLAT|WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP, UINT nID = AFX_IDW_REBAR);
	BOOL    GetCurURL(CString &strURL);
	void    AddCurURL(BOOL bSave=TRUE);
protected:
	DECLARE_MESSAGE_MAP()
	void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
public:
	BOOL             CreateClient(void);
	CHistoryComboBox m_wndAddress;
	CSimToolBar		 m_wndToolBar;
	BOOL             SaveAllURL();
	BOOL             Check(CString strText);//�Ƿ��ظ�
public:
	afx_msg void OnDestroy();
//	public:
	afx_msg void OnCbnSelchangeLocationedit();
};


