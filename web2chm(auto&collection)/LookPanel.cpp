// AddPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "tabdoc.h"
#include "LookPanel.h"
#include ".\lookpanel.h"

CListCtrl * gp_DownTaskList;

// CAddPanel

IMPLEMENT_DYNCREATE(CLookPanel, CFormView)

CLookPanel::CLookPanel()
	: CFormView(0)
{
}

CLookPanel::~CLookPanel()
{
}

void CLookPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CODEVIEW, m_ViewEdit);
	 
}

BEGIN_MESSAGE_MAP(CLookPanel, CFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	 
END_MESSAGE_MAP()


// CAddPanel 诊断

#ifdef _DEBUG
void CLookPanel::AssertValid() const
{
	CFormView::AssertValid();
}

void CLookPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CAddPanel 消息处理程序

void CLookPanel::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	SIZE size = {cx,cy};
	if(cx >=5 && cy>= 5)
		SetScaleToFitSize(size);
	
	
	//GW_HWNDNEXT
	CRect rect;
	CWnd  *pChild = GetWindow(GW_CHILD);
	
	if(!pChild->GetSafeHwnd()|| cx<=6 ||cy<=6)return;

  	//pChild->GetWindowRect(&rect);

	GetClientRect(&rect);
	//rect.DeflateRect(1,1,1,1);
	pChild->MoveWindow(&rect);
	
	
}


 
int CLookPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

   
	return 0;
}

char *gColumnLabel[] = 
{
	"URL",
	"Size",
	"Completed",
	"Percent",
	"Escaped"
};

const int NUM_COLUMNS = 5;
bool CLookPanel::InitHelper(void)
{

	LV_COLUMN  lvc;

	if(!IsWindow(this->m_hWnd)) return false;
	CListCtrl *pList =(CListCtrl *) &m_DownTaskList;//(CListCtrl *) GetDlgItem(IDC_HITITEMS);
	
	if(!pList ) return false;
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList->ModifyStyle(0,WS_CLIPSIBLINGS | WS_CLIPCHILDREN,0);
	for(int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = gColumnLabel[i];
		lvc.cx = (i == 0)?300:80;
		lvc.fmt = LVCFMT_LEFT;
		pList->InsertColumn(i,&lvc);
	}

	return true;
}



void CLookPanel::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    static bool  hitlistinit;
    if(!hitlistinit)
	{
		m_DownTaskList.SubclassDlgItem(IDC_HITITEMS,this);
		gp_DownTaskList = &m_DownTaskList;
		hitlistinit = InitHelper();
	}
}

BOOL CLookPanel::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style |= WS_CLIPCHILDREN;
	cs.style |= WS_CLIPSIBLINGS;
	return CFormView::PreCreateWindow(cs);
}
