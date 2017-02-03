// CMYControlBar.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CMYControlBar.h"
#include <afxpriv.h>    // for CDockContext
#include "resource.h"
#include ".\cmycontrolbar.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyControlBar

#define IDC_BUTTON_HIDE 1000
#define IDC_BUTTON_MINI 1001

CMyControlBar::CMyControlBar()
{
    m_sizeMin		= CSize(32, 32);
    m_sizeHorz		= CSize(200, 200);
    m_sizeVert		= CSize(200, 200);
    m_sizeFloat		= CSize(200, 200);
    m_bTracking		= FALSE;
    m_bInRecalcNC	= FALSE;
    m_cxEdge		= 5;
	m_menuID		= 0;
	m_bGripper		= TRUE;
	m_bButtons		= TRUE;
	m_barFocus		= FALSE;
}
void CMyControlBar::OnSize(UINT nType,int cx,int cy)
{
   //Invalidate(); 
}
CMyControlBar::~CMyControlBar()
{
}

IMPLEMENT_DYNAMIC(CMyControlBar, CControlBar)

BEGIN_MESSAGE_MAP(CMyControlBar, CControlBar)
	//{{AFX_MSG_MAP(CMyControlBar)	//ON_WM_NCPAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_WINDOWPOSCHANGED()

	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
  //  ON_WM_PAINT();
	ON_WM_CREATE()
	ON_COMMAND(IDC_BUTTON_HIDE, OnButtonClose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HIDE, OnUpdateButtonClose)
	ON_COMMAND(IDC_BUTTON_MINI, OnButtonMinimize)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MINI, OnUpdateButtonMinimize)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_WM_NCLBUTTONUP()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyControlBar message handlers

void CMyControlBar::OnUpdateCmdUI(CFrameWnd * pTarget, BOOL bDisableIfNoHndler)
{
    UpdateDialogControls(pTarget, bDisableIfNoHndler);
    CWnd *pWnd = GetFocus();
	if(!pWnd)
		return;
	if(pWnd->m_hWnd == m_hWnd || ::IsChild(m_hWnd,pWnd->m_hWnd))
	{
		if(m_barFocus == FALSE)
		{
			m_barFocus = TRUE; 
			CRect  rct = GetGripperRect();
			m_btnClose.SetIcon(m_ImageList->ExtractIcon(1));
			InvalidateRect(&rct);
			m_btnClose.RedrawWindow();
		}
	}else
	{
		if(m_barFocus)
		{
			m_barFocus = FALSE;
			CRect  rct = GetGripperRect();
			m_btnClose.SetIcon(m_ImageList->ExtractIcon(0));
			InvalidateRect(&rct);
			m_btnClose.RedrawWindow();
		}
		
	}
}

BOOL CMyControlBar::Create(CWnd * pParentWnd, UINT nID, LPCTSTR lpszWindowName, CSize sizeDefault, DWORD dwStyle)
{
    ASSERT_VALID(pParentWnd);   // must have a parent
    ASSERT (((dwStyle & CBRS_SIZE_FIXED)   != CBRS_SIZE_FIXED) && 
	    	((dwStyle & CBRS_SIZE_DYNAMIC) != CBRS_SIZE_DYNAMIC));

    // save the style
    SetBarStyle(dwStyle & CBRS_ALL);
 
	//获得自己的窗口类，其实不必要CS_DBLCLKS
    CString wndclass = ::AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
       ::GetSysColorBrush(COLOR_BTNFACE), 0);

    m_sizeHorz = m_sizeVert = m_sizeFloat = sizeDefault;

    dwStyle &= ~CBRS_ALL;
    dwStyle &= WS_VISIBLE | WS_CHILD;
    
	return CWnd::Create(wndclass, lpszWindowName, dwStyle|WS_CLIPCHILDREN,
		CRect(0,0,0,0), pParentWnd, nID);
}

CSize CMyControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    CRect rc;
	
    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rc);
    int nHorzDockBarWidth = bStretch ? 32767 : rc.Width() + 4;
    m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rc);
    int nVertDockBarHeight = bStretch ? 32767 : rc.Height() + 4;

    if (bHorz)
        return CSize(nHorzDockBarWidth, m_sizeHorz.cy);
    else
        return CSize(m_sizeVert.cx, nVertDockBarHeight);
}

CSize CMyControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{

	//extern 	CMainFrame  *pMainFrame;
	//pMainFrame->RecalcLayout();

    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK))
    {
        if (nLength == -1)
            GetDockingFrame()->DelayRecalcLayout();
        return CControlBar::CalcDynamicLayout(nLength,dwMode);
    }

    if (dwMode & LM_MRUWIDTH)
        return m_sizeFloat;

    if (dwMode & LM_COMMIT)
    {
        m_sizeFloat.cx = nLength;
        return m_sizeFloat;
    }

    if (dwMode & LM_LENGTHY)
        return CSize(m_sizeFloat.cx,
            m_sizeFloat.cy = max(m_sizeMin.cy, nLength));
    else
        return CSize(max(m_sizeMin.cx, nLength), m_sizeFloat.cy);
}

void CMyControlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{


	CControlBar::OnWindowPosChanged(lpwndpos);
	
	m_nDockBarID = GetParent()->GetDlgCtrlID();

    if (m_bInRecalcNC == FALSE)
	{
        m_bInRecalcNC = TRUE;

        // Force recalc the non-client area
        SetWindowPos(NULL, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE |
            SWP_NOACTIVATE | SWP_NOZORDER |
            SWP_FRAMECHANGED);

        m_bInRecalcNC = FALSE;

		RecaclMainFrame();
		 
	}
	if (m_bButtons)
	{
		ASSERT(m_ImageList);

		if (IsFloating()) {
			m_btnClose.ShowWindow(SW_HIDE);
			return;
		}
		else {
			m_btnClose.ShowWindow(SW_SHOW);

		}

		CRect rcClose(GetButtonRect());

		if (IsHorzDocked()) {
			//rcClose.OffsetRect(0,14);
			//rcMinim.OffsetRect(0,14);
			//m_btnMinim.SetIcon(m_ImageList->ExtractIcon(2),CSize(13,13));
			//m_btnMinim.SetIcon(m_ImageList->ExtractIcon(2));
		}
		else {
			rcClose.OffsetRect(14,0);
			//m_btnMinim.SetIcon(m_ImageList->ExtractIcon(1),CSize(13,13));
			//m_btnMinim.SetIcon(m_ImageList->ExtractIcon(1));
		}

		m_btnClose.MoveWindow(rcClose);

	}
 
	
}

//#define MFCXLIB_STATIC

BOOL CMyControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//return 0;
    if ((nHitTest != HTSIZE) || m_bTracking)
        return CControlBar::OnSetCursor(pWnd, nHitTest, message);

#if defined(MFCXLIB_STATIC) 
	HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(AFX_IDC_VSPLITBAR), RT_GROUP_CURSOR);

    if (IsHorzDocked())
        ::SetCursor(::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_VSPLITBAR)));
    else
        ::SetCursor(::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR)));
#else
    if (IsHorzDocked())
        ::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
    else
        ::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
#endif
    return TRUE;
}

BOOL CMyControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

BOOL CMyControlBar::IsVertDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

void CMyControlBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	//return ;
    // Compute the rectangle of the mobile edge
    GetWindowRect(m_rectBorder);
    m_rectBorder.OffsetRect(-m_rectBorder.left, -m_rectBorder.top);
    m_rectBorder.DeflateRect(1,1);
    
    CRect rcWnd = lpncsp->rgrc[0];
  //  DWORD dwBorderStyle = m_dwStyle | CBRS_BORDER_ANY;

    switch (m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
		{
			//dwBorderStyle &= ~CBRS_BORDER_BOTTOM;
			rcWnd.DeflateRect(2, 2, 2, m_cxEdge + 2);
			m_rectBorder.top = m_rectBorder.bottom - m_cxEdge;
			break;
		}
		
    case AFX_IDW_DOCKBAR_BOTTOM:
		{
			//dwBorderStyle &= ~CBRS_BORDER_TOP;
			rcWnd.DeflateRect(2, m_cxEdge + 2, 2, 2);
			m_rectBorder.bottom = m_rectBorder.top + m_cxEdge;
			lpncsp->rgrc[0].left += 10;
			break;
		}
		 
    case AFX_IDW_DOCKBAR_LEFT:
		{
			//dwBorderStyle &= ~CBRS_BORDER_RIGHT;
			rcWnd.DeflateRect(2, 2, m_cxEdge + 2, 6);
			m_rectBorder.left = m_rectBorder.right - m_cxEdge;
			break;
		}

    case AFX_IDW_DOCKBAR_RIGHT:
		{
			//dwBorderStyle &= ~CBRS_BORDER_LEFT;
			rcWnd.DeflateRect(m_cxEdge + 2, 2, 2, 6);
			m_rectBorder.right = m_rectBorder.left + m_cxEdge;
			break;
		}
    default:
		{
			m_rectBorder.SetRectEmpty();
			break;
		}
    }

    lpncsp->rgrc[0] = rcWnd;

	RecaclMainFrame();
   // SetBarStyle(dwBorderStyle);
}

void CMyControlBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawGripper(&dc);
}

void CMyControlBar::OnNcPaint() 
{

	//return;
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);
    //return;
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);

	dc.ExcludeClipRect(rectClient);
	
    // draw borders in non-client area
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

 //   // erase parts not drawn
	dc.IntersectClipRect(rectWindow);
	//
	//   // erase NC background the hard way CPen    cPen;::GetSysColor(COLOR_BTNFACE)
    

	dc.FillSolidRect(rectWindow, ::GetSysColor(COLOR_3DFACE));	

	CPen cPen;

	cPen.CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
	dc.SelectObject(cPen);
	
	if(m_nDockBarID == AFX_IDW_DOCKBAR_LEFT)
		dc.DrawEdge(m_rectBorder,BDR_RAISEDINNER|BDR_SUNKENOUTER,BF_BOTTOMLEFT);
	if(m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
		dc.DrawEdge(m_rectBorder,BDR_RAISEDINNER|BDR_SUNKENOUTER,BF_BOTTOMRIGHT);
  
	
    ReleaseDC(&dc);
}

LRESULT CMyControlBar::OnNcHitTest(CPoint point) 
{
//	return 0;
    if (IsFloating())
        return CControlBar::OnNcHitTest(point);

    CRect rc;
    GetWindowRect(rc);
    point.Offset(-rc.left, -rc.top);
  
    if (m_rectBorder.PtInRect(point))
        return HTSIZE;
    else
        return HTCLIENT;
}

BOOL CMyControlBar::IsFloating()
{
	return (!IsHorzDocked() && !IsVertDocked());
}

void CMyControlBar::StartTracking()
{
    SetCapture();//捕捉鼠标开始

    // make sure no updates are pending
 //  RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
    m_pDockSite->LockWindowUpdate();

    m_ptOld = m_rectBorder.CenterPoint();//保存当前的中心点
    m_bTracking = TRUE; //设置追踪标志
    
    m_rectTracker = m_rectBorder;//设置RECT为窗体的Border RECT
    if (!IsHorzDocked()) m_rectTracker.bottom -= 4;

    OnInvertTracker(m_rectTracker);//m_rectTracker 为拖动器的矩形大小
}

void CMyControlBar::StopTracking(BOOL bAccept)
{


    OnInvertTracker(m_rectTracker);
    m_pDockSite->UnlockWindowUpdate();
    m_bTracking = FALSE;
    ReleaseCapture();
	
    if (!bAccept)
		return;

    int maxsize, minsize, newsize;
    CRect rcc;
    m_pDockSite->GetClientRect(rcc);

    newsize = IsHorzDocked() ? m_sizeHorz.cy : m_sizeVert.cx;
    maxsize = newsize + (IsHorzDocked() ? rcc.Height() : rcc.Width());
    minsize = IsHorzDocked() ? m_sizeMin.cy : m_sizeMin.cx;

    CPoint point = m_rectTracker.CenterPoint();
    switch (m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
		{
			newsize += point.y - m_ptOld.y;
			break;
		}
    case AFX_IDW_DOCKBAR_BOTTOM:
		{
			newsize += -point.y + m_ptOld.y;
			break;
		}
    case AFX_IDW_DOCKBAR_LEFT:
		{
			newsize += point.x - m_ptOld.x;
			break;
		}
    case AFX_IDW_DOCKBAR_RIGHT:
		{
			newsize += -point.x + m_ptOld.x;
			break;
		}
    }

    newsize = max(minsize, min(maxsize, newsize));

    if (IsHorzDocked())
        m_sizeHorz.cy = newsize;
    else
        m_sizeVert.cx = newsize;

  ///  m_pDockSite->DelayRecalcLayout();

	RecaclMainFrame();
}
void CMyControlBar::OnInvertTracker(const CRect &rect)
{
    ASSERT_VALID(this);
    ASSERT(!rect.IsRectEmpty());
    ASSERT(m_bTracking);

    CRect rct = rect, rcc, rcf;
    GetWindowRect(rcc);//	得到自身的窗体RECT
    m_pDockSite->GetWindowRect(rcf);//得到DOCK容器窗体的RECT

    rct.OffsetRect(rcc.left - rcf.left, rcc.top - rcf.top);//	
    rct.DeflateRect(1, 1);

	/*
		m_pDockSite 为BAR此时的着陆点，当前若是DOCK在主框架内，指代主框架，否则
		指代的是 CMiniFrameWnd 
	*/
    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);

    CBrush* pBrush = CDC::GetHalftoneBrush();
    HBRUSH hOldBrush = NULL;
    if (pBrush != NULL)
        hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

    pDC->PatBlt(rct.left, rct.top, rct.Width(), rct.Height(), PATINVERT);

    if (hOldBrush != NULL)
        SelectObject(pDC->m_hDC, hOldBrush);

    m_pDockSite->ReleaseDC(pDC);
}

void CMyControlBar::OnCaptureChanged(CWnd *pWnd) 
{
    if (m_bTracking && pWnd != this)
        StopTracking(FALSE); // cancel tracking
	
	CControlBar::OnCaptureChanged(pWnd);
}

void CMyControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{

    if (m_bTracking)
		return;
    
    if ((nHitTest == HTSIZE) && !IsFloating())
        StartTracking();
    else    
		CControlBar::OnNcLButtonDown(nHitTest, point);
}

void CMyControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{


	//return;
    if (IsFloating() || !m_bTracking) {//是浮动或非TRACK状态不做处理
        CControlBar::OnMouseMove(nFlags, point);
        return;
    }

    CPoint cpt = m_rectTracker.CenterPoint();
    ClientToWnd(point);

    if (IsHorzDocked()) {//dock在底部或上部
        if (cpt.y != point.y) {
            OnInvertTracker(m_rectTracker);//擦掉以前的（xor）


            m_rectTracker.OffsetRect(0, point.y - cpt.y);//移动边框的位置
            OnInvertTracker(m_rectTracker);//画新的位置
			
        }
    }

    else {
        if (cpt.x != point.x) {
            OnInvertTracker(m_rectTracker);
            m_rectTracker.OffsetRect(point.x - cpt.x, 0);
            OnInvertTracker(m_rectTracker);
        }
	}
}

void CMyControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//set caption high light
	

    //set start darg
    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
        m_pDockContext->StartDrag(point);
    }
    else
		CControlBar::OnLButtonDown(nFlags, point);

}

void CMyControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (!m_bTracking)
        CControlBar::OnLButtonUp(nFlags, point);
    else
    {
        ClientToWnd(point);
        StopTracking(TRUE);
    }
}

void CMyControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
	
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
		CControlBar::OnLButtonDblClk(nFlags, point);
}

CPoint& CMyControlBar::ClientToWnd(CPoint &point)
{
    point.Offset(2, 2);

    if (m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
        point.y += m_cxEdge;
    else if (m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
        point.x += m_cxEdge;

    return point;
}

CRect CMyControlBar::GetButtonRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	if(IsHorzDocked()) {
		pRect.top	 += 3;
		pRect.bottom = pRect.top+12;
		pRect.left  += 3;
		pRect.right  = pRect.left+12;
	}
	else
	{
		pRect.right -= 16;
		pRect.left   = pRect.right-12;
		pRect.top   += 2;
		pRect.bottom = pRect.top+12;
	}
	return pRect;
}

CRect CMyControlBar::GetGripperRect()
{

	//CControlBar::m_g
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	if(IsHorzDocked()) {
		pRect.DeflateRect(3,3);
		pRect.left		+= 1;
		pRect.right		 = pRect.left+3;
		pRect.bottom	-= 1;
		pRect.top		+= m_bButtons?30:1;
	}
	else {
		pRect.DeflateRect(4,4);
		pRect.top		+= 2;
		pRect.bottom	 = pRect.top+3;
		pRect.right		-= m_bButtons?30:2;
	}

	return pRect;
}

void CMyControlBar::DrawGripper(CDC* pDC)
{

//	CControlBar::DrawGripper(pDC,); 
	if (IsFloating())
		return;

	if (m_bGripper)
	{
		// draw the gripper.
		CRect pRect(GetGripperRect());
		CRect Rc;
		GetClientRect(&Rc);
		m_dwStyle |= CBRS_GRIPPER; 
		pDC->FillRect(&Rc,CBrush::FromHandle(GetSysColorBrush(COLOR_BTNFACE))); 
		CControlBar::DrawGripper(pDC,pRect); 
		if(IsHorzDocked())
			pRect.OffsetRect(3,0);
		else
			pRect.OffsetRect(0,3);
		CControlBar::DrawGripper(pDC,pRect);
	
		if(IsHorzDocked())
			pRect.OffsetRect(-6,0);
		else
			pRect.OffsetRect(0,-6);
		CControlBar::DrawGripper(pDC,pRect);
	}
    
	//m_ImageList->Draw(pDC,0,CPoint(0,0),ILD_TRANSPARENT);
	//m_pDockSite->RecalcLayout();
}

int CMyControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_bButtons)
	{
		ASSERT(m_ImageList);

		if(!m_btnClose.Create(NULL, WS_CHILD | WS_VISIBLE |BS_OWNERDRAW | WS_TABSTOP |
			BS_NOTIFY | BS_ICON,
			CRect(0,0,0,0), this, IDC_BUTTON_HIDE ))
		{
			TRACE0("Unable to create CMyControlBar close button\n");
			return -1;
		}
#ifdef  _BTN_MINIM
		if(!m_btnMinim.Create(NULL, WS_CHILD | WS_VISIBLE |BS_OWNERDRAW| WS_TABSTOP |
			BS_NOTIFY|BS_ICON,
			CRect(0,0,0,0), this, IDC_BUTTON_MINI ))
		{
			TRACE0("Unable to create CMyControlBar close button\n");
			return -1;
		}
#endif
		//int cx,cy;
		//::ImageList_GetIconSize(*m_ImageList, &cx, &cy);

	//	m_btnClose.SetIcon(m_ImageList->ExtractIcon(0));
//		m_btnClose.DisableFlatLook();
//		m_btnMinim.DisableFlatLook();
		
	//	m_btnClose.SetIcon(m_ImageList->ExtractIcon(0),CSize(13,13));

		
		::SetWindowPos(m_hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOREDRAW);
		m_ToolTip.Create(this);
	
		m_ToolTip.Activate(TRUE);
		//m_ToolTip.AddTool(this,_T("This is a Control Bar Demo!!")); 
		m_ToolTip.AddTool (&m_btnClose, _T("Hide Bar"));
#ifdef  _BTN_MINIM
		m_ToolTip.AddTool (&m_btnMinim, _T("Minimize Docked Window"));
#endif
	}
	
	return 0;
}

CImageList* CMyControlBar::SetBtnImageList(CImageList *pImageList)
{
	CImageList* pPrevList = m_ImageList;
	m_ImageList = pImageList;
	return pPrevList;
}

void CMyControlBar::OnButtonClose()
{
	GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
}

void CMyControlBar::OnUpdateButtonClose(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(true);
}

void CMyControlBar::OnButtonMinimize()
{
	m_pDockContext->ToggleDocking();
}

void CMyControlBar::OnUpdateButtonMinimize(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(true);
}

BOOL CMyControlBar::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bButtons)
		m_ToolTip.RelayEvent(pMsg);

	if(m_barFocus == FALSE
		&&(pMsg->message == WM_LBUTTONDOWN))
	{
		m_barFocus = TRUE;
		CRect rct = GetGripperRect();
		//
		SetFocus();
		m_btnClose.SetIcon(m_ImageList->ExtractIcon(1));
		InvalidateRect( &rct);
		m_btnClose.RedrawWindow();
	}

	return CControlBar::PreTranslateMessage(pMsg);
}

void CMyControlBar::SetMenuID(UINT nID)
{
	m_menuID = nID;
}

UINT CMyControlBar::GetMenuID()
{
	return m_menuID;
}

void CMyControlBar::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
	// if no menu, just return.
//	m_menuID = IDR_DEMO;
	if (m_menuID == 0 ) {
		TRACE0("No control bar menu defined.\n");
		return;
	}

	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(m_menuID));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

void CMyControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CControlBar::OnNcLButtonUp(nHitTest, point);
}
CWnd *  CMyControlBar:: GetMainFrame(void)
{
		CWnd *tpwnd,*ppwnd;
		tpwnd = ppwnd = GetParent();
		while(tpwnd = ppwnd->GetParent())/*找到头父母窗体*/
		ppwnd = tpwnd;
		return  ppwnd;
}
void CMyControlBar::RecaclMainFrame(void)
{

	CWnd *ppwnd = GetMainFrame();
    ASSERT(ppwnd != NULL);

	if(ppwnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		CFrameWnd * pFrm =(CFrameWnd*) ppwnd;
		pFrm->RecalcLayout();
	}
}

//LRESULT CMyControlBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if( (message == WM_LBUTTONDOWN ||
//		message == WM_MBUTTONDBLCLK||
//		message == WM_MBUTTONDOWN||
//		message == WM_NCMBUTTONDOWN ) &&(m_bButtons == FALSE))
//	{
//		m_bButtons = TRUE;
//		CRect rct = GetGripperRect();
//		InvalidateRect(&rct);
//	}
//	return CControlBar::WindowProc(message, wParam, lParam);
//}


//显隐浮动窗口
CWnd * CMyControlBar::ShowFloatFrame(BOOL enable)
{
	CWnd *pWnd = this;;

	pWnd->ShowWindow(enable);
	pWnd = GetParent();

	while(!pWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
	{

		pWnd->ShowWindow(enable);
		pWnd = pWnd->GetParent();

	}
	pWnd->ShowWindow(enable);
	return pWnd;
}


void  CMyControlBar::ShowBar( BOOL bEnable)
{
	if(bEnable)
	{
		if(IsFloating())
			m_pDockContext->ToggleDocking();

		ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		if(IsFloating())
			ShowFloatFrame(SW_HIDE);//->ShowWindow(FALSE);
		else
			ShowWindow(FALSE);
	}
}
void CMyControlBar::OnKillFocus(CWnd* pNewWnd)
{
	CControlBar::OnKillFocus(pNewWnd);
	CRect rct = GetGripperRect();
	InvalidateRect(&rct);
	// TODO: 在此处添加消息处理程序代码
}



BOOL CMyControlBar::GetFocusStatus(void)
{
	return m_barFocus;
}



void CMyControlBar::SetBarSize(int cx , int cy)
{
	if(!IsHorzDocked())
	{
		m_sizeHorz.cx = cx;
		m_sizeHorz.cy = cy;
	}else
	{
		m_sizeVert.cx = cx;
		m_sizeVert.cy = cy;
	}
	RecaclMainFrame();
}
