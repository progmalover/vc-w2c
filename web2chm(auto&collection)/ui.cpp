#include "stdafx.h"
#include "HttpApi.h"
#include "ui.h"
#include "TWebTask.h"
#include "shlwapi.h"
#include "tabdoc.h"
 
TASK_LIST  GP_SessionList;
namespace ui {
BOOL AddToSessionTree(TWebSession *pSession)
{
	URL_STRING  &str = pSession->GetTaskPathName();
	URL_STRING  sName = PathFindFileName(str.GetBuffer());
    str.ReleaseBuffer(str.GetLength());

	sName = sName.Left( sName.Find(".",0));

	HTREEITEM hNextItem;
	HTREEITEM hRootItem; 
	HTREEITEM hChildItem;

	hRootItem  = gp_SessionTree->GetRootItem();
	hChildItem = gp_SessionTree->GetChildItem(hRootItem);

	while (1)
	{
		hNextItem = gp_SessionTree->GetNextItem(hChildItem, TVGN_NEXT);
		if(hNextItem)
			hChildItem = hNextItem;
		else
			break;
	}
	gp_SessionTree->Expand(hChildItem,TVE_EXPAND);
	hChildItem = gp_SessionTree->InsertItem(sName,3,3,hChildItem,TVI_LAST);
	
	gp_SessionTree->SetItemData(hChildItem,(DWORD_PTR )pSession);
	gp_SessionTree->Expand(hChildItem,TVE_EXPAND);
	GP_SessionList.push_back((_URL_TASK *)pSession);//
	gp_SessionTree->Invalidate();
	return TRUE;
}


void FreeAllTask(void)
{
	DWORD nCount = GP_SessionList.size();
	if(nCount)
	{
		for(TASK_LIST::iterator it = GP_SessionList.begin();
			it != GP_SessionList.end();it++)
		{
			TWebSession *p = (TWebSession *)*it;
			p->Destroy();
			p->UnPrepare();
			delete p;
		}
		GP_SessionList.clear();

	}
}

BOOL RemoveFromSessionTree(TWebSession *pSession)
{
	HTREEITEM hItem ;//= gp_SessionTree->GetRootItem();
    HTREEITEM hChildItem ;// = gp_SessionTree->GetChildItem(hItem);
	TWebSession  *p;

     if(gp_SessionTree->GetCount() <= 0)
		 return FALSE;

	 hItem = gp_SessionTree->GetRootItem();
	 hChildItem  = gp_SessionTree->GetChildItem(hItem);
	 hChildItem  = gp_SessionTree->GetChildItem(hChildItem);
	while( hChildItem)
	{
		p = (TWebSession *)gp_SessionTree->GetItemData(hChildItem);
		if(p == pSession)
		{
			gp_SessionTree->DeleteItem(hChildItem);
			hChildItem = NULL;
			if(p == GP_CurrentSession)
				GP_CurrentSession = NULL;
			//GP_SessionList.remove(p);
			return TRUE;
		}
		hChildItem = gp_SessionTree->GetNextItem(hChildItem,TVGN_NEXT);
	}
	return FALSE;
}


BOOL SendMessageToSessionTree(DWORD dwMsg,LPARAM lParam,WPARAM wParam)
{
	return TRUE;
}
BOOL ClearSessionTree(void)
{
	return TRUE;
}

BOOL  AddToPrepareListBack(URL_STRING &urlStr)
{

	return TRUE;
}

BOOL  AddToPrepareListFront(URL_STRING &urlStr)
{

	return TRUE;
}

BOOL  ClearPrepareList(void)
{

	return TRUE;
}

BOOL  AddToPrepareList(URL_LIST &urllist)
{

	return TRUE;
}
/*加入串名字。返回item*/
BOOL  AddToPrepareList(URL_STRING *purlStr)
{
	
	return TRUE;
}
BOOL  RemoveFromPrepareList(URL_STRING *purlStr)
{
	return TRUE;
}

BOOL  AddToDownTaskList(TWebTask *pTask)
{


	return TRUE;
}
BOOL  RemoveFromTaskList(TWebTask *pTask)
{
	return TRUE;
}
BOOL  SendMessageToTaskList(DWORD dwMsg,LPARAM lParam,WPARAM wParam)
{
	return TRUE;
}

void OutputToPanel(CString *csStr,_URL_TASK *pTask)
{

}

void SendMessageToMainFrame(UINT nMsg,WPARAM wparam,LPARAM lparam)
{
	theApp.GetMainWnd()->SendMessage(nMsg,wparam,lparam);
}
};