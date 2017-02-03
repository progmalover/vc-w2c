#include "StdAfx.h"
#include ".\toolbarex.h"
#include "afxres.h"
#include "WorkModeDlg.h"
//#include "afximpl.h"
CToolBarEx::CToolBarEx(void)
{
}

CToolBarEx::~CToolBarEx(void)
{
}
void CToolBarEx::DrawBorders(CDC* pDC, CRect& rect)
{
	 
	// TODO: Add your message handler code here
	// Do not call CToolBar::OnNcPaint() for painting messages
	//
	//CControlBar::DrawBorders(pDC,rect);
	pDC->FillRect(rect,CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
}BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
ON_WM_CREATE()
ON_WM_SIZE()
END_MESSAGE_MAP()

int CToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//GO_WorkModeDlg.Create(MAKEINTRESOURCE(IDD_DLG_WORKMODE),this);
	//GO_WorkModeDlg.ShowWindow(SW_SHOW);
	 
	return 0;
}

void CToolBarEx::OnSize(UINT nType, int cx, int cy)
{
	CToolBar::OnSize(nType, cx, cy);
#if 0
    if(!::IsWindow(GO_WorkModeDlg.m_hWnd))return ;

	CToolBarCtrl  &tbCtrl = this->GetToolBarCtrl();
	int btCount = tbCtrl.GetButtonCount();
	
	if(0 == btCount) return;
	
	DWORD  btnDwSize = tbCtrl.GetButtonSize();
    DWORD  dwWidth = LOWORD(btnDwSize);

	CRect dlgRect;
	GO_WorkModeDlg.GetWindowRect(dlgRect);
	dlgRect.OffsetRect(-dlgRect.TopLeft());

	//this->ScreenToClient(&dlgRect);
	dlgRect.OffsetRect(dwWidth * (btCount),0);

	GO_WorkModeDlg.MoveWindow(&dlgRect);
	GO_WorkModeDlg.ShowWindow(SW_SHOW);
#endif
	// TODO: Add your message handler code here
}
