#include "StdAfx.h"
#include "resource.h"
#include ".\outbar.h"
#include "ConfigManager.h"
CTskPrgList *GP_TskPrgList = NULL;
COutBar::COutBar(void):m_pSysConf(NULL)
{

}

COutBar::~COutBar(void)
{
UGXPThemes::CleanUp();
}

BEGIN_MESSAGE_MAP(COutBar, CAppDockablePane)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
//	ON_WM_CONTEXTMENU()
ON_WM_SIZE()
END_MESSAGE_MAP()

int COutBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	UGXPThemes::UseThemes(true);
	
	m_prgList.CreateGrid(WS_CHILD|WS_VISIBLE|WS_BORDER ,CRect(0,0,0,0),this,IDC_GRID);
	
	::SetWindowLong(m_prgList.m_hWnd,GWL_EXSTYLE,::GetWindowLong(m_prgList.m_hWnd,GWL_EXSTYLE) | WS_EX_CLIENTEDGE);


	m_pSysConf = ConfigManager::Instance()->GetSystemConfig();
	GP_TskPrgList  =&m_prgList;

	if(m_pSysConf)
	{
		GP_TskPrgList->Lock();
		GP_TskPrgList->ClearAllCellInf();
		GP_TskPrgList->SetProgressSells(GA_dwConnectTrdsArray[m_pSysConf->dwThrdNum]);
		GP_TskPrgList->UnLock();
		ResizeContrlBar();
	}
	
	return 0;
}


void COutBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	__super::OnNcCalcSize(bCalcValidRects, lpncsp);

}


void COutBar::ResizeContrlBar(void)
{
	int colNum;
	if(m_pSysConf->dwThrdNum > 1)
		colNum = 1;
	else
		colNum = m_pSysConf->dwThrdNum;
	//SetBarSize(0xefffff,GA_dwConnectTrdsArray[colNum]* 20 + 50);
}



void COutBar::OnSize(UINT nType, int cx, int cy)
{
	CAppDockablePane::OnSize(nType, cx, cy);
		if(::IsWindow(m_prgList.m_hWnd))
	{
		CRect rect0,rect1,rect2;
		GetClientRect(&rect0);

		m_prgList.MoveWindow(&rect0);
		m_prgList.SetColWidth(0,rect0.Width()/3 -3);
		m_prgList.SetColWidth(1,rect0.Width()/3 -2);
		m_prgList.SetColWidth(2,rect0.Width()/3 -1);
	}

	// TODO: Add your message handler code here
}
