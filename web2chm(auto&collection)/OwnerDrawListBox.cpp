// OwnerDrawListBox.cpp : implementation file
//

#include "stdafx.h"
#include "OwnerDrawListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// COwnerDrawListBox
 IMPLEMENT_DYNCREATE(COwnerDrawListBox,CListBox) 
COwnerDrawListBox::COwnerDrawListBox()
{
	m_bPropertyInit = false;
    hitIconItem = -1;
	m_bEnableFullPath = FALSE ;

}

COwnerDrawListBox::~COwnerDrawListBox()
{
 
}


BEGIN_MESSAGE_MAP(COwnerDrawListBox, CListBox)
//{{AFX_MSG_MAP(COwnerDrawListBox)
ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()

ON_WM_MOUSEWHEEL()
ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawListBox message handlers

// Delete the Pointer which holds the List Box Data Items

BOOL  COwnerDrawListBox::IsMouseOnIcon2()const 
{
	return (hitIconItem != -1)?1:0;
}
void COwnerDrawListBox::Destroy()
{
	int ItemCount;
	ItemCount = GetCount();

	if (ItemCount != 0)
		for (int i = 0; i < ItemCount; i++)
		{
			m_pListBoxDataItems = (DataItems*) GetItemSubDataPtr(i);
			delete m_pListBoxDataItems;
		}
}

// This is where is handle the Drag and Drop Event
void COwnerDrawListBox::OnDropFiles(HDROP hDropInfo)
{
	//TCHAR strFileName[MAX_PATH], strDrive[MAX_PATH], strDir[MAX_PATH], strExt[MAX_PATH];
	//TCHAR szFilePath[MAX_PATH];

}

//
//rebuild this func again.
//
int  COwnerDrawListBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const  
{
		ASSERT(lpszFind != NULL);

		if(NULL == lpszFind)
			return LB_ERR;
		int iCount = this->GetCount();
		if(nIndexStart >= iCount)
			return LB_ERR;

		for( int i = nIndexStart ;i < iCount;i++)
		{
			struct DataItems *pListDataItem = ( struct DataItems *)GetItemSubDataPtr(i);
  			if(pListDataItem->strItemData.Compare(lpszFind) == 0) //not call compareItem here,for i neednt it.
				return i;

		}
		return LB_ERR;
}

int COwnerDrawListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	//lpCompareItemStruct;
	return TRUE;
}

void COwnerDrawListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
}



CSize COwnerDrawListBox::GetItemIconSize(HICON hicon)
{
		CSize sz;

		ICONINFO  info;
		BOOL bSucc = ::GetIconInfo(hicon ,&info);

		DWORD dw ;
		if(!bSucc)
		{
			  dw = GetLastError();  
			  ASSERT(FALSE);
		}
		sz.cx = info.xHotspot ;
		sz.cy = info.yHotspot ;

		DeleteObject(info.hbmColor);
		DeleteObject(info.hbmMask);
		return sz;
}



CRect  COwnerDrawListBox::GetItemTextRect(int iItem)
{
	CRect itemRect(0,0,0,0);

	this->GetItemRect(iItem,&itemRect);

	 DataItems *pListDataItem = (DataItems *)GetItemSubDataPtr(iItem);

	 if(NULL == pListDataItem)
		 return itemRect;

	 if(pListDataItem->hIcon)
	 {
		 CSize sz = pListDataItem->icoSize;
            if(m_IconPos == ITEM_RIGHT)
			{
				itemRect.left += 10;			
				itemRect.right = itemRect.right - sz.cx;
			}
			else
				itemRect.left += sz.cx;
			
	 }

	 return itemRect;
}

CRect  COwnerDrawListBox::GetItemIconRect( int iItem )
{
	   CRect itemRect(0,0,0,0);
	
	   DataItems *pListDataItem = (DataItems *)GetItemSubDataPtr(iItem);
	   if(NULL == pListDataItem )
		   return itemRect ;
	   if(pListDataItem->hIcon)
	   {
		   CSize sz  = pListDataItem->icoSize;
			GetItemRect(iItem,&itemRect);

			TRACE("Icon handle:%d",pListDataItem->hIcon);
			if(m_IconPos == ITEM_RIGHT)
			{
				itemRect.left = itemRect.right - sz.cx;
				itemRect.top += (itemRect.Height() - sz.cy) / 2;
			}
			else
			{
				itemRect.right = itemRect.left + sz.cx;
				itemRect.top += (itemRect.Height() - sz.cy) / 2;
			}

	   }

	   return itemRect;
	
}


struct AFX_CHECK_DATA
{
public:
	int m_nCheck;
	BOOL m_bEnabled;
	UINT_PTR m_dwUserData;

	AFX_CHECK_DATA()
	{
		m_nCheck = 0;
		m_bEnabled = TRUE;
		m_dwUserData = 0;
	};
};


BOOL COwnerDrawListBox::IsEnabled(int nIndex)
{
	ASSERT(::IsWindow(m_hWnd));

	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, nIndex, 0);
	if (lResult != LB_ERR)
	{
		AFX_CHECK_DATA* pState = (AFX_CHECK_DATA*)lResult;
		if (pState != NULL)
			return pState->m_bEnabled;
	}
	return TRUE; // The default
}


void COwnerDrawListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// You must override DrawItem and MeasureItem for LBS_OWNERDRAWVARIABLE
   ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

   if(lpDrawItemStruct->CtlType != ODT_LISTBOX)
	   return;

   LPCTSTR lpszText ;//(LPCTSTR) lpDrawItemStruct->itemData;

   if(this->GetCount() <= 0)
		return;
  /* if(!IsEnabled(lpDrawItemStruct->itemID))
	   return;*/

   DataItems *pListDataItem = (DataItems *)GetItemSubDataPtr(lpDrawItemStruct->itemID);
   if(NULL == pListDataItem)
	   return;
   lpszText = pListDataItem->strItemData;
   ASSERT(lpszText != NULL);

 
   CRect rectText = this->GetItemTextRect(lpDrawItemStruct->itemID);
   CRect rectIcon = this->GetItemIconRect(lpDrawItemStruct->itemID);
	
  
   CDC dc;
   dc.Attach(lpDrawItemStruct->hDC);

    
   COLORREF crOldTextColor = dc.GetTextColor();
   COLORREF crOldBkColor = dc.GetBkColor();

 	if (((LONG)(lpDrawItemStruct->itemID) >= 0) &&
		(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{

	   if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		  (lpDrawItemStruct->itemState & ODS_SELECTED))
	   {
		  dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		  dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		  dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
			 ::GetSysColor(COLOR_HIGHLIGHT));
	   }
	   else
	   {
		  dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	   }
	 
	   if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		  (lpDrawItemStruct->itemState & ODS_FOCUS))
	   {
		  CBrush br(RGB(255, 0, 0));
		  dc.FrameRect(&rectText, &br);
	   }
 
	   CString strPath = lpszText;
	   if(FALSE == m_bEnableFullPath)
	   {
		   PathStripPath(strPath.GetBuffer());
		   strPath.ReleaseBuffer();
	   }
		  
   // Draw the text.
	  dc.DrawText(
      strPath,
	  (int)strPath.GetLength(),
      &rectText,
      DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX); 

 	   if(pListDataItem->hIcon)
	   {
		::DrawIconEx(dc.GetSafeHdc(),rectIcon.left,rectIcon.top,pListDataItem->hIcon,
									rectIcon.Width(),rectIcon.Height(),0,0,DI_NORMAL | DI_COMPAT ); 
	   }  
	
	}

   // Reset the background color and the text color back to their
   // original values.
   dc.SetTextColor(crOldTextColor);
   dc.SetBkColor(crOldBkColor);
   dc.Detach();

}

void COwnerDrawListBox::SetIconPosition(Item_Pos Ico_Pos)
{
	m_IconPos = Ico_Pos;
}

int COwnerDrawListBox::SetItemSubDataPtr(int nIndex ,void *pData)
{
	int nSize = this->m_itemSubDataPtrs.size();
	if( nIndex < 0|| nIndex >nSize)
		return -1;
	if(nIndex <= (nSize - 1))
		this->m_itemSubDataPtrs[nIndex] = pData;
	else
		this->m_itemSubDataPtrs.push_back(pData);
   return nIndex;
}

void * COwnerDrawListBox::GetItemSubDataPtr(int nIndex) const
{
	if(nIndex < 0 || nIndex >= this->m_itemSubDataPtrs.size())
		return NULL;
	return this->m_itemSubDataPtrs[nIndex];
}
int  COwnerDrawListBox::AddString(CString strItemName, HICON hDataIcon)
{
	DWORD  dwStyle = this->GetStyle();

	if(!m_bPropertyInit)
	{
		dwStyle &= ~LBS_SORT ;
		dwStyle &= ~LBS_HASSTRINGS; 
		dwStyle |=  LBS_NOINTEGRALHEIGHT ;
		dwStyle |=  LBS_OWNERDRAWVARIABLE;
	//	dwStyle |=	LBS_NOTIFY;
	 
		this->ModifyStyle(0xffffffff,dwStyle,SWP_NOSIZE|SWP_NOMOVE|SWP_NOCOPYBITS);
		m_bPropertyInit = true;
	}

	m_pListBoxDataItems = new DataItems;
	m_pListBoxDataItems->hIcon = hDataIcon;
	m_pListBoxDataItems->strItemData = strItemName;

	
	int index = CListBox::AddString(_T(" "));
	SetItemSubDataPtr(index, m_pListBoxDataItems); 

	if(hDataIcon)
	{
		m_pListBoxDataItems->icoSize = GetItemIconSize(hDataIcon);
		this->SetItemHeight(index,m_pListBoxDataItems->icoSize.cy);
	}
	return index;
	
	
}
 

BOOL COwnerDrawListBox::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
    int itemsCount = this->GetCount();
	for(int i = 0;i < itemsCount;i++)
	{
		struct DataItems *p = (DataItems *)this->GetItemSubDataPtr(i);
		delete p;
	}
	return TRUE;//CListBox::DestroyWindow();
}

void COwnerDrawListBox::OnDestroy()
{

	DestroyWindow();
	CListBox::OnDestroy();

	// TODO: Add your message handler code here
}

void COwnerDrawListBox::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bPropertyInit = false;
	CListBox::PreSubclassWindow();
}

LRESULT COwnerDrawListBox::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default


	BOOL  outSide;
	
	this->ScreenToClient(&point);
	int iItem =  this->ItemFromPoint(point,outSide);

	int item = hitIconItem;
	if(iItem >= 0  && this->GetSelCount() <= 1) //item 内部，进行点击测试。
	{
		//get icon rect.
        CRect iconRect = this->GetItemIconRect(iItem);

		if(::PtInRect( &iconRect,point))
		{
			hitIconItem = iItem;
			if(hitIconItem != item)
			{

				if(hitIconItem != -1)
				{
					CRect cliRect;
					CRgn rgn;
					CDC *pDC = this->GetDCEx( NULL   ,DCX_CACHE|DCX_LOCKWINDOWUPDATE);
					
					GetClientRect(&cliRect);
					pDC->SaveDC();
					iconRect.bottom = (iconRect.bottom > cliRect.bottom)? cliRect.bottom:iconRect.bottom; 
					pDC->Draw3dRect(iconRect,RGB(191,191,191),RGB(0,0,0));
					this->ReleaseDC(pDC);
				}

				if(item != -1)
				{
					iconRect = this->GetItemIconRect(item);
					this->InvalidateRect(iconRect);
					hitIconItem = -1;
				}
			} 

		}
		else
		{
			if( hitIconItem != -1)
			{
				iconRect = this->GetItemIconRect(item);
				this->InvalidateRect(iconRect);
				hitIconItem = -1;
			}
		}

	}else
		hitIconItem = -1;


	return CListBox::OnNcHitTest(point);
}


int  COwnerDrawListBox::DeleteString(UINT nIndex)
{
	if( nIndex >= this->GetCount() && nIndex < 0)
		return FALSE;

    struct DataItems *p = (DataItems *)this->GetItemSubDataPtr(nIndex);
	delete p;

	if(nIndex < this->m_itemSubDataPtrs.size())
	{
		this->m_itemSubDataPtrs.erase(m_itemSubDataPtrs.begin() + nIndex);
	}
        
	return __super::DeleteString(nIndex);	
}

void COwnerDrawListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

//test pt on item button.
	if(IsMouseOnIcon2())
	{ 

		if(hitIconItem >= 0 && (this->GetSelCount() <= 1) && hitIconItem <= this->GetCount())
		{
			OnBeforeDelItem();
		}
		else
			return;
//free item data ptr,which create by   "new"
		struct DataItems *p = (DataItems *)this->GetItemSubDataPtr(hitIconItem);
		GetOwner()->SendMessage(WM_LBDELETEITEM ,(WPARAM)(LPCTSTR)p->strItemData,(LPARAM)0); 

		this->DeleteString(hitIconItem);	
	
		int iCount = this->GetCount();
		if(iCount)
		{
			if(hitIconItem >= iCount)
			{
				hitIconItem = iCount;
				hitIconItem --;
			}
			
 //reset all selected items.
			iCount = this->GetSelCount();

			CArray<int,int> aryListBoxSel;
			aryListBoxSel.SetSize(iCount + 1);

			this->GetSelItems(iCount,aryListBoxSel.GetData());

			while(--iCount >= 0)
				this->SetSel(aryListBoxSel[iCount],0);
//select one,or the next.
			this->SetSel(hitIconItem,1);
		}
		hitIconItem = -1;
	}
	CListBox::OnLButtonUp(nFlags, point);
}

void COwnerDrawListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

void COwnerDrawListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
}

//
//防止闪动，及过多绘画。
//
BOOL COwnerDrawListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
    //CScrollView *pView = (CScrollView *)this;

	int startPos = this->GetScrollPos(SB_VERT);
	int scrlMax = this->GetScrollLimit(SB_VERT);
	 
	if(startPos >= scrlMax && zDelta < 0)
		return TRUE;

	if(startPos <= 0 && zDelta > 0)
		return TRUE;

	this->LockWindowUpdate();
	BOOL bRet = __super:: OnMouseWheel(nFlags, zDelta, pt);
	this->UnlockWindowUpdate();
	 
	int endPos = this->GetScrollPos(SB_VERT);
	if(startPos != endPos)
		this->Invalidate();
	return bRet;

}

void COwnerDrawListBox::OnSize(UINT nType, int cx, int cy)
{
	CListBox::OnSize(nType, cx, cy);

	this->Invalidate();
	// TODO: Add your message handler code here
}
