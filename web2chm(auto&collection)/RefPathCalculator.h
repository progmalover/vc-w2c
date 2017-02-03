#pragma once
#include <vector>
typedef struct Section {
char *pStart;
char *pEnd;
CString str;
}STR_SECTION;

using namespace ::std;
typedef vector<STR_SECTION > SECTION_LIST;

class CRefPathCalculator
{
public:
	CRefPathCalculator(LPCSTR lpRoot,LPCSTR lpDesk);
	~CRefPathCalculator(void);
	void	Exec(CString &tagStr);
	void	ExecEx(CString &tagStr);
	
	int		ParseSectionToList(SECTION_LIST &list,LPSTR lpStr);
	static void    ToSafeWay(CString &tagStr);
public:
	CString  m_BasePath;
	CString  m_RefPath;

	char *m_pcp1,*m_pcp2;
	SECTION_LIST  m_root,m_dsk;
};
//BOOL   BufferEqual(LPSTR pbuf,LPSTR p)