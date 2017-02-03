#pragma once
#include "Defs.h"
extern  CTreeCtrl	*gp_SessionTree;
extern  CListBox		*gp_PrepareList;
extern  CListCtrl	*gp_DownTaskList;


//extern  CTskPrgList	 *GP_TskPrgList;
/*
valotile
extern *gp_OutpList
extern *gp_OutInfList;
*/
#define  WM_TSESSIONWORKING  WM_USER + 44
namespace ui{

	BOOL AddToSessionTree(TWebSession *pSession);
	BOOL RemoveFromSessionTree(TWebSession *pSession);
	BOOL SendMessageToSessionTree(DWORD dwMsg,LPARAM lParam,WPARAM wParam);
    BOOL ClearSessionTree(void);

	BOOL  AddToPrepareListBack(URL_STRING &urlStr);
	BOOL  AddToPrepareListFront(URL_STRING &urlStr);

	BOOL  ClearPrepareList(void);
	BOOL  AddToPrepareList(URL_LIST &urllist);
	/*加入串名字。返回item*/
	BOOL  AddToPrepareList(URL_STRING *purlStr);
	BOOL  RemoveFromPrepareList(URL_STRING *purlStr);
	
	BOOL  AddToDownTaskList(TWebTask *pTask);
	BOOL  RemoveFromTaskList(TWebTask *pTask);
	BOOL  SendMessageToTaskList(DWORD dwMsg,LPARAM lParam,WPARAM wParam);

	void SendMessageToMainFrame(UINT nMsg,WPARAM wparam,LPARAM lparam);


	void OutputToPanel(CString *csStr,_URL_TASK *pTask);//输出出错信息到面板。。。。
	void FreeAllTask(void);

};