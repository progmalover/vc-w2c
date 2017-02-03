// SilibingPanelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SilibingPanelWnd.h"
#include ".\silibingpanelwnd.h"
#include  "DockPanel.h"
/////////////////////////////////////////////////////////////////
//
//zxy 2007.12
//
/////////////////////////////////////////////////////////////////
#define AFX_SILIBING_PANEL   WM_USER + 101
// CTabPanelWnd

#define WM_SIZEPARENT       0x0361 
#define  MMINISIZE_Y   TAB_TITLE_HTIGHT //for title area

#define DEFAULT_MAX_Y  200  //max show area
#define DEFAULT_MAX_X  200

#define DEFAULT_MIN_X	60 // mini show area
#define DEFAULT_MIN_Y   60

IMPLEMENT_DYNAMIC(CTabPanelWnd, CWnd)

CTabPanelWnd::CTabPanelWnd(CWnd *pParent)
{
	m_bShowTitle = TRUE;
	m_pTabPanelTitle = NULL;
   Create(pParent);
}
CTabPanelWnd::CTabPanelWnd()
{
   m_bShowTitle = TRUE;
   m_pTabPanelTitle = NULL;
}

CTabPanelWnd::~CTabPanelWnd()
{
}


BEGIN_MESSAGE_MAP(CTabPanelWnd, CWnd)
	ON_WM_CREATE()
//	ON_WM_SIZE()
	ON_MESSAGE(WM_SIZEPARENT,OnSizeParent)

END_MESSAGE_MAP()


BOOL CTabPanelWnd::Create(CWnd * pParent)
{
	ASSERT(pParent);
	/*WNDCLASS wc;
	wc.lpszMenuName = 0;
	if (!GetClassInfo(AfxGetInstanceHandle(), "#32770", &wc))
	{
		TRACE0(" failed.\n");
		return FALSE;
	
	wc.lpszClassName = "SothinkControlBar";
	wc.hbrBackground = ::GetSysColorBrush(COLOR_3DFACE);
	if (!AfxRegisterClass(&wc))
	{
		TRACE0("AfxRegisterClass failed.\n");
		return FALSE;
	}*/
	LPCSTR strClass = AfxRegisterWndClass(CS_HREDRAW|CS_HREDRAW|CS_DBLCLKS|CS_GLOBALCLASS|CS_CLASSDC,::LoadCursor(0,IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));
     
	BOOL bRet =  __super::Create(strClass,"TabPanel",WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|
				WS_CLIPSIBLINGS,CRect(0,0,0,0),pParent,AFX_SILIBING_PANEL);

	m_bExtend = TRUE;

	m_defaultMiniSize.cx = DEFAULT_MIN_X;
	m_defaultMiniSize.cy = DEFAULT_MIN_Y;
	m_defaultMaxSize.cx = DEFAULT_MAX_X;
	m_defaultMaxSize.cy = DEFAULT_MAX_Y;

	m_Size = m_defaultMiniSize;

	m_ConstrictSize.cx = MMINISIZE_Y;
	m_ConstrictSize.cy = MMINISIZE_Y;
	 
	return bRet;
}

void CTabPanelWnd::ShowTitle(BOOL bShow)
{
	if(m_bShowTitle != bShow)
	{
		m_bShowTitle = bShow;
		RecalcLayout();
	}
}

void CTabPanelWnd:: RecalcLayout(void)
{
	if(m_pTabPanelTitle && ::IsWindow(m_pTabPanelTitle->m_hWnd))
		m_pTabPanelTitle->RecalcLayout();
	if(m_Size.cy <= m_defaultMiniSize.cy)
	{
		m_Size = m_defaultMiniSize;
		//m_bExtend = FALSE;
	}
}

BOOL CTabPanelWnd::CreateTitleWnd(void)
{
	m_pTabPanelTitle = new CTabPanelTitleWnd(this);
	return  m_pTabPanelTitle != NULL;
}

int CTabPanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return    CreateTitleWnd();;
}

void CTabPanelWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PostNcDestroy();
	delete this;
}

void CTabPanelWnd::Constrict(void)
{
	if(!m_bShowTitle)
		return ;
}

void CTabPanelWnd::HideChildrenExcludeTitle(void)
{
	/*CWnd *pChild = GetWindow(GW_CHILD);
	while(pChild)
	{
			if(!pChild->IsKindOf(RUNTIME_CLASS(CTabPanelTitleWnd)))
			 pChild->ShowWindow(SW_HIDE);	
			pChild = pChild->GetWindow(GW_HWNDNEXT);
	}*/
}

void CTabPanelWnd::ShowChildren(void)
{

	/*CWnd *pChild = GetWindow(GW_CHILD);
	while(pChild)
	{
		pChild->ShowWindow(SW_SHOW);	
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}*/
}

void CTabPanelWnd::Extend(void)
{
		m_bExtend = TRUE;
}

 LRESULT CTabPanelWnd::OnSizeParent(WPARAM WParam, LPARAM lParam)
{
   
	RecalcLayout();
    return TRUE; 
}

void CTabPanelWnd::SetPanelSize(int cx,int cy)
{
	m_Size.cx = cx;
	m_Size.cy = cy;
}

void CTabPanelWnd::GetClientRectExcludeTitle(CRect &cltRect)
{ 
	GetClientRect(&cltRect);
	if(m_bShowTitle)
	{	  
		CRect titleRect;
		if(m_pTabPanelTitle && ::IsWindow(m_pTabPanelTitle->m_hWnd))
		  m_pTabPanelTitle->GetClientRect(titleRect);
       
		 cltRect.top += titleRect.Height();
	}
}

void CTabPanelWnd::SetTitleText(LPCTSTR strName)
{
	ASSERT(strName && strName[0]);
	if(m_pTabPanelTitle && ::IsWindow(m_pTabPanelTitle->m_hWnd))
		m_pTabPanelTitle-> SetWindowText(strName);
}
 
void CTabPanelWnd::SetMaxDefaultSize(CSize maxSize)
{
   m_defaultMaxSize = maxSize;
   
	if(m_defaultMaxSize.cx <= DEFAULT_MIN_X && m_defaultMaxSize.cy <= DEFAULT_MIN_Y)
	{
		m_defaultMaxSize.cx = DEFAULT_MAX_X;
		m_defaultMaxSize.cy = DEFAULT_MAX_Y + MMINISIZE_Y;
	}
}

void CTabPanelWnd::SetMiniDefaultSize(CSize miniSize)
{
	m_defaultMiniSize = miniSize;
	if(m_defaultMiniSize.cx <=MMINISIZE_Y && m_defaultMiniSize.cy<= MMINISIZE_Y)
	{
		m_defaultMiniSize.cx = DEFAULT_MIN_X;
		m_defaultMiniSize.cy = DEFAULT_MIN_Y + MMINISIZE_Y;
	}
}

CTabPanelWnd * CTabPanelWnd::GetLastBuddy(void)
{
	CDockPanel  *pPanel = (CDockPanel *)GetOwner();
	if(pPanel->m_arrPanel.size() <= 1)
		return NULL;
    if(pPanel->m_arrPanel[0] == this)
		return NULL;
	for(int i = 0;i < pPanel->m_arrPanel.size();i++)
	{
		if(pPanel->m_arrPanel[i] == this)
			return  pPanel->m_arrPanel[--i];
	}
	return NULL;
}

void CTabPanelWnd::SaveState(LPCTSTR lpszProfileName,int nIndex)
{
	ASSERT_VALID(this);
	ASSERT(GetSafeHwnd());

	CWinApp* pApp = AfxGetApp();

	TCHAR szSection[256];
	wsprintf(szSection, _T("%s-SCTabPanel-%d"), lpszProfileName,
		nIndex);
	AfxGetApp()->WriteProfileInt(szSection,_T("SizeCx"),m_Size.cx);
	AfxGetApp()->WriteProfileInt(szSection,_T("SizeCy"),m_Size.cy);
	AfxGetApp()->WriteProfileInt(szSection,_T("Extend"),m_bExtend);
}

void CTabPanelWnd::LoadState(LPCTSTR lpszProfileName,int nIndex)
{
	ASSERT_VALID(this);
	ASSERT(GetSafeHwnd()); // must be called after Create()

	CWinApp* pApp = AfxGetApp();

	TCHAR szSection[256];
	wsprintf(szSection, _T("%s-SCTabPanel-%d"), lpszProfileName,
		nIndex);
	m_Size.cx = AfxGetApp()->GetProfileInt(szSection,_T("SizeCx"),DEFAULT_MIN_X);
	m_Size.cy = AfxGetApp()->GetProfileInt(szSection,_T("SizeCy"),DEFAULT_MIN_Y);
	m_bExtend = AfxGetApp()->GetProfileInt(szSection,_T("Extend"),1);

}

