// DownTaskList.cpp : 实现文件
//

#include "stdafx.h"
//#include "web2chm.h"
#include "DownTaskList.h"
#include ".\downtasklist.h"

//#include "Defs.h"
//#include "HttpApi.h"
#include "TWebTask.h"
// CDownTaskList

IMPLEMENT_DYNAMIC(CDownTaskList, CListCtrl)
CDownTaskList::CDownTaskList()
{
}

CDownTaskList::~CDownTaskList()
{
}


BEGIN_MESSAGE_MAP(CDownTaskList, CListCtrl)
 
//	ON_WM_DROPFILES()
ON_WM_LBUTTONDBLCLK()
ON_MESSAGE(WM_DOWNPROGRESS,OnDownProcess)
END_MESSAGE_MAP()



HRESULT CDownTaskList::OnDownProcess(WPARAM wparam ,LPARAM lparam)
{
	//TRACE("hello this is list !\n");
	TWebTask  *tsk =  (TWebTask *)wparam;
//	TASK_DOWN_STATUS &status = tsk->m_TaskDownStatus;

	int nItem  = FindInItems(tsk);

	//if(status.bDownSuccess)
	//	SetItemData(nItem,NULL);
	

    if( nItem )
	{

		
	}
	else
	{
			
	
	}
		 
	 
	return S_OK;
}

void CDownTaskList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnLButtonDblClk(nFlags, point);
    PostMessage(WM_DOWNPROGRESS,0,0);
	//AfxMessageBox("hello this is list !");
}

int CDownTaskList::FindInItems(_URL_TASK  *pTask)
{
	int nCount = GetItemCount();
	--nCount;
	for(int i = nCount;i >= 0;i--)
	{
		DWORD_PTR  dwPtr = GetItemData(i);
		if((DWORD)dwPtr == (DWORD)pTask)
		  return i;
	}
	return -1;
}
