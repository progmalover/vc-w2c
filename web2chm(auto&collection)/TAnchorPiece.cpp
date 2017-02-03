#include "StdAfx.h"
#include ".\tanchorpiece.h"

TAnchorPiece::TAnchorPiece(void)
{
}
TAnchorPiece::TAnchorPiece(TAnchorPage  *pRoot)
{
   m_rootPage = pRoot;
}
TAnchorPiece::~TAnchorPiece(void)
{
}
void TAnchorPiece:: AddToPiece(URL_STRING  &urlStr)
{
	m_urlsList.push_back(urlStr);
}

void  TAnchorPiece::FreeAll(void)
{
	m_urlsList.clear();
}

UINT TAnchorPiece:: GetCount()
{
	return (UINT)m_urlsList.size();
}