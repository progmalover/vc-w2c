// TabDocTemp.cpp : implementation file
//

#include "stdafx.h"
#include "tabdoc.h"
#include "TabDocTemp.h"
#include ".\tabdoctemp.h"
#include "afximpl.h"

// CTabDocTemp

IMPLEMENT_DYNAMIC(CTabDocTemp, CTabCtrl)
CTabDocTemp::CTabDocTemp()
{
}

CTabDocTemp::~CTabDocTemp()
{
}


BEGIN_MESSAGE_MAP(CTabDocTemp, CTabCtrl)
	ON_WM_CREATE()
//	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
//	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()



// CTabDocTemp message handlers


int CTabDocTemp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
  //  ModifyStyle(0,TBS_BOTTOM,0);

  // 
  // if(m_AddPanel.Create(MAKEINTRESOURCE(IDD_ADDFORM),this)!=TRUE )
  // {
	 //  TRACE0("Create Dlg Add Panel Error!!\n ");
	 //  return -1;
  // }

  //if( m_LookPanel.Create(MAKEINTRESOURCE(IDD_LOOKFORM),this)!=TRUE)
  // {
	 //  TRACE0("Create Dlg Look Panel Error!!\n ");
	 //  return -1;
  // }
  //
  // m_PanelArray.Add(&m_LookPanel);
  // m_PanelArray.Add(&m_AddPanel);

  // for(int i = 0;i < m_PanelArray.GetCount();i++)
  // {
	 //  CString str;
	 //  CWnd *pWnd  = (CWnd *)m_PanelArray[i];
	 //  
	 //  // pWnd->GetWindowText(str);
	 //  str.LoadString(IDD_LOOKFORM - i);
	 //  CTabCtrl::InsertItem(i,str);
  // }
  //
  // m_CurPanel = (CWnd *)m_PanelArray[0];
  //  

	 CTabCtrl::InsertItem(0,_T("工作区"));
	return 0;
}

BOOL CTabDocTemp::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	// initialize common controls
	VERIFY(AfxEndDeferRegisterClass(0x00100));
     dwStyle &= (~WS_BORDER);   
	//CWnd* pWnd = this;
	return CWnd ::Create(WC_TABCONTROL, NULL, dwStyle|TCS_FLATBUTTONS, rect, pParentWnd, nID);
//	return CTabCtrl::Create(dwStyle, rect, pParentWnd, nID);
}



//void CTabDocTemp::OnSize(UINT nType, int cx, int cy)
//{
//	CTabCtrl::OnSize(nType, cx, cy);
//
//	//// TODO: Add your message handler code here
//	//CRect  rect;
//	//CWnd *pParent;
//	//pParent = GetParent();
//	//GetWindowRect(&rect);
//	//pParent->ScreenToClient(&rect);
// //   
//	// rect.top += 40;
//	// rect.left += 5;
//	// rect.right -= 5;
//	// rect.bottom -= 20;
//
//	// for(int i = 0;i < m_PanelArray.GetCount();i++)
//	// {
//	//	((CWnd *)m_PanelArray[i])->MoveWindow(&rect);
//	// }
//	//// m_CurPanel->MoveWindow(&rect);
//	// m_CurPanel->ShowWindow(SW_SHOW);
//
//}

void CTabDocTemp::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	/**pResult = 0;
	int cursel = GetCurSel();
	if(m_CurPanel != m_PanelArray[cursel])
	{
		m_CurPanel->ShowWindow(SW_HIDE);
		m_CurPanel = (CWnd *)m_PanelArray[cursel];
		m_CurPanel->ShowWindow(SW_SHOW);
	}*/
}

void CTabDocTemp::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CTabCtrl::OnWindowPosChanged(lpwndpos);

	// TODO: 在此处添加消息处理程序代码
	// TODO: Add your message handler code here
	//CRect  rect;
	//
 //   GetClientRect(&rect);
	// rect.top += 25;
	// rect.left += 5;
	// rect.right -= 5;
	// rect.bottom -= 20;

	// for(int i = 0;i < m_PanelArray.GetCount();i++)
	// {
	//	((CWnd *)m_PanelArray[i])->MoveWindow(&rect);
	// }
	//// m_CurPanel->MoveWindow(&rect);
	// m_CurPanel->ShowWindow(SW_SHOW);
}
