#include "StdAfx.h"
#include ".\refpathcalculator.h"
#include "defnamemanager.h"
#include "HttpApi.h"
CRefPathCalculator::CRefPathCalculator(LPCSTR lpRoot,LPCSTR lpDesk)
{
	ASSERT(lpRoot != NULL);
	ASSERT(lpDesk != NULL);

	m_BasePath = lpRoot;
	m_RefPath = lpDesk;
	if(m_BasePath[m_BasePath.GetLength() - 1] == '/')
	{
		//TWebSession  *pCurSession = GetCurrentSession();
		//ASSERT(pCurSession);
		m_BasePath += CDefnameManager::Instance()->GetDefName(m_BasePath); //GetCurrentSession()->GetDefName(m_BasePath);
	}
	if(m_RefPath[m_RefPath.GetLength() - 1] == '/')
	{
		//TWebSession  *pCurSession = GetCurrentSession();
		//ASSERT(pCurSession);
		m_RefPath += CDefnameManager::Instance()->GetDefName(m_RefPath);
	}
}

CRefPathCalculator::~CRefPathCalculator(void)
{
	m_root.clear();
	m_dsk.clear();
}

int  CRefPathCalculator::ParseSectionToList(SECTION_LIST &list,LPSTR lpStr)
{
	STR_SECTION  section;
	int sCount = 0;
	char *p1,*p2;
	p1 = lpStr;
	p2 = lpStr;
	while(*p2)
	{
		if(*p2 == '/')
		{
			section.pStart = p1;
			section.pEnd =   p2;
			section.str.SetString(p1,(int)p2-(int)p1);
			list.push_back(section);
			p2++;
			p1 = p2;
			sCount++;
		}
		p2++;
	}
	section.pStart  = p1;
	section.pEnd = p2;
	section.str.SetString(p1,(int)p2-(int)p1);
	list.push_back(section);
	return ++sCount;
}


void CRefPathCalculator::Exec( CString &tgtStr)
{

	//CString tgtStr;
	m_pcp1 = m_BasePath.GetBuffer();
	m_pcp2 = m_RefPath.GetBuffer();

	m_BasePath.ReleaseBuffer();
	m_RefPath.ReleaseBuffer();

	if(strncmp(m_pcp1,"http://",7) == 0)
		m_pcp1 += 7;
	if(strncmp(m_pcp2,"http://",7) == 0)
		m_pcp2 += 7;

	if(strcmp(m_pcp1,m_pcp2) == 0)
	{
		tgtStr = "#";
		return ;	
	}
	int rootScCount = ParseSectionToList(m_root,m_pcp1);
	int dskScCount  = ParseSectionToList(m_dsk,m_pcp2);

	if(rootScCount > dskScCount)
	{
	
		int nSc = dskScCount;
		int nRoot = rootScCount;
		int i;
		for( i = 0;i < dskScCount;i++)/* 按最短的算*/
		{
			if(m_root[i].str.Compare(m_dsk[i].str) != 0)
			{
				break;
			}
			nSc--;
			nRoot--;
			
		}

		if(nSc == 0)//子路径包含在基路径中
		{
			for(int j = 0;j < nRoot ;j++)
				tgtStr += "../";
			/*STR_SECTION  &strSC1 = m_root[i];
			STR_SECTION  &strSC2 = m_root[m_root.size() - 1];

			CString  strTp;
			strTp.SetString(strSC1.pStart,(int)strSC2.pEnd - (int)strSC1.pStart);*/
			//tgtStr += strTp;
			return ;
		}

		if(nSc == dskScCount)//路径完全不同
		{

			for(int j = 0;j < nRoot -1;j++)
				tgtStr += "../";
			tgtStr += m_pcp2;
			return ;
		}

		if(nSc < dskScCount) // 路径有相同的部分
		{
			for(int j = 0;j < nRoot-1;j++)
				tgtStr += "../";
			STR_SECTION  &strSC1 = m_dsk[i];
			STR_SECTION  &strSC2 = m_dsk[m_dsk.size() - 1];

			CString  strTp;
			strTp.SetString(strSC1.pStart,(int)strSC2.pEnd - (int)strSC1.pStart);
			tgtStr += strTp;
			return ;
		}

	}else
	{
		
		int nSc = dskScCount;
		int nRoot = rootScCount;
		int i;
		for( i = 0;i < rootScCount;i++)
		{
			if(m_root[i].str.Compare(m_dsk[i].str) != 0)
			{
				break;
			}

			
			nSc--;
			nRoot--;
		}

		if(nRoot == 0)//基路径包含在子路径中
		{
			STR_SECTION  &strSC1 = m_dsk[i];
			STR_SECTION  &strSC2 = m_dsk[m_dsk.size() - 1];

			CString  strTp;
			strTp.SetString(strSC1.pStart,(int)strSC2.pEnd - (int)strSC1.pStart);
			tgtStr = "./";
			tgtStr += strTp;
			return ;
		}

		if(nRoot == rootScCount)//路径完全不同
		{
			for(int j = 0;j < nRoot - 1;j++)
				tgtStr += "../";
			tgtStr += m_pcp2;
			return ;
		}
		if(nRoot < rootScCount)// 路径有相同的部分
		{
			for(int j = 0;j < nRoot -1;j++)
				tgtStr += "../";
			if(nRoot -1 <= 0)
				tgtStr += "./";
			STR_SECTION  &strSC1 = m_dsk[i];
			STR_SECTION  &strSC2 = m_dsk[m_dsk.size() - 1];

			CString  strTp;
			strTp.SetString(strSC1.pStart,(int)strSC2.pEnd - (int)strSC1.pStart);
			tgtStr += strTp;
			return ;
		}

	}
	
}

void   CRefPathCalculator::ToSafeWay(CString &tagStr)
{
	
	CString  bufStr;
	LPSTR pBuf = (LPSTR)tagStr.GetBuffer();
	LPSTR pBuf2 = bufStr.GetBufferSetLength(tagStr.GetLength() + 1);

	memset(pBuf2,0,tagStr.GetLength() +1);
	while(pBuf &&  *pBuf)
	{
		switch( *pBuf)
		{
		case '%':
			{
			/*	CString valStr;*/

				pBuf++;
				while(  '0' <= *pBuf && *pBuf <= '9')
					pBuf++;

			}
			break;

		case '&':
			if(strncmp(pBuf,"&amp;",5) == 0)//url =" & " ,url = ... 
			{
				pBuf += 5;
				break;
			}
		case '#':
			{
				pBuf++;
			}
			break;

		case ';':
		case '=':
			{
				*pBuf2  = '_';
				pBuf2++;
				pBuf++;

			}
			break;

		default:
			{
				*pBuf2 = *pBuf;
				pBuf2++;
				pBuf++;
				
			}
			break;

		}
	}

	tagStr.ReleaseBuffer();
	bufStr.ReleaseBufferSetLength(tagStr.GetLength() + 1);

	tagStr = bufStr;
	if(WayIsFileOf(tagStr,".aspx"))
	{
		tagStr.SetString(tagStr,tagStr.ReverseFind('.'));
		tagStr += ".asp ";
	}
	/*if(WayIsFileOf(tagStr,".asp"))
	{
		tagStr.SetString(tagStr,tagStr.ReverseFind('.'));
		tagStr += ".html ";
	}

	if(WayIsFileOf(tagStr,".php"))
	{
		tagStr.SetString(tagStr,tagStr.ReverseFind('.'));
		tagStr += ".html ";
	}*/
	//tagStr.ReleaseBuffer();
}

void CRefPathCalculator::ExecEx(CString &tagStr)
{
	Exec(tagStr);
	ToSafeWay(tagStr);
}