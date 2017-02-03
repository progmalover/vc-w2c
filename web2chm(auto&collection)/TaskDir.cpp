#include "stdafx.h"
#include "TaskDir.h"
#include <wininet.h>
#include <afxinet.h>

#include "TaskDir.h"
#include "httpApi.h"
#include "refpathcalculator.h"
#include "defnamemanager.h"
TaskDirCollection::TaskDirCollection(PATH_STRING &strRoot)
{
	m_RootPath = strRoot;
}
TaskDirCollection::TaskDirCollection(void)
{
	
}

void TaskDirCollection::SetRootPath(PATH_STRING &strRoot)
{
	m_pathList.clear();
	m_RootPath = strRoot;
}


inline BOOL PassFilter(PATH_STRING &strPath)
{
	/*const int filterNum = 7;
	TCHAR  *filterName[]= {
		"com","net","www","cn","org","gov","edu","us","uk"
	};*/
	return strPath.CompareNoCase("www") != 0;

}
inline void ToDosPath(PATH_STRING &strPath)
{
	char *pchar = strPath.GetBuffer(strPath.GetLength());
	while(pchar)
	{
		pchar = (char *)strstr((const char *)pchar,"/");
		if(pchar)
			*pchar = '\\';
	}
	strPath.ReleaseBuffer(strPath.GetLength());
}

/*
由url_str计算本地的路径
与根url对比后生成带缩进的得路径名
*/
int inline CountUrlStrDir(LPCSTR pstr)
{
	URL_STRING str;
	int ncount  =0;
	
	while(*pstr)
	{
		if(*pstr == '/')
			ncount++;
		pstr++;
	}
	return ncount;
}

//many bug here !!!!
//1) same as root url....
//2) same /*/ more than 2....
//3) no same...
void TaskDirCollection::CreateLocalUrlFromUrl(URL_STRING *purlRoot,URL_STRING *purlDesk,URL_STRING &strUrl )
{
	CRefPathCalculator   pathCalculator(*purlRoot,*purlDesk);
	pathCalculator.ExecEx(strUrl);
	
}
#if 0
void TaskDirCollection::CreateLocalUrlFromUrl(URL_STRING *purlRoot,URL_STRING *purlDesk,URL_STRING &strUrl )
{
	int len;
	char *pchs1,*pchs2;

	len = purlRoot->GetLength();
	pchs1 = purlRoot->GetBufferSetLength(len +1);
	pchs1[len] = '\0';

	len = purlDesk->GetLength();
	pchs2 = purlDesk->GetBufferSetLength(len +1);
	pchs2[len] = '\0';

	if(strncmp(pchs1,"http://",7) == 0)
		pchs1 += 7;
	if(strncmp(pchs2,"http://",7) == 0)
		pchs2 += 7;

	CString  str1,str2;
    int count1 = CountUrlStrDir(pchs1);
	int count2 = CountUrlStrDir(pchs2);

	count1 = (count1)?count1:1;
	int i = 0;
	while(1)
	{
		AfxExtractSubString(str1,pchs1,i,'/');
		AfxExtractSubString(str2,pchs2,i,'/');
		if(str1.IsEmpty() || str2.IsEmpty())
			break;
		if(str1.Compare(str2) == 0)
			i++;
		else
			break;
		
	}
	
	if(i >= 2 )
		int dfd = 32;
	int zoomNum ;
	
	if( i ==0 )
		zoomNum = count1;
	else
		zoomNum = count1 - i;//缩紧量。。。也就是产生 “../”得数量。。。
	//得到紧缩开始的字符位置。。。
	char *ptchar = pchs2; 
	char *tp = pchs2;
	for(int j = 0;j < i;j++)
	{
		ptchar = strstr(tp,"\/");
		ptchar++;
		tp++;
	}
	
	if((int)ptchar == 1)
		ptchar = pchs2;

	if(zoomNum == 0)//error
		strUrl = "./";
	else
	for( i = 0;i < zoomNum;i++)
		strUrl += "../";
	strUrl += ptchar;

	if(strUrl[strUrl.GetLength() -1] == '/')
	{
		//TWebSession  *pCurSession = GetCurrentSession();
		//ASSERT(pCurSession);
		strUrl +=  CDefnameManager::GetDefName(strUrl);
	}
	purlRoot->ReleaseBuffer();
	purlDesk->ReleaseBuffer();
}
#endif
#if 0
/*由url_str计算出存储的路径名和文件名*/
void TaskDirCollection::CreatePathFromUrl(URL_STRING *urlStr,URL_STRING &strFile ,BOOL bAddRoot=TRUE)
{
	URL_STRING   strServer;
	URL_STRING   strObj;
	URL_STRING   strUrl;
	DWORD        dwType;
	INTERNET_PORT nPort;

	strUrl = *urlStr;
	AfxParseURL(strUrl,dwType,strServer,strObj,nPort);
	//if(strFile == '\\' || strFile == '/')/*可能有错误这里。。暂且认为文件扩展名==3*/
	{
		//strFile = strServer + ".html";
		_URL_DOMAIN _urlDomain(urlStr);
		
		int nums = _urlDomain.GetDomainNums();
		PATH_STRING strDomain;

		strFile = '\\';
		//strFile += _urlDomain.GetDomainName();
		for(int i = nums-1;i >= 0;i--)
		{
			_urlDomain.GetDomainName(strDomain,i);
				strFile += strDomain;
				if(i > 0)
				strFile += ".";
		}

		int len = strObj.GetLength();
		len = len?len:1;

		ToDosPath(strObj);
		if( strObj.IsEmpty()|| strObj[len -1] == '\\')
		{		
			strFile += strObj;
			//strFile += "default.html";

			//TWebSession  *pCurSession = GetCurrentSession();
			//ASSERT(pCurSession);
			//strFile +=  GetCurrentSession()->GetDefName(*urlStr);
		}
		else
		{
			if(strObj[0] == '\\')
				strFile += strObj;
			else{
				strFile += '\\';
				strFile += strObj;
			}

		}
	}
	if(m_RootPath[m_RootPath.GetLength()-1] == '\\' || 
						m_RootPath[m_RootPath.GetLength()-1] == '/')
		m_RootPath = m_RootPath.Left(m_RootPath.GetLength()-1);

	if(bAddRoot)
	{
		strServer = m_RootPath + strFile;
		strFile = strServer;
	}
	else if(strFile[0] == '\\')
	{
		strServer = strFile.Right(strFile.GetLength()-1);
		strFile = strServer;
	} 
	
}
#endif
/*转换url路径到本地的存储路径*/
PATH_STRING	TaskDirCollection:: UrlToLocalURL(URL_STRING *pstrRoot,URL_STRING *pstrDesk)
{
	URL_STRING strFile;
	
	CreateLocalUrlFromUrl(pstrRoot,pstrDesk,strFile);
	//ToDosPath(strFile);
	return strFile;
}
PATH_STRING	TaskDirCollection:: UrlToLocalPath(URL_STRING *strURL,BOOL bSave,BOOL bAddRoot)
{
    URL_STRING strFile;
    if(m_RootPath.IsEmpty())
		return m_RootPath;

	CreatePathFromUrl(&m_RootPath,strURL,strFile,bAddRoot);
	//ToDosPath(strFile);
	CRefPathCalculator::ToSafeWay(strFile);
	if(bSave)
		m_pathList.push_back(strFile);
	return strFile;
	
}

int	TaskDirCollection::	GetFathNum(void)
{
	return m_pathList.size();
}

//PATH_DIR & TaskDirCollection:: GetDirRoot(void)
//{
//	PATH_DIR Root;
//	return Root;
//}
//
//PATH_DIR & TaskDirCollection:: GetNextPath(PATH_DIR &PathDir)
//{
//	PATH_DIR NextDir;
//	return NextDir;
//}
void CreatePathFromUrl(URL_STRING *dirRoot,URL_STRING *urlStr,URL_STRING &strFile ,BOOL bAddRoot)
{
	URL_STRING   strServer;
	URL_STRING   strObj;
	URL_STRING   strUrl;
	URL_STRING	 strRoot;

	DWORD        dwType;
	INTERNET_PORT nPort;

	strRoot = *dirRoot;
	strUrl = *urlStr;
	AfxParseURL(strUrl,dwType,strServer,strObj,nPort);
	//if(strFile == '\\' || strFile == '/')/*可能有错误这里。。暂且认为文件扩展名==3*/
	{
		//strFile = strServer + ".html";
		_URL_DOMAIN _urlDomain(urlStr);
		
		int nums = _urlDomain.GetDomainNums();
		PATH_STRING strDomain;

		strFile = '\\';
		//strFile += _urlDomain.GetDomainName();
		for(int i = nums-1;i >= 0;i--)
		{
			_urlDomain.GetDomainName(strDomain,i);
				strFile += strDomain;
				if(i > 0)
				strFile += ".";
		}

		int len = strObj.GetLength();
		len = len?len:1;

		ToDosPath(strObj);
		if( strObj.IsEmpty()|| strObj[len -1] == '\\')
		{		
			strFile += strObj;
			//strFile += "default.html";

			//TWebSession  *pCurSession = GetCurrentSession();
			//ASSERT(pCurSession);
			strFile +=  CDefnameManager::Instance()->GetDefName(*urlStr);
		}
		else
		{
			if(strObj[0] == '\\')
				strFile += strObj;
			else{
				strFile += '\\';
				strFile += strObj;
			}

		}
	}
	if(strRoot[strRoot.GetLength()-1] == '\\' || 
						strRoot[strRoot.GetLength()-1] == '/')
		strRoot = strRoot.Left(strRoot.GetLength()-1);

	if(bAddRoot)
	{
		strServer = strRoot + strFile;
		strFile = strServer;
	}
	else if(strFile[0] == '\\')
	{
		strServer = strFile.Right(strFile.GetLength()-1);
		strFile = strServer;
	} 
	
}
