// PrimarySheet.cpp : 实现文件
//

#include "stdafx.h"
#include "PrimarySheet.h"


// CPrimarySheet
IMPLEMENT_DYNAMIC(CPrimarySheet, CPropertySheet)
CPrimarySheet::CPrimarySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSH_WIZARD97|PSH_HEADER|PSH_WATERMARK;
	m_psh.pszbmWatermark = MAKEINTRESOURCE(IDB_LOGO);
	m_psh.hInstance = AfxGetInstanceHandle();
	AddPages();
}

CPrimarySheet::CPrimarySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSH_WIZARD97|PSH_HEADER|PSH_WATERMARK;
	m_psh.pszbmWatermark = MAKEINTRESOURCE(IDB_LOGO);
	m_psh.hInstance = AfxGetInstanceHandle();
	AddPages();
}

void  CPrimarySheet::AddPages()
{
	AddPage(&m_NewPage);
	AddPage(&m_Generalpage);
	AddPage(&m_DownPage);
	AddPage(&m_Finishpage);
}

CPrimarySheet::~CPrimarySheet()
{
}


BEGIN_MESSAGE_MAP(CPrimarySheet, CPropertySheet)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CPrimarySheet 消息处理程序

BOOL CPrimarySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return bResult;
}


void CPrimarySheet::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_CLOSE)
	{
		pSYSTEM_CONFIG pSysConfig = ConfigManager::Instance()->GetSystemConfig();
		if(pSysConfig->bAskBeforclose)
		{
			if(AfxMessageBox(IDS_QUITWARN,MB_YESNO|MB_ICONQUESTION)==IDNO)
				return;
		}
		
	}
	CPropertySheet::OnSysCommand(nID, lParam);
}

BOOL CPrimarySheet::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message== WM_KEYDOWN)//按下ESC    
	{
		if(pMsg->wParam==VK_ESCAPE)
		{
			return   TRUE;   
		}
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

BOOL CPrimarySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (CWnd::OnCommand(wParam, lParam))
		return TRUE;

	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

	if (hWndCtrl != NULL && nCode == BN_CLICKED)
	{
		if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0) & (DLGC_BUTTON|DLGC_DEFPUSHBUTTON))
		{
			LONG lStyle = ::GetWindowLong(hWndCtrl, GWL_STYLE) & 0x0F;
			if (lStyle == BS_PUSHBUTTON || lStyle == BS_DEFPUSHBUTTON ||
				lStyle == BS_USERBUTTON || lStyle == BS_OWNERDRAW)
			{
				m_nModalResult = nID;
				if(nID == IDCANCEL)
				{
					pSYSTEM_CONFIG pSysConfig = ConfigManager::Instance()->GetSystemConfig();
					if(pSysConfig->bAskBeforclose)
					{
						if(AfxMessageBox(IDS_QUITWARN,MB_YESNO|MB_ICONQUESTION)==IDNO)
							return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

