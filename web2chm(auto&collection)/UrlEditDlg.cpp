// UrlEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabdoc.h"
#include "UrlEditDlg.h"
#include ".\urleditdlg.h"
#include "Defs.h"
#include "httpapi.h"
// CUrlEditDlg dialog

IMPLEMENT_DYNAMIC(CUrlEditDlg, CDialog)
CUrlEditDlg::CUrlEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUrlEditDlg::IDD, pParent)
{
	m_pList = NULL;
}

CUrlEditDlg::~CUrlEditDlg()
{
}

void CUrlEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_UrlList);
}


BEGIN_MESSAGE_MAP(CUrlEditDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_URL_SELECT_ALL, OnBnClickedUrlSelectAll)
	ON_BN_CLICKED(IDC_UNDO_SELECT, OnBnClickedUndoSelect)
	ON_BN_CLICKED(IDC_URL_DELETE_ALL, OnBnClickedUrlDeleteAll)
	ON_BN_CLICKED(IDC_URL_DELETE, OnBnClickedUrlDelete)
	//ON_UPDATE_COMMAND_UI(IDC_URL_DELETE,OnUpdateBnDelete)
	ON_BN_CLICKED(IDC_URL_EXIT, OnBnClickedUrlExit)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnLvnItemchangedList2)
END_MESSAGE_MAP()


// CUrlEditDlg message ha0ndlers
#define  BUTTON_HIGH  20
void CUrlEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	Recalculate();
	// TODO: Add your message handler code here
}

#define MARGIN   3
#define V_SPACE  5
#define H_SPACE  10
void CUrlEditDlg::Recalculate(void)
{
	if( m_UrlList.GetSafeHwnd())
	{
		CRect cltRect;
		GetClientRect(&cltRect);
		cltRect.DeflateRect(MARGIN,MARGIN,MARGIN,MARGIN);

		CRect btnRectCheck;
		GetDlgItem(IDC_AUDIT_CANCEL)->GetClientRect(&btnRectCheck);
		int h = btnRectCheck.Height(),w = btnRectCheck.Width();

		CRect  listRect;
		listRect.CopyRect(&cltRect);
		listRect.bottom -= (h+BUTTON_HIGH+2*V_SPACE);
		m_UrlList.MoveWindow(&listRect);
		btnRectCheck.SetRect(listRect.left,listRect.bottom+V_SPACE,listRect.left+w,listRect.bottom+V_SPACE+h);
		GetDlgItem(IDC_AUDIT_CANCEL)->MoveWindow(&btnRectCheck);

		CRect btnRect;
		GetDlgItem(IDC_URL_DELETE)->GetClientRect(&btnRect);
		int  width =btnRect.Width(),heigh= btnRect.Height(),
			 left = listRect.left,right= left+width,
			 top = btnRectCheck.bottom+V_SPACE,bottom = top+heigh;
		btnRect.SetRect(left,top,right,bottom);
		GetDlgItem(IDC_URL_DELETE)->MoveWindow(&btnRect);

		left =  right+H_SPACE;
		right = left+width;
		btnRect.SetRect(left,top,right,bottom);
		GetDlgItem(IDC_URL_DELETE_ALL)->MoveWindow(&btnRect);

		left =  right+H_SPACE;
		right = left+width;
		btnRect.SetRect(left,top,right,bottom);
		GetDlgItem(IDC_URL_SELECT_ALL)->MoveWindow(&btnRect);

		left =  right+H_SPACE;
		right = left+width;
		btnRect.SetRect(left,top,right,bottom);
		GetDlgItem(IDC_UNDO_SELECT)->MoveWindow(&btnRect);

		left =  right+H_SPACE;
		right = left+width;
		btnRect.SetRect(left,top,right,bottom);
		GetDlgItem(IDC_URL_EXIT)->MoveWindow(&btnRect);
	}
}

void CUrlEditDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
   
	//for(int i = 0;i < 100;i++)
	//	m_UrlList.InsertItem(i,"this is an demo");
	Recalculate();
	// TODO: Add your message handler code here
}



BOOL CUrlEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    //m_UrlList.SetExtendedStyle(LVS_EX_CHECKBOXES);
	
	if(m_pList)
	{
		URL_LIST::iterator it;
		int i = 0;
		for( i = 0,it = m_pList->begin();it != m_pList->end();it++,i++)
		{
			_URL  urlObj(*it);
			m_UrlList.InsertString(i,*urlObj.GetURL());
			URL_STRING &str = *it;
			m_UrlList.SetItemData(i,(DWORD_PTR)str.GetBuffer());
			str.ReleaseBuffer();
		}
		CButton *pBtn = (CButton *)GetDlgItem(IDC_AUDIT_CANCEL);
		pPRIVATE_CONFIG pConf = ConfigManager::Instance()->GetDefaultSessionConf();
		pBtn->SetCheck(0);

	}
	else
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_AUDIT_CANCEL);
		pBtn->EnableWindow(FALSE);
	}
	return TRUE;   
	 
}

void CUrlEditDlg::OnBnClickedUrlSelectAll()
{
	// TODO: Add your control notification handler code here
	for(int i = 0;i < m_UrlList.GetCount();i++ )
		m_UrlList.SetCheck(i,1);
}

void CUrlEditDlg::OnBnClickedUndoSelect()
{
	// TODO: Add your control notification handler code here
	for(int i = 0;i < m_UrlList.GetCount();i++ )
		m_UrlList.SetCheck(i,FALSE);
}
void FreeListBoxStrings(CExListBox *pListBox)
{
	int iCount = pListBox->GetCount();
	while( --iCount >= 0)
		pListBox->DeleteString(iCount);
}

void CUrlEditDlg::OnBnClickedUrlDeleteAll()
{
	// TODO: Add your control notification handler code here
	if(m_UrlList.GetCount() <= 0) return;
	m_UrlList.LockWindowUpdate();

	FreeListBoxStrings(&m_UrlList);
	m_pList->clear();
	m_UrlList.UnlockWindowUpdate();

}
void CUrlEditDlg::OnBnClickedUrlDelete()
{
	if(m_UrlList.GetCount() <= 0) return;
	m_UrlList.LockWindowUpdate();

	BOOL bChecked = FALSE;
	for(int i = 0 ;i < m_UrlList.GetCount();i++)
	{
		if(m_UrlList.GetCheck(i))
		{
			
			LPCSTR lp  = (LPCSTR )m_UrlList.GetItemData(i);
			m_pList->remove(lp);
			//m_UrlList.DeleteItem(i);
			//i--;
			bChecked = TRUE;
		}
	}

	if(bChecked == FALSE ) return ;

	FreeListBoxStrings(&m_UrlList);
	for(URL_LIST::iterator it = m_pList->begin();it != m_pList->end();it++)
	{
		int iItem = m_UrlList.GetCount();
		_URL objStr(*it);
		m_UrlList.InsertString(iItem,*objStr.GetURL());
		m_UrlList.SetItemData( iItem,(DWORD_PTR)(*it).GetBuffer());
		(*it).ReleaseBuffer();
	}
	m_UrlList.UnlockWindowUpdate();
	/*CWnd *pWnd = GetDlgItem(IDC_URL_DELETE);
	pWnd->EnableWindow(FALSE);*/

}

void CUrlEditDlg::OnBnClickedUrlExit()
{
	// TODO: Add your control notification handler code here
	if(NULL != m_pList) 
	{
	   CButton *pBtn = (CButton *)GetDlgItem(IDC_AUDIT_CANCEL);
	   pPRIVATE_CONFIG pConf = ConfigManager::Instance()->GetDefaultSessionConf();
	   pConf->bEnableEdit = (pBtn->GetCheck())?FALSE:TRUE;
	}
	__super::OnOK();
}


//void CUrlEditDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//
//	if(m_UrlList.GetCheck(pNMLV->iItem))
//	{
//		
//			CWnd *pWnd = GetDlgItem(IDC_URL_DELETE);
//			pWnd->EnableWindow();
//		
//	}
//	*pResult = 0;
//}
