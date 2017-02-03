#include "StdAfx.h"
#include ".\anchorlistwnd.h"
#include "ExListBox.h"
#include "AnchorUrlsList.h"
#include "resource.h"
#include "AnchorTree.h"
#include ".\tanchorpiece.h"
#include ".\tanchorpage.h"
#include  <algorithm>
#define ID_DELETE_BTN  WM_USER + 23
#define ID_DELETEALL_BTN  WM_USER + 24
#define ID_SELECTALL_BTN  WM_USER + 25
#define ID_UNSELECT_BTN  WM_USER + 26


#define IDM_SELECTALL  WM_USER + 1
#define IDM_DELALL     WM_USER + 2
#define IDM_DELSELECTION  WM_USER + 3
CAnchorListWnd::CAnchorListWnd(CWnd *pParent)
{
	__super::Create(pParent);
}

CAnchorListWnd::~CAnchorListWnd(void)
{ 
}
BEGIN_MESSAGE_MAP(CAnchorListWnd, CTabPanelWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
	ON_WM_PAINT()
	ON_COMMAND(IDM_SELECTALL,OnSelectAllClick)
	ON_COMMAND(IDM_DELALL,OnDelectAllClick)
	ON_COMMAND(IDM_DELSELECTION,OnDelectSelectionClick)
	
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

int CAnchorListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabPanelWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ToolTip.Create(this);
	// TODO:  Add your specialized creation code here
	GO_AnchorUrlsList.Create(this);

	return 0;
}

#define BTN_WIDTH  70
#define BTN_HEIGHT 0
void CAnchorListWnd::OnSize(UINT nType, int cx, int cy)
{
	CTabPanelWnd::OnSize(nType, cx, cy);

	if(cy <= BTN_HEIGHT + 20)
		return;

	if(::IsWindow(GO_AnchorUrlsList.m_hWnd))
	{
		CRect  cltRect;
		CRect  lstRect;
		this->GetClientRectExcludeTitle(cltRect);
		cltRect.bottom -= BTN_HEIGHT;
		lstRect = cltRect;
		lstRect.bottom -= 1;
		GO_AnchorUrlsList.MoveWindow(lstRect);

		SCROLLBARINFO scBarInf;
		scBarInf.cbSize = sizeof(SCROLLBARINFO);
		
		 GO_AnchorUrlsList.GetScrollBarInfo(OBJID_VSCROLL,&scBarInf);
		
	
		CRect scRect;
		if(scBarInf.rgstate[0] != STATE_SYSTEM_INVISIBLE)
			scRect = scBarInf.rcScrollBar;
		else
			scRect.SetRect(0,0,0,0);	


		//
	}
	// TODO: Add your message handler code here
}

void CAnchorListWnd::OnDelectSelectionClick(void)
{
 
	HTREEITEM  htItem = GO_AnchorTree.GetNextItem(TVGN_ROOT,TVGN_CARET); 
	if(htItem == NULL) 
		return;

	TAnchorPage *pPage = NULL;
	PIECES_VECT::iterator  pageIt;
	PIECES_VECT::iterator  pageEndIt;
	if(GO_AnchorTree.GetParentItem(htItem) == GO_AnchorTree.m_hroot) //current is page item.
		pPage = reinterpret_cast<TAnchorPage *>(GO_AnchorTree.GetItemData(htItem));

	TAnchorPiece *pPiece =NULL ;
	if(pPage)
	{
		PIECES_VECT &Pieces = pPage->GetPieces();
		pageIt = Pieces.begin();
		pageEndIt = Pieces.end();
		pPiece = *pageIt;
	}
	else
	    pPiece = reinterpret_cast<TAnchorPiece *>(GO_AnchorTree.GetItemData(htItem));

	if(pPiece == NULL)
		return;

	GO_AnchorUrlsList.LockWindowUpdate();

	if(GO_AnchorUrlsList.GetCount()<= 0)
		return;

	BOOL  bHasSel= FALSE;
	int i = GO_AnchorUrlsList.GetCount();
	while( --i >= 0)
	{
		if(GO_AnchorUrlsList.GetSel(i))
		{
			URL_STRING *ptr = (URL_STRING *)GO_AnchorUrlsList.GetItemData(i);
			URL_LIST::iterator it ;
			if(pPiece->m_urlsList.size())
			{

LOOP_START:
			    it =pPiece->m_urlsList.begin();
				for( ; it != pPiece->m_urlsList.end();it++)
					if( &(*it) == ptr)
						break;
			}else
				goto LOOP;

			if(pPiece->m_urlsList.size() && it != pPiece->m_urlsList.end())
			{
				pPiece->m_urlsList.erase( it);
				GO_AnchorUrlsList.DeleteString(i);
			}else
			{ 
LOOP:
				if(pPage == NULL)
					break;

				pageIt++;
				if(pageIt != pageEndIt )
				{
					 pPiece = 	*pageIt ;
					 goto LOOP_START;
				}
				else
					 break;
			}
		}
	}
	

	GO_AnchorUrlsList.UnlockWindowUpdate();
	GO_AnchorUrlsList.SetClmCaption(0,0);
	OnSize(0,0,0);
	
 
}

void CAnchorListWnd::OnDelectAllClick(void)
{
 
	HTREEITEM  htItem = GO_AnchorTree.GetNextItem(TVGN_ROOT,TVGN_CARET); 
	if(htItem == NULL)
		return ;
	HTREEITEM hParent = GO_AnchorTree.GetParentItem(htItem);
	TAnchorPiece *pPiece ;
	if(hParent == GO_AnchorTree.m_hroot)//page item;
	{
		TAnchorPage *pPage = reinterpret_cast<TAnchorPage *>(GO_AnchorTree.GetItemData(htItem));
		if(pPage == NULL)
			return;
		PIECES_VECT &Pieces = pPage->GetPieces();
		for(PIECES_VECT::iterator it = Pieces.begin();it != Pieces.end();it++ )
		{
			pPiece = *it;
			while(pPiece->m_urlsList.size())
				pPiece->m_urlsList.erase(pPiece->m_urlsList.begin());
		}
		//return;
		goto _EXIT;
	}

	pPiece = reinterpret_cast<TAnchorPiece *>(GO_AnchorTree.GetItemData(htItem));
	if(pPiece == NULL)
		return;
	
	while(pPiece->m_urlsList.size())
		pPiece->m_urlsList.erase(pPiece->m_urlsList.begin());
_EXIT:
	GO_AnchorUrlsList.LockWindowUpdate();
	GO_AnchorUrlsList.DeleteAllItems();
	GO_AnchorUrlsList.UnlockWindowUpdate();

	GO_AnchorUrlsList.SetClmCaption(0,0);
	GO_AnchorUrlsList.SetSelLong(0);
 
}

void CAnchorListWnd::OnSelectAllClick(void)
{

	GO_AnchorUrlsList.LockWindowUpdate();
	for(int i = 0 ;i <GO_AnchorUrlsList.GetCount();i++ )
		GO_AnchorUrlsList.SetSel(i);
	GO_AnchorUrlsList.UnlockWindowUpdate();

}

 

void CAnchorListWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTabPanelWnd::OnPaint() for painting messages
	COLORREF  bkColor =  RGB(255,255,255);
	CRect cltRect;
	CBrush brush,brush2;
	
	int btm = cltRect.bottom;

	GetClientRect(&cltRect);
	cltRect.top = cltRect.bottom - BTN_HEIGHT - 1;
	cltRect.bottom  = cltRect.top + 2;
	brush2.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
    dc.FillRect(&cltRect,&brush2);
	
	cltRect.top++;
 
	//cltRect.bottom = btm;
	brush.CreateSolidBrush(bkColor);
	dc.FillRect(&cltRect,&brush);
}

 

BOOL CAnchorListWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||
        pMsg->message== WM_LBUTTONUP ||
        pMsg->message== WM_MOUSEMOVE)
			m_ToolTip.RelayEvent(pMsg);

	return CTabPanelWnd::PreTranslateMessage(pMsg);
}

void CAnchorListWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	 POINT pt ;
	 GetCursorPos(&pt);

	 CWnd *curWnd = CWnd::WindowFromPoint(pt);
	 if(!curWnd) return;

	 if( !curWnd->IsKindOf(RUNTIME_CLASS(CListBox)))
		 return;


	CMenu  popmenu;
	popmenu.CreatePopupMenu();
	popmenu.AppendMenu(0 ,IDM_DELSELECTION,_T("Delete Selection"));
	
	popmenu.AppendMenu(MF_SEPARATOR);
    popmenu.AppendMenu(0,IDM_SELECTALL,_T("Select All"));
	popmenu.AppendMenu(MF_SEPARATOR);
	popmenu.AppendMenu(0,IDM_DELALL,_T("Delete All"));
	
	
	popmenu.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,AfxGetMainWnd());


}
