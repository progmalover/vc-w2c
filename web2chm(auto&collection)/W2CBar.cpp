#include "StdAfx.h"
#include ".\w2cbar.h"
IMPLEMENT_DYNAMIC(CW2CBar, CMyControlBar)
CW2CBar::CW2CBar(void)
{
}

CW2CBar::~CW2CBar(void)
{
}

 CRect CW2CBar:: GetGripperRect()
 {
	CRect cltRect;
	GetClientRect(&cltRect);
	if( IsHorzDocked())
	{
		cltRect.left = 1;
		cltRect.right = cltRect.left + 15;

	}else
	{
		cltRect.left = 3;
		cltRect.bottom = cltRect.top + 15;
	}
	return cltRect;

 }

 void CW2CBar::DrawGripper(CDC* pDC)
 {
	if(IsFloating())
		return;
	CRect grpRect = GetGripperRect(); 
	CRect rcSrc(grpRect);

	CBrush  cBrush;
	CPen    cPen;
	CDC	  memDc;
	CBitmap bitmap;
	 
	rcSrc.OffsetRect(-grpRect.left,-grpRect.top);
	memDc.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC,grpRect.Width(),grpRect.Height());
	cPen.CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));

	CWnd *pWnd = GetFocus();
	if(GetFocusStatus()/*&&(pWnd->m_hWnd == m_hWnd || IsChild(pWnd))*/)
		cBrush.CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));
	else
		cBrush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

	memDc.SelectObject(&bitmap); 
	memDc.FillRect(rcSrc,CBrush::FromHandle(::GetSysColorBrush(COLOR_BTNFACE)));

	memDc.SelectObject(&cPen);
	memDc.SelectObject(&cBrush); 
	  
	memDc.RoundRect(&rcSrc,CPoint(2,2));
	 
	 
	pDC->BitBlt(grpRect.left,grpRect.top,grpRect.Width(),grpRect.Height(),&memDc,0,0,SRCCOPY);
	  

 }
 void CW2CBar:: GetClientRectExcludeTitle(CRect *rcRect)
 {
	GetClientRect(rcRect);
	if(this->IsFloating())
		rcRect->top += 3;
	else
		rcRect->top += 15;
	rcRect->left += 3;
	rcRect->right -= 5;

	if(this->IsFloating())
		rcRect->bottom -= 8;
	else
		rcRect->bottom -= 20;
	 

 }BEGIN_MESSAGE_MAP(CW2CBar, CMyControlBar)
	 ON_WM_SHOWWINDOW()
//	 ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

 void CW2CBar::OnShowWindow(BOOL bShow, UINT nStatus)
 {
	 CMyControlBar::OnShowWindow(bShow, nStatus);

	 // TODO: Add your message handler code here
	 LoadBarStatus();
 }


 void CW2CBar::LoadBarStatus()
 {
	/* CString strSec;
	 strSec.Format(_T("%s%d"),this->GetWindowText(),this->GetDlgCtrlID());
	 AfxGetApp()->WriteProfileInt(strSec,_T("ShowStatus"),this->IsWindowVisible());

	 CRect wndRect;
	 this->GetWindowRect(wndRect);
	 AfxGetApp()->WriteProfileBinary(strSec,_T("barSize"),&wndRect,sizeof(wndRect));*/

	 
 }

 void CW2CBar::SaveBarStatus()
 {
	 CString strSec;
	 CString strWndText;
	 this->GetWindowText(strWndText);

	 strSec.Format(_T("%s%d"),strWndText,this->GetDlgCtrlID());
	 AfxGetApp()->WriteProfileInt(strSec,_T("ShowStatus"),this->IsWindowVisible());

	/* CRect wndRect;
	 this->GetWindowRect(wndRect);
	 AfxGetApp()->WriteProfileBinary(strSec,_T("barSize"),wndRect.g,sizeof(Size));*/
 }

// void CW2CBar::OnClose()
// {
//	 // TODO: Add your message handler code here and/or call default
//	 SaveBarStatus();	
//	 CMyControlBar::OnClose();
// }

 void CW2CBar::OnDestroy()
 {

	  SaveBarStatus();
	  CMyControlBar::OnDestroy();

	 // TODO: Add your message handler code here
 }
