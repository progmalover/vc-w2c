#pragma once



// CElementPropertySheet
#include "ElementProperty.h"
class CElementPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CElementPropertySheet)

public:
	CElementPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CElementPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
	virtual ~CElementPropertySheet();
private:
	CElementProperty  m_ElementPage;
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	static IHTMLElement* m_pCurElement;
};


