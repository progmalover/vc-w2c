#include "StdAfx.h"
#include ".\tanchormanager.h"
#include "tanchorpage.h"
#include "tanchorpiece.h"
#include "TCatalogBuilder.h"
#include ".\refpathcalculator.h"
#include "httpapi.h"
#include "AnchorTree.h"
TAnchorManager::TAnchorManager(void)
{
}

TAnchorManager::~TAnchorManager(void)
{
	PAGE_LIST::iterator _end =  m_pageList.end();
	for(PAGE_LIST::iterator it = m_pageList.begin();
		it != _end;it++)
	{
		TAnchorPage *pPage = (*it).pPage;
		delete pPage;
	}
}

HTREEITEM	 TAnchorManager::GetPageItemHandle(TAnchorPage * pAnchorPage)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *pPage = (*it).pPage;
		if(  pAnchorPage  == pPage)
			return (*it).hItem;
	}
	return NULL;
}

TAnchorPage *TAnchorManager::GetPage(CString &strUrl)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *pPage = (*it).pPage;
		if(pPage->m_BindPageUrl.Compare(strUrl) == 0)
			return pPage;
	}
	return NULL;
}

TAnchorPage* TAnchorManager::AddPage(HTREEITEM tritm,TAnchorPage *pPage)
{
	
	for(PAGE_LIST::iterator it = m_pageList.begin();it != m_pageList.end();it++)
	{
			TAnchorManager::PAGE_ITEM  &item = *it;
			if(item.hItem == tritm)
				return item.pPage ;
	}

	TAnchorManager::PAGE_ITEM iItem;
	iItem.hItem = tritm;
	iItem.pPage = pPage;
	m_pageList.push_back(iItem);
	return pPage;
}

void TAnchorManager::SavePages(CString  localName)
{
	if(m_pageList.size() <= 0)
		return ;
	
	PAGE_LIST::iterator _end =  m_pageList.end();
	for(PAGE_LIST::iterator it = m_pageList.begin();
		it != _end;it++)
	{
		TAnchorPage *pPage = (*it).pPage;
		pPage->SavePage(localName);
	}
}

CString TAnchorManager::CreateIndexContent(void)
{
	CString  pageTxt;
/*here must modify the url to disk path.....*/
	
	pageTxt = "<html> \n<body>";
	//<DIV class=chapter><A title="第三十章 杀戮音刃（一）" href="./118.html">第三十章 杀戮音刃（一）</A></DIV>

	HTREEITEM iItem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CHILD );
	while(iItem)
	{
		TAnchorPage *pPage = this->GetPage(iItem)  ;
		if(pPage)
		{
			CString strFormat;
			CString strRefName,strFile;
		
			ExtractValidUrl(&pPage->m_BindPageUrl,&strRefName);//for 带参数的url.....
			CreatePathFromUrl(&pPage->m_PageTitle,&strRefName,strFile,0);
			CRefPathCalculator::ToSafeWay(strFile);

			strFile.Insert(0,"./");
			strFormat.Format("<DIV><br><A title=\"%s\" href=\"%s\" target=_top>%s</A></br></DIV>\n",
						pPage->m_PageTitle,strFile,pPage->m_PageTitle);

			pageTxt += strFormat;

		}
		iItem = GO_AnchorTree.GetNextItem(iItem,TVGN_NEXT );
	}

	pageTxt += "</body> \n</html> \0";

	return pageTxt;

}
		
UINT TAnchorManager::GetPageCount(void)
{
	return m_pageList.size();
}
void TAnchorManager::RemovePage(HTREEITEM hItem)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		 
		if( (*it).hItem  == hItem)
		{
			
			delete (*it).pPage;
			m_pageList.erase(it);
			break;
		}
			 
	}

}
void TAnchorManager::RemovePage(TAnchorPage* pPage)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *page = (*it).pPage;
		if(page  == pPage)
		{
			
			delete pPage;
			m_pageList.erase(it);
			break;
		}
			 
	}
	 
}

int TAnchorManager::GetUrlsCount(void)
{
	int nCount = 0;
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *page = (*it).pPage;
		nCount++;
		for( PIECES_VECT::iterator it = page->m_piecsVect.begin();
			it != page->m_piecsVect.end();it++)
		{
			TAnchorPiece *piece = *it;
			nCount += piece->GetCount();	
		}
			 
	}
	return nCount;
}

void TAnchorManager::GetAllUrls(URL_LIST &list)
{
	int nCount = 0;
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *page = (*it).pPage;
	//	list.push_back(page->m_BindPageUrl);//存放当页面的url

		for( PIECES_VECT::iterator it = page->m_piecsVect.begin();
			it != page->m_piecsVect.end();it++)
		{
			TAnchorPiece *piece = *it;
			for(URL_LIST::iterator it = piece->m_urlsList.begin();
				it != piece->m_urlsList.end();it++)
			{
				URL_STRING  str = *it;
				str.Insert(0,1);
				list.push_back(str);
			}
			 
		}
			 
	}
}

void TAnchorManager::CreatePages(CString strDir)
{
	 
	HTREEITEM iItem = GO_AnchorTree.GetNextItem(GO_AnchorTree.m_hroot,TVGN_CHILD );
	while(iItem)
	{
		TAnchorPage *page = this->GetPage(iItem)  ;
		if(page)
		TCatalogBuilder::m_pageNameList.push_back(page->SavePage(strDir));
		iItem = GO_AnchorTree.GetNextItem(iItem,TVGN_NEXT);
	}

}
void TAnchorManager::GetPageUrls(URL_LIST & list)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		TAnchorPage *page = (*it).pPage;
		list.push_back(page->m_BindPageUrl);
	}

}

void TAnchorManager::SetPageItemHandle(TAnchorPage *pPage,HTREEITEM hItem)
{
	if(pPage == NULL) return;
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		if( (*it).pPage  == pPage)
			  (*it).hItem = hItem;	  
	}

}

TAnchorPage * TAnchorManager::GetPage(HTREEITEM iTreeItem)
{
	for( PAGE_LIST::iterator it = m_pageList.begin();
		it != m_pageList.end();it++)
	{
		 
		if( (*it).hItem  == iTreeItem)
			return  (*it).pPage;
		  
	}
	return NULL;
}
CString TAnchorManager::GetFirstUrl(void)
{
	
	if(m_pageList.size() <= 0)
		return CString();

	PAGE_LIST::iterator it = m_pageList.begin();
	TAnchorPage *page = (*it).pPage;
	 
	if(page->m_piecsVect.size() <= 0)
		return CString();

	PIECES_VECT::iterator it2 = page->m_piecsVect.begin();
	TAnchorPiece *piece = *it2;

	if(piece->m_urlsList.size() <= 0)
		return CString();

	URL_LIST::iterator it3 = piece->m_urlsList.begin();
	URL_STRING  str = *it3;
	return str;
			 
}
