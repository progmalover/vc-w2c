#include "stdafx.h"
#include "Wininet.h"
#include <afxinet.h>
#include <mmsystem.h>
#include "filepath.h"
#include "HttpApi.h"
#include "TWebTask.h"
#include "defnamemanager.h"
#include <comdef.h>
#include <mshtml.h>
//#include "Mshtml.idl"
#include "ui.h"
#include "hmCompiler.h"
#include "comrun.h"
#include "htmlparse.h"
#include "defs.h"
#include "resource.h"
#pragma warning(disable : 4146)	//see Q231931 for explaintation
#import <mshtml.tlb> no_auto_exclude
using namespace ::ui;

///////////////////////////////////
//global config val 
DWORD GA_dwWaitTimeArray[CONNECT_WAIT_TIME]={ 400,800,1200,1600,2000,4000,10000};
DWORD GA_dwConnectRepondsArray[] = {1,3,5,10,20};
///////////////////////////////////

const int   HTML_TYPE_NUM  = 6;
const TCHAR *htmlFileType[]={
".htm",".html",".shtml",".asp",".aspx",".php"
};
 //url link to
inline void ToLowerAlphas(LPSTR pstr,int len)
{

	if(len <= 0)return;
	int t = 'a' - 'A';
	for(int i = 0;i < len;i++)
	{
		if(pstr[i] >= 'A' && pstr[i] <= 'Z')
		{
			pstr[i] += (char)t;
		}
	}

   
}

BOOL   IsInpageLink(LPCSTR  url)
{
	char *p  = (char *)strstr(url,"#");
	if(p)
		return TRUE;
	return FALSE;
}
//过滤  \0
void SafeStrMoudleScan(LPSTR lpstr,int len)
{
	for(int i = 0;i < len ;i++)
	{
		if( *lpstr == '\0')
			*lpstr = ' ';
		lpstr++;
	}
}

BOOL  IsHtmlTypeFile(PATH_STRING &strPath,DWORD &nfType)
{
	 
	if(strPath.IsEmpty())
		return FALSE;
	
	if(!::PathFileExists(strPath))
	    return FALSE;

	if(::PathIsHTMLFile(strPath))//一般的类型判断。。。
	{
		nfType = 1;
		return TRUE;
	}


	CString  str = ::PathFindExtension(strPath);
	if(!str.IsEmpty())
    for( int i = 0;i < HTML_TYPE_NUM;i++)
	{
		if(str.Compare(htmlFileType[i]) == 0)
		{
			nfType = i;
			return TRUE;
		}
	}


	 //无对应扩展名的文件处理。。。
	CStdioFile  stdFile;
		
	if(stdFile.Open(strPath,CStdioFile::modeRead))
	{
		if(stdFile.GetLength() <= 0)
		{
			stdFile.Close();
			return FALSE;
		}

		char *pcfind;
		char *pDoc = (char *)new char[512];//512应该可以包括进 "<html" 

		UINT number = stdFile.Read(pDoc,510);
		pDoc[number] = '\0';
		pcfind = StrStrI(pDoc,"<html");
	 
		delete []pDoc;

		stdFile.Close();
		if(pcfind)
			return TRUE;
	}

		
	
	return FALSE;
}


// 整理url缺失的部分。。。
void FixUrlStr(URL_STRING  &strUrl)
{

	BOOL bRet = AssertUrlHttpStr(strUrl);
	if(!bRet)
		strUrl.Insert(0,"http://");

	int len,i,j = 0;
	len = strUrl.GetLength();
	for( i = 0;i <len;i++)
		if(strUrl[i] == '/')
			j++;
	if(j == 2) //路径中只有2个'/‘也就是。。。
		strUrl +='/';
}

/*需要提取出来各种符合字串类型在这里识别
http://wh.focus.cn/housemarket/zxlp.php?page=2
http://survey.163.com/survey/survey/user_consumption_survey2007/survey.php?site=homenews
*/
  void ConvertStrIntData(char *pstr)
 {
	char *pt = pstr;
	char intData[3];

	int len = strlen(pt);
	if(len >  2)
	for(int i = 0;i < len;i++)
	{
		if(pt[i] == '%' && (len - i >=2))
		{
			intData[0] = pt[i+1];
			intData[1] = pt[i + 2];
			intData[2] = '\0';

			pt[i] = StrToInt(intData);
			
			strncpy(&pt[i],&pt[i + 2],(len - i -2));
			pt[len -1] = '\0';
			pt[len -2] = '\0';
			ConvertStrIntData( &pt[i]);
		}
	}
 }
 /*
 再原来的url的基础上产生一个能用来作为文件名的url，并不改变原始的url.
 */
BOOL ExtractValidUrl(URL_STRING *urlStr,URL_STRING *pValidUrl)
{
	char *pChar = urlStr->GetBufferSetLength(urlStr->GetLength() +1);
	char *pChar1;
	//urlStr->MakeLower();
	//ToLowerAlphas(urlStr->GetBuffer(),urlStr->GetLength());
	//urlStr->ReleaseBuffer();

	pChar[urlStr->GetLength()] = '\0';

	pChar1 = strstr(pChar,"?");
	if(pChar1 == NULL)
		pChar1 = strstr(pChar,"#");

	if(pChar1)
	{
		int num = (int)pChar1 - (int)pChar;
		if(num > 0)
			pValidUrl->SetString(pChar,num);
		
		pChar1++;
		num = pValidUrl->ReverseFind('/');
		if(2 == (pChar1 - pChar)  - num)	
		{
			//TWebSession  *pCurSession = GetCurrentSession();
			//ASSERT(pCurSession);
			*pValidUrl +=   CDefnameManager::Instance()->GetDefName(*urlStr);//"default.html";
		}
		if(num > 0)
		{	
			CString  tmpstr = pChar1;
			ConvertStrIntData(tmpstr.GetBuffer());
			tmpstr.ReleaseBuffer();
			pValidUrl->Insert(num + 1,tmpstr);
			
		}

	}else
	  *pValidUrl = *urlStr;
	urlStr->ReleaseBuffer();
	return TRUE;
}

/*
 为相对链结
*/
#define  DOMAIN_TAG_NUM   18
BOOL HasDomain(URL_STRING &urlStr)
{
 
	static  char *domainText[DOMAIN_TAG_NUM] = 
	{
		"www","com","cn","org",
		"us","enu","pro","biz",
		"museum","coop","net",
		"aero","idv","int","mil",
		"gov","info","name"
	};

	char *pStr,ch1,ch2;
	for( int i = 0;i < DOMAIN_TAG_NUM;i++)
	{
		if((pStr = StrStrI(urlStr,domainText[i])) != NULL )//.*.;.*/;/*.
		{
			pStr--;
			ch1 = *pStr ;
			pStr++;
			pStr += strlen(domainText[i]);
			ch2 = *pStr;
			switch(ch1)
			{
				case '.':
					if(ch2 == '.' &&( ch2 == '\\'|| ch2 == '/'))
						return TRUE;
				break;

				case '\\':
				case '/':
					if(ch2 == '.')
						return TRUE;
				break;
			}
		
		}
	}
	return FALSE;

}

BOOL	 IsRefLink(URL_STRING & urlStr)
{
	URL_STRING strServer,strObject,url;
	DWORD dwType;
	WORD  nPort;
	BOOL  bRet ;
	url = urlStr;

	if(urlStr[0] == '/')//first is '/' is must be ref....
		return TRUE;

	if(urlStr[0] == '.')
		return TRUE;

	bRet = AssertUrlHttpStr(url);//has http,isnt ref.....
	if(bRet)
		return FALSE;

	if(urlStr[0] != '/')//for url: “0755.html"
	{
		if(HasDomain(url)) //has domain format in first str union isnt ref......
		{
			urlStr.Insert(0,"http://"); 
			return FALSE;
		}	 

		urlStr.ReleaseBuffer();
		urlStr.Insert(0,"./");
		return TRUE;
	}

	return TRUE;
}

/*简单通过协字串的个数,判断是否复合串*/
BOOL	 IsUntLink(LPCSTR  purlStr)
{
	const char *pProtocal = "http";
	char *tp;

	int count = 0;
	if(!purlStr)
		return FALSE;
	tp = const_cast<char *>(purlStr);
	while(tp)
	{
		tp = StrStrI(tp,pProtocal);
		if(tp)
		{
			count++;
			tp ++;
		}
	}
	return count > 1;
}

#define  STR_NUM   4
BOOL	 IsRightLink(URL_STRING & urlStr)
{
	const char *pStrs[STR_NUM]= {
		"onclick",
		"javascript:",
		"mailto:",
		"about:blank"
	};
	 
	char *tp,*tp1;

	if(IsUntLink(urlStr)) //ignor unit link....
		return FALSE;

	int count = 0;
	if(urlStr.IsEmpty())
		return FALSE;

	tp1  = urlStr.GetBufferSetLength(urlStr.GetLength() +1);
	urlStr.ReleaseBuffer(-1);
	tp1[urlStr.GetLength()] = '\0';

	for(int i = 0;i < STR_NUM;i++)
	{
		tp = strstr(tp1,pStrs[i] );
		if(tp)
			return FALSE;
	}
	return TRUE;
}
 
BOOL IsAllowFileType(_URL_TASK *pParent,DWORD nType)
{
	pPRIVATE_CONFIG pconf = ConfigManager::Instance()->GetSessionConfig(pParent);
	if(pconf == NULL)
		pconf = ConfigManager::Instance()->GetDefaultSessionConf();
	
	switch(nType)
	{
	case _URL::FILE_JPG:
	case _URL::FILE_GIF:
	case _URL::FILE_JPEG:
	case _URL::FILE_PNG:
	case _URL::FILE_BMP:	
	case _URL::FILE_JFIF:
		{
			if(pconf->enableTypeArray[1])
				return TRUE;
		}
		break;
	case _URL::FILE_ZIP:
		{
			if(pconf->enableTypeArray[2])
				return TRUE;
		}break;
	case _URL::FILE_MP3:
	case _URL::FILE_WMA:
	case _URL::FILE_RM:
	case _URL::FILE_WMV:
		{
			if(pconf->enableTypeArray[3])
				return TRUE;
		}break;
	case _URL::FILE_SWF:
		{
			if(pconf->enableTypeArray[4])
				return TRUE;
		}break;
	case _URL::FILE_PDF:
		{
			if(pconf->enableTypeArray[5])
				return TRUE;
		}break;
	case _URL::FILE_CHM:
		{
			if(pconf->enableTypeArray[6])
				return TRUE;
		}break;

	case _URL::FILE_TEXT:
	case FILE_URL://默认支持。。。。
		 return TRUE;
		break;
	}

	return  FALSE;
}

/*避免添加重复的*/
 BOOL  AlreadInList( URL_LIST &listUrl,URL_STRING &strUrl)
{
	if(listUrl.size() <= 0)
		return FALSE;
	for(URL_LIST::iterator lit = listUrl.begin();lit != listUrl.end();lit++)
	{	
		URL_STRING &str = *lit;
		if(str.GetLength() != strUrl.GetLength()) /*长度不等*/
			continue;
		if(str.Compare(strUrl) == NULL)
			return TRUE;
	}
	return FALSE;
}

/*得到叶面上的资源包括图片或者txt..flash资源等*/
BOOL  GetHttpPageSource(CString &urlName,URL_LIST &urlList)
{
	return TRUE; 
}

BOOL  inline HtmlDocToFile(CFile &pFile,MSHTML::IHTMLDocument2Ptr &piDoc)
{
	CString str;


	MSHTML::IHTMLDocument3Ptr  pDoc3;
	MSHTML::IHTMLElementPtr ptElement;
	MSHTML::IHTMLElementCollectionPtr ptCollection;


    pDoc3 = piDoc;
	pDoc3->get_documentElement(&ptElement);
	str = (LPCTSTR)ptElement->outerHTML;
	pFile.Write(str.GetBuffer(),str.GetLength());

#if 0
	piDoc->get_all(&ptCollection);
	 
	TRACE("***(:)***len = %d\n",ptCollection->length); 
	for( long len = 0;len < ptCollection->length;len++)
	{
		ptElement  = ptCollection->item(len,0);
		str = (LPCTSTR)ptElement->outerHTML;
		//TRACE("%s\n",str);
		if(str.GetLength() )
		{
			pFile.Write(str.GetBuffer(),str.GetLength());
			str.ReleaseBuffer();
			return TRUE;	
		}
		
	}
#endif
	return FALSE;
}


/*组合参考url和根url，生成绝对url*/
inline int CountUrlSpash(URL_STRING &urlStr)
{
	char *p = urlStr.GetBuffer();
	int i = 0;
	while(*p)
	{
		if(*p == '/')
			i++;
		p++;
	}
	urlStr.ReleaseBuffer();
	return i;
}


void	 ToDosFileNameStr( CString  &strStr)
{
	char *pbuf = strStr.GetBuffer();
	strStr.ReleaseBuffer();

	int len = strStr.GetLength();
	for(int i = 0;i < len;i++)
	{
		switch(pbuf[i])
		{
		
		case ':' :
		case '\\':
		case '/' :
		case '*' :
		case '?' :
		case '"' :
		case '<' :
		case '>' :
		case '|' :
			pbuf[i] = ' ';
			break;

		}
	}
}

URL_STRING ComposeRefURL(URL_STRING *pRoot,URL_STRING *pRef  )
{
	URL_STRING  fullUrl;
	if(!pRef ||!pRoot ||pRoot->IsEmpty()||pRef->IsEmpty())
		return fullUrl;

	URL_STRING  rootDir,refStr;
	char *pchs = pRef->GetBuffer();

	rootDir = *pRoot;
	refStr = *pRef;

	/*ToLowerAlphas(rootDir.GetBuffer(),rootDir.GetLength());
	rootDir.ReleaseBuffer();

	ToLowerAlphas(refStr.GetBuffer(),rootDir.GetLength());
	refStr.ReleaseBuffer();*/
	 

	if(CountUrlSpash(rootDir) > 2)
	{
		
		int num = rootDir.ReverseFind('/');
		rootDir = rootDir.Left(num);
	}

	switch(pchs[0])
	{
	case '/'://直接主机目录。。。。
	case '\\'://这里要增加带参数url 的判断。。。。。。 zxy  另外 “/”意义理解不对。。处理不准确。。。
		{
			 
			URL_STRING   strServer;
			URL_STRING   strObj;
			URL_STRING   strUrl;
			DWORD        dwType;
			INTERNET_PORT nPort;

			strUrl = *pRoot;
			AfxParseURL(strUrl,dwType,strServer,strObj,nPort);
			
			
			fullUrl = strServer;
			fullUrl.Insert(0,"http://");
			fullUrl += pchs;
			 
		}
		break;
	case '.':// "./"当前目录。。。。这里对于“。/。/“或者”。。/。。/“ 的情况要做循环处理。。。。。
		{
			while(pchs[0] == '.' &&(pchs[1] == '\\' ||pchs[1] == '/'))
			{
				refStr = refStr.Right(refStr.GetLength() -2 );//去掉 ‘./’
				pchs += 2;
			}

			while(pchs[0] == '.' && pchs[1] == '.' &&(pchs[2] == '\\' ||pchs[2] == '/'))//"../"则上升一层。。。
			{
				 int num = rootDir.ReverseFind('/');
				 rootDir = rootDir.Left(num);
				 refStr = refStr.Right(refStr.GetLength() - 3);//去掉 “。。/”
				 pchs += 3;
			}

			refStr.Insert(0,'/');
			fullUrl = rootDir;
			fullUrl +=  refStr;

		}
	    break;
	default://仅有名字存在。。。也是当前目录
		{
			//in IsRefLink() add " ./"
		}
		break;
	}
	pRef->ReleaseBuffer();
	return fullUrl;
}

BOOL  GetHttpPageUrls(_URL_TASK *pTask,_URL *urlObj,URL_LIST &urlList ,URL_LIST &urlSrcsList,DWORD nDownMode ,DWORD nTag)
{
	CHtmlParse	parse;
	 
	if(parse.LoadFromFile(urlObj->m_hstSavePath) == S_FALSE)
		return FALSE;
	else //filter the range that neednt...
	{
		//parse.DetectRawStr();//clear '0' in file.
		pPRIVATE_CONFIG  pConf = ConfigManager::Instance()->GetDefaultSessionConf();
		{
			if(pConf->bCNoScript)
			  parse.ClearRangeByName("noscript");
			if(pConf->bCScript)
			  parse.ClearRangeByName("script");

			REFRESH_FILE(urlObj->m_hstSavePath);
		}
		{
			BOOL bSuc ;
			bSuc = parse.InsertStrToRow(_T("</title>"),_T( "<script>"\
													"onerror=function(){return true}"\
													"<\/script>"));

			if(!bSuc)
			 bSuc = parse.InsertStrToRow( _T("<head"),_T( "<script>"\
													"onerror=function(){return true}"\
													"<\/script>"));
			REFRESH_FILE(urlObj->m_hstSavePath);
		}

		if(pConf->bCLoad)
			parse.ClearPropertyByName("onload");
		if(pConf->bCUnload)
			parse.ClearPropertyByName("onunload");
		if(pConf->bCMouseClick)
			parse.ClearPropertyByName("onclick");
		if(pConf->bCMouseLeave)
			parse.ClearPropertyByName("onmouseleave");
		if(pConf->bCMouseEnter)
			parse.ClearPropertyByName("onmouseenter");
		if(pConf->bCMouseOver)
			parse.ClearPropertyByName("onmouseover");
		if(pConf->bCMouseOut)
			parse.ClearPropertyByName("onmouseout");
		if(pConf->bCMouseDown)
			parse.ClearPropertyByName("onmousedown");
		//parse.ClearPropertyByName("align");
		REFRESH_FILE(urlObj->m_hstSavePath);
	}

	TWebSession  *pSession = (TWebSession *)pTask;
	TWebTask     *pWebTask = (TWebTask *)pSession->m_pCurrentTask;

	THREAD_EXIT_CHECK(pTask,return FALSE);

	/*Get The Ref Number*/
	long refNumber = 0,refCount = 0;
	//if((nTag & TAG_SRC) != 0)
	{
	  ENTRY_LIST &list = parse.GetByPropertyName("src");
	  refNumber = list.size();
	}
	//if((nTag & TAG_URL) != 0)
	{
	  ENTRY_LIST &list = parse.GetByPropertyName("href");
	  refNumber += list.size();
	}

	URL_STRING urlstr;
	URL_STRING strPath;
	//if((nTag & TAG_SRC) != 0)
	{
		
	    ENTRY_LIST &list = parse.GetByPropertyName("src");
		ENTRY_LIST::iterator _end = list.end();
		for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
		{	
            TagEntry &tagEn = *it;
			THREAD_EXIT_CHECK(pTask,return FALSE);
		
			urlstr = tagEn.m_strTagText;

			 
			//ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
			//urlstr.ReleaseBuffer();

			if(urlstr.GetLength() <= 3)
				continue;

			if(IsInpageLink(urlstr))//页面内链接
				continue;

			//urlstr.MakeLower(); //all the lower case
			ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
			urlstr.ReleaseBuffer();

			if(IsRefLink(urlstr)) // 相对路径。。。。
				urlstr = ComposeRefURL(urlObj->GetURL(),&urlstr);

			if(FALSE == IsRightLink(urlstr))
			{
				tagEn.ModifyText(" ");
				continue;
			}

			_URL obj(urlstr);
			if(obj.m_UrlTagType != TAG_UNKNOW/* &&  (nTag & TAG_SRC) != 0*/)//src 里面避免下到html类型的文件
			{
				if( AlreadInList(urlSrcsList,urlstr))
					continue;
				
				urlSrcsList.push_back(urlstr);
				
				//TRACE(":%s\n",urlstr);
				if(AssertUrlHttpStr(urlstr) == FALSE)
					urlstr.Insert(0,"http://");

				URL_STRING  validUrl;
				ExtractValidUrl(&urlstr,&validUrl);//for 带参数的url.....

				strPath = pSession->GetUrlLocalUrl( urlObj->GetURL(),validUrl);
				tagEn.ModifyText(strPath);
				strPath.Empty();
			}
		
			/*************progress***************/
			refCount++;
			pWebTask->m_prgCtrl.SetProgressVal(100 * refCount/refNumber);
			
		}
		
	}

/*maybe here can select url reference urser config zxy*/
 	 
    ENTRY_LIST &list = parse.GetByPropertyName("href");
	ENTRY_LIST::iterator _end = list.end();
	for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
	{
		
		TagEntry &tagEn = *it;
		THREAD_EXIT_CHECK(pTask,return FALSE);
	
		urlstr = tagEn.m_strTagText;

		//ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
		//urlstr.ReleaseBuffer();
		//urlstr.MakeLower();

		if(urlstr.GetLength() <= 3)
			continue;
		if(IsInpageLink(urlstr))//页面内链接
			continue;
		
		//urlstr.MakeLower();
	
		ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
		urlstr.ReleaseBuffer();
		 

		TRACE("*%s\n",urlstr);
		if(IsRefLink(urlstr)) // 相对url的话，先生成完整的url
			urlstr = ComposeRefURL(urlObj->GetURL(),&urlstr);

		if(FALSE == IsRightLink(urlstr))
		{
			tagEn.ModifyText(" ");
			continue;
		}
	    _URL Obj(urlstr);
		if(Obj.m_UrlTagType == TAG_UNKNOW) //
			continue;
		if(AlreadInList(urlList,urlstr) == FALSE)
		{
			BYTE nLay = urlObj->GetLay();
			URL_STRING strUrl = urlstr;
			urlstr.Insert(0,(char)++nLay); //put lay info at begining of string....
			urlList.push_back(urlstr);//保存完整的有效url，用以下载。。。
			urlstr.Delete(0,1);
		}
		else
			continue;

		//if(IsUntLink(urlstr))//是跳转的url的话，抽出第二个跳转链接
		{
			URL_STRING  str;
			ExtractValidUrl(&urlstr,&str);
			urlstr = str;	
		}
		
		if(AssertUrlHttpStr(urlstr) == FALSE)
			urlstr.Insert(0,"http://");
	 //当前前是下载 url状态。。或者。。。网页上得url已经进入下载流程。。。。
		{
			strPath = pSession->GetUrlLocalUrl( urlObj->GetURL(),urlstr);
			tagEn.ModifyText(strPath);
			strPath.Empty();
		} 
	/*************progress***************/
		refCount++;
		pWebTask->m_prgCtrl.SetProgressVal(100 * refCount/refNumber);
			
	}
	pWebTask->m_prgCtrl.SetProgressVal(100);
	
	parse.SaveToFile(urlObj->m_hstSavePath);
	//parse.FreeAll();

	return TRUE;
}



//验证是不是带有 “http”协议头
BOOL AssertUrlHttpStr(LPCSTR url)
{
	CString  httpStr1 = "http://";
	CString  httpStr2 = "http:\\";
	CString  httpTpStr;
	if(strlen(url) < httpStr1.GetLength())
		return FALSE;
	
	httpTpStr.SetString(url,httpStr1.GetLength());
	//httpTpStr.MakeLower();


	return  (httpTpStr.Compare(httpStr1) == 0) || (httpTpStr.Compare(httpStr2)==0);
}

BOOL WayIsFileOf(CString &strPath,LPCSTR lpExtStr)
{
	CString  strExt =  ::PathFindExtension(strPath);
	int len = strExt.GetLength();
	if(len <= 0)
		return FALSE;
	for(int i = 0;i < len ;i++)
	{
		if(_ismbblead(strExt[i]))
			return FALSE;
	}
	strExt.MakeLower();
	//ToLowerAlphas(strExt.GetBuffer(),strExt.GetLength());
	//strExt.ReleaseBuffer();
	
	if(strExt.IsEmpty()) return FALSE;
	if(strExt.Compare(lpExtStr) == 0)
		return TRUE;

	return FALSE;
}

HRESULT  ConnectTest(CWnd *pWnd,CString Url,CONNECT_INF &CInf)
{	
	if(!pWnd)return FALSE;

	CString		strAgent;
	CString		strHeader;
	CString     strTextUrl;
	DWORD		dwFlags;
	DWORD		dwFSize;
	HINTERNET	inetHandle;
	HINTERNET   hConnect;

	//check connect option and open url
	InternetGetConnectedState(&dwFlags, 0);//得到连接状态..是否使用代理
	AfxParseURL(Url,CInf.serverType,CInf.nameServer,CInf.nameObject,CInf.port);
	
	strAgent.Format("Agent :%ld",timeGetTime());
	strHeader = "Accept: text/*\r\n\r\n";
	
	if(dwFlags & INTERNET_CONNECTION_PROXY)
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	else
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);

	strTextUrl = CInf.nameServer;//仅下载默认的首页。。
	strTextUrl.Insert(0,"http://");
	
		 
	if(!inetHandle)
		return INET_INIT_FAILURE;

	//之下载一个网页文本进行测试。。不包含 文件。。。
	CInf.respondTime = timeGetTime();
	if ( !(hConnect = InternetOpenUrlA ( inetHandle, strTextUrl, strHeader,
		strHeader.GetLength(), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
		return URL_OPEN_FAILURE;
	CInf.respondTime =(timeGetTime() - CInf.respondTime);

	
	CStdioFile	cFile;
	CString		tpFile;
	
	DWORD		dwReadSize;
	TCHAR		tdBuf[1028];
	DWORD		timeStart = 0;
	DWORD		timeUse = 0;
	BOOL		enable = 0;

	DWORD		dwByteToRead = 0;
	DWORD		dwSizeOfRq = 4;
	DWORD		dwBytes = 0;


	if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}

#ifdef _DEBUG
	tpFile = "d://test.tmp";
#else
	tpFile = GetTempFile(NULL,"~tmp","tst");
#endif

	if(!cFile.Open(tpFile,CStdioFile::typeBinary | 
			CStdioFile::modeCreate|CStdioFile::modeWrite))
		return BUFF_FILE_FAILURE;
	

	timeStart = timeGetTime();
	while(1)
	{
		InternetReadFile(hConnect,tdBuf,1024,&dwReadSize);
		cFile.Write(tdBuf,dwReadSize);
		pWnd->SendMessage(WM_DOWNPROGRESS,dwByteToRead,cFile.GetLength());
		
		if(dwReadSize != 1024)
			break;
	}
	timeUse = timeGetTime() - timeStart;

	if(timeUse == 0)
		timeUse = 10;
    
	CInf.bitRat = (float)8*cFile.GetLength() * 1000.0f / (float)timeUse;
	CInf.useTime = timeUse;
	CInf.downSize = cFile.GetLength();

	cFile.Close();
#ifndef _DEBUG
	DeleteFile(tpFile);
#endif
	InternetCloseHandle(inetHandle);
	return S_OK;
}

/*
为使用 internet  api做准备。。。
主要进行选项设置。。。这里进行代理服务器设置。。。explore5时
*/

#define  CASE_MSG(T) case T:\
	TRACE("error message %s\n",#T);
	
HINTERNET OpenInternetUrl(HINTERNET hInternet,URL_STRING *pStr ,DWORD &dwError)
{
	HINTERNET	hConnect ;
	URL_STRING	strHeader = "Accept: */*\r\n\r\n";
	hConnect = InternetOpenUrlA ( hInternet, pStr->GetBuffer(),strHeader.GetBuffer() ,
		strHeader.GetLength(), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE , 0);
	dwError = 0;
	if(hConnect == NULL)
	{
		//DWORD dwError;

		dwError = GetLastError();
		TRACE("err:%d \n",dwError);

		
		switch(dwError)
		{
		
			
			CASE_MSG(ERROR_HTTP_COOKIE_DECLINED) 
			break;

			CASE_MSG(ERROR_HTTP_COOKIE_NEEDS_CONFIRMATION)
			break;
		
			CASE_MSG(ERROR_HTTP_DOWNLEVEL_SERVER)
			break;

			CASE_MSG(ERROR_HTTP_HEADER_ALREADY_EXISTS)
			break;

			CASE_MSG(ERROR_HTTP_HEADER_NOT_FOUND)
			break;

			CASE_MSG(ERROR_HTTP_INVALID_HEADER)
			break;

			CASE_MSG(ERROR_HTTP_INVALID_QUERY_REQUEST)
			break;

			CASE_MSG(ERROR_HTTP_INVALID_SERVER_RESPONSE)
			break;

			CASE_MSG(ERROR_HTTP_NOT_REDIRECTED)
			break;

			CASE_MSG(ERROR_HTTP_REDIRECT_FAILED)
			break;

			CASE_MSG(ERROR_HTTP_REDIRECT_NEEDS_CONFIRMATION)
			break;

			CASE_MSG(ERROR_INTERNET_ASYNC_THREAD_FAILED)
			break;

			CASE_MSG(ERROR_INTERNET_BAD_AUTO_PROXY_SCRIPT)
			break;

			CASE_MSG(ERROR_INTERNET_BAD_OPTION_LENGTH)
			break;

			CASE_MSG(ERROR_INTERNET_CANNOT_CONNECT)
			break;

			CASE_MSG(ERROR_INTERNET_CONNECTION_ABORTED)
			break;

			CASE_MSG(ERROR_INTERNET_CONNECTION_RESET)
			//maybe need reset connect here !!!
			break;
		default:
			TRACE("error message : else error %d\n",dwError);
			break;
		}
	}	
	pStr->ReleaseBuffer();	
	strHeader.ReleaseBuffer();
	return hConnect;
}

HINTERNET CreateInternetHandle(DWORD &dwError)
{
    HINTERNET	inetHandle;
	DWORD		dwFlags;
	CString     strAgent;
	
	dwError = NULL;
	//check connect option 
	InternetGetConnectedState(&dwFlags, 0);//得到连接状态..是否使用代理
	strAgent.Format("Firefox Agent :%ld",timeGetTime());


	//here i will decide  how to process proxy configure.....
	//has two mode : global mode,or private mode.... 
	if(dwFlags & INTERNET_CONNECTION_PROXY)
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	else
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);

	if(inetHandle == NULL)
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if(inetHandle == NULL)
		dwError =	GetLastError();
	return inetHandle;
}


HRESULT SetPrivateConfInternet(HINTERNET hNet,PRIVATE_CONFIG *pPrConfig)
{


	if(pPrConfig->bEnableProxy)
	{
		char *pbuf ;
		
		pbuf = pPrConfig->strProxyName.GetBuffer();
		InternetSetOption(hNet, INTERNET_OPTION_PROXY,
			pbuf, DWORD(pPrConfig->strProxyName.GetLength())+1);
		pPrConfig->strProxyName.ReleaseBuffer();


		pbuf = pPrConfig->strUserName.GetBuffer();
		InternetSetOption(hNet, INTERNET_OPTION_PROXY_USERNAME,
			pbuf, DWORD(pPrConfig->strUserName.GetLength())+1);
		pPrConfig->strUserName.ReleaseBuffer();

		pbuf = pPrConfig->strPassWord.GetBuffer();
		InternetSetOption(hNet, INTERNET_OPTION_PROXY_PASSWORD,
			pbuf, DWORD(pPrConfig->strPassWord.GetLength())+1);
		pPrConfig->strPassWord.ReleaseBuffer();

		InternetSetOption(hNet, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
		InternetSetOption(hNet, INTERNET_OPTION_REFRESH , NULL, 0);
	}

	return S_OK;
}


/*Beginning with Internet Explorer 5, Internet options can be set for on a specific
connection. Previously, all connections shared the same Internet option */

HRESULT SetSysConfInternet(HINTERNET hnet,SYSTEM_CONFIG *pSysConfig)
{

#if 0
	BOOL bRet ;
	if(pSysConfig == NULL)
		return S_FALSE;

	INTERNET_PER_CONN_OPTION_LIST list;
	INTERNET_PER_CONN_OPTION Option[2];
    DWORD   dwBufSize = sizeof(list);

	memset(&list,0,sizeof(INTERNET_PER_CONN_OPTION_LIST));
    list.dwSize = dwBufSize;
    list.pszConnection = NULL;

    list.dwOptionCount = 2;
    list.pOptions = Option;//new INTERNET_PER_CONN_OPTION[2];

    if(NULL == list.pOptions)
        return FALSE;
  
	list.pOptions[0].dwOption = INTERNET_OPTION_CONNECT_TIMEOUT;
    list.pOptions[0].Value.dwValue = GA_dwWaitTimeArray[pSysConfig->dwWaitTime];
    
    list.pOptions[1].dwOption = INTERNET_OPTION_CONNECT_RETRIES;
    list.pOptions[1].Value.dwValue = GA_dwConnectRepondsArray[pSysConfig->dwConnectTimes];


	bRet = InternetSetOption(hnet,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

  //  delete [] list.pOptions;

	
	if(bRet == FALSE)
		AfxMessageBox("Use Session Config Faulure!",MB_OK|MB_ICONEXCLAMATION );

	InternetSetOption(hnet, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(hnet, INTERNET_OPTION_REFRESH , NULL, 0);
#endif
	BOOL bRet ;
	if(pSysConfig == NULL)
		return S_FALSE;


    DWORD  timeOut, conTimes;
	timeOut = GA_dwWaitTimeArray[pSysConfig->dwWaitTime];
	conTimes = GA_dwConnectRepondsArray[pSysConfig->dwConnectTimes];

	bRet = InternetSetOption(hnet,INTERNET_OPTION_CONNECT_TIMEOUT, &timeOut    ,sizeof(DWORD) );
	if(bRet == FALSE)
		AfxMessageBox("User Session Config Faulure!",MB_OK|MB_ICONEXCLAMATION );
	bRet = InternetSetOption(hnet,INTERNET_OPTION_CONNECT_RETRIES, &conTimes  , sizeof(DWORD));
	if(bRet == FALSE)
		AfxMessageBox("User Session Config Faulure!",MB_OK|MB_ICONEXCLAMATION );

	InternetSetOption(hnet, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(hnet, INTERNET_OPTION_REFRESH , NULL, 0);

	return S_OK;
}

BOOL  VerifyFile(LPCSTR strFile)
{
	if(NULL == strFile)
			return FALSE;
	CFile  cfile/*(strFile,CFile::modeRead)*/;
	
	TRY
	{
		BOOL bOpen = cfile.Open(strFile,CFile::modeRead);

		if(!bOpen)
			return FALSE;
		if(cfile.GetLength() <= 0)
		{
			cfile.Close();
			return FALSE;
		}
		cfile.Close();
	}
	CATCH(CFileException ,e)
	{
		e->ReportError();
		return FALSE;
	}
	END_TRY
	return TRUE;
}
//static
void RunChmCompiler(CString  strTaskfName)
{ 
	ChmCompiler  compiler;
	compiler.CompilerExec(strTaskfName);
}

