#include "StdAfx.h"
#include ".\defnamemanager.h"


CCriticalSection CDefnameManager:: m_Critical;
CDefnameManager::CDefnameManager(void)
{

}

CDefnameManager::~CDefnameManager(void)
{

}

CString CDefnameManager::GetDefName(URL_STRING url)
{

	 m_Critical.Lock();
	 STR_MAP::iterator it = m_name_Map.find(url);
	 if(it != m_name_Map.end())
	 {
		 m_Critical.Unlock();
		 return (*it).second;
	 }
	 CString tmpName;
	 tmpName.Format("default%d.html",m_name_Map.size());

	 m_name_Map.insert(make_pair(url,tmpName));
	 m_Critical.Unlock();
	 return tmpName;
}