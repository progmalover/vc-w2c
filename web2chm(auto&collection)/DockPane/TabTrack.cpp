// TabTrack.cpp : implementation file
/////////////////////////////////////////////////////////////
//
//zxy  .2007.12
//
/////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "PanelCtrlBarDemo.h"
#include "TabTrack.h"
#include "SilibingPanelWnd.h"
#include "TopTitleBarWnd.h""
#include ".\tabtrack.h"
#include "DockPanel.h"

//CWnd * CTabTrack::pLastTrackWnd;
CSize CTabTrack::oldBuddySize;
CTabTrack::CTabTrack()
{
}

CTabTrack:: CTabTrack(CTabPanelTitleWnd *pParent = NULL)
{
	ASSERT(pParent);
	ASSERT(::IsWindow(pParent->m_hWnd));
	m_pParent = pParent;
}
CTabTrack::~CTabTrack()
{
}
 
void CTabTrack:: Track()
{
	CDockPanel  *pPanel;
	CPoint ptStart,ptEnd;

	CDC *pDC;
	CBrush* pBrush;
	CBrush* pBrushOld;
	CTabPanelWnd *pWnd;
    BOOL  bTrackStart = FALSE;
	CRect  rctTitle;

	GetCursorPos(&ptStart); 
	pPanel = (CDockPanel *)m_pParent->GetDockedPanel(); 
	pPanel->ScreenToClient(&ptStart);

	
   pWnd = (CTabPanelWnd *) m_pParent->GetParent();
   CTabPanelWnd *pBuddy = pWnd->GetLastBuddy();
  // pLastTrackWnd = (CWnd *)pWnd;
   oldBuddySize = pBuddy->m_Size;
   /**/

	while (true)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != m_pParent)
			break;

		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			{
				GetCursorPos(&ptEnd);
				pPanel->ScreenToClient(&ptEnd);
		
				int dy = ptEnd.y - ptStart.y;

				m_pParent->GetWindowRect(rctTitle);
				pPanel->ScreenToClient(&rctTitle);

				CRect wndRect;
				pWnd->GetClientRect(wndRect);

				if(dy > 0 &&  ptEnd.y <= rctTitle.top) //向下拖动
					continue;
				if(dy <0 && ptEnd.y >= rctTitle.top)
					continue;
				/*if(wndRect.Height() <= pWnd->m_defaultMiniSize.cy +10)
				{
					pWnd->m_Size.cy = pWnd->m_defaultMiniSize.cy +11;
					pPanel->RecalcLayout();
					continue;
				}*/
				/*这里根据光标的位置改变位移大小*/

				if(pPanel->m_bOrder == FALSE) //正序排列
				{
					pWnd->m_Size.cy -= (ptEnd.y - rctTitle.top);

					if(pWnd->m_Size.cy <= pWnd->m_defaultMiniSize.cy) // 限定最小默认尺寸。。。。
						pWnd->m_Size.cy = pWnd->m_defaultMiniSize.cy;
				}else //倒序排列
				{
					pBuddy->m_Size.cy += (ptEnd.y - rctTitle.top);

					if(pBuddy->m_Size.cy <= pBuddy->m_defaultMiniSize.cy) // 限定最小默认尺寸。。。。
						pBuddy->m_Size.cy = pBuddy->m_defaultMiniSize.cy;

					if(wndRect.Height() <= TAB_TITLE_HTIGHT)
					{
						//pWnd->m_Size.cy = 200;
						//pWnd->m_ConstrictSize = pWnd->m_defaultMiniSize.cy ;
						pBuddy->m_Size = oldBuddySize;//pBuddy->m_defaultMiniSize.cy;
						pWnd->m_bExtend = FALSE;
					}
					else
						pWnd->m_bExtend = TRUE;
				}

				pPanel->RecalcLayout();

				pWnd->RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
				ptStart= ptEnd;
			}
			break;

		case WM_LBUTTONUP:
			goto _exit;


		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;

		case WM_RBUTTONDOWN:
			//		CRect::CopyRect(rcSave);
			goto _exit;


		default:
			DispatchMessage(&msg);
			break;
		}
	}

_exit:
	GetCursorPos(&ptEnd);
	pPanel->ScreenToClient(&ptEnd);
	ReleaseCapture();
}

// CTabTrack message handlers


void CTabTrack::DrawTracker(CDC* pDC, CPoint csPoint)
{
	CRect rcTracker;
	m_pParent->GetDockedPanel()->GetClientRect(rcTracker);
	rcTracker.top = csPoint.y;
	rcTracker.bottom = csPoint.y  + 4;
	pDC->PatBlt(rcTracker.left, rcTracker.top, rcTracker.Width(), rcTracker.Height(), PATINVERT);
}
