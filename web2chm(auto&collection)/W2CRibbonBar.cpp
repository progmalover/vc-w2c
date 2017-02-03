// W2CRibbonBar.cpp : implementation file
//

#include "stdafx.h"
#include "W2CRibbonBar.h"


// CW2CRibbonBar

IMPLEMENT_DYNAMIC(CW2CRibbonBar, CMFCRibbonBar)

CW2CRibbonBar::CW2CRibbonBar()
{

}

CW2CRibbonBar::~CW2CRibbonBar()
{
}


BEGIN_MESSAGE_MAP(CW2CRibbonBar, CMFCRibbonBar)
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


CSize CW2CRibbonBar:: CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize size = __super::CalcFixedLayout(bStretch,bHorz);
	size.cy -= 23;
	return size;
}
void CW2CRibbonBar::OnRButtonUp(UINT nFlags, CPoint point)
{

}
void CW2CRibbonBar::OnPaint()
{
	class CPrvCaptionButton :public CMFCRibbonCaptionButton{
		friend class CW2CRibbonBar;
	};
	class CPrvContextCaption :public CMFCRibbonContextCaption
	{
		friend class CW2CRibbonBar;
	};
	CPaintDC dc(this); // device context for painting

	int i = 0;

	CMemDC memDC(dc, this);
	CDC* pDC = &memDC.GetDC();

	CRect rectClip;
	dc.GetClipBox(rectClip);

	CRgn rgnClip;

	if (!rectClip.IsRectEmpty())
	{
		rgnClip.CreateRectRgnIndirect(rectClip);
		pDC->SelectClipRgn(&rgnClip);
	}

	pDC->SetBkMode(TRANSPARENT);

	CRect rectClient;
	GetClientRect(rectClient);

	OnFillBackground(pDC, rectClient);

	CFont* pOldFont = pDC->SelectObject(GetFont());
	ENSURE(pOldFont != NULL);

	// Draw caption bar:
 
	if (!m_rectCaption.IsRectEmpty())
	{
		CRect rectFill = m_rectCaption;
		rectFill.top = 0;

		if (m_bIsTransparentCaption)
		{
			pDC->FillSolidRect(rectFill, RGB(0, 0, 0));

			CMFCToolBarImages::m_bIsDrawOnGlass = TRUE;
		}

		CMFCVisualManager::GetInstance()->OnDrawRibbonCaption
			(pDC, this, m_rectCaption, m_rectCaptionText);

		for (i = 0; i < AFX_RIBBON_CAPTION_BUTTONS; i++)
		{
			if (!m_CaptionButtons [i].GetRect ().IsRectEmpty ())
			{
				CPrvCaptionButton *pBtn = (CPrvCaptionButton*)&m_CaptionButtons [i];
				pBtn->OnDraw(pDC);
			}
		}

		for (i = 0; i < m_arContextCaptions.GetSize(); i++)
		{
			ASSERT_VALID(m_arContextCaptions [i]);
			CPrvContextCaption * pCaption = (CPrvContextCaption *)m_arContextCaptions [i];
			pCaption->OnDraw(pDC);
		}

		CMFCToolBarImages::m_bIsDrawOnGlass = FALSE;
	}

	if (m_bIsTransparentCaption && m_bQuickAccessToolbarOnTop)
	{
		CMFCToolBarImages::m_bIsDrawOnGlass = TRUE;
	}
	#if 0
	// Draw quick access toolbar:
	COLORREF cltTextOld = (COLORREF)-1;
	COLORREF cltQATText = CMFCVisualManager::GetInstance()->GetRibbonQuickAccessToolBarTextColor();

	if (cltQATText != (COLORREF)-1)
	{
		cltTextOld = pDC->SetTextColor(cltQATText);
	}

	m_QAToolbar.OnDraw(pDC);

	if (cltTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(cltTextOld);
	}
#endif
	CMFCToolBarImages::m_bIsDrawOnGlass = FALSE;

	// Draw active category:
	if (m_pActiveCategory != NULL && m_dwHideFlags == 0)
	{
		ASSERT_VALID(m_pActiveCategory);
		m_pActiveCategory->OnDraw(pDC);
	}
#if 0
	// Draw tabs:
	CRect rectTabs = rectClient;
	rectTabs.top = m_rectCaption.IsRectEmpty() ? rectClient.top : m_rectCaption.bottom;
	rectTabs.bottom = rectTabs.top + m_nTabsHeight;

	COLORREF clrTextTabs = CMFCVisualManager::GetInstance()->OnDrawRibbonTabsFrame(pDC, this, rectTabs);

	for (i = 0; i < m_arCategories.GetSize(); i++)
	{
		CMFCRibbonCategory* pCategory = m_arCategories [i];
		ASSERT_VALID(pCategory);

		if (pCategory->IsVisible())
		{
			pCategory->m_Tab.OnDraw(pDC);
		}
	}

	// Draw elements on right of tabs:
	COLORREF clrTextOld = (COLORREF)-1;
	if (clrTextTabs != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor(clrTextTabs);
	}

	m_TabElements.OnDraw(pDC);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}
#endif
	// Draw main button:
	if (m_pMainButton != NULL)
	{
		ASSERT_VALID(m_pMainButton);

		if (!m_pMainButton->GetRect().IsRectEmpty())
		{
			CMFCVisualManager::GetInstance()->OnDrawRibbonApplicationButton(pDC, m_pMainButton);

			m_pMainButton->OnDraw(pDC);
		}
	}

	pDC->SelectObject(pOldFont);
}
// CW2CRibbonBar message handlers


