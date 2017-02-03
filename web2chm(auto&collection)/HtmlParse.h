#pragma once
#include <list>
#include <map>
using namespace::std;
class  Tag
{
public:
	Tag(void);
	~Tag(void);
public:
	CString m_strTag;
	CString m_strTagText;
};

class TagEntry:public Tag
{
public:
	TagEntry(void);
	~TagEntry(void);

public:
	//Tag   m_Tag;
	void  *m_startRawPos; //指向原始数据的指针的tag开始部分
	void  *m_endRawPos; // 。。。tag的结束部分 
	BOOL  m_bUpdata;
	void  ModifyText(CString & txt);
public:

	void operator=(TagEntry  &tagEntry)
	{
		m_bUpdata = tagEntry.m_bUpdata;
		m_strTag = tagEntry.m_strTag;
		m_strTagText = tagEntry.m_strTagText;
		m_startRawPos = tagEntry.m_startRawPos;
		m_endRawPos = tagEntry.m_endRawPos;
	};

	void ModifyText(LPCSTR lpTxt);
};

typedef list<TagEntry>  ENTRY_LIST;
typedef map<CString,   ENTRY_LIST *> TAGS_MAP;

class CHtmlParse
{
public:
	CHtmlParse(void);
	~CHtmlParse(void);
protected:
	CString   m_rawData;

	class  CHtmlFind
	{
	public :
		CHtmlFind(void){ m_rawlpc = NULL;};
		CHtmlFind(CHtmlParse *pParse)
		{
			  Attach(pParse);
		};
		~CHtmlFind(void){};
		void Attach(CHtmlParse *pParse)
		{
				m_rawlpc = pParse->m_rawData.GetBuffer();
				pParse->m_rawData.ReleaseBuffer();

				m_rawbeginc = m_rawlpc;
				m_rawendc =  LPCSTR(m_rawlpc + pParse->m_FileLen);
				
				m_pParser = pParse;
				ASSERT(m_rawlpc != NULL);
				ASSERT(m_rawbeginc != m_rawendc);
		};
	public:
		BOOL			   GetRootTitle(CString &strTitle ,int &nlen);
		BOOL			   FindRootProper(LPCSTR properName,TagEntry &tagEntry);
		BOOL			   FindNextProper(TagEntry &tagEntry);
		

		BOOL			   FindRootRange(CString strName,TagEntry &tagEntry);
		BOOL			   FindNextRange(TagEntry &tagEntry);

		LPCSTR		   FindNextRAB(LPCSTR  lpc);
		LPCSTR		   FindPreRAB(LPCSTR  lpc);
		LPCSTR		   FindNextLAB(LPCSTR  lpc);
		LPCSTR		   FindPreLAB(LPCSTR  lpc);
		LPCSTR		   FindLRAB(LPCSTR lpc, BOOL & IsLeft);
		LPCSTR		   FindPreLRAB(LPCSTR lpc, BOOL &IsRight);

		LPSTR		   GetText(DWORD &nCount);// 
	protected:
		LPCSTR		   FindNextCh(LPCSTR  lpc,LPCSTR lpRange,char ch);
		LPCSTR		   FindPreCh(LPCSTR  lpc,LPCSTR lpRange,char ch);
	protected:
		LPCSTR		   m_rawlpc;

		LPCSTR		   m_rawbeginc;
		LPCSTR		   m_rawendc;
		CHtmlParse    *m_pParser;
		
	
	public:
		void JumpSpace(LPSTR & lpc);
		BOOL GetCharSet(CString& strCharSet);
		BOOL FindTail(void);
	 
	};
protected:
	//decide how to parse tag here ....
	TAGS_MAP   m_tagMap;
	ULONGLONG  m_FileLen;
public:
	ENTRY_LIST &GetByPropertyName(CString &ProperName);
	HRESULT	   SaveToFile(CString  &htmlName);
	HRESULT	   SaveToTextFile(CString  &txtName);
	HRESULT	   SaveToTextFile(LPCSTR  txtName);
	HRESULT	   LoadFromFile(CString &htmlName);
	HRESULT	   LoadFromFile(LPCTSTR htmlName);
 
	BOOL	    GetPageTitle(CString & titleStr,int &nlen);
	BOOL		EntryInRange(TagEntry &tagEntry,LPSTR rangName);
	BOOL		EntryInRange(TagEntry &tagEntry,CString & rangName);
	BOOL		PointInRange(LPCSTR lpPoint,LPCSTR rangName);
	ENTRY_LIST& GetByPropertyName(LPCSTR lpStr);
	ENTRY_LIST& GetRangeByName(LPCSTR lpStr);
	ENTRY_LIST& GetRangeByName(CString &strName);


	ENTRY_LIST & GetTitles(void);

	void FreeAll();
	void ClearRangeByName(LPCSTR lpName);
	void ClearRangeByName(CString &  strName);
	void ClearPropertyByName(CString & strName);
	void ClearPropertyByName(LPCSTR lpName);
	void AttachBuffer(CString & htmlData);
	BOOL GetCharSetName(CString& strCharSet);
	BOOL SimpleTest(void);
	BOOL InsertStrToRow( LPSTR lpSecName,LPSTR lpDataStr );
	//void DetectRawStr(void);
	CString  &GetRawData(){return m_rawData;};
};

#define REFRESH_FILE(T)\
parse.SaveToFile(T);\
parse.FreeAll();\
if(parse.LoadFromFile(T) == S_FALSE)\
		return FALSE;