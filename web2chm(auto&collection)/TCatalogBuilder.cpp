#include "StdAfx.h"
#include ".\tcatalogbuilder.h"
#include "tanchormanager.h"
#include "filepath.h"

URL_LIST TCatalogBuilder::m_pageNameList;
CString	 TCatalogBuilder::m_IndexName;
//BOOL TCatalogBuilder::m_bOK = 0;


void TCatalogBuilder::InitBuilder(void)
{
	m_pageNameList.clear();
	m_IndexName.Empty();
//	m_bOK = 0;
}

HRESULT TCatalogBuilder::BuilderIndex(LPCSTR strDir)
{
	int pageNum = TAnchorManager::Instance()->GetPageCount();
	if(pageNum <= 1)
		return NO_NEED;
	if(pageNum > 1)
	{
		CString  pageContent = TAnchorManager::Instance()->CreateIndexContent();

		//build dir here
		PrepareDirectory(strDir,0);
		//save indexpage to dir here ..
		CString indexName = strDir;


		indexName += "index.html";

		SaveToFile(pageContent,indexName);
		m_IndexName = indexName;
		//m_IndexName = 
	}
//	m_bOK = TRUE;
	return BUILD_SUCCESS;
}

HRESULT TCatalogBuilder::BuilderStartPages(LPCSTR strDir)
{
    int pageNum = TAnchorManager::Instance()->GetPageCount();
	if(pageNum <= 0) return S_FALSE;

	PrepareDirectory(strDir,0);
	
	TAnchorManager::Instance()->CreatePages(strDir);
//	m_bOK = TRUE;
   	return S_OK;
}

HRESULT	 TCatalogBuilder::GetIndexPageName(CString &indexName)
{
	if(m_IndexName.IsEmpty())
		return S_FALSE;

	indexName = m_IndexName;
	return S_OK;
}

HRESULT	 TCatalogBuilder::GetStartPagesName(CString &startPageName)
{
	if(m_pageNameList.size() <= 0)
		return S_FALSE;
	URL_LIST::iterator it = m_pageNameList.begin();
	startPageName = *it;
	return S_OK;
}

URL_LIST &  TCatalogBuilder::GetStartPagesNameList(void)
{
    return m_pageNameList;
}

BOOL TCatalogBuilder::SaveToFile(CString &strContent,CString &strName)
{
	CStdioFile  cfile;
	if(::PathFileExists(strName))
		return FALSE;
	
	BOOL bRet = cfile.Open(strName,CStdioFile::modeCreate|CStdioFile::modeWrite);
	if(bRet == FALSE)
		return bRet;
	cfile.Write(strContent,strContent.GetLength());


	cfile.Close();
	return TRUE;
}