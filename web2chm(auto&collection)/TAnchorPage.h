#pragma once
#include <list>
#include "defs.h"

using namespace std;
class TAnchorPiece;
class TAnchorManager;
class CHtmlParse;
typedef  list<TAnchorPiece *>  PIECES_VECT;

class TAnchorPage
{
	friend class TAnchorManager;
public:
	TAnchorPage(void);
	~TAnchorPage(void);
	void	AddPiece(TAnchorPiece *pPiece);
	CString	SavePage(CString  localName);
public:
	CString		 m_BindPageUrl;
	CString		 m_PageTitle;
	
protected:
	PIECES_VECT  m_piecsVect;

public:
	int GetPieceCount(void);
	void RemovePiece(TAnchorPiece* pPiece);
	BOOL HasThisPiece(TAnchorPiece* pPiece);
	void ModifyPageContent(CHtmlParse &parse,CString &strDir,URL_STRING strRoot);
	BOOL  DoPageCHTFilter(CHtmlParse & parse);
	int HasHtmlBodyAndHead(CString & htmText);
	PIECES_VECT &GetPieces(){return m_piecsVect;};
};
