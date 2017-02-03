// ExListBox.cpp : implementation file
//

#include "stdafx.h"

#include "ExListBox.h"
#include "OwnerDrawListbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CExListBox
IMPLEMENT_DYNCREATE(CExListBox,COwnerDrawListBox) 
CExListBox::CExListBox()
{
	m_dxItem = 0;
	 
}

CExListBox::~CExListBox()
{
}


BEGIN_MESSAGE_MAP(CExListBox, COwnerDrawListBox)
	//{{AFX_MSG_MAP(CExListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.

	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListBox message handlers

void CExListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	 

	COwnerDrawListBox::PreSubclassWindow();
	EnableToolTips(TRUE);

}

CString  toolTipStr;
INT_PTR CExListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI)const   
{
	int row;
	RECT cellrect;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	//note that in NT this function may fail  use the ItemFromPointNT member function

	if ( row == -1 ) 
		return -1;
    
	if( IsMouseOnIcon2())
		return -1;

	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row));  
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return pTI->uId;

}

void CExListBox::OnDeleteString(LPCTSTR lpStr)
{
	 
	 
}

BOOL CExListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	
    TOOLTIPTEXT *pTT = ( TOOLTIPTEXT *)pNMHDR;
	UINT nID = pNMHDR->idFrom;

	
	GetText( nID ,toolTipStr);

	if(pNMHDR->code == TTN_NEEDTEXT)
		pTT->lpszText = toolTipStr.GetBuffer();

	*pResult = 0;

	return TRUE;    
}




UINT CExListBox::ItemFromPoint2(CPoint pt, BOOL& bOutside) 

// CListBox::ItemFromPoint does not work on NT.

{
	int nFirstIndex, nLastIndex;
	//GetFirstAndLastIndex(nFirstIndex, nLastIndex);
	nFirstIndex = GetTopIndex();
	nLastIndex = nFirstIndex  + GetCount(); 

	bOutside = TRUE;
	
	CRect Rect;
	int nResult = -1;
	
	for (int i = nFirstIndex; nResult == -1 && i <= nLastIndex; i++)
	{
		if (GetItemRect(i, &Rect) != LB_ERR)
		{
			if (Rect.PtInRect(pt))
			{
				nResult  = i;
				bOutside = FALSE;
			}
		}
		
	}
	
	return nResult;
}


void CExListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//if(IsMouseOnIcon2())
	//	this->DeleteString(this->GetCurSel());
	if(TRUE == IsMouseOnIcon2())
		return;

	COwnerDrawListBox::OnLButtonDown(nFlags, point);
}

void CExListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	COwnerDrawListBox::OnLButtonUp(nFlags, point);
	
}

void CExListBox::SetNoSel(void)
{
	int iSel = 0;
	for(;iSel < GetCount();iSel++)
	{
		if(GetSel(iSel) > 0)
			SetSel(iSel,0);
	}
}


void  CExListBox::GetText(int iItem,CString &str)
{
	COwnerDrawListBox::DataItems  *pItem = (COwnerDrawListBox::DataItems *)this->GetItemSubDataPtr(iItem);
	if(pItem)
		str = pItem->strItemData ;
}

void CExListBox::SetRawWidth(int nWidth)
{
	m_dxItem = nWidth;
	SetHorizontalExtent(m_dxItem);
}
