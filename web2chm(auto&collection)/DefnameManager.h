#pragma once
#include <afxmt.h>
#include "defs.h"
#include "staticinstance.h"
#include <map>
using namespace std;
typedef      map<URL_STRING ,CString>  STR_MAP;  
 
class CDefnameManager: public StaticInst<CDefnameManager>
{
public:
	CDefnameManager(void);
	~CDefnameManager(void);
	CString GetDefName(URL_STRING url);
private:
	STR_MAP	m_name_Map; 
	static CCriticalSection    m_Critical;
};
