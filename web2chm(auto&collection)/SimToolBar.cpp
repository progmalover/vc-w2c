// SimToolBar.cpp : 实现文件
//

#include "stdafx.h"
//#include "web2chm.h"
#include "SimToolBar.h"
#include "afxpriv.h"
#include ".\simtoolbar.h"

// CSimToolBar

IMPLEMENT_DYNAMIC(CSimToolBar, CMFCToolBar)
CSimToolBar::CSimToolBar()
{
}

CSimToolBar::~CSimToolBar()
{
}


BEGIN_MESSAGE_MAP(CSimToolBar, CMFCToolBar)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

#define ID_TIMER_WAIT   0xE000  // timer while waiting to show status
#define ID_TIMER_CHECK  0xE001  // timer to check for removal of status
BOOL CSimToolBar::SetStatusText(INT_PTR nHit)
{
#ifdef _WIN64
	// Start by allowing historical MFC 7.0 64 bit overload a chance
	int niHit=nHit>0 ? 1 : -1;
	if(SetStatusText(niHit))
	{
		return TRUE;
	}
#endif

	CWnd* pOwner =AfxGetApp()->GetMainWnd();
#if 0
	AFX_MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();
	if (nHit == -1)
	{
		// handle reset case
		pModuleThreadState->m_pLastStatus = NULL;
		if (m_nStateFlags & statusSet)
		{
			pOwner->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
			m_nStateFlags &= ~statusSet;
			return TRUE;
		}
		KillTimer(ID_TIMER_WAIT);
	}
	else
	{
		// handle setnew case
		if (!(m_nStateFlags & statusSet) || pModuleThreadState->m_nLastStatus != nHit)
		{
			pModuleThreadState->m_pLastStatus = this;
			pOwner->SendMessage(WM_SETMESSAGESTRING, nHit);
			m_nStateFlags |= statusSet;
			ResetTimer(ID_TIMER_CHECK, 200);
			return TRUE;
		}
	}
#endif
	return FALSE;
}

// CSimToolBar 消息处理程序


BOOL CSimToolBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return 1;
	//return CToolBar::OnEraseBkgnd(pDC);
}
