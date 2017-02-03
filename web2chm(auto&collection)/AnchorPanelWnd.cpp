#include "StdAfx.h"
#include "resource.h"
#include ".\anchorpanelwnd.h"
#include "AnchorTree.h"
#include ".\tanchorpage.h"
#include ".\tanchorpiece.h"
#include ".\tanchormanager.h"
 
CAnchorPanelWnd::CAnchorPanelWnd(CWnd *pParent)
{
	__super::Create(pParent);
}

CAnchorPanelWnd::~CAnchorPanelWnd(void)
{
}
BEGIN_MESSAGE_MAP(CAnchorPanelWnd, CTabPanelWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_ENDLABELEDIT ,ID_ANCHOR_TREE,OnTreeEndLabelEdit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_CUT,OnCut)
	ON_COMMAND(IDM_CUT_ALL,OnCutAll)
	ON_UPDATE_COMMAND_UI(IDM_CUT, OnUpdateCut)
	//ON_NOTIFY(TVN_BEGINDRAG,ID_ANCHOR_TREE,OnBeginTreeDrag)
END_MESSAGE_MAP()


int CAnchorPanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabPanelWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	 
	//m_ImagList.Create();
	// TODO:  Add your specialized creation code here
	GO_AnchorTree.Create(WS_CHILD|WS_VISIBLE|TVS_EDITLABELS |TVS_TRACKSELECT|
				TVS_SHOWSELALWAYS|TVS_HASLINES|TVS_HASBUTTONS|TVS_INFOTIP |TVS_LINESATROOT ,CRect(0,0,0,0),this ,ID_ANCHOR_TREE);

	GO_AnchorTree.m_hroot = GO_AnchorTree.InsertItem("Pages");

	GO_AnchorTree.m_hListIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_IDELETEITEM));

	return 0;
}

void CAnchorPanelWnd::OnSize(UINT nType, int cx, int cy)
{
	CTabPanelWnd::OnSize(nType, cx, cy);
	if(::IsWindow(GO_AnchorTree.m_hWnd))
	{
		CRect cltRect;
		this->GetClientRectExcludeTitle(cltRect);
		GO_AnchorTree.MoveWindow(&cltRect);
	}
	// TODO: Add your message handler code here
}

void CAnchorPanelWnd::OnTreeEndLabelEdit(NMHDR* pHdr,LRESULT *lpResult)
{
	LPNMTVDISPINFO ptvHdr  = (LPNMTVDISPINFO) pHdr;
	*lpResult = S_OK;
	if(ptvHdr->item.hItem == GO_AnchorTree.m_hroot)return;
	if(GO_AnchorTree.GetParentItem(ptvHdr->item.hItem) ==  GO_AnchorTree.m_hroot)
	{
		TAnchorPage *pPage = (TAnchorPage *)GO_AnchorTree.GetItemData(ptvHdr->item.hItem);
		if(pPage && ptvHdr->item.pszText)
			pPage->m_PageTitle = ptvHdr->item.pszText;
	}else
	{
		TAnchorPiece *pPiece = (TAnchorPiece *)GO_AnchorTree.GetItemData(ptvHdr->item.hItem);
		if(pPiece && ptvHdr->item.pszText)
			pPiece->m_PieceTitle = ptvHdr->item.pszText;
	}
	*lpResult = S_FALSE;
}


void CAnchorPanelWnd::OnCut(void)
{
	HTREEITEM hitem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CARET);
	if(hitem == NULL)
		return;
	if(hitem == GO_AnchorTree.m_hroot)return;
	if(GO_AnchorTree.GetParentItem(hitem) == GO_AnchorTree.m_hroot ) //ispage
	{
		TAnchorManager::Instance()->RemovePage(hitem);
		GO_AnchorTree.DeleteItem(hitem);
	}else //piece
	{
		HTREEITEM  hParent = GO_AnchorTree.GetParentItem(hitem);
		TAnchorPage *pPage = (TAnchorPage *)GO_AnchorTree.GetItemData(hParent);

		pPage->RemovePiece((TAnchorPiece *)GO_AnchorTree.GetItemData(hitem));
		GO_AnchorTree.DeleteItem(hitem);

		if(pPage->GetPieceCount() <= 0)
		{
			TAnchorManager::Instance()->RemovePage(hParent);
			GO_AnchorTree.DeleteItem(hParent);
		}
	}
}

void CAnchorPanelWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	HTREEITEM hitem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CARET);
	if(hitem == GO_AnchorTree.m_hroot)return;

	CRect  irect;
	CPoint tvpoint = point;
	GO_AnchorTree.GetItemRect(hitem,&irect,TRUE);
	GO_AnchorTree.ScreenToClient(&tvpoint);

	if(::PtInRect(irect,tvpoint))
	{
		CMenu menu;
		CString  strMenu;

		menu.CreatePopupMenu();
		strMenu.LoadString(IDM_CUT);
		menu.AppendMenu((MF_POPUP|MF_STRING) ,IDM_CUT,strMenu);
		strMenu.LoadString(IDM_CUT_ALL);
		menu.AppendMenu((MF_POPUP|MF_STRING) ,IDM_CUT_ALL,strMenu);
		menu.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,::AfxGetMainWnd());
	}
}

void CAnchorPanelWnd::OnUpdateCut(CCmdUI *pCmdUI)
{
	
}

void CAnchorPanelWnd::OnCutAll(void)
{

	HTREEITEM hitem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CARET);
	HTREEITEM hparent = GO_AnchorTree.GetNextItem(hitem,TVGN_PARENT);
	hitem = GO_AnchorTree.GetNextItem(hparent,TVGN_CHILD);
	
	while(hitem)
	{
		HTREEITEM hnext;
		hnext = GO_AnchorTree.GetNextItem(hitem,TVGN_NEXT);
		if(hparent == GO_AnchorTree.m_hroot)
			TAnchorManager::Instance()->RemovePage(hitem);
		else
		{
			TAnchorPage *page = TAnchorManager::Instance()->GetPage(hparent);
			page->RemovePiece((TAnchorPiece *)GO_AnchorTree.GetItemData(hitem));
		}
		GO_AnchorTree.DeleteItem(hitem);
		hitem = hnext;
	}
}

void CAnchorPanelWnd::OnUpdateCutAll(CCmdUI* pCmdUI)
{
	//HTREEITEM hitem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CHILD);
	////if(hitem == GO_AnchorTree.m_hroot)return;
	////if(GO_AnchorTree.GetParentItem(hitem) == GO_AnchorTree.m_hroot ) //ispage
	//while(hitem)
	//{
	//	TAnchorManager::Instance()->RemovePage(hitem);
	//	GO_AnchorTree.DeleteItem(hitem);
	//	hitem = GO_AnchorTree.GetNextItem(hitem,TVGN_NEXT);
	//}
}

void CAnchorPanelWnd::OnEndTreeDrag(NMHDR * phdr, LRESULT * pResult)
{
	*pResult = TRUE;
}

void CAnchorPanelWnd::OnBeginTreeDrag(NMHDR * phdr, LRESULT * pResult)
{
	*pResult = TRUE;
}
