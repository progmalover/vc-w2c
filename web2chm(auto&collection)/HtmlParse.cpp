#include "StdAfx.h"
#include ".\htmlparse.h"
#include "shlwapi.h"

///////////////////////////////////////////////////////////////////////////////////
Tag ::Tag(void):m_strTag(""),m_strTagText("")
{
	
};
Tag::~Tag(void){};
///////////////////////////////////////////////////////////////////////////////////
//tag entry

TagEntry::TagEntry(void):m_bUpdata(FALSE),m_startRawPos(0),m_endRawPos(0)
{


}

TagEntry::~TagEntry(void)
{

}

void  TagEntry::ModifyText(CString & txt)
{
	m_strTagText.ReleaseBuffer();
	m_strTagText = txt;
	m_bUpdata = TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
#define  NULL_TEST(T )  if(T==NULL)return T

void inline FilterZeroBytes(CString &strData,int fileLen)
{
		LPBYTE pBytes = (LPBYTE) strData.GetBuffer();

		if(!IsTextUnicode(pBytes,fileLen,NULL))
		{
			for(int i = 0;i < fileLen;i++)
			{
				if(pBytes[i] == 0)
				{
					TRACE("local:%d is zero !\n",i);
					pBytes[i] = ' ';
				}
			}
		}else
		{
			for(int i = 0;i < fileLen;i+= 2)
			{
				if(pBytes[i] == 0 && pBytes[i+1]== 0)
				{
					TRACE("local:%d is zero !\n",i);
					pBytes[i+1] = ' ';
				}
			}
		}

}

BOOL	 CHtmlParse::CHtmlFind::GetRootTitle(CString &strTitle ,int &nLen)
{
	NULL_TEST(m_rawlpc) !=NULL;
	LPSTR  lp,lp1,lp2;

	lp  = const_cast<LPSTR>(m_rawbeginc);
	strTitle.Empty();

	nLen = 0;
LOOP:
	lp1 = StrStrI(lp,"<title");
	
	if(lp1)
	{
		lp2 = (LPSTR)FindNextRAB(lp1);
		if(lp2)
		{
			lp2++;
			lp1 = (LPSTR)FindNextLAB(lp2);
			if(lp1 == NULL) //not in >" " < ,继续找。。。
			{
				lp = lp2;
				goto LOOP;
			}
			//strTitle.SetString(lp2,lp1 - lp2);
			//nLen = lp1-lp2;
			for(;lp2 < lp1;lp2++) // 防止 "\r\n"造成  chm中的索引字符错误.
			{
				if(*lp2 != '\r' && *lp2 != '\n')
				{
					strTitle += *lp2;
					nLen ++;
				}
			}
			return  TRUE;
		}
	}
	return FALSE;
}

BOOL	 CHtmlParse::CHtmlFind:: FindRootProper(LPCSTR properName,TagEntry &tagEntry)
{
	NULL_TEST(m_rawbeginc)!=NULL;

	int   len   =  strlen(m_rawbeginc);
	if(len <= 13) return FALSE; //<html> </html>

	//intliaze first status....,m_rawlpc为当前字符指示器。。。
	m_rawlpc = m_rawbeginc;
	tagEntry.m_strTag	= properName;
	//起始状态都指向开始rowdata开始部分。。。
	tagEntry.m_startRawPos		= (LPVOID)m_rawlpc;
	tagEntry.m_endRawPos		= (LPVOID)m_rawlpc;

	LPSTR  lp,lp1,lp2;
	BOOL   isLeft;

LOOP:
	lp  = (char *)StrStrI(m_rawlpc, properName);
	if(lp)
	{
		lp2 = (LPSTR)FindLRAB((LPCSTR)lp,isLeft); //right ">"

		if(lp2 == NULL)
			return FALSE;
		else
			m_rawlpc = lp2;

		if(isLeft) //><之间的不做处理。。。
			goto LOOP;

		lp1 = (LPSTR)FindNextCh((LPCSTR)lp,m_rawlpc,'=');

		if(lp1 == NULL)
			goto LOOP;

		JumpSpace(++lp1);

		if( *lp1 != '\"' && *lp1 != '\'')
			goto LOOP;

		lp2 = ++lp1;
		if(*(--lp1) == '\"')
			lp1 = (LPSTR)FindNextCh((LPCSTR)lp2,m_rawlpc,'\"');
		else
			lp1 = (LPSTR)FindNextCh((LPCSTR)lp2,m_rawlpc,'\'');

		if(lp1 == NULL)
			return FALSE;
	
		tagEntry.m_startRawPos = lp2;
		tagEntry.m_endRawPos =  lp1;
		tagEntry.m_strTagText.SetString(lp2,lp1 - lp2);
		return TRUE;
	}
	return FALSE;
}

BOOL	 CHtmlParse::CHtmlFind::FindNextProper(TagEntry &tagEntry)
{
	NULL_TEST(m_rawbeginc)!=NULL;
	if(m_rawlpc >= m_rawendc || m_rawlpc == NULL)
		return FALSE;
	
	LPSTR  lp,lp1,lp2;
	BOOL isLeft;
LOOP:
	lp  = (char *)StrStrI(m_rawlpc, tagEntry.m_strTag.GetBuffer());
	tagEntry.m_strTag.ReleaseBuffer();
	if(lp)
	{
		lp2 = lp + tagEntry.m_strTag.GetLength();
		JumpSpace(lp2);

		lp1 = (LPSTR)FindNextCh((LPCSTR)lp,m_rawendc,'=');

		if(lp2 != lp1)
		{
			m_rawlpc = lp2;
			goto LOOP;
		}
		if(lp1 == NULL)
			goto LOOP;

		JumpSpace(++lp1);

		char cCurMark = '\0';
	
		switch(*lp1)
		{
			case '\"':
			case '\'':
				lp2 = lp1;
				lp2++;
				cCurMark = *lp1;
				if(*lp2 == *lp1) // property=""
				{
					m_rawlpc = lp2;
					goto LOOP;
				}
				break;
			default:
				{

					lp2 = lp1;
				}
				break;
		}
		
		if(cCurMark != '\0')
		    lp1 = (LPSTR)FindNextCh((LPCSTR)lp2,m_rawendc,cCurMark);
		else
		{
			LPSTR tp;
			lp1 = (LPSTR)FindNextCh((LPCSTR)lp2,m_rawendc,' ');
			tp  = (LPSTR)FindNextCh((LPCSTR)lp2,m_rawendc,'>');
			
			lp1 = ((int)lp1 > (int)tp)?tp:lp1;
		}

		m_rawlpc = lp1;

		if(lp1 == NULL)
			return FALSE;

		tagEntry.m_startRawPos = lp2;
		tagEntry.m_endRawPos =  lp1;
		
		if((int)lp2 < (int)lp1)
		tagEntry.m_strTagText.SetString(lp2,lp1 - lp2);
		
		return TRUE;
	}
	return FALSE;
}

LPCSTR	 CHtmlParse::CHtmlFind::FindPreLAB(LPCSTR  lpc) // find "<"
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( --lp && lp != m_rawbeginc)
	{
		 if(*lp == '<')
			 return lp;
		 if(*lp == '>')
			 return NULL;
	}
	return NULL;
}

LPCSTR	 CHtmlParse::CHtmlFind::FindNextLAB(LPCSTR  lpc) // find "<"
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( ++lp && lp != m_rawendc)
	{
		if(*lp == '<')
			return lp;
		if(*lp == '>')
			return NULL;
	}
	return NULL;
}

LPCSTR	 CHtmlParse::CHtmlFind::FindNextRAB(LPCSTR  lpc) // find ">"
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( ++lp && lp != m_rawendc)
	{
		if(*lp == '>')
			return lp;
		if(*lp == '<')
			return NULL;
	}
	return NULL;
}

LPCSTR	 CHtmlParse::CHtmlFind::FindPreRAB(LPCSTR  lpc)  // find ">"
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( --lp && lp != m_rawbeginc)
	{
		if(*lp == '>')
			return lp;
		if(*lp == '<')
			return NULL;
	}
	return NULL;
}


LPCSTR	 CHtmlParse::CHtmlFind:: FindNextCh(LPCSTR  lpc,LPCSTR lpRange,char ch)
{
	NULL_TEST(m_rawbeginc);
	while(lpc <= lpRange)
	{
		if(*lpc == ch)
			return  lpc;
		lpc++;
	}
	return NULL;
}

LPCSTR	 CHtmlParse::CHtmlFind:: FindPreCh(LPCSTR  lpc,LPCSTR lpRange,char ch)
{		
	NULL_TEST(m_rawbeginc);
	while(lpc>= lpRange)
	{
		if(*lpc == ch)
			return  lpc;
		lpc--;
	}
	return NULL;
}

 LPSTR	 CHtmlParse::CHtmlFind::GetText(DWORD &nCount)//获得html中的文本显示部分。。。。
 {
   //第一步查找 '>'，然后查找下个'<,看下两者之间有没有内容。。
	NULL_TEST((LPSTR)m_rawbeginc);
	
	int   len   =  strlen(m_rawbeginc);
	if(len <= 13) return FALSE; //<html> </html>

	LPSTR  lpStart,lpEnd;
	nCount = 0;
BEGIN:
	lpStart = (LPSTR)StrStrI(m_rawlpc,">");

	if(lpStart == NULL)
		return NULL;
	lpStart++;
	if(lpStart >= m_rawendc)
		return NULL;
	lpEnd = (LPSTR)FindNextLAB(lpStart);//找下个 “<”
	if(lpEnd==NULL)
	{
		m_rawlpc = lpStart;
		goto BEGIN;
	}
	
	nCount = lpEnd - lpStart;	
	m_rawlpc = lpEnd;
	if(nCount > 2)
		return lpStart;
	else
		goto BEGIN;


 }
///////////////////////////////////////////////////////////////////////////////
CHtmlParse::CHtmlParse(void):m_FileLen(0)
{
    
}

CHtmlParse::~CHtmlParse(void)
{
	FreeAll();
}

ENTRY_LIST & CHtmlParse::GetByPropertyName(CString &ProperName)
{
	if(ProperName.IsEmpty())
		return ENTRY_LIST();

	ProperName.MakeLower();

	if(m_tagMap.size())
	{
		TAGS_MAP::const_iterator it = m_tagMap.find(ProperName);
		if(it != m_tagMap.end())
			return   *it->second;
	}

	ENTRY_LIST  *plist = new ENTRY_LIST(); 
	
	CHtmlFind  htmFind(this);
	TagEntry   tagEntry;

	tagEntry.m_strTag = ProperName;
	BOOL bFind = htmFind.FindNextProper(tagEntry);
	while(bFind)
	{
	//	TRACE("get %s ref : %s\n",tagEntry.m_strTag,tagEntry.m_strTagText);
		plist->push_back(tagEntry);	

		bFind = htmFind.FindNextProper(tagEntry);
	}
	m_tagMap.insert(make_pair(ProperName,plist)); //put to map ....
	return  *plist;
}
#include <vector>
using namespace std;
typedef	vector<ENTRY_LIST *>  LIST_ARRAY;

 TagEntry GetModifedEntryByOrder( LIST_ARRAY &pArray,LPVOID ptr)
{
	
	ENTRY_LIST *pCurList = NULL;
	ENTRY_LIST::iterator pcurIt ;
	
	TagEntry  rtEntry;
	int size = pArray.size();
	
	for(int i = 0;i < pArray.size();i++)
	{
			ENTRY_LIST &list =  *pArray[i];
			ENTRY_LIST::iterator _end = list.end();

			for(ENTRY_LIST::iterator pit = list.begin();pit !=_end ;pit++)
			{
						if(pcurIt._Ptr == NULL)
						{
							TagEntry &tag = *pit;
							if(tag.m_bUpdata && tag.m_startRawPos >= ptr)
							{
								pcurIt =  pit;
								pCurList  = &list;
								break;
							}
							 
						}
						else
						{
							TagEntry &tag1= *pit;
							TagEntry &tag2 = *pcurIt;
							if(tag1.m_bUpdata == FALSE)
								continue;
							if(tag1.m_startRawPos >= ptr)
							{
								if(tag1.m_startRawPos < tag2.m_startRawPos)
								{
									pCurList = &list;
									pcurIt = pit;
									break;
								}
								break;
							}
							
						}
			}

	}

	 	
	if(pCurList && pcurIt._Ptr!=NULL)
	{
	
		if(pcurIt != pCurList->end())
		{
			rtEntry = (*pcurIt);
		//	TRACE("Get Entry :%s \n",rtEntry.m_strTagText);
		}
	}
    return rtEntry;
}

HRESULT CHtmlParse::SaveToTextFile(LPCSTR  txtName)
{
	CString  txtStrName(txtName);
	return SaveToTextFile(txtStrName);
}
class  CMemFileEx  :public CMemFile
{
	friend class CHtmlParse;
};
void TxtFormat(CString &cstr)//处理&nbsp;....
{
	
	CString  strTxt;

	LPSTR lp,lpS,lpE;
	
	strTxt = cstr;
	lpS = strTxt.GetBuffer();
	lpE = lpS + strTxt.GetLength();
	cstr.ReleaseBuffer();

	
	lp = StrStrI(lpS,"&nbsp;");
	while(lp)
	{
		if( lp + 6 < lpE)
		{
			*lp = ' ';
			memcpy(lp + 1,lp + 6,lpE - lp - 6);
			*(lpE - 5) = '\0';
			lpE -= 5;
		}
		else
		{	
			*lp = ' ';
			*(lp + 1) = '\0';
		}
		lp = StrStrI(lp,"&nbsp;");
	}
	cstr = lpS;
}

HRESULT CHtmlParse::SaveToTextFile(CString  &txtName)
{

	CMemFileEx   txtImg;
	CHtmlFind  htmFind(this);
	DWORD txtCount;

	ASSERT(m_FileLen != 0);
	txtImg.SetLength(m_FileLen);
	
	LPSTR  lpStr = htmFind.GetText(txtCount);

	CString  tpStr;
	while(lpStr)
	{
		if(!PointInRange(lpStr,"script") && !PointInRange(lpStr,"noscript")
			&&!PointInRange(lpStr,"style"))
		{
			tpStr.SetString(lpStr,txtCount);
			tpStr.TrimLeft();
			TxtFormat(tpStr);
			tpStr += "\n";	
			if(tpStr.GetLength() > 1)
			txtImg.Write(tpStr.GetBuffer(),tpStr.GetLength() );
			tpStr.ReleaseBuffer();
		}
		lpStr = htmFind.GetText(txtCount);
	}
	txtImg.Write("\0",1);
    
	CStdioFile  txtFile;
	BOOL bopen = txtFile.Open(txtName,CStdioFile::modeCreate | CStdioFile::modeWrite);

	if(bopen)
	{
	  txtFile.WriteString((LPCSTR)txtImg.m_lpBuffer);
	  txtFile.Close();
	}
	 return S_OK;
}

HRESULT CHtmlParse::SaveToFile(CString  &htmlName)
{
    CStdioFile   sF;
	BOOL			bRet;
	
	if(::PathFileExists(htmlName))
		::DeleteFile(htmlName);
	bRet = sF.Open(htmlName,CStdioFile::modeCreate|CStdioFile::modeWrite|CStdioFile::typeBinary);
	if(bRet == FALSE)
	{
		AfxMessageBox("Open File Failure!");
		return S_FALSE;
	}
	if(m_rawData.IsEmpty())
	{
		sF.Close();
		return S_FALSE;
	}

	if(m_tagMap.size() <= 0)
	{
	   sF.Write(m_rawData.GetBuffer(),m_rawData.GetLength());
	   m_rawData.ReleaseBuffer();
	}
	else
	{
		//find 
		int lsSize = m_tagMap.size();
	    LIST_ARRAY  pListArray;
		
		for(TAGS_MAP::iterator it = m_tagMap.begin(); 
								it != m_tagMap.end();it++)
				pListArray.push_back((*it).second);	

		/*find first modifed entry,then second*/
		 LPSTR  lptBuf = m_rawData.GetBuffer();
		 m_rawData.ReleaseBuffer();

		 while(1)
		{
			TagEntry  tagEntry  = GetModifedEntryByOrder(pListArray,lptBuf);
			if(tagEntry.m_endRawPos !=LPVOID(0))
			{
				sF.Write(lptBuf,(DWORD) tagEntry.m_startRawPos - (DWORD)lptBuf);
				lptBuf = (LPSTR) tagEntry.m_endRawPos;
				
				CString  &str = tagEntry.m_strTagText;
				sF.Write(str.GetBuffer(),str.GetLength()-1);
				str.ReleaseBuffer();
			}else
			{
				LPSTR  lpend = (m_rawData.GetBuffer() + m_rawData.GetLength());
				int	   slen = (int)lpend - (int)lptBuf ;
				if(slen > 0)
					sF.Write(lptBuf,slen);
				m_rawData.ReleaseBuffer();
				break;
			}
		}
	 
		//delete []pListArray;
	}

	//sF.Flush();
	sF.Close();
	return S_OK;
}

HRESULT CHtmlParse::LoadFromFile(LPCTSTR htmlName)
{
	CString strFileName(htmlName);
	return LoadFromFile(strFileName);
}
HRESULT CHtmlParse::LoadFromFile(CString &htmlName)
{
	CStdioFile		cf ;
	BOOL				bRet;

	try{
		
		bRet = cf.Open(htmlName,CStdioFile::modeRead);
		if(bRet == FALSE)
			return S_FALSE;
		DWORD fLeng = cf.GetLength();

		if(fLeng <= 0)
		{
			cf.Close();
			return S_FALSE;
		}

		//fLeng += 20;//pad 

		LPSTR  lpbuf = m_rawData.GetBufferSetLength(fLeng + 2);
		memset(lpbuf,0,fLeng + 2);
		DWORD dwRdSize = cf.Read(lpbuf,fLeng);

		if(dwRdSize <= 0)
		{
			cf.Close();
			return S_FALSE;
		}
		m_FileLen = dwRdSize;
		
		if(strlen(lpbuf) < m_FileLen)
			FilterZeroBytes(m_rawData,m_FileLen);
		m_rawData.ReleaseBuffer();
		//m_rawData.Trim();//filelen include "\0"

	}catch(...)
	{
		if(bRet)
			cf.Close();
		return S_FALSE;
	}
	cf.Close();
	return S_OK;
}

void CHtmlParse::FreeAll()
{

	int size= m_tagMap.size();

	for(TAGS_MAP::iterator it = m_tagMap.begin(); it != m_tagMap.end();it++)
	{
		ENTRY_LIST *plist =  (*it).second;
		plist->clear();
		delete  plist;
	}
	 m_tagMap.clear();
	 m_rawData.Empty();
}
BOOL CHtmlParse::GetPageTitle(CString & titleStr ,int &nlen)
{ 
	CHtmlFind  hf(this);
	return  hf.GetRootTitle(titleStr,nlen);
}

LPCSTR CHtmlParse::CHtmlFind::FindPreLRAB(LPCSTR lpc, BOOL &IsRight)
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( ++lp && lp != m_rawendc)
	{
		if(*lp == '>')
		{
			IsRight = TRUE;
			return lp;
		}
		if(*lp == '<')
		{
			IsRight = FALSE;
			return lp;
		}
	}
	return NULL;
	 
}

LPCSTR CHtmlParse::CHtmlFind::FindLRAB(LPCSTR lpc, BOOL &IsLeft)
{
	NULL_TEST(m_rawbeginc);
	LPSTR  lp = const_cast<LPSTR>(lpc);
	while( ++lp && lp != m_rawendc)
	{
		if(*lp == '<')
		{
			IsLeft = TRUE;
			return lp;
		}
		if(*lp == '>')
		{
			IsLeft = FALSE;
			return lp;
		}
	}
	return NULL;
	 
}

void CHtmlParse::CHtmlFind::JumpSpace(LPSTR & lpc)
{
	 while(*lpc == ' ')
		 lpc++;
}

void TagEntry::ModifyText(LPCSTR lpTxt)
{
	CString str(lpTxt);
	ModifyText(str);
}

ENTRY_LIST& CHtmlParse::GetByPropertyName(LPCSTR lpStr)
{
	//TODO: return statement
	CString  strName(lpStr);
	return GetByPropertyName(strName);
}

ENTRY_LIST& CHtmlParse::GetRangeByName(LPCSTR lpStr)
{
	//TODO: return statement
	CString strName(lpStr);
	return  GetRangeByName(strName);
}

#include <stack>
typedef stack<TagEntry>  ENTRY_STACK ;
BOOL CHtmlParse::CHtmlFind::FindNextRange(TagEntry &tagEntry)
{
	NULL_TEST(m_rawbeginc)!=NULL;
	if(m_rawlpc >= m_rawendc || m_rawlpc == NULL)
		return FALSE;
	CString &strName = tagEntry.m_strTag;
	LPSTR  lp,lp1,lp2;
	BOOL isLeft;
	lp  = (LPSTR)StrStrI(m_rawlpc, strName);

	ENTRY_STACK stack;
	while(lp)
	{
		char *tp = lp;
		tp--;
		switch(*tp)
		{
		case '<':
			{
				tp--;
				if(*tp != '"')
				{
					TagEntry tag;
					tag.m_startRawPos = ++tp;
					stack.push(tag);
					m_rawlpc = lp;
					m_rawlpc += strName.GetLength();    
				}else
				{
					m_rawlpc = lp;
					m_rawlpc += strName.GetLength();    
				}
			}break;
		case '/':
			{
				tp--;
				if(*tp == '<')
				{
					if(stack.size() > 0)
					{
						TagEntry &tag = stack.top();
						tagEntry.m_startRawPos = tag.m_startRawPos;
						tagEntry.m_endRawPos = (void *)FindNextCh(tp,m_rawendc,'>');

						if(tagEntry.m_endRawPos <=0)//file error end
						{
							stack.pop();
							return FALSE;
						}
						tagEntry.m_endRawPos = (void *)((int)tagEntry.m_endRawPos +1);
						tagEntry.m_strTagText.SetString((const char *)tagEntry.m_startRawPos,(int)tagEntry.m_endRawPos - (int)tagEntry.m_startRawPos);
						m_rawlpc =(LPCSTR) tagEntry.m_endRawPos;
						stack.pop();
						if(stack.size() <= 0)
						 return TRUE;
					}else
					{
						//error here.... i decide to clear the wrong section.....
						TRACE("***wrong in page.....\n");
						tagEntry.m_endRawPos = (void *)FindNextCh(tp,m_rawendc,'>');
						if(tagEntry.m_endRawPos <=0)//file error end
							return FALSE;

						tagEntry.m_endRawPos = (void *)((int)tagEntry.m_endRawPos +1);
						tagEntry.m_startRawPos = (void *)FindPreCh(tp ,m_rawbeginc,'>');
						tagEntry.m_startRawPos = (void *)((int)tagEntry.m_startRawPos +1);
						tagEntry.m_strTagText.SetString((const char *)tagEntry.m_startRawPos,(int)tagEntry.m_endRawPos - (int)tagEntry.m_startRawPos);
						if(tagEntry.m_startRawPos == NULL)
							tagEntry.m_startRawPos = (LPVOID)m_rawbeginc;

						m_rawlpc = lp;
						m_rawlpc += strName.GetLength();
						 return TRUE;
					}
					
				} 
				
					m_rawlpc = lp;
					m_rawlpc += strName.GetLength();
				
			}break;
		default:
			{
					m_rawlpc = lp;
					m_rawlpc += strName.GetLength();
			}
			break;
		}
		lp  = (LPSTR)StrStrI(m_rawlpc, strName);

	}

	if(stack.size() > 0)
	{
		TRACE("***wrong in page.....\n");
		TagEntry &tag = stack.top();
		tagEntry.m_endRawPos = (void *)m_rawendc;//((int)tagEntry.m_endRawPos +1);
		tagEntry.m_startRawPos = tag.m_startRawPos;

		if((int)tagEntry.m_endRawPos - (int)tagEntry.m_startRawPos > 0)
		{
			tagEntry.m_strTagText.SetString((const char *)tagEntry.m_startRawPos,
				(int)tagEntry.m_endRawPos - (int)tagEntry.m_startRawPos);
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;

}

BOOL CHtmlParse::CHtmlFind::FindRootRange(CString strName,TagEntry &tagEntry)
{
	NULL_TEST(m_rawbeginc)!=NULL;

	int   len   =  strlen(m_rawbeginc);
	if(len <= 13) return FALSE; //<html> </html>

	//intliaze first status....,m_rawlpc为当前字符指示器。。。
	m_rawlpc = m_rawbeginc;
	tagEntry.m_strTag	= strName;
	//起始状态都指向开始rowdata开始部分。。。
	tagEntry.m_startRawPos		= (LPVOID)m_rawlpc;
	tagEntry.m_endRawPos		= (LPVOID)m_rawlpc;

	LPSTR  lp,lp1,lp2;
	BOOL   isLeft;

LOOP:
	lp  = (LPSTR)StrStrI(m_rawlpc, strName);
	if(lp)
	{

		lp2 = lp;
		if(*(--lp2) != '<')
		{
			m_rawlpc = lp + strName.GetLength();
			goto LOOP;
		}

		lp1 = (LPSTR)FindLRAB((LPCSTR)lp,isLeft); //right ">"

		if(lp1 == NULL)
			return FALSE;
		else
			m_rawlpc = lp1;

		if(isLeft) //><之间的不做处理。。。
			goto LOOP;

LOOP2:
		lp1 = (LPSTR)StrStrI(m_rawlpc, strName);

		if(lp1 == NULL)
			return FALSE;

		//lp = lp1;
		//lp1 += strName.GetLength();

		//JumpSpace(++lp1);
		lp1--;
		if( *lp1 != '\\' && *lp1 != '/')
		{
			++lp1 += strName.GetLength();
			m_rawlpc = lp1;
			goto LOOP2;
		}
	
		lp1--;
		if(*lp1 != '<')
		{
			lp1+=2;
			lp1 += strName.GetLength();
			m_rawlpc = lp1;
			goto LOOP2 ;
		}


		lp1 = (LPSTR)FindNextCh(lp1,m_rawendc,'>');

		if(lp1 == NULL)
			return FALSE;
	
		lp1++;
		m_rawlpc = lp1;

		tagEntry.m_startRawPos = lp2;
		tagEntry.m_endRawPos =  (void *)lp1;
		tagEntry.m_strTagText.SetString(lp2,lp1 - lp2);
		return TRUE;
	}
	return FALSE;
    
}

ENTRY_LIST&  CHtmlParse::GetRangeByName(CString &strName)
{
  if(strName.IsEmpty())
		return ENTRY_LIST();

	strName.MakeLower();

	if(m_tagMap.size())
	{
		TAGS_MAP::const_iterator it = m_tagMap.find(strName);
		if(it != m_tagMap.end())
			return   *it->second;
	}

	ENTRY_LIST  *plist = new ENTRY_LIST(); 
	
	CHtmlFind  htmFind(this);
	TagEntry   tagEntry;
	tagEntry.m_strTag = strName;
	BOOL bFind = htmFind.FindNextRange(tagEntry);
	while(bFind)
	{
		//TRACE("get %s ref : %s\n",tagEntry.m_strTag,tagEntry.m_strTagText);
		plist->push_back(tagEntry);	
        TRACE("***find range %s!!\n",strName);
		bFind = htmFind.FindNextRange(tagEntry);
	}

	m_tagMap.insert(make_pair(strName,plist)); //put to map ....
	return  *plist;
}


BOOL  CHtmlParse::EntryInRange(TagEntry &tagEntry,LPSTR rangName)
{
	CString  strName(rangName);
	return EntryInRange(tagEntry, strName);
}

BOOL CHtmlParse::PointInRange(LPCSTR lpPoint,LPCSTR rangName)
{
	ENTRY_LIST &list = GetRangeByName(rangName);
		if(lpPoint  <= 0)
			return FALSE;
		if(list.size() <= 0)
			return FALSE;

		ENTRY_LIST::iterator  _end = list.end();
		for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
		{
			TagEntry  &tag = *it;
			if(lpPoint  >=  tag.m_startRawPos && 
					lpPoint  <= tag.m_endRawPos)
				return TRUE;
		}

		return FALSE;
}

BOOL  CHtmlParse::EntryInRange(TagEntry &tagEntry,CString & rangName)
{
		ENTRY_LIST &list = GetRangeByName(rangName);
		if(tagEntry.m_endRawPos  <= 0)
			return FALSE;
		if(list.size() <= 0)
			return FALSE;

		ENTRY_LIST::iterator  _end = list.end();
		for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
		{
			TagEntry  &tag = *it;
			if(tagEntry.m_startRawPos  >=  tag.m_startRawPos && 
					tagEntry.m_startRawPos  <= tag.m_endRawPos)
				return TRUE;
			if(tagEntry.m_endRawPos  <=  tag.m_endRawPos && 
					tagEntry.m_endRawPos  >= tag.m_startRawPos)
				return TRUE;
		}

		return FALSE;
}

ENTRY_LIST & CHtmlParse::GetTitles(void)
{
	
	return  ENTRY_LIST();
}
void CHtmlParse::ClearRangeByName(LPCSTR lpName)
{
	CString strRange(lpName);
    ClearRangeByName(strRange);
}

void CHtmlParse::ClearRangeByName(CString &  strName)
{
	ENTRY_LIST &list = GetRangeByName(strName);
	ENTRY_LIST::iterator  _end = list.end();
	for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
	{
		TagEntry  &tag = *it;
		if(FALSE == tag.m_strTagText.IsEmpty())
		  tag.ModifyText(" ");
	}

}

void CHtmlParse::ClearPropertyByName(CString & strName)
{
	ENTRY_LIST &list = GetByPropertyName(strName);
	ENTRY_LIST::iterator  _end = list.end();
	for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
	{
		TagEntry  &tag = *it;
		tag.ModifyText(" ");
	}
}
/*
<script>
onerror=function(){return true}
</script>
*/
BOOL CHtmlParse:: InsertStrToRow( LPSTR lpSecName,LPSTR lpDataStr )
{
	if(this->m_rawData.GetLength() <= 0)
		return FALSE;

	LPSTR  lpRow = m_rawData.GetBuffer();
	m_rawData.ReleaseBuffer();
	LPSTR lpf = StrStrI(lpRow,lpSecName);
	lpf += strlen(lpSecName);
	if(lpf)
	{
		m_rawData.Insert(lpf - lpRow,lpDataStr);
		return TRUE;
	}
	return FALSE;
	
}

void CHtmlParse::ClearPropertyByName(LPCSTR lpName)
{
	CString strProper(lpName);
    ClearPropertyByName(strProper);
}

void CHtmlParse::AttachBuffer(CString & htmlData)
{
	this->m_rawData = htmlData ;
	//this->m_rawData.MakeLower();
	m_FileLen = m_rawData.GetLength();
}

BOOL CHtmlParse::GetCharSetName(CString& strCharSet)
{
	CHtmlFind  hf(this);
	return  hf.GetCharSet(strCharSet);
}

BOOL CHtmlParse::CHtmlFind::GetCharSet(CString& strCharSet)
{
	NULL_TEST(m_rawlpc) !=NULL;
	LPSTR  lp,lp1,lp2;

	lp  = const_cast<LPSTR>(m_rawbeginc);
LOOP:
	lp1 = StrStrI(lp,"charset");
	
	if(lp1)
	{
		lp1 += 7;
		lp2 = lp1;
	    JumpSpace(lp2);
		if(*lp2 != '\=' )
			goto LOOP;
		lp2++;
		JumpSpace(lp2);
		
		while( *lp2 != '\"' && *lp2 != '\>')
		{
			strCharSet += *lp2;
			lp2++;
		}
		return TRUE;
		
	}
	return FALSE;
}

BOOL CHtmlParse::SimpleTest(void)
{

	CHtmlFind  hf(this);
	return  hf.FindTail();
}

BOOL CHtmlParse::CHtmlFind::FindTail(void)
{
	NULL_TEST(m_rawlpc) !=NULL;
	LPSTR  lp,lp1;

	lp  = const_cast<LPSTR>(this->m_rawbeginc);
	lp  += m_pParser->m_FileLen / 2;
	lp1 = StrStrI(lp,_T("</html>"));
	if(lp1)
		return TRUE;
	return FALSE;
}

//void CHtmlParse::DetectRawStr(void)
//{
//	if(m_FileLen <= 0) return;
//	LPSTR lpRaw = m_rawData.GetBuffer();
//	m_rawData.ReleaseBuffer();
//	for(int i = 0;i < m_FileLen;i++)
//	{
//		if( *lpRaw == '\0')
//			*lpRaw = ' ';
//	}
//
//}
