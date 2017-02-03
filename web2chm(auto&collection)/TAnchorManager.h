#pragma once
#include <map>
#include "staticInstance.h"
#include "defs.h"
class TAnchorPage;
//
using namespace std;
//
//typedef map<HTREEITEM,TAnchorPage *>  PAGE_LIST;
class TCatalogBuilder;
class TAnchorManager:public StaticInst<TAnchorManager>
{
	friend class TCatalogBuilder;
public:
	TAnchorManager(void);
	~TAnchorManager(void);

	TAnchorPage *GetPage(CString &strUrl);
	TAnchorPage *GetPage(HTREEITEM iTreeItem);
	TAnchorPage* AddPage(HTREEITEM tritm,TAnchorPage *pPage);
    HTREEITEM	 GetPageItemHandle(TAnchorPage * pPage);
	UINT		 GetPageCount(void);
	
	void		SavePages(CString localName);
	CString		CreateIndexContent(void);
public:
	void RemovePage(TAnchorPage* pPage);
	void RemovePage(HTREEITEM hItem);
	int  GetUrlsCount(void);
	void GetAllUrls(URL_LIST &list);
	void CreatePages(CString strDir);
	void GetPageUrls(URL_LIST & list);
private:
	typedef struct {
		HTREEITEM  hItem;
		TAnchorPage *pPage;
	}PAGE_ITEM;
	typedef list<PAGE_ITEM> PAGE_LIST;

	PAGE_LIST m_pageList;
	
public:
	void SetPageItemHandle(TAnchorPage *pPage,HTREEITEM hItem);
	CString GetFirstUrl(void);
};
