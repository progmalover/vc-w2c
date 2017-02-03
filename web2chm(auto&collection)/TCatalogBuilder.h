#pragma once

#define NO_NEED 1
#define BUILD_SUCCESS 2
#define BUILD_FAILURE 3
//this lay relation to session....
#include "defs.h"
//#define list<TAnchorPage *> PAGE_LIST ;
class TCatalogBuilder
{
public:
  static  void			InitBuilder(void);
  static  HRESULT		BuilderIndex(LPCSTR strDir);
  static  HRESULT		BuilderStartPages(LPCSTR strDir);
  static  HRESULT		GetIndexPageName(CString &);
  static  HRESULT		GetStartPagesName(CString &);
  static  URL_LIST     &GetStartPagesNameList(void);
  static  BOOL			SaveToFile(CString &strContent,CString &strName);
//  static  PAGE_LIST    &GetStartPagesList(void);

  static  URL_LIST		m_pageNameList;
  static  CString		m_IndexName;
  //static  BOOL			m_bOK;
};
