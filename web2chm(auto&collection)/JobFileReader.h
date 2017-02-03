#pragma once
#include "defs.h"
class CJobFileReader
{
public:
	CJobFileReader(void);
	~CJobFileReader(void);
protected:
	
	
public:
	UINT		 m_downloadMode;
	UINT		 m_downStatus;
	int			 m_downLoadPageNum;
	int          m_downLoadResourceNum;
	URL_STRING	 m_rootURL;
	URL_LIST	 m_listPageURL;
	URL_LIST     m_listDowned;
	URL_LIST	 m_listSourceURL;
public:
	BOOL LoadFile(LPCTSTR lpStrFile);

};
