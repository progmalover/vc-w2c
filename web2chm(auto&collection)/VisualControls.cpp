#include "StdAfx.h"
#include "VisualControls.h"

namespace VIRSUAL_CONTROLS{
static CMFCRibbonBaseElementEx *pCurElement;
//VISUAL_LIST  CVisualManager::visualList;
LRESULT CVisualManager:: WndProc(HWND hWnd,UINT message,WPARAM wparam,LPARAM lparam)
{
	if(!pCurElement)
		return S_FALSE;

	BOOL bOnVisualControl = FALSE;
	switch(message)
	{
	//case WM_SIZE:
	//	{
	//	/*	VISUAL_LIST::iterator _it = visualList.begin();
	//		VISUAL_LIST::iterator _end = visualList.end();
	//		for(;_it != _end;_it++)
	//		{
	//			CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)*_it;
	//			p->OnAfterChangeRect(NULL);
	//		} 
	//		return S_FALSE;*/
	//	}
	//	break;
	case WM_SHOWWINDOW:
		{
				VISUAL_LIST::iterator _it = visualList.begin();
				VISUAL_LIST::iterator _end = visualList.end();
				for(;_it != _end;_it++)
				{
					CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)*_it;
					p->OnShow(p->m_bIsVisible);
				}return S_FALSE;
		}
		break;
	case WM_IDLEUPDATECMDUI:
		{
			CPoint pt;
			::GetCursorPos(&pt);
			if(pCurElement->GetParentWnd() == CWnd::WindowFromPoint(pt))
				break ;
			 
		  
			CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)pCurElement; 
			if(p->m_bIsHighlighted == FALSE)
				break;
			p->m_bIsHighlighted = FALSE;

			p->Redraw();
			CWnd *pParent = p->GetParentWnd();
			if(pParent)::ReleaseCapture();
		}
		break;
		
	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC hDC = BeginPaint(hWnd,&paint);
			{
				CDC DC;
				DC.Attach(hDC);
				VISUAL_LIST::iterator _it = visualList.begin();
				VISUAL_LIST::iterator _end = visualList.end();
				for(;_it != _end;_it++)
				{
					CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)*_it;
					p->OnAfterChangeRect(NULL);
					p->OnDraw(&DC);
				}
				DC.ReleaseAttribDC();
			}
			EndPaint(hWnd,&paint);
			return S_FALSE;
			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			CPoint point(GET_X_LPARAM(lparam),GET_Y_LPARAM(lparam));
			CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)pCurElement;
			if(p->m_rect.PtInRect(point))
			{
				bOnVisualControl = TRUE;
				BOOL bChange = p->m_bIsPressed ;
				p->m_bIsPressed = TRUE;
				 
				p->OnLButtonDown(point);
				if(bChange !=  p->m_bIsPressed )
				  p->Redraw();
				CWnd *pParent = p->GetParentWnd();
				if(pParent)
				{
					CWnd *pWnd = pParent->SetCapture();
				}
			}
			
		}
		break;
	case WM_LBUTTONUP:
		{
			CPoint point(GET_X_LPARAM(lparam),GET_Y_LPARAM(lparam));
		
			CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)pCurElement;
			p->m_bIsPressed = FALSE;
			

			if(p->m_rect.PtInRect(point))
			{
				bOnVisualControl = TRUE;
				p->OnLButtonUp(point);
			}
			else
				p->m_bIsHighlighted = FALSE;
			p->Redraw();
			CWnd *pParent = p->GetParentWnd();
			if(pParent)::ReleaseCapture();
				
		}
		break;
	case WM_MOUSEMOVE:
		{
			CDC *pDC = CDC::FromHandle(GetDC(hWnd));
			if(!visualList.empty())
			{
				CPoint point(GET_X_LPARAM(lparam),GET_Y_LPARAM(lparam));
				VISUAL_LIST::iterator _it = visualList.begin();
				VISUAL_LIST::iterator _end = visualList.end();
				CPoint pos;
				::GetCursorPos( &pos);

				CWnd *pcurWnd =  CWnd::WindowFromPoint(pos) ;
				if(pCurElement && (pCurElement->GetParentWnd() != pcurWnd
					&& pCurElement->GetParentWnd() != pcurWnd->GetParent()))
				{
                     
					pCurElement->m_bIsHighlighted = FALSE;
					//pCurElement->m_bIsDisabled = FALSE;
					pCurElement->m_bIsPressed = FALSE;
					pCurElement->OnHighlight(FALSE);
					ReleaseCapture();
				//	pCurElement->Redraw();
				
					break ;
				}

				if(pCurElement && pCurElement->m_bIsPressed)
				{
					pCurElement->OnMouseMove(point);
					BOOL bChange = pCurElement->m_bIsHighlighted ;
					if(pCurElement->m_rect.PtInRect(point))
					{
						pCurElement->OnHighlight(TRUE);
						pCurElement->m_bIsHighlighted = TRUE;
					}
					else
					{
						pCurElement->m_bIsHighlighted = FALSE;
						pCurElement->OnHighlight(FALSE);
					}
					if(bChange != pCurElement->m_bIsHighlighted )
				      pCurElement->Redraw();	 
					break ;
				   
				}

				if(pCurElement && pCurElement->m_rect.PtInRect(point))
				{
					BOOL bChange = 	pCurElement->m_bIsHighlighted;
					pCurElement->OnMouseMove(point);
					
					pCurElement->OnHighlight(TRUE);
					pCurElement->m_bIsHighlighted = (pCurElement->m_bIsDisabled )? FALSE:TRUE;
					if(bChange != 	pCurElement->m_bIsHighlighted)
					pCurElement->Redraw();
					break ;
					 
				}
				for(;_it != _end;_it++)
				{
					CMFCRibbonBaseElementEx *p = (CMFCRibbonBaseElementEx *)*_it;
					
					BOOL bChange = p->m_bIsHighlighted ;
					
					p->OnHighlight(FALSE);
					
					
					if(p->m_rect.PtInRect(point))
					{
						bOnVisualControl = TRUE;
						p->OnMouseMove(point);
						if(p != pCurElement)
							pCurElement = p;
					
						p->m_bIsHighlighted = TRUE;
					}else
						p->m_bIsHighlighted = FALSE;
					if(bChange != p->m_bIsHighlighted)
					{
						p->OnHighlight(p->m_bIsHighlighted);
						p->Redraw();;
					}
				}
			
				 
			}
			//pDC->Detach();
			
		}
		break;
	default:
		return S_FALSE;
	}

	if(bOnVisualControl)
		return S_OK;
	else
		return S_FALSE;
}

void CVisualManager:: AddToManager(CMFCRibbonBaseElement *pElement)
{
	visualList.push_back(pElement);
	pCurElement = (CMFCRibbonBaseElementEx *)pElement;
}

void CVisualManager:: Clear()
{
	VISUAL_LIST::iterator _it = visualList.begin();
	VISUAL_LIST::iterator _end = visualList.end();
	for(;_it != _end;_it++)
	{
		delete *_it;
	}
	visualList.clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CMFCRibbonSliderEx::CMFCRibbonSliderEx(CWnd *pParent,UINT nID, int nWidth ):CMFCRibbonSlider(nID,nWidth)
{
	m_pOwner = pParent;
	this->OnAutoRepeat();
}
CWnd * CMFCRibbonSliderEx::GetParentWnd()const
{
	 CWnd *pWnd = __super::GetParentWnd();
 	 ASSERT(m_pOwner!=NULL);
	 if(!pWnd)
		return m_pOwner;
	 return pWnd;
}

void CMFCRibbonSliderEx::Redraw(BOOL bAll)
{
		if(bAll)
		{
			CRect reDrawRect = m_rect;
			 
			CWnd * pWndParent = this->GetParentWnd();
			ASSERT(pWndParent != NULL);
			pWndParent->RedrawWindow(&reDrawRect);
		}
};

void CMFCRibbonSliderEx::OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	
	__super::OnDraw(pDC);
	
}

void VIRSUAL_CONTROLS::CMFCRibbonSliderEx::SetDisable(bool bDisabled)
{
	this->m_bIsDisabled = bDisabled;
}

void CMFCRibbonSliderEx::MoveControl(CRect rect)
{
	
	if(m_rect != rect)
	{
	
		CWnd *pParentWnd = this->GetParentWnd();
		pParentWnd->InvalidateRect(m_rect);
	}
	m_rect = rect;
	//this->Redraw(1);
}

void CMFCRibbonSliderEx::OnMouseMove(CPoint point)
{
	//ASSERT_VALID(this); 
	
	if(IsDisabled())
		return;
	__super::OnMouseMove(point);
	

	if (m_bIsPressed && !IsDisabled())
	{

		if(this->m_pOwner)
			m_pOwner->SendMessage(WM_COMMAND, m_nID);
	}
}


void CMFCRibbonSliderEx::OnLButtonDown(CPoint point)
{
	if(IsDisabled())
		return;
	__super::OnLButtonDown(point);
}
void CMFCRibbonSliderEx::OnLButtonUp(CPoint point)
{
	ASSERT_VALID(this);

	if(IsDisabled())
		return;

	CMFCRibbonSlider::OnLButtonUp(point);

	if (m_nPressed == m_nHighlighted && !IsDisabled())
	{

		if(this->m_pOwner)
			m_pOwner->SendMessage(WM_COMMAND, m_nID);
	}
	
}

int noldHighlighted  = 0;
void CMFCRibbonSliderEx::Redraw()
{
	ASSERT_VALID(this);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	CWnd* pWndParent = GetParentWnd();

	if (pWndParent && ::IsWindow(pWndParent->m_hWnd) )
	{
		CRect rect = this->m_rectThumb;
	 
		CRgn rgn1,rgn2,rgn3,rgn4,rgn ;
		rgn1.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		rgn2.CreateRectRgn(m_oldThumbRect.left,m_oldThumbRect.top,m_oldThumbRect.right,m_oldThumbRect.bottom);
		rgn.CreateRectRgn(0,0,0,0);
		rgn.CombineRgn(&rgn1,&rgn2,RGN_OR);
		
		if(this->m_bZoomButtons)
		{
			CRect zoomRect;
			zoomRect =m_rectZoomIn ;
			rgn3.CreateRectRgn(zoomRect.left,zoomRect.top,zoomRect.right,zoomRect.bottom);	
			rgn.CombineRgn(&rgn,&rgn3,RGN_OR);

			zoomRect = m_rectZoomOut ;
			rgn4.CreateRectRgn(zoomRect.left,zoomRect.top,zoomRect.right,zoomRect.bottom);
			rgn.CombineRgn(&rgn,&rgn4,RGN_OR);
		}	
		CDC *pDC = pWndParent->GetDC();
		pDC->SelectObject(&rgn);
		 
		pWndParent->SendMessage(WM_ERASEBKGND ,(WPARAM)pDC->GetSafeHdc(),(LPARAM)NULL);
		
		this->OnDraw(pDC);
		pWndParent->ReleaseDC(pDC);
		m_oldThumbRect = m_rectThumb;
		
	}

   noldHighlighted = m_nHighlighted;

}
/////////////////////////////////////////////////////////////////////////////////////////////////application main button
CWnd* CMFCRibbonApplicationButtonEx::GetParentWnd() const
 {
	  CWnd *pWnd = __super::GetParentWnd();
 	 ASSERT(m_pOwner!=NULL);
	 if(!pWnd)
		return m_pOwner;
	 return pWnd;
 }
void CMFCRibbonApplicationButtonEx::OnLButtonDown(CPoint point)
{
	//CMFCRibbonBaseElement::OnLButtonDown(point);
	if (m_bIsPressed && !IsDisabled())
	{

		if(this->m_pOwner)
			m_pOwner->SendMessage(WM_COMMAND, m_nID);
	}
}

 
void DrawImage2( CMFCToolBarImages *pImages,CDC *pDC, CMFCRibbonBaseElement::RibbonImageType imageType,CRect rectImage)
{

	 ASSERT_VALID(pDC);
	pImages->SetTransparentColor(afxGlobalData.clrBtnFace);
	pImages->DrawEx(pDC, rectImage, 0);

}

void CMFCRibbonApplicationButtonEx::OnDraw(CDC *pDC)
{
	
	if (!GetRect().IsRectEmpty())
		{
			CRect rect = m_rect;
		
			if(this->IsDisabled())
			{
				this->m_bIsHighlighted = FALSE;
				this->m_bIsPressed = FALSE;
				CMFCVisualManager::GetInstance()->OnDrawRibbonApplicationButton(pDC, this);
			}
			else
				CMFCVisualManager::GetInstance()->OnDrawRibbonApplicationButton(pDC, this);
			

			CRect rectImage = m_rect;
			CSize  sizeImage = GetImageSize((RibbonImageType)0);
			rectImage.DeflateRect(m_szMargin);

			const RibbonImageType imageType = m_bIsLargeImage ? RibbonImageLarge : RibbonImageSmall;
			if (IsApplicationButton())
			{
			rectImage.left += (rectImage.Width () - sizeImage.cx) / 2;
			rectImage.top  += (rectImage.Height () - sizeImage.cy) / 2;
			}
			else if (m_bIsLargeImage && !m_bTextAlwaysOnRight)
			{
			rectImage.left = rectImage.CenterPoint().x - sizeImage.cx / 2;
			rectImage.top += m_szMargin.cy + 1;
			}
			else
			{
			rectImage.top = rectImage.CenterPoint().y - sizeImage.cy / 2;
			}

			rectImage.right = rectImage.left + sizeImage.cx;
			rectImage.bottom = rectImage.top + sizeImage.cy;
 
			if(this->IsDisabled())
				 DrawImage2( &m_ImagesDisable,pDC, imageType, rectImage);
			else
			{
				 if(m_dwStatus == 1)
					DrawImage(pDC, imageType, rectImage);
				 if(m_dwStatus == 2)
				    DrawImage2( &m_ImagesNext,pDC, imageType, rectImage);
			}
			
		}
}

 void CMFCRibbonApplicationButtonEx::OnMouseMove(CPoint point)
 {
	 ASSERT_VALID(this);

	CMFCRibbonBaseElement::OnMouseMove(point);

	//BOOL bMenuWasHighlighted = m_bIsMenuHighlighted;
	BOOL bCommandWasHighlighted = m_bIsCommandHighlighted;

	//m_bIsMenuHighlighted = m_rectMenu.PtInRect(point);
	m_bIsCommandHighlighted = m_rect.PtInRect(point);

	if (bCommandWasHighlighted != m_bIsCommandHighlighted)
	{
		Redraw();

		if (m_pParentMenu != NULL)
		{
			ASSERT_VALID(m_pParentMenu);
			m_pParentMenu->OnChangeHighlighted(this);
		}
	}
 }

  void  CMFCRibbonApplicationButtonEx::Redraw()
  {
	ASSERT_VALID(this);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	CWnd* pWndParent = GetParentWnd();

	if ( pWndParent && ::IsWindow(pWndParent->m_hWnd))
	{
		pWndParent->RedrawWindow(m_rect);
	}
  }

////////////////////////////////////////////////////////////////////////////////////////////////mfc button
CMFCRibbonButtonEx::CMFCRibbonButtonEx(CWnd *pParent,UINT nID, LPCTSTR lpszText, int nSmallImageIndex  , int nLargeImageIndex , BOOL bAlwaysShowDescription)
:CMFCRibbonButton(nID,lpszText,nSmallImageIndex,nLargeImageIndex,bAlwaysShowDescription)
{
	m_pOwner = pParent;
}

 CMFCRibbonButtonEx::CMFCRibbonButtonEx(CWnd *pParent,UINT nID, LPCTSTR lpszText, HICON hIcon, BOOL bAlwaysShowDescription , HICON hIconSmall, BOOL bAutoDestroyIcon , BOOL bAlphaBlendIcon)
 :CMFCRibbonButton(nID,lpszText,hIcon,bAlwaysShowDescription,hIconSmall,bAutoDestroyIcon,bAlphaBlendIcon)
 {
	m_pOwner = pParent;
 }

 CMFCRibbonButtonEx::CMFCRibbonButtonEx(CWnd *pParent,CRect rect)
 {
		m_pOwner = pParent;
		this->m_rect = rect;
 }

 CWnd* CMFCRibbonButtonEx::GetParentWnd() const
 {
	  CWnd *pWnd = __super::GetParentWnd();
 	 ASSERT(m_pOwner!=NULL);
	 if(!pWnd)
		return m_pOwner;
	 return pWnd;
 }

 void CMFCRibbonButtonEx::OnLButtonDown(CPoint point)
{
	CMFCRibbonBaseElement::OnLButtonDown(point);
	if (m_bIsPressed && !IsDisabled())
	{

		if(this->m_pOwner)
			m_pOwner->SendMessage(WM_COMMAND, m_nID);
	}
}

  

 //////////////////////////////////////////////////////////////////////////////////////////////
 CMFCRibbonCheckBoxEx::CMFCRibbonCheckBoxEx(CWnd *pParent):CMFCRibbonCheckBox()
 {
	 ASSERT(pParent != NULL);
	 m_pOwner = pParent;

 }
 CMFCRibbonCheckBoxEx::CMFCRibbonCheckBoxEx(CWnd  *pParent,UINT nID, LPCTSTR lpszText):CMFCRibbonCheckBox(nID,lpszText)
 {
	 ASSERT(pParent != NULL);
	 m_pOwner = pParent;
 }

 CMFCRibbonCheckBoxEx:: ~CMFCRibbonCheckBoxEx()
 {

 }

 CWnd* CMFCRibbonCheckBoxEx::GetParentWnd() const
 {
	  CWnd *pWnd = __super::GetParentWnd();
 	 ASSERT(m_pOwner!=NULL);
	 if(!pWnd)
		return m_pOwner;
	 return pWnd;
 }

void CMFCRibbonCheckBoxEx::OnLButtonDown(CPoint point)
{
	CMFCRibbonBaseElement::OnLButtonDown(point);
	if (m_bIsPressed && !IsDisabled())
	{

		if(this->m_pOwner)
			m_pOwner->SendMessage(WM_COMMAND, m_nID);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
CMFCRibbonFocusCaption::CMFCRibbonFocusCaption(CWnd *pParent,CRect rect):CMFCRibbonButtonEx(pParent,rect)
{
	m_alignMode = 0;
	m_pHolder = NULL;
	m_bIsDisabled = TRUE;
	m_bEnableMouseTouch = false;
}

CMFCRibbonFocusCaption::CMFCRibbonFocusCaption(CWnd *pParent,UINT nID, LPCTSTR lpszText, int nSmallImageIndex , int nLargeImageIndex , BOOL bAlwaysShowDescription )
:CMFCRibbonButtonEx(pParent,nID,lpszText,nSmallImageIndex,nLargeImageIndex,bAlwaysShowDescription)
{
	m_alignMode = 0;
	m_pHolder = NULL;
	m_bIsDisabled = TRUE;
	m_bEnableMouseTouch = false;
}
CMFCRibbonFocusCaption::CMFCRibbonFocusCaption(CWnd *pParent,UINT nID, LPCTSTR lpszText, HICON hIcon, BOOL bAlwaysShowDescription , HICON hIconSmall , BOOL bAutoDestroyIcon, BOOL bAlphaBlendIcon )
:CMFCRibbonButtonEx(pParent,nID,lpszText,hIcon,bAlwaysShowDescription,hIconSmall,bAutoDestroyIcon,bAlphaBlendIcon)
{
	m_alignMode = 0;
	m_pHolder = NULL;
	m_bIsDisabled = TRUE;
	m_bEnableMouseTouch = false;
}


int CMFCRibbonFocusCaption::DrawRibbonText(CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags, COLORREF clrText)
 {
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (CMFCToolBarImages::m_bIsDrawOnGlass)
	{
		const BOOL bIsZoomed = GetParentRibbonBar()->GetSafeHwnd() != NULL && GetParentRibbonBar()->GetParent()->IsZoomed();

		CMFCVisualManager::GetInstance()->DrawTextOnGlass(pDC, strText, rectText, uiDTFlags, 0, bIsZoomed ? RGB(255, 255, 255) : clrText);

		return pDC->GetTextExtent(strText).cy;
	}

	clrText = afxGlobalData.clrBarText;
	if((m_bIsHighlighted || m_bIsPressed)&&!m_bIsDisabled)
		clrText = afxGlobalData.clrHotLinkVisitedText;
	 
	COLORREF clrTextOld= pDC->SetTextColor(clrText);
	
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)));
	int nRes = pDC->DrawText(strText, rectText, uiDTFlags|DT_CENTER);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}

	return nRes;
 }
///////////////////////////////////////////////////////////////////////////////////////////////CMFCRibbonPlayerSlider
CMFCRibbonPlayerSlider::CMFCRibbonPlayerSlider(CWnd *pParent )
:CMFCRibbonSliderEx(pParent)
{
	m_pCaption = NULL;
	m_bTimeMode = FALSE;
}
	
CMFCRibbonPlayerSlider::CMFCRibbonPlayerSlider(CWnd *pParent,UINT nID, int nWidth )
:CMFCRibbonSliderEx(pParent,nID,nWidth)
{
    m_pCaption = NULL;
	m_bTimeMode = FALSE;
}


static const int nThumbIndex = 0;
static const int nSliderIndex = 1;
static const int nZoomInIndex = 2;
static const int nZoomOutIndex = 3;
void CMFCRibbonPlayerSlider::  OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (m_rect.IsRectEmpty())
	{
		return;
	}


	if(m_pCaption )
	{
		CString posStr;
		CRect clipRect;

		pDC->GetClipBox(&clipRect);

		int iPos = this->GetPos();
		int iRange = this->GetRangeMax();

		//m_bTimeMode = 1;
		if(m_bTimeMode) // display time  ,unit : second.
		{
			//calculate time 
		/*	TCHAR *timeFormat = _T("%d%d:%d%d:%d%d/%d%d:%d%d:%d%d");
			int timeSec1[3],timeSec2[3] ;
			timeSec2[0] =  iRange / 3600;
			timeSec2[1] =  (iRange%3600) / 60;
			timeSec2[2] =  iRange % 60;

			timeSec1[0] =  iPos / 3600;
			timeSec1[1] =  (iPos %3600) / 60;
			timeSec1[2] =  iPos % 60;

			posStr.Format(timeFormat,timeSec1[0]/10,timeSec1[0]%10,timeSec1[1]/10,timeSec1[1]%10,
				timeSec1[2]/10,timeSec1[2]%10,timeSec2[0]/10,timeSec2[0]%10,timeSec2[1]/10,timeSec2[1]%10,
				timeSec2[2]/10,timeSec2[2]%10);*/
		 
		 
			int timeSec1[3],timeSec2[3] ;
			timeSec2[0] =  iRange / 3600;
			timeSec2[1] =  (iRange%3600) / 60;
			timeSec2[2] =  iRange % 60;

			timeSec1[0] =  iPos / 3600;
			timeSec1[1] =  (iPos %3600) / 60;
			timeSec1[2] =  iPos % 60;
			
			CString  csTimeSec;
			CString  strTotalTime;
			CString  strCurTime;

			for(int i = 0;i < 3;i++)
			{
				if(timeSec2[i] || i >= 1) //至少显示2个时间段。
				{
					csTimeSec.Format(_T("%d%d"),timeSec1[i] /10,timeSec1[i]%10); // cur time sec
					strCurTime += csTimeSec;

					csTimeSec.Format(_T("%d%d"),timeSec2[i] /10,timeSec2[i]%10); // total time sec
					strTotalTime += csTimeSec;
					if(i < 2)
					{
						strCurTime += _T(":");
						strTotalTime += _T(":");
					}

				}
			}

            posStr = strCurTime;
			posStr += _T("/");
			posStr += strTotalTime;
		}
		else
		{
			
			posStr.Format(_T("%d/%d"),iPos,iRange);
		}

		m_pCaption->SetText(posStr);
		m_pCaption->OnDraw(pDC,1);
	}

	//draw zoom buttons
	if (m_bZoomButtons)
	{
		// Draw zoom buttons:
		CMFCVisualManager::GetInstance()->OnDrawRibbonSliderZoomButton(pDC, this, m_rectZoomOut, TRUE,
			m_bIsHighlighted && m_nHighlighted == nZoomOutIndex&&!IsDisabled(), m_bIsPressed && m_nPressed == nZoomOutIndex, IsDisabled());
		CMFCVisualManager::GetInstance()->OnDrawRibbonSliderZoomButton(pDC, this, m_rectZoomIn, FALSE,
			m_bIsHighlighted && m_nHighlighted == nZoomInIndex&&!IsDisabled(), m_bIsPressed && m_nPressed == nZoomInIndex, IsDisabled());
	}
	// Draw channel:
	CRect rectChannel = m_rectSlider;
	rectChannel.top = rectChannel.CenterPoint().y - 1;
	rectChannel.bottom = rectChannel.top + 1;

	CMFCVisualManager::GetInstance()->OnDrawRibbonSliderChannel(pDC, this, rectChannel);


	// Draw thumb:
	CMFCVisualManager::GetInstance()->OnDrawRibbonSliderThumb(pDC, this, m_rectThumb,
		m_bIsHighlighted && (m_nHighlighted == nThumbIndex || m_nHighlighted == nSliderIndex), m_bIsPressed && m_nPressed == nThumbIndex&&!IsDisabled(), IsDisabled());
}

void CMFCRibbonPlayerSlider::Redraw(BOOL bAll)
{
		if(bAll)
		{
			CRect reDrawRect = m_rect;
		//	if(m_pCaption)
				//reDrawRect.bottom = m_pCaption->m_rect.bottom;
			CWnd * pWndParent = this->GetParentWnd();
			ASSERT(pWndParent != NULL);
			pWndParent->RedrawWindow(&m_pCaption->m_rect);
			pWndParent->RedrawWindow(&reDrawRect);
			//return;
		}
};

void CMFCRibbonPlayerSlider::SetCaption(CMFCRibbonFocusCaption *pCaption)
{ 
	ASSERT(pCaption);
	m_pCaption = pCaption;
	pCaption->SetHolder((CMFCRibbonBaseElementEx *)this);
}
//CRect oldThumbRect(0,0,0,0);
void CMFCRibbonPlayerSlider:: SetDisable(BOOL bDisable)
{
		this->m_bIsDisabled = bDisable;
		if(m_pCaption) m_pCaption->m_bIsDisabled =  bDisable;
}

void  CMFCRibbonPlayerSlider::EnableTimeMode(BOOL bEnable)
{
	  m_bTimeMode = bEnable ;
	  if(m_pCaption) 
	  {		
		  Redraw(1);
		  Redraw();
	  }
}

//#define mode_one 
//#define mode_two
#define mode_three
void CMFCRibbonPlayerSlider::Redraw()
{
   ASSERT_VALID(this);

	if (m_rect.IsRectEmpty())
	{
		return;
	}

	if(this->m_pCaption)
	{
		m_pCaption->m_bIsPressed = m_bIsPressed;
		m_pCaption->m_bIsHighlighted = (m_bIsPressed)? TRUE:m_bIsHighlighted;
	}

	CWnd * pWndParent =  GetParentWnd();
	ASSERT_VALID(pWndParent);
#ifdef mode_one
	Redraw(1);
	return;
#endif
 
	if (pWndParent && ::IsWindow(pWndParent->m_hWnd) )
	{

		CRgn rgn;
#ifdef  mode_two
		CRect rect = m_rectThumb;
		
		int offx = 0;
		m_oldThumbRect.InflateRect(m_oldThumbRect.Width()*2 ,0);
		if(m_oldThumbRect.left < this->m_rect.left)
			offx  = m_rect.left - m_oldThumbRect.left;
		if(m_oldThumbRect.right > this->m_rect.right)
			offx = this->m_rect.right -  m_oldThumbRect.right;
		m_oldThumbRect.OffsetRect(offx,0);

		/*CRect rct;
		if(rct.IntersectRect(m_oldThumbRect,rect))
		{
			m_oldThumbRect -= rct;
		}*/

		offx = 0;
		rect.InflateRect(rect.Width()*2 ,0);
		if(rect.left < this->m_rect.left)
			offx  = m_rect.left - rect.left;
		if(rect.right > this->m_rect.right)
			offx = this->m_rect.right -  rect.right;
		rect.OffsetRect(offx,0);

		CRgn rgn1,rgn2,rgn3 ;
		rgn1.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		
		rgn3.CreateRectRgn(m_oldThumbRect.left,m_oldThumbRect.top,m_oldThumbRect.right,m_oldThumbRect.bottom);
		rgn.CreateRectRgn(0,0,0,0);
		if(m_pCaption != NULL)
		{
			rgn2.CreateRectRgn(m_pCaption->m_rect.left,m_pCaption->m_rect.top,m_pCaption->m_rect.right,m_pCaption->m_rect.bottom);
			rgn.CombineRgn(&rgn1,&rgn2,RGN_OR);
		}

		rgn.CombineRgn(&rgn,&rgn3,RGN_OR);
		m_oldThumbRect = m_rectThumb;
#endif

#ifdef mode_three
		CRgn  rgn1,rgn2;
		rgn1.CreateRectRgn(m_rect.left,m_rect.top,m_rect.right,m_rect.bottom);
		rgn.CreateRectRgn(0,0,0,0);
		rgn.CombineRgn(&rgn,&rgn1,RGN_OR);
		if(m_pCaption)
		{
			rgn2.CreateRectRgn(m_pCaption->m_rect.left,m_pCaption->m_rect.top,
						m_pCaption->m_rect.right,m_pCaption->m_rect.bottom);
			rgn.CombineRgn(&rgn,&rgn2,RGN_OR);
		}

#endif
		CDC *pDC = pWndParent->GetDC();
		if(!pDC) return ;

		pDC->SelectObject(&rgn);
		 
		pWndParent->SendMessage(WM_ERASEBKGND ,(WPARAM)pDC->GetSafeHdc(),(LPARAM)NULL);
		
		//pWndParent->OnEraseBkgnd(pDC);
		this->OnDraw(pDC);
		pWndParent->ReleaseDC(pDC);
		

	}
//#endif 
}

////////////////////////////////////////////////////////////////////////////////////////////////visual manager end
}
 