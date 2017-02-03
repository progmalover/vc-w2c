#include "StdAfx.h"
#include ".\historylist.h"
#include "filepath.h"

static LPCTSTR  lpHisFilePath = "\\W2Ebook\\";
static LPCTSTR  lpFileName = "hstRecord.data";
CHistoryList::CHistoryList(void)
{

}

CHistoryList::~CHistoryList(void)
{
	if(m_infList.size())
		m_infList.empty();
}

/*
delete by pointer,or by inf...
*/
void CHistoryList::DeleteFromList(SessionInf *p)
{
	SessionInf *pInf = AreadInList(p);

	if(pInf)
		m_infList.remove(*pInf);
}

void CHistoryList::AddToList(SessionInf *p)
{
	if(AreadInList(p)!= NULL)
	  return;
	m_infList.push_back(*p);
}


/*
±È½Ï¡£¡£¡£¡£
*/
inline SessionInf * CHistoryList::AreadInList(SessionInf *p)
{
	int  iret;
	for(SESSION_INF_LIST::iterator it = m_infList.begin();it!= m_infList.end();it++)
	{
		SessionInf &inf = (*it);
		if(&inf == p)
		   return p;
		iret = inf.m_rootUrl.Compare(p->m_rootUrl);
		if( 0 != iret)
			continue;
		iret = inf.m_TaskFile.Compare(p->m_TaskFile);
		if( 0 != iret)
			continue;
		iret = inf.m_TaskName.Compare(p->m_TaskName);
		if( 0 == iret)
			return  &inf;
	}

	return NULL;
}


/*
load record from .....
*/
void CHistoryList::Load()
{

	CString		strPath ;
	LPSTR  lpStr = strPath.GetBufferSetLength(_MAX_PATH);
	BOOL bGet = ::SHGetSpecialFolderPath(NULL,lpStr,CSIDL_APPDATA,0);
	strPath.ReleaseBuffer();
	if(!bGet)
	{
		strPath = GetModuleFilePath();
	}

	strPath += lpHisFilePath;
	PrepareDirectory(strPath,0);

	strPath += lpFileName ;
	if(::PathFileExists(strPath))
	{
		CStdioFile  hstFile;
		DWORD   dwnum;
		hstFile.Open(strPath,CStdioFile::modeRead);
		hstFile.Read(&dwnum,sizeof(DWORD));
		if(dwnum)
		{

			for( int i = 0;i < dwnum;i++)
			{
				SessionInf   inf;
				hstFile.ReadString(inf.m_rootUrl);
				hstFile.ReadString(inf.m_TaskFile);
				hstFile.ReadString(inf.m_TaskName);
				if(::PathFileExists(inf.m_TaskFile))
				m_infList.push_back(inf);
			}

		}
		hstFile.Close();
	
	}
}

/*
save inf to...
*/
void CHistoryList::Save()
{
	CStdioFile  hstFile;
	DWORD		dwnum;
	CString		strPath ;

	LPSTR  lpStr = strPath.GetBufferSetLength(_MAX_PATH);
	BOOL bGet = ::SHGetSpecialFolderPath(NULL,lpStr,CSIDL_APPDATA,0);
	strPath.ReleaseBuffer();
	if(!bGet)
	{
		strPath = GetModuleFilePath();
	}

	strPath += lpHisFilePath;
	PrepareDirectory(strPath,0);
	strPath += lpFileName ;

	dwnum = m_infList.size();
	if(dwnum)
	{
		hstFile.Open(strPath,CStdioFile::modeCreate| CStdioFile::modeWrite);
		hstFile.Write(&dwnum,sizeof(DWORD));
		for(SESSION_INF_LIST::iterator it = m_infList.begin();it!= m_infList.end();it++)
		{
			SessionInf &inf = (*it);
			hstFile.WriteString(inf.m_rootUrl+ '\n');
			hstFile.WriteString(inf.m_TaskFile + '\n');
			hstFile.WriteString(inf.m_TaskName +'\n' );
		}
		hstFile.Close();
	}else
		DeleteFile(strPath);
	
}

SESSION_INF_LIST & CHistoryList::GetDataList()
{
	return m_infList;
}