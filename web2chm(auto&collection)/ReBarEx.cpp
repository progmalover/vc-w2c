// ReBarEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ReBarEx.h"
#include ".\rebarex.h"


// CReBarEx

IMPLEMENT_DYNAMIC(CReBarEx, CMFCReBar)
CReBarEx::CReBarEx()
{
}

CReBarEx::~CReBarEx()
{
}


BEGIN_MESSAGE_MAP(CReBarEx, CMFCReBar)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CReBarEx::DrawGripper(CDC* pDC, const CRect& rect)
{
	
}
// CReBarEx ��Ϣ�������

void CReBarEx::DrawBorders(CDC* pDC, CRect& rect)
{
	pDC->Draw3dRect(rect.left,rect.top,rect.right,rect.bottom,
		::GetSysColor(COLOR_3DLIGHT),::GetSysColor(COLOR_3DSHADOW));
}
void CReBarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
}
