// AnchorTree.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "defs.h"
#include "AnchorTree.h"
#include ".\anchortree.h"
#include "tanchorpage.h"
#include "tanchorpiece.h"
#include ".\anchorurlslist.h"
#include ".\tanchormanager.h"

CAnchorTree  GO_AnchorTree;
// CAnchorTree

IMPLEMENT_DYNAMIC(CAnchorTree, CTreeCtrl)
CAnchorTree::CAnchorTree()
{
	m_hDragItem = NULL;
	m_hDropItem = NULL;
	m_bDraging = FALSE;
	m_hListIcon = NULL;
}

CAnchorTree::~CAnchorTree()
{
}


BEGIN_MESSAGE_MAP(CAnchorTree, CTreeCtrl)
//	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_MESSAGE(WM_USER + 33,OnDelPiece)
	ON_NOTIFY_REFLECT(TVN_GETINFOTIP, OnTvnGetInfoTip)
//	ON_WM_CONTEXTMENU()
ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CAnchorTree message handlers
//CString  clmStr;
void CAnchorTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
 
	//LPNMTREEVIEW pTree = (LPNMTREEVIEW)pNMHDR;
	HTREEITEM selItem = this->GetNextItem(TVI_ROOT, TVGN_CARET);
	GO_AnchorUrlsList.DeleteAllItems();
	GO_AnchorUrlsList.SetSelLong(0);

	if(selItem  == m_hroot) //root nod
	{ 
		GO_AnchorUrlsList.PostMessage(WM_SETCLMCAPTION,0,0);
		return ;
	}

	HTREEITEM  hParent = GetParentItem(selItem);
	if(hParent == NULL) 
	{
		GO_AnchorUrlsList.PostMessage(WM_SETCLMCAPTION,0,0);
		return;
	}


	if(hParent == m_hroot)//page nod
	{
		TAnchorPage *pPage = (TAnchorPage *) GetItemData(selItem);	
		PIECES_VECT &Pieces = pPage->GetPieces();
		int nItemCount = 0;
		for(PIECES_VECT::iterator it = Pieces.begin();
			it != Pieces.end();it++)
		{
			 	TAnchorPiece *pPiece = *it;
				for(URL_LIST::iterator t = pPiece->m_urlsList.begin();
					t != pPiece->m_urlsList.end();t++)
				{
					int iItem = GO_AnchorUrlsList.AddString(*t,m_hListIcon);
					GO_AnchorUrlsList.SetItemData(iItem,(DWORD_PTR)&(*t));
				}
		}
		GO_AnchorUrlsList.PostMessage(WM_SETCLMCAPTION,0,0);

	}
	else//piece nod
	{
		TAnchorPiece *pPiece = (TAnchorPiece *) GetItemData(selItem);	
	    
		for(URL_LIST::iterator t = pPiece->m_urlsList.begin();
			t != pPiece->m_urlsList.end();t++)
		{
			int iItem = GO_AnchorUrlsList.AddString(*t,m_hListIcon);
			//GO_AnchorUrlsList.SetCheck(iItem,1);
		    GO_AnchorUrlsList.SetItemData(iItem,(DWORD_PTR)&(*t));
		}

	}
	GO_AnchorUrlsList.PostMessage(WM_SETCLMCAPTION,0,0);
 
}

HRESULT  CAnchorTree::OnDelPiece(WPARAM wparam,LPARAM lparam)
{
  TAnchorPiece *pPiece = (TAnchorPiece *) wparam;
  if(pPiece->m_urlsList.size() <= 0)
  {
	  TAnchorPage *pPage = pPiece->m_rootPage;
	 
	  this->DeleteItem(pPiece->m_hitem);  
	  pPage->RemovePiece(pPiece);  
	 
	  if(pPage->GetPieceCount() <= 0)
	  {
		  this->DeleteItem(TAnchorManager::Instance()->GetPageItemHandle(pPage));
		  TAnchorManager::Instance()->RemovePage(pPage);
	  }
	
  }

  return S_OK;
}

static CString  toolTipStr;
void CAnchorTree::OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	/*if(m_bDraging)*/ //return;
	if(pGetInfoTip->hItem == m_hroot)
	{
		/*toolTipStr = "thisis the root...";
		pGetInfoTip->pszText = toolTipStr.GetBuffer();*/
		return ;
	}

	if(this->GetParentItem(pGetInfoTip->hItem) == m_hroot)
	{

		TAnchorPage *pPage = (TAnchorPage *)this->GetItemData(pGetInfoTip->hItem);

		toolTipStr.Format(_T("Title: %s \nPage Link:%s"),pPage->m_PageTitle,pPage->m_BindPageUrl);
		//toolTipStr =  pPage->m_PageTitle;
		pGetInfoTip->pszText = toolTipStr.GetBuffer();
		return ;
	}

	
	TAnchorPiece *pPiece = (TAnchorPiece *)this->GetItemData(pGetInfoTip->hItem);
	if(pPiece->m_urlsList.size() > 3)
	{
		URL_LIST::iterator it = pPiece->m_urlsList.begin();
		toolTipStr.Format(_T("URL Number:%d\n\n"),pPiece->m_urlsList.size());
		toolTipStr += *it++;
		toolTipStr += _T("\n");
		toolTipStr += *it++;
		toolTipStr += _T("\n");
		toolTipStr += *it;
		toolTipStr += _T("\n ...");
	}
	else
	{
		switch(pPiece->m_urlsList.size())
		{
			case  0:
			{
				toolTipStr =_T("URL Number:0");
			}
			break;
			case  1:
			{
				toolTipStr.Format(_T("URL Number: 1\n\n%s"),*(pPiece->m_urlsList.begin()));
			}
			break;
			case  2:
			{
				URL_LIST::iterator it = pPiece->m_urlsList.begin();
				toolTipStr.Format(_T("URL Number:2\n\n%s\n"),*it);
				it++;
				toolTipStr += *it;

			}
			break;
			case  3:
			{
				URL_LIST::iterator it = pPiece->m_urlsList.begin();
				toolTipStr.Format(_T("URL Number:%d\n\n"),pPiece->m_urlsList.size());
				toolTipStr += *it++;
				toolTipStr += _T("\n");
				toolTipStr += *it++;
				toolTipStr += _T("\n");
				toolTipStr += *it;
				
			}
			break;
	  
		}
	}
	 
	pGetInfoTip->pszText = toolTipStr.GetBuffer();
}



void CAnchorTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	 if(((NM_TREEVIEW *)pNMHDR)->itemNew.hItem == m_hroot)return; 
	 
	 m_hDragItem = ((NM_TREEVIEW *)pNMHDR)->itemNew.hItem;
	 m_bDraging = TRUE;

	 this->SetCapture();
	*pResult = 0;
}

void CAnchorTree::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt (point);
	UINT uFlags;

	//ClientToScreen (&pt);
	if(m_bDraging)
	{
		m_hDropItem = HitTest(pt,&uFlags);
		this->SelectDropTarget(m_hDropItem);
		//this->SetItemState(m_hDropItem,TVIS_FOCUSED ,TVIS_FOCUSED );
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CAnchorTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	

	if(m_hDragItem == NULL) return ;
	if(m_hDropItem == this->m_hroot) return ;
	if(m_hDragItem == m_hDropItem ) return ;

	SelectDropTarget (NULL);
	::ReleaseCapture ();

	 
	// get item that was dragged
	//VERIFY (GetItem (&tvi));
	

	if (m_hDropItem)
	{
		HTREEITEM hItemRoot = GetNextItem (m_hDropItem, TVGN_PARENT);
		if(hItemRoot != GetNextItem(m_hDragItem,TVGN_PARENT))
			return ;
		CString itemText ;
		itemText = this->GetItemText(m_hDragItem);
		
		HTREEITEM hItemNew = this->InsertItem(itemText,hItemRoot,m_hDropItem);//新插入的位置 
		this->SetItemData(hItemNew,this->GetItemData(m_hDragItem));

		if(hItemRoot == this->m_hroot)//page item was draged
		{	
			TAnchorPage *pPage = TAnchorManager::Instance()->GetPage(m_hDragItem);//改变记录里面对应得 item handle
			TAnchorManager::Instance()->SetPageItemHandle(pPage,hItemNew);

			if(this->ItemHasChildren(m_hDragItem))
			{
				HTREEITEM  hChildItem = this->GetNextItem(m_hDragItem,TVGN_CHILD );
				while(hChildItem)
				{
					itemText = this->GetItemText(hChildItem);
					
					HTREEITEM hItem = this->InsertItem(itemText,hItemNew,TVI_LAST);
					this->SetItemData(hItem,this->GetItemData(hChildItem));

					hChildItem = this->GetNextItem(hChildItem,TVGN_NEXT);

				}
			}
		}
		DeleteItem (m_hDragItem);
	}
	


	m_hDragItem = NULL;
	m_hDropItem = NULL;
	m_bDraging  = FALSE;
	 
	CTreeCtrl::OnLButtonUp(nFlags, point);
}
