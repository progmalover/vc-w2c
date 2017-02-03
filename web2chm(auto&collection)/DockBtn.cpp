#include "StdAfx.h"
#include ".\dockbtn.h"
#include "CMYControlBar.h"
CDockBtn::CDockBtn(void)
{
}

CDockBtn::~CDockBtn(void)
{
}
void CDockBtn::DrawStuff(CDC *pDC, const CRect &rc)
{
	HICON hic = GetIcon();
	ASSERT(hic != NULL);

	CMyControlBar  *pBar = (CMyControlBar *)GetParent();
	 
//if(pBar->GetFocusStatus())
	DrawIconEx (pDC->GetSafeHdc(), rc.left-1, rc.top-1,
		hic, rc.Width(), rc.Height(), NULL, (HBRUSH)NULL, DI_NORMAL);

	
}
void CDockBtn::DrawBackground(CDC *pDC, const CRect &rc)
{
	
	CMyControlBar  *pBar = (CMyControlBar *)GetParent();
	COLORREF  faceColor;
	if(pBar->GetFocusStatus())
		faceColor = ::GetSysColor(COLOR_ACTIVECAPTION);
	else
		faceColor = ::GetSysColor(COLOR_BTNFACE);

	pDC->FillSolidRect(&rc,faceColor);
}

CSize CDockBtn::GetStuffSize()
{
	return CSize(13,13);
}

BEGIN_MESSAGE_MAP(CDockBtn, CFlatButton)
//ON_WM_CREATE()
END_MESSAGE_MAP()


