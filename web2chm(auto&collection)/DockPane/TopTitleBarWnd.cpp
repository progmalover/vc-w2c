// TopTitleBarWnd.cpp : implementation file
//

/////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Glanda.h"
#include "TopTitleBarWnd.h"
#include "SilibingPanelWnd.h"
#include ".\toptitlebarwnd.h"
#include "DockPanel.h"
#include "tools.h"
#include "TabTrack.h"
#include "Graphics.h"
#include "resource.h"
#define  IDW_TOP_TITLEBAR_WND     WM_USER + 100
// CTabPanelWnd
 
#define WM_IDLEUPDATECMDUI  0x0363  
IMPLEMENT_DYNAMIC(CTabPanelTitleWnd, CWnd)
CTabPanelTitleWnd::CTabPanelTitleWnd() 
{
	EnableActiveAccessibility();
}

CTabPanelTitleWnd::CTabPanelTitleWnd(CWnd *pParent)
{
	Create(pParent);
	m_bPtInHandler = FALSE;
	m_bDraging = FALSE;
	m_bActive = FALSE;

	/*CDC *pDC = GetDC();

	CFont *pFont = pDC->GetCurrentFont();
	CFont font;
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	strcpy(logFont.lfFaceName , "MS PGothik");
	font.CreateFontIndirect(&logFont);
	this->SetFont(&font);
	font.Detach();
	ReleaseDC(pDC);*/
}

CTabPanelTitleWnd::~CTabPanelTitleWnd()
{
}


BEGIN_MESSAGE_MAP(CTabPanelTitleWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()

ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_CAPTURECHANGED ,OnCaptureChanged)
ON_WM_NCLBUTTONDOWN()
ON_WM_NCMOUSEMOVE()
ON_WM_CREATE()
ON_WM_NCLBUTTONUP()

ON_MESSAGE(WM_IDLEUPDATECMDUI,OnUpdateUI)
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CTabPanelTitleWnd:: Create(CWnd *pWnd)
{

	CRect  szRect;

	LPCTSTR  strRegist =  AfxRegisterWndClass( CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS,
								::LoadCursor(NULL, IDC_ARROW),
								(HBRUSH) ::GetStockObject(HOLLOW_BRUSH),
								::LoadIcon(NULL, IDI_APPLICATION));

	BOOL bret = CWnd::Create(strRegist,"TopTitleBarWnd",WS_CHILD|WS_VISIBLE,szRect,pWnd,IDW_TOP_TITLEBAR_WND);
	
	if(bret)
		SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	return bret;
}

// CTabPanelWnd message handlers
void CTabPanelTitleWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC dcMem;
	CBitmap bmpMem;
	CRect		rcRect;
	CRect    tRect;

	GetClientRect(&rcRect);
	
	dc.FillRect(&rcRect,CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH)));


	rcRect.DeflateRect(1,0,0,0);
	tRect = rcRect;
	tRect.OffsetRect(-rcRect.TopLeft());

	dcMem.CreateCompatibleDC( &dc);
	bmpMem.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());

	dcMem.SaveDC();
	dcMem.SelectObject(&bmpMem);
	 
	//dcMem.FillRect();
	int nX,nCx;
	/*if (!m_bActive)
	{
		DrawStretchBitmap(dcMem,0,0,m_bmpSizeArry[3].cx,tRect.Height(),(HBITMAP)m_bmpArry[3].GetSafeHandle(),0,0,m_bmpSizeArry[3].cx,m_bmpSizeArry[3].cy);
		nX = tRect.right - m_bmpSizeArry[5].cx;
		DrawStretchBitmap(dcMem,nX,0,m_bmpSizeArry[5].cx,tRect.Height(),(HBITMAP)m_bmpArry[5].GetSafeHandle(),0,0,m_bmpSizeArry[5].cx,m_bmpSizeArry[5].cy);

		nX = tRect.left + m_bmpSizeArry[3].cx;
		nCx = tRect.Width()-m_bmpSizeArry[3].cx-m_bmpSizeArry[5].cx;
		DrawTileBitmap(dcMem,nX,0,nCx,tRect.Height(),(HBITMAP)m_bmpArry[4].GetSafeHandle(),0,0,m_bmpSizeArry[4].cx,m_bmpSizeArry[4].cy,true);

	}
	else*/
	{
		DrawStretchBitmap(dcMem,0,0,m_bmpSizeArry[0].cx,tRect.Height(),(HBITMAP)m_bmpArry[0].GetSafeHandle(),0,0,m_bmpSizeArry[0].cx,m_bmpSizeArry[0].cy);
		nX = tRect.right - m_bmpSizeArry[2].cx;
		DrawStretchBitmap(dcMem,nX,0,m_bmpSizeArry[2].cx,tRect.Height(),(HBITMAP)m_bmpArry[2].GetSafeHandle(),0,0,m_bmpSizeArry[2].cx,m_bmpSizeArry[2].cy);

		nX = tRect.left + m_bmpSizeArry[0].cx;
		nCx = tRect.Width()-m_bmpSizeArry[0].cx-m_bmpSizeArry[2].cx;
		DrawTileBitmap(dcMem,nX,0,nCx,tRect.Height(),(HBITMAP)m_bmpArry[1].GetSafeHandle(),0,0,m_bmpSizeArry[1].cx,m_bmpSizeArry[1].cy,true);

	}

	DrawFace(&dcMem);

	CBrush  brush,brush1;
	brush.CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));//RGB(97,131,158)
	brush1.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	m_Handler.bottom = m_Handler.top  +2;
	dcMem.FillRect(&m_Handler,&brush);

	m_Handler.top++;
	m_Handler.bottom = m_Handler.top  +1;
	dcMem.FillRect(&m_Handler,&brush1);
	m_Handler.top--;

	CRgn  rgn;
	rgn.CreateRoundRectRgn(1,1,rcRect.Width()+1,rcRect.Height(),5,5);
	dc.SaveDC();
 
	dc.SelectObject(rgn);
	dc.BitBlt(1,0,rcRect.Width(),rcRect.Height(),&dcMem,0,0,SRCCOPY);
	dc.RestoreDC(-1);
   
	dcMem.RestoreDC(-1);
}

void CTabPanelTitleWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
    CDockPanel *pBar;

	CWnd::OnLButtonUp(nFlags, point);

	if(GetCapture() == this)
		ReleaseCapture();

	m_bDraging = FALSE;
	pBar = (CDockPanel *)GetDockedPanel();
	if(pBar->m_arrPanel.size() <= 1)
		return ;

	CTabPanelWnd *pOwner = (CTabPanelWnd *)GetOwner();
	if(!pOwner->m_bExtend)
		ExtendParent();
	else
		ConstrictParent( );
}

void CTabPanelTitleWnd::ConstrictParent(void)
{ 
    CDockPanel *pBar = (CDockPanel *)GetDockedPanel();
	CTabPanelWnd *power = (CTabPanelWnd *) GetOwner();
//	power->HideChildrenExcludeTitle();
	pBar->SetItemExtend(power,FALSE);
}

void CTabPanelTitleWnd::ExtendParent(void)
{
	CDockPanel *pBar = (CDockPanel *)GetDockedPanel();
	CTabPanelWnd *power = (CTabPanelWnd *) GetOwner();
//	power->ShowChildren();
	pBar->SetItemExtend(power,TRUE);
}

void CTabPanelTitleWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnRButtonDown(nFlags, point);
	//CSizingControlBarEx *pBar = (CSizingControlBarEx *)GetOwner();
	//pBar->ToggleDocking();
}
 
void  CTabPanelTitleWnd::  RecalcLayout()
{
	CRect rect;
    CWnd *pParent;

	pParent = GetOwner();
	pParent->GetClientRect(&rect);
	MoveWindow(0,0,rect.Width(),TAB_TITLE_HTIGHT);

    m_Handler = rect;
	m_Handler.bottom = rect.top + 4;
	Invalidate();
}
 
void CTabPanelTitleWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PostNcDestroy();
	delete this;
}

CWnd * CTabPanelTitleWnd::GetDockedPanel(void)
{ 
	CWnd *pOwner = GetOwner();
	while(pOwner)
	{
		if(pOwner->IsKindOf(RUNTIME_CLASS(CDockPanel) ) )
			break;
		pOwner = pOwner->GetOwner();
	}
	return  pOwner;
}

void CTabPanelTitleWnd::GetTitleBarWndSize(CSize &size)
{
	CRect cltRect;
	GetClientRect(&cltRect);
	size.cx = cltRect.Width();
	size.cy= cltRect.Height();
}

void CTabPanelTitleWnd::DrawFace(CDC * pDC)
{
	CTabPanelWnd *pOwner = (CTabPanelWnd *)GetOwner();
	CRect  cltRect;
	CPoint  point;
	GetClientRect(&cltRect);
	point.x = cltRect.left + 5;
	point.y = cltRect.top + 3;
	if(pOwner->m_bExtend)
		  DrawTrigon( pDC,8,8,RGB(255,255,255),point , TOP_BOTTOM);
	else
		  DrawTrigon( pDC,8,8,RGB(255,255,255),point , TOP_RIGHT);
    
	CString  text;
	GetWindowText(text);
	
	cltRect.left += 15;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(::GetStockObject(DEFAULT_GUI_FONT));
	pDC->DrawText(text,cltRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//cltRect.OffsetRect(2,2);
	//pDC->DrawText(text,cltRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	//cltRect.OffsetRect(-1,-1);
	//pDC->SetTextColor(RGB(143,190,163));
	//pDC->DrawText(text,cltRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//pDC->FillRect(&m_Handler,CBrush::FromHandle(::GetSysColorBrush(2)));
}

#define  HTSPLITER    355
LRESULT  CTabPanelTitleWnd::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
     
	CPoint  mPoint;
	GetCursorPos(&mPoint);
	ScreenToClient(&mPoint);
	//ScreenToClient(&point);
	m_bPtInHandler = FALSE;
	m_Handler.bottom = m_Handler.top  + 3;
	if(m_Handler.PtInRect(mPoint) && CanDrag())
	{
		m_bPtInHandler = TRUE;
		return HTSPLITER;
	}

	return CWnd::OnNcHitTest(point);
}

BOOL CTabPanelTitleWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	BOOL bextend =  ((CTabPanelWnd *) GetOwner())->m_bExtend;

	if( (nHitTest == HTSPLITER || m_bDraging) && bextend )
	{
		 ::SetCursor(::LoadCursor(NULL,IDC_SIZENS));
		 return TRUE;
	}

	m_bDraging = FALSE ;
	///*if(nHitTest == HTCLIENT2)
	{
		
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND_POINT ));
		return TRUE;
	}
	
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTabPanelTitleWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bPtInHandler)
	{ 
		CTabPanelWnd *pBar = (CTabPanelWnd *) GetOwner();
		if(pBar->m_bExtend)
		{
			m_bDraging = TRUE;
			SetCapture();
		}
	} 
	CWnd::OnLButtonDown(nFlags, point);
}

void CTabPanelTitleWnd::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//if(m_bDraging == TRUE)
	//{
	// 
	//	//------------------begin track  here

	//	m_Track.Track();

	//}
	//m_bDraging = FALSE;
	CWnd::OnNcMouseMove(nHitTest, point);
}
void CTabPanelTitleWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_bDraging == TRUE)
		m_Track.Track();
		 

    m_bDraging = FALSE;
	CWnd::OnMouseMove(nFlags, point);
}


HRESULT CTabPanelTitleWnd::OnCaptureChanged(WPARAM wparam,LPARAM lparam)
{
	CWnd  *pwnd = (CWnd *)lparam;
	if(pwnd == this)
	{	
		CTabPanelWnd *pPanelWnd = (CTabPanelWnd *)GetOwner();
		if(pPanelWnd->m_bExtend)
			m_bDraging = TRUE;
	}
	else
	{
		if(m_bDraging)
		{
			m_bDraging = FALSE;
			((CDockPanel *)GetDockedPanel())->RecalcLayout();
		}
			
	}
	return S_OK;
}


void CTabPanelTitleWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CTabPanelWnd *pBar = (CTabPanelWnd *) GetOwner();
	if(pBar->m_bExtend)
	{
		m_bDraging = TRUE;
		if(GetCapture()!= this)
		SetCapture();
	}
	CWnd::OnNcLButtonDown(nHitTest, point);
}



int CTabPanelTitleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_pTrack = new CTabTrack(this);
	m_bDraging = FALSE;
	m_bPtInHandler = FALSE;
    m_Track.m_pParent = this;

	//load bmp......
	InitTitleResource();
	return 0;
}

void CTabPanelTitleWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
    m_bDraging = FALSE;
	if(GetCapture() == this)
		ReleaseCapture();
	CWnd::OnNcLButtonUp(nHitTest, point);
}

BOOL CTabPanelTitleWnd::ImFirstOne(void)
{
	CDockPanel *pBar = (CDockPanel *)GetDockedPanel();
	if(pBar->m_arrPanel.size() <= 1)
		return TRUE;
	if(pBar->m_arrPanel[0] == (CTabPanelWnd *)GetOwner())
		return TRUE;
	return FALSE;
}

BOOL CTabPanelTitleWnd::CanDrag(void)
{
	BOOL bRet ;
	bRet = ImFirstOne();
	if(bRet)
		return FALSE;
	bRet = PreItemExtend();
	if(bRet)
		return TRUE;
	return FALSE;
}

BOOL CTabPanelTitleWnd::PreItemExtend(void)
{
	CDockPanel *pBar = (CDockPanel *)GetDockedPanel();
	if(pBar->m_arrPanel.size() <= 1)
		return FALSE;

	for(int i = 0;i < pBar->m_arrPanel.size();i++)
	{
		
	  if(pBar->m_arrPanel[i] == (CTabPanelWnd *)GetOwner())
		  break;
	  if(pBar->m_arrPanel[i]->m_bExtend)
		  return TRUE ;
	}
	return FALSE;
}


void CTabPanelTitleWnd::InitTitleResource(void)
{
	BITMAP bmp;
	for(int i = 0; i< 6; i++)
	{
		m_bmpArry[i].LoadBitmap(IDB_PANEL_BAR_5-i);
		m_bmpArry[i].GetBitmap(&bmp);
		m_bmpSizeArry[i].cx = bmp.bmWidth;
		m_bmpSizeArry[i].cy = bmp.bmHeight;
	}
}

HRESULT CTabPanelTitleWnd::OnUpdateUI(WPARAM wparam, LPARAM lparam)
{
	CPoint  point;
	CRect  cltRect;
	GetCursorPos(&point);
	ScreenToClient(&point);
	GetClientRect(&cltRect);
	
	BOOL bbk;
	if(PtInRect(&cltRect,point))
	{
		bbk = m_bActive;
		m_bActive = TRUE;
		if(!bbk)
			Invalidate();
	}
	else
	{
		bbk = m_bActive;
		m_bActive = FALSE;
		if(bbk)
			Invalidate();
	}
	return E_NOTIMPL;
}

BOOL CTabPanelTitleWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	/*CBrush brush;
	CRect  cltRect;
	brush.CreateSolidBrush(RGB(255,255,255));
	GetClientRect(&cltRect);
	pDC->FillRect(&cltRect,&brush);*/
	return CWnd::OnEraseBkgnd(pDC);
}
