// AnchorUrlsList.cpp : implementation file
//

#include "stdafx.h"

#include "AnchorUrlsList.h"
#include ".\anchorurlslist.h"
#include ".\anchortree.h"
#include "tanchorpage.h"
#include "tanchorpiece.h"

// CAnchorUrlsList
 CAnchorUrlsList GO_AnchorUrlsList;

//IMPLEMENT_DYNAMIC(CAnchorUrlsList, CExListBox)
CAnchorUrlsList::CAnchorUrlsList()
{
}

CAnchorUrlsList::~CAnchorUrlsList()
{
}


BEGIN_MESSAGE_MAP(CAnchorUrlsList, CExListBox)
//	ON_WM_ERASEBKGND()
 ON_MESSAGE(WM_SETCLMCAPTION ,SetClmCaption)
 //ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
 //ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
 ON_WM_SIZE()
// ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnLbnKillfocus)
ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
END_MESSAGE_MAP()



// CAnchorUrlsList message handlers

static CString clmStr = "URL Number:0";

LRESULT  CAnchorUrlsList::SetClmCaption(WPARAM,LPARAM)
{
	
	HDITEM  hdi;
	CRect hdRct;
	m_Head.GetClientRect(&hdRct);
	hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hdi.cxy = hdRct.Width(); // Make all columns 100 pixels wide.
	hdi.fmt = HDF_STRING | HDF_LEFT;
	

	HTREEITEM selItem = GO_AnchorTree.GetNextItem(TVI_ROOT, TVGN_CARET);
	HTREEITEM hParent = GO_AnchorTree.GetNextItem(selItem,TVGN_PARENT);
	int nUrlsCount  = 0;
	if(selItem == GO_AnchorTree.m_hroot)
	{
		HTREEITEM hChildItem = GO_AnchorTree.GetNextItem(selItem,TVGN_CHILD);
		while(hChildItem)
		{
			TAnchorPage *pPage = (TAnchorPage *) GO_AnchorTree.GetItemData(hChildItem);	
			if(pPage->GetPieceCount())
			{
				PIECES_VECT &Pieces = pPage->GetPieces();
				for(PIECES_VECT::iterator it = Pieces.begin();
				 it != Pieces.end();it++)
				{
					TAnchorPiece *pPiece = (TAnchorPiece *) *it;
					nUrlsCount += pPiece->m_urlsList.size();
				}
			}
			hChildItem = GO_AnchorTree.GetNextItem(hChildItem,TVGN_NEXT);
		}
	}
	else
	{
		if(hParent == GO_AnchorTree.m_hroot) //page
		{
			TAnchorPage *pPage = (TAnchorPage *) GO_AnchorTree.GetItemData(selItem);	
			if(pPage->GetPieceCount())
			{
				PIECES_VECT &Pieces = pPage->GetPieces();
				for(PIECES_VECT::iterator it = Pieces.begin();
				 it != Pieces.end();it++)
				{
					TAnchorPiece *pPiece = (TAnchorPiece *) *it;
					nUrlsCount += pPiece->m_urlsList.size();
				}
			}
		}
		else //piece
		{
			TAnchorPiece *pPiece = (TAnchorPiece *) GO_AnchorTree.GetItemData(selItem);	
			nUrlsCount =pPiece->m_urlsList.size();
		}
	}

	clmStr.Format("URL Number:%d ",nUrlsCount);
	hdi.pszText = clmStr.GetBuffer();
	clmStr.ReleaseBuffer();
	m_Head.SetItem(0,&hdi);
	return S_OK;
}




void  CAnchorUrlsList::Create(CWnd *pParent)
{
	 __super::Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|LBS_OWNERDRAWFIXED|
		 LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT|LBS_EXTENDEDSEL|LBS_NOTIFY ,CRect(0,0,0,0),pParent,ID_ANCHOR_URLS_LIST);

	SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))); 
	
	m_Head.Create(WS_CHILD|WS_VISIBLE|HDS_FLAT,CRect(0,0,0,0),pParent,0xffff);
	m_Head.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT))); 

	HDITEM  hdi;
	hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
 	hdi.cxy = 100; // Make all columns 100 pixels wide.
	hdi.fmt = HDF_STRING | HDF_LEFT;
	hdi.pszText = clmStr.GetBuffer();
	clmStr.ReleaseBuffer(); 
	m_Head.InsertItem(0,&hdi);

	m_Head.SetFont(CFont::FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT)));
	 
}
void CAnchorUrlsList::DeleteAllItems(void)
{
	int iCount = GetCount();
	for( int i = 0;i < iCount ;i++)
		DeleteString(0);	

}
#define HEAD_HEIGHT  16
void CAnchorUrlsList::MoveWindow(CRect & rect)
{
	CRect rct(rect);

	TRACE("RECT:left %d,top %d,right %d,botton %d \n",rect.left,rect.top,rect.right,rect.bottom);
	if(rct.IsRectEmpty())return ;
	 
	m_Head.MoveWindow(rct.left,rct.top,rct.Width(),HEAD_HEIGHT);

	rct.top += HEAD_HEIGHT;

	__super::MoveWindow(rct.left,rct.top,rct.Width(),rct.Height());
}

void CAnchorUrlsList::OnSize(UINT nType, int cx, int cy)
{
	CExListBox::OnSize(nType, cx, cy);
	if(!::IsWindow(m_Head.m_hWnd)) return ;
	if(m_Head.GetItemCount() <= 0) return ;
	HDITEM  hdi;
	 
	hdi.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hdi.fmt = HDF_STRING | HDF_LEFT;
	CRect rct ;
	GetWindowRect(&rct);
	hdi.cxy = rct.Width();
	hdi.pszText = clmStr.GetBuffer();
	clmStr.ReleaseBuffer();
	m_Head.SetItem(0, &hdi);
}

#include "mfcieVw.h"
void CAnchorUrlsList::OnLbnDblclk()
{
	// TODO: Add your control notification handler code here
	 
	int item = this->GetCaretIndex();
	if(item == LB_ERR) return ;
	CRect itRect;
	CString str;
	this->GetText(item,str);
	this->GetItemRect(item,&itRect);

	CPoint pt;
	::GetCursorPos(&pt);
	this->ScreenToClient(&pt);
	if(itRect.PtInRect(pt))

	AfxGetMainWnd()->LockWindowUpdate();
	CMfcieView::Instance()->Navigate(str);
	AfxGetMainWnd()->UnlockWindowUpdate();

}
