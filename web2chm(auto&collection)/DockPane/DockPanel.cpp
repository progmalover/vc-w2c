// DockPanel.cpp : implementation file
//
/*
AUTHOR:: ZHANGXY 
2007.12  DESIGNED FOR SOTHINK COMPANY

this panel has many ability like control bar
*/
#include "stdafx.h"
//#include "PanelCtrlBarDemo.h"
#include "DockPanel.h"
#include ".\dockpanel.h"
#include "SilibingPanelWnd.h"
//#include "PanelCtrlBar.h"
#define AFX_IDW_DOCKPANEL    WM_USER + 103
// CDockPanel
#define WM_SIZEPARENT       0x0361 
IMPLEMENT_DYNAMIC(CDockPanel, CWnd)
CDockPanel::CDockPanel()
{
	//m_pDockContext = NULL;
	m_storeSize.cx  = 0;
	m_storeSize.cy   =0;
}

CDockPanel::~CDockPanel()
{
}


BEGIN_MESSAGE_MAP(CDockPanel, CWnd)
	ON_WM_CREATE()
//	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


inline void GetOccupRect2(ARRAY_PANELS &arryPanel,CTabPanelWnd *pTabWnd,int iorder,CRect &srcRect,CRect &dskRect)
{
	CTabPanelWnd *ptab;
	dskRect = srcRect;
	if(!pTabWnd->m_bExtend)
	{
		dskRect.top = dskRect.bottom - pTabWnd->m_ConstrictSize.cy;
		srcRect.bottom = dskRect.top;
		return ;
	}

	for(	iorder-- ; iorder >=0 ;iorder--)
	{
		ptab = arryPanel[iorder];
		if(ptab->m_bExtend)
			dskRect.top += ptab->m_Size.cy;
		else
			dskRect.top += ptab->m_ConstrictSize.cy;
	}

	srcRect.bottom = dskRect.top;
}

// CDockPanel message handlers
inline void GetOccupRect(ARRAY_PANELS &arryPanel,CTabPanelWnd *pTabWnd,int iorder,CRect &srcRect,CRect &dskRect)
{
	CTabPanelWnd *ptab;
	dskRect = srcRect;
    if(!pTabWnd->m_bExtend)
	{
		dskRect.bottom = dskRect.top + pTabWnd->m_ConstrictSize.cy;
		srcRect.top += dskRect.Height();
		return ;
	}

   	iorder++;
	for(;iorder < arryPanel.size();iorder++)
	{
		 ptab = arryPanel[iorder];
		 if(ptab->m_bExtend)
			dskRect.bottom -= ptab->m_Size.cy;
		 else
		    dskRect.bottom -= ptab->m_ConstrictSize.cy;
	}

	srcRect.top = dskRect.bottom;
}

BOOL inline HasExtendItem(ARRAY_PANELS arryPanel)
{
	CTabPanelWnd *pItem;
	for(int i = 0;i < arryPanel.size();i++)
	{
		pItem = arryPanel[i];
		if(pItem->m_bExtend)
			return TRUE;
	}
	return FALSE;
}

void CDockPanel::RecalcLayout(void)
{
	
	if(m_arrPanel.size() <= 0)
		return ;
 
	//CPanelCtrlBar *pBar = (CPanelCtrlBar *)GetOwner();
	// if(pBar->m_bPop)//已经收起来了就不再计算
	//	 return ;

	CRect cltRect;
	CRect cltRectBk;

	GetClientRect(&cltRect);
	if(cltRect.right <= 0 || cltRect.bottom <= 0)
		return ;
	
	RestoreSpace(cltRect);//

	MakeChildrenFit(cltRect);
	cltRectBk = cltRect;

	if(cltRect.IsRectEmpty())
		return;

	int						iorder = 0;
	CRect					dskRect;
	CTabPanelWnd			*pWnd;
	
	if(m_bOrder == FALSE)
	{
JP:
		for(ARRAY_PANELS::iterator it = m_arrPanel.begin();
								it != m_arrPanel.end();it++,iorder++)
		{
			pWnd = *it;
			GetOccupRect(m_arrPanel,pWnd,iorder,cltRect,dskRect);	
			
			pWnd->SetWindowPos(NULL,dskRect.left,dskRect.top,dskRect.Width(),dskRect.Height(),
				SWP_NOZORDER|SWP_SHOWWINDOW);
		    
			pWnd->SendMessage(WM_SIZEPARENT,0,0);
		}
	}
	else
	{
        BOOL  bHasExtend = HasExtendItem(m_arrPanel);
		if(!bHasExtend)//保证全部收在上部。。。。
			goto JP;
		iorder = m_arrPanel.size();
		for(iorder--;iorder >= 0;iorder--)
		{
			pWnd = m_arrPanel[iorder];

			GetOccupRect2(m_arrPanel,pWnd,iorder,cltRect,dskRect);

			pWnd->SetWindowPos(NULL,dskRect.left,dskRect.top,dskRect.Width(),dskRect.Height(),
				SWP_NOZORDER|SWP_SHOWWINDOW);

			pWnd->SendMessage(WM_SIZEPARENT,0,0);
		}
  
	}


	StoreSpace(cltRectBk); // 空白的空间处理。。。
}

CDockPanel:: CDockPanel(CWnd *pParent)
{
	 BOOL ret;
	 ret = Create(pParent);
	 ASSERT(ret == TRUE);
	 m_storeSize.cx = 0;
	 m_storeSize.cy = 0;
	 m_bOrder = TRUE; // default  is  down to  up ....
}

BOOL CDockPanel::Create(CWnd* pParent)
{
	LPCSTR  strClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW ,::LoadCursor(NULL, IDC_ARROW), 
		(HBRUSH) ::GetStockObject(NULL_BRUSH),::LoadIcon(NULL, IDI_APPLICATION));
	return __super::Create(strClass,"DockPanel",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_BORDER,CRect(0,0,0,0),pParent,AFX_IDW_DOCKPANEL);
}


int CDockPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
    return CreateChildren();
}

BOOL CDockPanel::CreateChildren(void)
{
	 
	return TRUE ;
}


 BOOL CDockPanel::AssignChildrenTo(CWnd *pNewParent)
{
	  ASSERT(pNewParent);
      for(CWnd * pChild  = GetWindow(GW_CHILD);pChild;pChild = pChild->GetWindow(GW_HWNDNEXT))
		   pChild->SetParent(pNewParent);

	  return TRUE;
}

CTabPanelWnd * CDockPanel::AddTabPanelWnd(void)
{
	m_arrPanel.push_back(new CTabPanelWnd(this));
	return m_arrPanel[m_arrPanel.size()-1];
}

void CDockPanel::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PostNcDestroy();
	delete this;
}

void CDockPanel::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default
    
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	//if(this->m_arrPanel.size() * 40 >= )
	RecalcLayout();
}

BOOL CDockPanel::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush;
    CRect  bkRect;
    GetClientRect( &bkRect);
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
   
	pDC->FillRect(bkRect,&brush);
	return CWnd::OnEraseBkgnd(pDC);
}

void CDockPanel::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(this->m_arrPanel.size() * 40 >= cy)
		return;
	RecalcLayout();
	// TODO: Add your message handler code here
}
 
inline BOOL PlaceEnough(CRect &cltRect, ARRAY_PANELS &arrPanel)
{
		int hight = 0;
		CTabPanelWnd *pitem;
		for(int i = 0;i < arrPanel.size();i++)
		{
			pitem  = arrPanel[i];
			if(pitem->m_bExtend )
			hight += pitem->m_Size.cy;
			else
			hight += pitem->m_ConstrictSize.cy;
		}

		return  cltRect.Height() > hight;
}

void CDockPanel::SetItemExtend(CTabPanelWnd* pItem, BOOL bEnable)
{
	CTabPanelWnd *pitem;
	if(bEnable)
	{
		pItem->m_bExtend = bEnable;

		CRect  cltRect;
		GetClientRect(&cltRect);
		if(!PlaceEnough(cltRect,m_arrPanel))
		{
			for(int i = 0;i < m_arrPanel.size();i++)
			{
				pitem  = m_arrPanel[i];
				pitem->m_Size = pitem->m_defaultMiniSize;
			
				pitem->m_bExtend = FALSE;
				
				if(PlaceEnough(cltRect,m_arrPanel))
					break;
			}
			pItem->m_bExtend = TRUE;
			}
	}
	else
	{
		//tabpanel不可以全部回收。
		int nExtendItemCount = 0;
		for (int i=0; i< m_arrPanel.size(); i++)
		{
			pitem = m_arrPanel[i];
			if (pitem->m_bExtend == FALSE)
				nExtendItemCount++;
		}
		if (nExtendItemCount < m_arrPanel.size()-1)
			pItem->m_bExtend = bEnable;
	}
	 

	RecalcLayout();
}

void CDockPanel::AddTabPanelItem(CTabPanelWnd  *pItem)
{
    m_arrPanel.push_back(pItem);
}

#define   DEFAULT_HIGH     50
void  CDockPanel::RestoreSpace(CRect  &Rect)
{
	/*CPanelCtrlBar *pBar = (CPanelCtrlBar *) GetOwner();
	if(pBar->IsVertDocked())
		Rect.bottom -= 5;
	else*/
	{
		int storeHigh ;
		int miniHigh = m_arrPanel.size() * m_arrPanel[0]->m_ConstrictSize.cy + 10 ;

		/*if(pBar->m_bPop)
			return ;*/

		/*如果没有可以伸展的空间。。就调整dockbar 的大小*/
		//if(miniHigh  >=  pBar->m_szHorz.cy  
		//	&&HasExtendItem(m_arrPanel))
		//{
		//	storeHigh = m_storeSize.cy;

		//	TRACE("restoreHigh : %d\n",m_storeSize.cy);
		//	if(storeHigh <= 0)
		//		storeHigh = DEFAULT_HIGH;
		//	CSize  newSz ;
		//	newSz.cx = 0;
		//	newSz.cy = storeHigh +  miniHigh;
		//	pBar->SetSize(newSz);

		//	//GetClientRect(&Rect);
		//	Rect.bottom   = newSz.cy -  m_arrPanel[0]->m_ConstrictSize.cy /2 ;	

		//	m_storeSize.cy  = 0;
		//}
		//change dock bar size here!!
	}
}

void CDockPanel::StoreSpace(CRect  &Rect)
{
	//if(Rect.IsRectEmpty())
	//{
	//	//m_storeRect.SetRectEmpty();
	//	return ;
	//}

	//CPanelCtrlBar *pBar = (CPanelCtrlBar *) GetOwner();

	//int miniSize = m_arrPanel.size() * m_arrPanel[0]->m_ConstrictSize.cy  + 10;
	//int storeHigh = pBar->m_szHorz.cy -  miniSize;


	//if(pBar->IsHorzDocked() && storeHigh > 0 ) //top bottom dock，要缩减cotrolbar的高度
	//{
	//	//change deck bar size here
	//	CSize size = pBar->GetSize();
	//	if(!HasExtendItem(m_arrPanel))
	//	{
	//		TRACE("storeHigh  :%d \n",storeHigh);
	//		size.cy  =  miniSize ;
	//		pBar->SetSize(size);
	//		m_storeSize.cy = storeHigh;
	//	}
	//}
}

/*
 对于已经items所需空间超限的情况，采用缩减一部分item的尺寸
 或者改变item的收缩状态来处理
*/
void CDockPanel::MakeChildrenFit(CRect  cltRect)
{

	int			dHigh = 0,wantHigh = 0,usehigh;
	BOOL		bhasExtendItem = FALSE;
	
	CTabPanelWnd		*pWnd;
	//CPanelCtrlBar *pBar = (CPanelCtrlBar *) GetOwner();

	 if(m_arrPanel.size() <= 0)
		return;

	for(int i = 0;i < m_arrPanel.size();i++)//累计空间
	{
		pWnd = m_arrPanel[i];
		if(pWnd->m_bExtend)
		{
			wantHigh += pWnd->m_Size.cy;
			bhasExtendItem = TRUE;
		}
		else
			wantHigh += pWnd->m_ConstrictSize.cy;
	}

	dHigh = cltRect.Height();
	dHigh -= wantHigh;
	
	if(dHigh >= 0) //够用了，就不再计算了。。。。
		return ;

    if(!bhasExtendItem)  //都已经是回收状态了。。返回。。。。。
		return ;
   
	if(m_bOrder == FALSE) 
	for(int i = 0;i < m_arrPanel.size();i++)//顺序默认的减小大小。。。。
	{
		CTabPanelWnd *pWnd = m_arrPanel[i];
		if(pWnd->m_bExtend)
		{
			/*if(pBar->IsVertDocked())
			{
				 usehigh = pWnd->m_Size.cy - pWnd->m_defaultMiniSize.cy;
				if(usehigh + dHigh >= 0)
				{
					pWnd->SetPanelSize(0,usehigh + dHigh +pWnd->m_defaultMiniSize.cy );
					break;
				}else
					dHigh += usehigh;
			}else*/
			{
				usehigh = pWnd->m_Size.cy - pWnd->m_ConstrictSize.cy;
				if(usehigh + dHigh >= 0)
				{
					pWnd->SetPanelSize(0,usehigh + dHigh +pWnd->m_ConstrictSize.cy );
					break;
				}else
				{
					dHigh += usehigh;
					//pWnd->m_bExtend = FALSE;
				}
		
			}
		}
	}
 
	if(m_bOrder == TRUE) //倒序
	{
		int i = m_arrPanel.size();
		i--;
		for(;i >= 0 ;i--)//倒序默认的减小大小。。。。
		{
			CTabPanelWnd *pWnd = m_arrPanel[i];
			if(pWnd->m_bExtend)
			{
				/*if(pBar->IsVertDocked())
				{
					usehigh = pWnd->m_Size.cy - pWnd->m_defaultMiniSize.cy;
					if(usehigh + dHigh >= 0)
					{
						pWnd->SetPanelSize(0,usehigh + dHigh +pWnd->m_defaultMiniSize.cy );
						break;
					}else
						dHigh += usehigh;
				}else*/
				{
					usehigh = pWnd->m_Size.cy - pWnd->m_ConstrictSize.cy;
					if(usehigh + dHigh >= 0)
					{
						pWnd->SetPanelSize(0,usehigh + dHigh +pWnd->m_ConstrictSize.cy );
						break;
					}else
					{
						dHigh += usehigh;
						//pWnd->m_bExtend = FALSE;
					}

				}
			}
		}
	}
}

void  CDockPanel::PlayMessage(UINT msg,WPARAM wparam,LPARAM lparam)
{
		CTabPanelWnd *pWnd;
	
		for(int i = 0;i < m_arrPanel.size();i++)
		{
			pWnd  = m_arrPanel[i];
			pWnd->SendMessage(msg, wparam, lparam);
		}
		
}