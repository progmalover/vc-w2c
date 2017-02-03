#pragma once
#include "StaticInstance.h"
#include <list>

using namespace  ::std;

class SessionInf{
public:
	SessionInf(void)
	{
			m_rootUrl = _T("");
			m_TaskFile = _T("");
			m_TaskName = _T("");
	};
	~SessionInf(void){};
public:
	CString  m_rootUrl;
	CString  m_TaskFile;
	CString  m_TaskName;
//	BOOL operator=(SessionInf )
	BOOL operator==( SessionInf inf)
	{
		if(m_rootUrl.Compare(inf.m_rootUrl) == 0
			&& m_TaskFile.Compare(inf.m_TaskFile) == 0
			&& m_TaskName.Compare(inf.m_TaskName) == 0)
			return  TRUE;
		return FALSE;
	};
};
typedef  list<SessionInf> SESSION_INF_LIST;
class CHistoryList :
	public StaticInst<CHistoryList>
{
public:
	CHistoryList(void);
	~CHistoryList(void);
protected:
	SESSION_INF_LIST  m_infList;
public:
	void DeleteFromList(SessionInf *p);
	void AddToList(SessionInf *p);
	SessionInf * AreadInList(SessionInf *p);
	SESSION_INF_LIST & GetDataList();
	void Load();
	void Save();
};

