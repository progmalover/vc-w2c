// PadFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "mainfrm.h"
#include "tabdoc.h"
#include "PadFrame.h"
#include ".\padframe.h"

#include "LookPanel.h"
#include "AddPanel.h"
#include "TIESinkManager.h"
// CPadFrame
//TWebSession	* CPadFrame::gpCurSession = NULL;
IMPLEMENT_DYNCREATE(CPadFrame, CFrameWnd)

CPadFrame::CPadFrame()
{
}

CPadFrame::~CPadFrame()
{
	//m_dwRef--;
	
}


BEGIN_MESSAGE_MAP(CPadFrame, CFrameWnd)
//	ON_WM_CREATE()
//ON_WM_CREATE()
ON_WM_ERASEBKGND()
ON_WM_SIZE()
ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnDropDown)
ON_COMMAND(ID_FAVORITES_DROPDOWN, DoNothing)
ON_WM_DESTROY()
ON_COMMAND(IDOK, OnNewAddressEnter)
//ON_WM_PAINT()
END_MESSAGE_MAP()


// CPadFrame 消息处理程序
	 
BOOL CPadFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

  
	if(!CreateIEBar())
		return FALSE;

	 
	if(CMfcieView::Instance()->Create(this))
	{
		SetActiveView(CMfcieView::Instance());
		CMfcieView::Instance()->SetOwner(AfxGetApp()->GetMainWnd());
	}
#if 0
	// set up Favorites menu
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD           dwSize;
	LONG		lRet;

	m_FavMenu.CreateMenu();
	// find out from the registry where the favorites are located.
	if(RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
	{
		TRACE0("Favorites folder not found\n");
		return 0;
	}

	// Guarantee NULL-termination
	sz[sizeof(sz)-1] = '\0';
	dwSize = sizeof(sz);
	lRet = RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
	RegCloseKey(hKey);
	if (lRet != ERROR_SUCCESS || sz[sizeof(sz)-1] != '\0')
	{
		TRACE0("Favorites folder not found\n");
		return 0;
	}

	ExpandEnvironmentStrings(sz, szPath, sizeof(szPath));
	BuildFavMenu(szPath, 0, &m_FavMenu);
#endif
	return TRUE;
}


BOOL CPadFrame::OnEraseBkgnd(CDC* pDC)
{
	
	return TRUE;//CFrameWnd::O
}

BOOL CPadFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style = 0;
	cs.style  |= WS_CLIPCHILDREN;
	cs.style  |= WS_CLIPSIBLINGS;
	cs.style  |= WS_CHILD;
	cs.style  &= (~WS_BORDER);
	cs.style  &= (~WS_CAPTION);
	cs.style  |= WS_VISIBLE;
	cs.dwExStyle |= WS_EX_TRANSPARENT;
	//cs.style  &= ~WS
	cs.lpszClass = AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION));
;
	return TRUE;
	//return CFrameWnd::PreCreateWindow(cs);
}

void CPadFrame::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CFrameWnd::PostNcDestroy();
	
	CSimIEBar::ReleaseInstance();
	TIESinkManager::ReleaseSinkInstance();
	CMfcieView::ReleaseInstance();
}

bool CPadFrame::CreateIEBar(void)
{
	return  CSimIEBar::Instance()->Create(this);
}

void CPadFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if(!::IsWindow(CMfcieView::Instance()->m_hWnd))
		return;

	CRect cltRect;
	CRect rct;
	CSimIEBar::Instance()->GetClientRect(&rct);
	GetClientRect(&cltRect);

	cltRect.OffsetRect(0,rct.Height() + 2);
	cltRect.DeflateRect(0,0,0,rct.Height()+2);
	CMfcieView::Instance()->MoveWindow(&cltRect,TRUE);
	// TODO: 在此处添加消息处理程序代码
}

void CPadFrame::RecalcLayout(BOOL bNotify)
{
	// TODO: 在此添加专用代码和/或调用基类
	//SendMessage(WM_SIZE,0,0);
	
	CFrameWnd::RecalcLayout(bNotify);
	CRect cltRect;
	CRect rct;
	if(::IsWindow(CSimIEBar::Instance()->GetSafeHwnd()) && ::IsWindow(CMfcieView::Instance()->GetSafeHwnd()))
	{
		CSimIEBar::Instance()->GetClientRect(&rct);
		GetClientRect(&cltRect);

		cltRect.OffsetRect(0,rct.Height() + 2);
		cltRect.DeflateRect(0,0,0,rct.Height()+2);
		CMfcieView::Instance()->MoveWindow(&cltRect,TRUE);
		SetActiveView(CMfcieView::Instance());
	}
	
}


 

//BOOL CPadFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	BOOL bRet =  CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
//	return TRUE;
//}

BOOL CPadFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}
void CPadFrame::OnDropDown(NMHDR* pNotifyStruct, LRESULT* pResult)
{

#if 0
	// this function handles the dropdown menus from the toolbar
	NMTOOLBAR* pNMToolBar = (NMTOOLBAR*)pNotifyStruct;
	CRect rect;

	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	m_IEBar.m_wndToolBar.GetToolBarCtrl().GetRect(pNMToolBar->iItem, &rect);
	rect.top = rect.bottom;
	::ClientToScreen(pNMToolBar->hdr.hwndFrom, &rect.TopLeft());
	
	if(pNMToolBar->iItem == ID_FAVORITES_DROPDOWN)
	{
		CMenu* pPopup;

		// for the favorties popup, just steal the menu from the main window
		pPopup = m_FavMenu.GetSubMenu(1);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.left, rect.top + 1, AfxGetMainWnd());
	}
	*pResult = TBDDRET_DEFAULT;
#endif
}
void CPadFrame::DoNothing()
{
	// this is here only so that the toolbar buttons for the dropdown menus
	// will have a callback, and thus will not be disabled.
}
int CPadFrame::BuildFavMenu(LPCTSTR pszPath, int nStartPos , CMenu * pMenu)
{
#if 0
	CString         strPath(pszPath);
	CString         strPath2;
	CString         str;
	WIN32_FIND_DATA wfd;
	HANDLE          h;
	int             nPos = 0;
	int             nEndPos = 0;
	int             nNewEndPos = 0;
	int             nLastDir = 0;
	TCHAR           buf[INTERNET_MAX_PATH_LENGTH];
	CStringArray    astrFavorites;
	CStringArray    astrDirs;
	CMenu*          pSubMenu;

	// make sure there's a trailing backslash
	if(strPath[strPath.GetLength() - 1] != _T('\\'))
		strPath += _T('\\');
	strPath2 = strPath;
	strPath += "*.*";

	// now scan the directory, first for .URL files and then for subdirectories
	// that may also contain .URL files
	h = FindFirstFile(strPath, &wfd);
	if(h != INVALID_HANDLE_VALUE)
	{
		nEndPos = nStartPos;
		do
		{
			if((wfd.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM))==0)
			{
				str = wfd.cFileName;
				if(str.Right(4) == _T(".url"))
				{
					// an .URL file is formatted just like an .INI file, so we can
					// use GetPrivateProfileString() to get the information we want
					DWORD dwSize;
					dwSize = ::GetPrivateProfileString(_T("InternetShortcut"), _T("URL"),
						_T(""), buf, INTERNET_MAX_PATH_LENGTH,
						strPath2 + str);
					if (dwSize < INTERNET_MAX_PATH_LENGTH &&
						buf[dwSize] == '\0')
					{
						str = str.Left(str.GetLength() - 4);

						// scan through the array and perform an insertion sort
						// to make sure the menu ends up in alphabetic order
						for(nPos = nStartPos ; nPos < nEndPos ; ++nPos)
						{
							if(str.CompareNoCase(astrFavorites[nPos]) < 0)
								break;
						}
						astrFavorites.InsertAt(nPos, str);
					//	m_astrFavoriteURLs.InsertAt(nPos, buf);
						++nEndPos;
					}
				}
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);
		// Now add thes/
		// now that we've got all the .URL files, check the subdirectories for more
		nLastDir = 0;
		h = FindFirstFile(strPath, &wfd);
		ASSERT(h != INVALID_HANDLE_VALUE);
		do
		{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// ignore the current and parent directory entries
				if(lstrcmp(wfd.cFileName, _T(".")) == 0 || lstrcmp(wfd.cFileName, _T("..")) == 0)
					continue;

				for(nPos = 0 ; nPos < nLastDir ; ++nPos)
				{
					if(astrDirs[nPos].CompareNoCase(wfd.cFileName) > 0)
						break;
				}
				pSubMenu = new CMenu;
				pSubMenu->CreatePopupMenu();

				// call this function recursively.
				nNewEndPos = BuildFavMenu(strPath2 + wfd.cFileName, nEndPos, pSubMenu);
				if(nNewEndPos != nEndPos)
				{
					// only intert a submenu if there are in fact .URL files in the subdirectory
					nEndPos = nNewEndPos;
					pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT_PTR)pSubMenu->m_hMenu, wfd.cFileName);
					pSubMenu->Detach();
					astrDirs.InsertAt(nPos, wfd.cFileName);
					++nLastDir;
				}
				delete pSubMenu;
			}
		} while(FindNextFile(h, &wfd));
		FindClose(h);
	}
	return nEndPos;
#endif
	return 0;
}

void CPadFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	m_FavMenu.DestroyMenu();
	
	// TODO: 在此处添加消息处理程序代码
}

CWnd* CPadFrame::GetMessageBar()
{
	//return CFrameWnd::GetMessageBar();
  CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
  if(!IsWindow(pMainFrame->m_wndStatusBar.GetSafeHwnd()))
	  return NULL;
  return  &pMainFrame->m_wndStatusBar;
}

void CPadFrame:: OnNewAddressEnter()
{
	CString str;

	CSimIEBar::Instance()->m_wndAddress.GetWindowText(str);//GetLBText(CSimIEBar::Instance()->m_wndAddress.GetCurSel(), str);
	((CMfcieView*)GetActiveView())->Navigate2(str, 0, NULL);
}
