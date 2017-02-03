// AppDockablePane.cpp : implementation file
//

#include "stdafx.h"
 
#include "AppDockablePane.h"
#include "AfxTabCtrl.h"

// CAppDockablePane

IMPLEMENT_DYNAMIC(CAppDockablePane, CDockablePane)

CAppDockablePane::CAppDockablePane()
{

}

CAppDockablePane::~CAppDockablePane()
{
}
void  CAppDockablePane::GetTitleRect(CRect *pRect)
{
	ASSERT(pRect != NULL);
	pRect->SetRectEmpty();
}
void   CAppDockablePane:: GetClientRectExcludeTitle(CRect *pRect)
{
	ASSERT(pRect != NULL);

	GetClientRect(pRect);
}
void  CAppDockablePane::GetClientRectExcludeBorder(CRect *pRect)
{
	ASSERT(pRect != NULL);
	
	GetClientRect(pRect);
	
	//pRect->DeflateRect(1,1,1,1);
}

BOOL  CAppDockablePane::IsHorzDocked()
{
	return CDockablePane::IsHorizontal();
}
BEGIN_MESSAGE_MAP(CAppDockablePane, CDockablePane)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, &CAppDockablePane::OnNeedTipText)
END_MESSAGE_MAP()



// CAppDockablePane message handlers

BOOL CAppDockablePane::Create(CWnd *pParent,LPCTSTR  lpCaption,  CSize &size,UINT dwStyle,UINT nID)
{
	return CDockablePane::Create(lpCaption,pParent,CRect(-1,-1,size.cx,size.cy),TRUE,nID,dwStyle| CBRS_FLOAT_MULTI);
}

void CAppDockablePane::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDockablePane::PostNcDestroy();
	//TRACE("Pane Create Failure!!\n");
}

void CAppDockablePane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    PaintClient(dc);
	
}

BOOL CAppDockablePane::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	CRect cliRect;
	GetClientRect(&cliRect);
	pDC->FillRect(cliRect,&afxGlobalData.brHilite);

	PaintClient(*pDC);
	return  TRUE;//CDockablePane::OnEraseBkgnd(pDC);
}

BOOL CAppDockablePane:: IsTabDocked()
{
	return IsTabbed();
}

void CAppDockablePane::SetCaptionButtons()
{
	__super::SetCaptionButtons();
}

BOOL CAppDockablePane::OnNeedTipText(UINT /*id*/, NMHDR* pNMH, LRESULT* /*pResult*/)
{
	static CString strTipText;

	ENSURE(pNMH != NULL);

	if (m_pToolTip->GetSafeHwnd() == NULL || pNMH->hwndFrom != m_pToolTip->GetSafeHwnd())
	{
		return FALSE;
	}

	LPNMTTDISPINFO pTTDispInfo = (LPNMTTDISPINFO) pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	UINT nTooltipResID = 0;

	switch (pNMH->idFrom)
	{
	case 1:
		nTooltipResID = IDS_AFXBARRES_CLOSEBAR;
		break;

	case 2:
		{
			SHORT state = GetAsyncKeyState(VK_CONTROL);
			nTooltipResID = IDS_AFXBARRES_AUTOHIDEBAR;

			if ((state & 0x8000) && IsAutohideAllEnabled())
			{
				nTooltipResID = IDS_AFXBARRES_AUTOHIDE_ALL;
			}
		}
		break;

	case 3:
		nTooltipResID = IDS_AFXBARRES_MENU;
		break;
	}

	if (nTooltipResID == 0)
	{
		return FALSE;
	}

	//ENSURE(strTipText.LoadString(nTooltipResID));
	 
	pTTDispInfo->lpszText = const_cast<LPTSTR>((LPCTSTR) strTipText);
	return TRUE;
}
