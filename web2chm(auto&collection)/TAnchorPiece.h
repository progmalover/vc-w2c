#pragma once
#include "defs.h"
#include "Mshtml.h"
class TAnchorPage ;
class TAnchorManager;
class TAnchorPiece
{
	friend class TAnchorManager;
	friend class TAnchorPage;
public:
	TAnchorPiece(void);
	TAnchorPiece(TAnchorPage  *pRoot);
	~TAnchorPiece(void);
	void  AddToPiece(URL_STRING  &urlStr);
	void  FreeAll(void);
	UINT  GetCount();
public:
	CString   m_rawHtml;
public:
	URL_LIST  m_urlsList;
public:	
	TAnchorPage  *m_rootPage;
	HTREEITEM   m_hitem;
	CString		m_PieceTitle;
	//IHTMLElement  *m_pBindElement;
};
