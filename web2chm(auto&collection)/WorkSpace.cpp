#include "StdAfx.h"
#include ".\workspace.h"
#include "afxpriv.h"
#include "resource.h"
#include "httpapi.h"
//#include "string"
//using namespace std;
static const int  TOOLBAR_HEIGH = 22;

CTreeCtrl  *gp_SessionTree;
UINT  ID_TABLE[]= {
ID_START_TASK,
ID_STOP_TASK,
ID_KILL_TASK
};
CWorkSpace::CWorkSpace(void)
{
}

CWorkSpace::~CWorkSpace(void)
{

}
BEGIN_MESSAGE_MAP(CWorkSpace, CAppDockablePane)
ON_COMMAND(ID_FILE_NEW, OnFileNew)
ON_WM_CREATE()
ON_WM_SETFOCUS()


ON_NOTIFY(TVN_GETINFOTIP  ,IDC_SESSIONTREE,OnTvToolTip)
ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CWorkSpace, CAppDockablePane)
void CWorkSpace::OnFileNew()
{
	// TODO: Add your command handler code here
}


BOOL CWorkSpace:: Create(CWnd * pParentWnd, UINT nID, LPCTSTR lpszWindowName, CSize sizeDefault, DWORD dwStyle)
{
	// TODO: Add your specialized code here and/or call the base class
	
	int ret = __super ::Create(pParentWnd,lpszWindowName,sizeDefault,dwStyle|WS_CLIPCHILDREN,nID);

	VERIFY(ret != FALSE);
	if(m_TypeTree.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD 
		| TVS_HASBUTTONS  | TVS_HASLINES|TVS_INFOTIP 
		| TVS_DISABLEDRAGDROP,
		CRect(10, 10, 300, 100), this, IDC_SESSIONTREE)!= ret)
	{
			TRACE0("Create Type Tree Failure!\n");
			return -1;
	}
    
  
	InitTaskTree();
    return ret;
}
//进行控键的布局调整
 

int CWorkSpace::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	//m_TskToolDlg.Create(MAKEINTRESOURCE(IDD_DIALOG_PAD),this);
	//m_TskToolDlg.ShowWindow(SW_SHOW);

	return 0;
}

void CWorkSpace::InitTaskTree(void)
{
	TVINSERTSTRUCT tvInsert;
      
	tvInsert.hParent = TVI_ROOT;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT|TVIF_IMAGE;
	tvInsert.item.pszText = _T("Project");
	tvInsert.item.iImage= 0;
	tvInsert.item.iSelectedImage = 0;

	m_TypeTree.InitImageList();

	HTREEITEM hCountry = m_TypeTree.InsertItem(&tvInsert);
	m_TypeTree.Expand(hCountry,TVE_EXPAND);
	
    m_TypeTree.InsertItem(_T("Missions"),  1,  1,hCountry, TVI_LAST);
	m_TypeTree.Expand(hCountry,TVE_EXPAND);
	
	::SetWindowLong(m_TypeTree.m_hWnd,GWL_EXSTYLE,::GetWindowLong(m_TypeTree.m_hWnd,
		GWL_EXSTYLE) |WS_EX_CLIENTEDGE);
	gp_SessionTree = &m_TypeTree;
	/*若选择新建*/
}

void CWorkSpace::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	// TODO: 在此添加专用代码和/或调用基类
	__super::OnUpdateCmdUI(pTarget,bDisableIfNoHandler);
	//m_TskToolDlg.UpdateDialogControls(pTarget,bDisableIfNoHandler);
}

void CWorkSpace::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	m_TypeTree.SetFocus();
	// TODO: 在此处添加消息处理程序代码
}
CString  g_tipString;
void  CWorkSpace:: OnTvToolTip( NMHDR * pNotifyStruct, LRESULT * result )
{
	LPNMTVGETINFOTIP lptvtp = (LPNMTVGETINFOTIP)pNotifyStruct;
	//lptvtp->pszText  = "hello word !!";
	TWebSession *pSession = (TWebSession *)m_TypeTree.GetItemData(lptvtp->hItem);
	if(pSession == NULL)
		return ;

	CString  &taskName = pSession->GetTaskName();
	CString   sName = taskName;
	CString   sRootUrl;

	sRootUrl = pSession->GetRootUrlObj().GetURL()->GetBuffer();

	if(sRootUrl.IsEmpty())
		sRootUrl = "No Root URL";
	char *pName = sName.GetBuffer();
	sName.ReleaseBuffer();
	::PathRemoveExtensionA(pName);
	if(pSession)
	{

		g_tipString.Format(
			"Name:%s\n"
			"URL:%s\n"
			"DownLoad File:%d\n"
			"All Page:%d\n"
		
			"All Source:%d\n"
#ifdef _DEBUG
			"session->m_UrlRefFile.AtEnd():%d\n"
			"session->m_UrlRefSrcFile.AtEnd():%d\n",
#else
			,
#endif
			sName,
			sRootUrl,
			pSession->downFilesNum,
			pSession->m_UrlRefFile.m_iRecordNum,
		
			pSession->m_UrlRefSrcFile.m_iRecordNum
#ifdef _DEBUG
			,
			pSession->m_UrlRefFile.AtEnd(),
			pSession->m_UrlRefSrcFile.AtEnd()
#endif
			);
	

		lptvtp->pszText = g_tipString.GetBuffer();
		g_tipString.ReleaseBuffer();
	}
	
}

void CWorkSpace::OnSize(UINT nType, int cx, int cy)
{
	CAppDockablePane::OnSize(nType, cx, cy);
	if(m_TypeTree.GetSafeHwnd()!=NULL)
	{

	//由 dock的位置定义大小
		CRect rect0,rect1,rect2;
		GetClientRect(&rect0);
          
		m_TypeTree.MoveWindow(rect0);
	}
	// TODO: Add your message handler code here
}
