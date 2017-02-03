// LookPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "tabdoc.h"
#include "AddPanel.h"
#include ".\addpanel.h"



// CLookPanel
CListBox  *gp_PrepareList;
IMPLEMENT_DYNCREATE(CAddPanel, CFormView)

CAddPanel::CAddPanel()
	: CFormView(1)
{
}

CAddPanel::~CAddPanel()
{
}

void CAddPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_EDIT3, m_AddEdit);
	DDX_Control(pDX, IDC_PREPARELIST, m_PrepareList);
}

BEGIN_MESSAGE_MAP(CAddPanel, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLookPanel 诊断

#ifdef _DEBUG
void CAddPanel::AssertValid() const
{
	CFormView::AssertValid();
}

void CAddPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CLookPanel 消息处理程序

void CAddPanel::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);	
	SIZE size = {cx,cy};
	if(cx >=5 && cy>= 5)
		SetScaleToFitSize(size);

	CRect rect;
	CWnd *pChild = GetWindow(GW_CHILD);
	
	if(!pChild->GetSafeHwnd())return;

	GetClientRect(&rect);
	rect.top += 30;
	pChild->MoveWindow(&rect);
}

BOOL CAddPanel::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= WS_CLIPCHILDREN;
	//cs.style |= WS_CLIPSIBLINGS;

	return CFormView::PreCreateWindow(cs);
}

void CAddPanel::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    static int listPointInit;
	if(listPointInit  == 0)
	{
		gp_PrepareList = &m_PrepareList;
		listPointInit = 1;
	}
	// TODO: 在此添加专用代码和/或调用基类
}
