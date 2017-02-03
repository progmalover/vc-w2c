#include "StdAfx.h"
#include "resource.h"
#include ".\tasktree.h"
#include "HttpApi.h"
#include "CProgressList.h"
#include "Defs.h"
#include "configmanager.h"

DWORD GA_dwConnectTrdsArray[CONNECT_TRD_NUMBER] = {3,5,7,10,20};
CTaskTree::CTaskTree(void)
{
}

CTaskTree::~CTaskTree(void)
{
	m_ImageList.DeleteImageList();
}
BEGIN_MESSAGE_MAP(CTaskTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_WM_LBUTTONDOWN()
//	ON_WM_CREATE()
END_MESSAGE_MAP()

void CTaskTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if 0
	if(GetCount() <= 1)
		return ;
	HTREEITEM hCurSel = GetNextItem(TVI_ROOT, TVGN_CARET);

	if(hCurSel)
		GP_CurrentSession  = (TWebSession *)GetItemData(hCurSel); 
	else
		GP_CurrentSession = NULL;
	//zxy ��������˫�����͸�ie��ִ�С�����
#endif
	*pResult = 0;
}

void CTaskTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CTreeCtrl::OnLButtonDown(nFlags, point);	
	if(GetCount() <= 1)
		return ;
	HTREEITEM hCurSel = GetNextItem(TVI_ROOT, TVGN_CARET);
	TWebSession *pSession  = (TWebSession *)GetItemData(hCurSel); 
	if(GP_CurrentSession == pSession)
		return ;
	if(pSession == NULL)
		return ;

	pSYSTEM_CONFIG  pSysCf = ConfigManager::Instance()->GetSystemConfig();
	if(GP_CurrentSession)
	{
		GP_TskPrgList->Lock();
		GP_CurrentSession->UnActiveTaskUI();
		GP_TskPrgList->ClearAllCellInf();
		GP_TskPrgList->SetProgressSells(GA_dwConnectTrdsArray[pSysCf->dwThrdNum]);
		GP_TskPrgList->UnLock();
	}

	GP_CurrentSession = pSession;
	if(GP_CurrentSession)
		GP_CurrentSession->ActiveTaskUI();
}



void CTaskTree::InitImageList(void)
{
		// TODO:  �ڴ������ר�õĴ�������MAKEINTRESOURCE(IDB_TASK_LIST_IMAGE)
	CBitmap  bmp;
	BOOL	bRet;
	bRet = bmp.LoadBitmap(MAKEINTRESOURCE(IDB_TASK_LIST_IMAGE));
	ASSERT(bRet);

	bRet = m_ImageList.Create(16,16,ILC_COLORDDB|ILC_MASK,0,10);
	ASSERT(bRet);

	m_ImageList.Add(&bmp,RGB(255,255,255));
	bmp.Detach();
	ASSERT(bRet == TRUE);
	SetImageList(&m_ImageList,TVSIL_NORMAL);
}
