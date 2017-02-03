#include "StdAfx.h"
#include ".\anchorsbar.h"

BEGIN_MESSAGE_MAP(CAnchorsBar, CAppDockablePane)
	ON_WM_CREATE()
//	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CAnchorsBar, CAppDockablePane)

CAnchorsBar::CAnchorsBar(void)
{
	m_pDockPanel = NULL;
}

CAnchorsBar::~CAnchorsBar(void)
{
}

CDockPanel * CAnchorsBar::GetPanel()
{
	  if(!m_pDockPanel)
		  return NULL;
	  if(::IsWindow(m_pDockPanel->m_hWnd))
		  return m_pDockPanel;

	  return NULL;
}

int CAnchorsBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return OnAddDockPanel();
	 
}

 void CAnchorsBar:: RecalcLayout()
 {
	// __super::RecalcLayout(); 
	 if(m_pDockPanel && ::IsWindow(m_pDockPanel->m_hWnd))
	 {
		 CRect rcRect;
		 GetClientRectExcludeTitle(&rcRect);
		 m_pDockPanel->SetWindowPos(NULL,rcRect.left,rcRect.top,
								rcRect.right,rcRect.bottom,SWP_NOZORDER);
	 }
 }
 BOOL CAnchorsBar::OnAddDockPanel(void)
 {
	 if(!m_pDockPanel)
	     m_pDockPanel = new CDockPanel(this);
	 return TRUE;
 }

 void CAnchorsBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
 {
	 // TODO: Add your message handler code here and/or call default
	 __super::OnNcCalcSize(bCalcValidRects, lpncsp);
	 RecalcLayout();
 }


 void CAnchorsBar::OnSize(UINT nType, int cx, int cy)
 {
	 __super::OnSize(nType, cx, cy);
	 RecalcLayout();
	 // TODO: Add your message handler code here
 }


 BOOL  TransCmdToChilds(CWnd *pWnd,UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
 {
	 
     BOOL bRet = 0;
	 CWnd  * pChild = pWnd->GetWindow(GW_CHILD);
	 if(pChild)
	 {
		 bRet = pChild->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		 if(!bRet)
		 for( CWnd *pSlibing = pChild->GetWindow(GW_HWNDNEXT);pSlibing != NULL; pSlibing = pSlibing->GetWindow(GW_HWNDNEXT))
		 {
          
			 bRet = pSlibing->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			 if(bRet)
				 break;

			 if(pSlibing->GetWindow(GW_CHILD))
                 bRet =  TransCmdToChilds(pSlibing,nID, nCode, pExtra, pHandlerInfo);

			 if(bRet)
				 break;
   
			 
		 } 

	 }
  
	 return bRet;
 }
 BOOL CAnchorsBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
 {
	 // TODO: Add your specialized code here and/or call the base class
	 if(!IsWindow(m_hWnd))
		 return 0;
	

	 BOOL bRet = TransCmdToChilds(this,nID, nCode, pExtra, pHandlerInfo);

	 if(!bRet)
	  bRet =  __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	 return bRet;
 }
