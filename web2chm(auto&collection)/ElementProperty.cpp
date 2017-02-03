// ElementProperthy.cpp : implementation file
//

#include "stdafx.h"
//#include "web2chm.h"
#include "ElementProperty.h"
#include ".\elementproperty.h"

#include "ElementProperSheet.h"
// CElementProperty dialog

IMPLEMENT_DYNAMIC(CElementProperty, CPropertyPage)
CElementProperty::CElementProperty()
	: CPropertyPage(CElementProperty::IDD)
{
}

CElementProperty::~CElementProperty()
{
}

void CElementProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RAW_DATA_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CElementProperty, CPropertyPage)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CElementProperty message handlers

void CElementProperty::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	 
	BSTR  bstr;
	CString astr;
	CElementPropertySheet::m_pCurElement->get_tagName(&bstr);
	astr = bstr;
	astr.Insert(0,"\"");
	astr += "\"";
	SetDlgItemText(IDC_TAG_TYPE,astr);
	
	long left,top,width,height;
	CElementPropertySheet::m_pCurElement->get_offsetLeft(&left);
	CElementPropertySheet::m_pCurElement->get_offsetTop(&top);
	CElementPropertySheet::m_pCurElement->get_offsetWidth(&width);
	CElementPropertySheet::m_pCurElement->get_offsetHeight(&height);
	
	astr.Format("(%d,%d)-(%d,%d),%dx%d",left,top,left + width,top + height,width,height);
	SetDlgItemText(IDC_RECTANGLE,astr);

	CElementPropertySheet::m_pCurElement->get_innerHTML(&bstr);
	astr = bstr;

	m_List.SetWindowText(astr);
	//SetDlgItemText(IDC_RAW_DATA_LIST,astr);

}
