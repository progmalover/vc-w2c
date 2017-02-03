#include "stdafx.h"
#include "httpapi.h"
#include "Afxinet.h"
#include "ConfigManager.h"
#include ".\httpapi\httpapi.h"
#include ".\httpapi\filepath.h"
#include "TWebTask.h"
#include "ui.h"
#include "comrun.h"
#include "hmcompiler.h"
#include  "Defs.h"
#include "historylist.h"
#include "Waitdlg.h"
#include "particular.h"
#include "ShellLink.h"
#include ".\tanchormanager.h"
#include "defnamemanager.h"
#include "DlgFailureUrls.h"

using namespace ui;
#define  EXIT_NO_INIT_THREADS   9
#define  EXIT_COMPLEATE_WORKS	10
#define  EXIT_USER_FINISH		11


/*
URL DOMAIN
*/

_URL_DOMAIN:: _URL_DOMAIN(URL_STRING *pstrUrl)
{
	m_Url = *pstrUrl;
	ParseUrlDomain();
}
_URL_DOMAIN::~_URL_DOMAIN(void)
{
	m_DmArray.clear();
	m_Url.Empty();
}
int _URL_DOMAIN:: GetDomainNums()
{
	return  m_DmArray.size();
}

BOOL _URL_DOMAIN:: GetDomainName( URL_STRING &strUrl,int stage)
{
	if(stage <0 ||stage> m_DmArray.size()-1)
		return FALSE;
	strUrl = m_DmArray[stage];
	return TRUE;
}

//将服务器 主机 域名 分级别 解出
BOOL _URL_DOMAIN:: ParseUrlDomain(void)
{
	INTERNET_PORT	port;
	URL_STRING		strServer,strObject;
	DWORD			dwServerType;

	if(m_Url.IsEmpty())
		return FALSE;

	if(m_DmArray.size())
		m_DmArray.clear();

	//解出服务器名
	AfxParseURL(m_Url,dwServerType,strServer,strObject,port);

	if(strServer.IsEmpty())
		return FALSE;

	for(int i = 0;;i++)
	{
		CString strDomain;
		AfxExtractSubString(strDomain,strServer,i,L'.');
		if(strDomain.IsEmpty())
			break;
		
		DOMAIN_ARRAY::iterator it;
	    
		it = m_DmArray.begin();
		m_DmArray.insert(it,strDomain);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*
url class
*/
//FILE_JPG = 0,FILE_GIF,FILE_PNG,FILE_BMP,FILE_JPEG,FILE_JFIF,FILE_ZIP,FILE_MP3,FILE_WMA,FILE_RM,FILE_SWF
//  其他的类型默认为 FILE_URL类型。。。。
#define  FILE_TYPE_NUMS   17
static struct {
	DWORD fType;
	TCHAR  *pfName;
} FTypeArray[FILE_TYPE_NUMS] = {
	{_URL::FILE_TEXT,_T(".txt")},
	{_URL::FILE_JPG,_T(".jpg")},
	{_URL::FILE_GIF,_T(".gif")},
	{_URL::FILE_GIF,_T(".ico")},
	{_URL::FILE_JPEG,_T(".jpeg")},
	{_URL::FILE_PNG,_T(".png")},
	{_URL::FILE_BMP,_T(".bmp")},
	{_URL::FILE_JFIF,_T(".jfif")},
	{_URL::FILE_ZIP,_T(".zip")},
	{_URL::FILE_ZIP,_T(".rar")},
	{_URL::FILE_MP3,_T(".mp3")},
	{_URL::FILE_WMA,_T(".wma")},
	{_URL::FILE_WMV,_T(".wmv")},
	{_URL::FILE_RM,_T(".rm")},
	{_URL::FILE_SWF,_T(".swf")},
	{_URL::FILE_PDF,_T(".pdf")},
	{_URL::FILE_CHM,_T(".chm")}

};
_URL ::_URL(URL_STRING &strUrl)
{
	//m_Url = strUrl;
	SetURL(strUrl);
}
_URL ::~_URL(void)
{
  m_hstSavePath.Empty();
  m_hstDirctory.Empty();
  m_Object.Empty();
  m_Url.Empty();
}

inline BOOL IsLayNumber(BYTE nLay)
{
	if(FIRST_LAY <= nLay && nLay <= END_LAY)
		return TRUE;
	return FALSE;
}

void _URL::SetURL(URL_STRING strUrl)
{
	TCHAR *pBuf = strUrl.GetBuffer();
	strUrl.ReleaseBuffer();

	m_nLay = (BYTE)pBuf[0];//判断的一个 字符是不是 lay number...

	if(IsLayNumber(m_nLay))
		pBuf++;
	else
		m_nLay = 0;

	m_Url = pBuf;
	/*
	Parse Url  here
	*/
	ParseUrl();
}

//内容在同一网络公司的网站上
BOOL _URL:: SameWebStation( _URL &_url)
{
	//进行域名比较判断是否同一网站主机 :同一个网站主机 至少2级 域名是一至 的
	_URL_DOMAIN  _url1(&m_Url),_url2(&_url.m_Url);
	URL_STRING  urlstr1,urlstr2;
	
	if(_url1.GetDomainNums()< 2)
		return FALSE;
	if(_url2.GetDomainNums()< 2)
		return FALSE;
	
	if(_url1.GetDomainNums() != _url2.GetDomainNums())
		return FALSE;

	//root domain
	_url1.GetDomainName(urlstr1,0);
	_url2.GetDomainName(urlstr2,0);
	if(urlstr1.Compare(urlstr2) != 0)
		return FALSE;


	//second domain
	_url1.GetDomainName(urlstr1,1);
	_url2.GetDomainName(urlstr2,1);
	if(urlstr1.Compare(urlstr2) != 0)
		return FALSE;

	return TRUE;
}
/*内容在同一主机上，domain要完全相等*/
BOOL _URL:: SameWebHost( _URL &_url)
{
	//进行域名比较判断是否同一网站主机 :同一个网站主机 至少2级 域名是一至 的
	_URL_DOMAIN  _url1(&m_Url),_url2(&_url.m_Url);
	URL_STRING  urlstr1,urlstr2;

	if(_url1.GetDomainNums()< 2)
		return FALSE;
	if(_url2.GetDomainNums()< 2)
		return FALSE;
	
	for( int i = 0;i < _url1.GetDomainNums();i++)
	{
 
		_url1.GetDomainName(urlstr1,i );
		_url2.GetDomainName(urlstr2,i );
		if(urlstr1.Compare(urlstr2) != 0)
			return FALSE;
	}

	return TRUE;
}

URL_STRING * _URL:: GetURL()
{

	/*这里返回的是应该已经经过解析了的，,比较“规整，干净的”url*/
	return  &m_Url;
}


BOOL _URL::IsEmpty(void)
{
	return m_Url.IsEmpty();
}

void _URL::Empty(void)
{
	m_Url.Empty();
}

void _URL::SetHstFileName(PATH_STRING & strPath)
{
	m_hstSavePath = strPath;
}

  
void _URL::ParseUrl(void)
{

	INTERNET_PORT	port;
	URL_STRING		strServer,strObject;
	DWORD			dwServerType;

	//解出服务器名
	AfxParseURL(m_Url,dwServerType,strServer,strObject,port);


	m_UrlFileType = FILE_URL;
	m_UrlTagType =  TAG_UNKNOW; 

	m_UrlExtension = ::PathFindExtension(m_Url);

	if(m_UrlExtension.GetLength() <= 2)
		return ;

	if(strServer.IsEmpty() == false)
		m_UrlTagType =  TAG_URL; 

//	m_UrlExtension.MakeLower();
	ToLowerAlphas(m_UrlExtension.GetBuffer(),m_UrlExtension.GetLength());
	m_UrlExtension.ReleaseBuffer();

	const int   HTML_TYPE_NUM  = 6;
	const TCHAR *htmlFileType[]={
		_T(".htm"),_T(".html"),_T(".shtml"),_T(".asp"),_T(".aspx"),_T(".php")
	};
	 
    for( int i = 0;i < HTML_TYPE_NUM;i++)
	{
		if(m_UrlExtension.Compare(htmlFileType[i]) == 0)
		{
			m_UrlTagType = TAG_URL; 
			return ;
		}
	}

	
	/*if( 
		str.CompareNoCase(".jpg") == 0
		||str.CompareNoCase(".gif") == 0
		||str.CompareNoCase(".png") == 0
		||str.CompareNoCase(".bmp") == 0
		||str.CompareNoCase(".jpeg")== 0
		||str.CompareNoCase(".jfif")== 0
	  )*/
	for(int i = 0 ;i < FILE_TYPE_NUMS;i++)
	{
		if( m_UrlExtension.Compare(FTypeArray[i].pfName) == 0)
		{
			m_UrlTagType = TAG_SRC; 
			m_UrlFileType = FTypeArray[i].fType;
			return ;
		}
	}

	//m_UrlTagType = TAG_UNKNOW; 
	//m_UrlFileType = TAG_UNKNOW;
}

void _URL::CopyObj(_URL & obj)
{
//	URL_STRING  m_Url;/*保存的原始url,此类中的*/
//	URL_STRING  m_Domain;//主域名
//	URL_STRING  m_Object;//请求对象名
//
//	
//	DWORD		m_dwSize;	  //文件的尺寸
//	DWORD		m_dwReadSize; //已经得到的尺寸
//
//	DWORD       m_nServerType;
//	WORD		m_nPort;
//	
//	BYTE	    m_nLay;
//	DWORD		m_UrlTagType;
//	DWORD		m_UrlFileType;
//	URL_STRING  m_UrlExtension;
//	URL_STRING	m_hstDirctory;//主机的路径
//	URL_STRING  m_hstSavePath;//下载的保存的文件路径
	m_Url = obj.m_Url;
	m_Domain = obj.m_Domain;
	m_Object = obj.m_Object;
	m_dwSize = obj.m_dwSize;
	m_dwReadSize = obj.m_dwReadSize;
	m_nServerType = obj.m_nServerType;
	m_nPort = obj.m_nPort;
	m_nLay = obj.m_nLay;
	m_UrlTagType = obj.m_UrlTagType;
	m_UrlFileType = obj.m_UrlFileType;
	m_UrlExtension = obj.m_UrlExtension;
	m_hstDirctory = obj.m_hstDirctory;
	m_hstSavePath = obj.m_hstSavePath;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
URL_TASK
*/
_URL_TASK::THREAD_STARTUP  _URL_TASK ::  _StartUp;
//BOOL Create(void);

BOOL _URL_TASK :: Create(_URL_TASK *pParent)
{

	/*if(!pParent)
		return FALSE;*/
	
	memset(&_StartUp,0,sizeof(THREAD_STARTUP));
	_StartUp.pTask = this;
	_StartUp.hEvent = ::CreateEvent(0,TRUE,FALSE,_T("event1"));

	m_hThread = CreateThread( 
		NULL,                        // default security attributes 
		0,                           // use default stack size  
		( LPTHREAD_START_ROUTINE )TaskEntry,                  // thread function 
		(LPVOID)&_StartUp,                // argument to thread function 
		 /*CREATE_SUSPENDED*/THREAD_GET_CONTEXT  ,                           // use default creation flags 
		&m_dwThreadID);

	m_pParent = pParent;
	m_dwThreadStatus = THREAD_FREE;

	ASSERT(m_hThread != NULL);

	::WaitForSingleObject(_StartUp.hEvent,INFINITE);//这里确定工作线程已经进入run()
	::CloseHandle(_StartUp.hEvent);
	
	m_trdObj.Attach(m_hThread);
	return  (m_hThread != NULL);
}

_URL_TASK ::_URL_TASK(void)
{
	m_dwThreadStatus = _URL_TASK::THREAD_FREE ; // 初始 状态
	memset(&m_DownStatus,sizeof(TASK_DOWN_STATUS),0);
}

_URL_TASK::~_URL_TASK(void)
{
	//Destroy();
}

DWORD _URL_TASK::GetTaskStatus(void)
{
	return m_dwThreadStatus;
}

BOOL _URL_TASK::SetURLObj(_URL & urlObj)
{
	memset( &m_DownStatus,0,sizeof(m_DownStatus));
	m_UrlObj.CopyObj(urlObj);	
	return TRUE;
}

BOOL _URL_TASK::SetURL(URL_STRING uStr)
{
	if(uStr.IsEmpty()) return FALSE;
	//m_UrlObj.SetURL(uStr);
	memset( &m_DownStatus,0,sizeof(m_DownStatus));
	m_UrlObj.SetURL(uStr);
	TRACE("%s\n",uStr);
	
	return TRUE;
}

BOOL _URL_TASK::Start()
{
	if(m_hThread == NULL)
		return FALSE;
	m_dwThreadStatus = _URL_TASK::THREAD_RUNING ;
	ResumeThread(m_hThread);
	return TRUE;
}

BOOL _URL_TASK::Stop()
{
	if(m_hThread == NULL)
		return FALSE;
	m_dwThreadStatus = _URL_TASK::THREAD_SUSPEND ;

	return TRUE;
}
void _URL_TASK ::ForceTerminate()
{
	TerminateThread(this->m_hThread,0x99);
}
BOOL _URL_TASK::Destroy(void)
{
	DWORD  status;
	URL_STRING str;
	status = m_dwThreadStatus;
    m_dwThreadStatus = _URL_TASK::THREAD_EXIT;
	
	if(m_hThread)
	{
	try{
		DWORD dwExitCode;
		if(GetExitCodeThread(m_hThread,&dwExitCode)== FALSE)
		{
			DWORD err = GetLastError();
#ifdef _DEBUG
			TRACE("\n***************Error Code:%x \n!!",err);
#endif
			return FALSE ;
		} 
		if(dwExitCode == STILL_ACTIVE)
		{
			
			while(ResumeThread(m_hThread));
			
			DWORD dw = ::WaitForSingleObject(m_hThread,2000);
			if(dw != WAIT_OBJECT_0)
				TerminateThread(m_hThread,0x99);
		}
		
		}catch(...){}
	}
	return TRUE;
}

static void Exit()
 {
	ExitThread(0xabc);
 };
 void _URL_TASK::SafExit(void)
 {
	 this->SetTaskStatus(THREAD_EXIT);
	 CONTEXT _context;
	 _context.ContextFlags = CONTEXT_ALL;
	 GetThreadContext(m_hThread,&_context);
	 
	// _context.SegCs =  _TrdSafeStartContext.SegCs;
	 _context.Eip = (DWORD) &Exit;
	 SetThreadContext(m_hThread,&_context);
	 while(::ResumeThread(m_hThread));

	// this->ForceDestroy();
 }

////////////////////////////////////////////////////////////////////////////////////////////
/*
Task Session
*/

BOOL TWebSession::SaveUrlLocalPath(PATH_STRING & strPath)
{
	m_DirCollection.m_pathList.push_back(strPath);
	return TRUE;
}
TWebSession::TWebSession(void)
{  
//	m_Status = TWebSession::TASK_COMPLETE;
	m_Model = NONE_MODE;
	m_bInitThreads = FALSE;
	m_bSaved = FALSE;
	m_pSysConf = NULL;
	m_pConf = NULL;

   downFileSize = 0;
   downAllFileSize = 0;
   downFilesNum = 0;
   downHtmlNum = 0;
   bSucces = FALSE;
	//m_bDisplay = TRUE;
}
TWebSession::TWebSession(URL_STRING &strUrl)
{
	m_Model = NONE_MODE;
	m_bInitThreads = FALSE;
	m_bSaved = FALSE;

	downFileSize = 0;
	downAllFileSize = 0;
	downFilesNum = 0;
	downHtmlNum = 0;
	bSucces = FALSE;

	m_pSysConf = NULL;
	m_pConf = NULL;
	SetRootUrl(strUrl);
	m_bSaved = FALSE;

	TxtCompiler::Instance();
	 
}

TWebSession::~TWebSession(void)
{
	/*TASK_Destroy(); 
	for(TASK_LIST ::iterator it = m_UrlTaskList.begin();it != m_UrlTaskList.end();it++)
	{
		delete  (*it);
	}
	m_UrlTaskList.clear();*/

	if(m_UrlRefFile.m_pStream)
	{
		m_UrlRefFile.Close();
		
		DeleteFile(m_UrlRefFile.m_filePathName);
	}	
	if(m_UrlRefSrcFile.m_pStream)
	{
		m_UrlRefSrcFile.Close();
		DeleteFile(m_UrlRefSrcFile.m_filePathName);
	}
	//if(PathFileExists(m_UrlRefSrcFile.m_filePathName))
	//	
	//if(PathFileExists(m_UrlRefFile.m_filePathName))
	//	
	TxtCompiler::Uninstance();
}

void TWebSession::TASK_Start(void)
{	
	_URL_TASK::Start();

	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
	it != m_UrlTaskList.end();it++)
	{
		_URL_TASK  &ut = *(*it);
/*只是被挂起来的线程被启动*/
		if(ut.GetStatus() == THREAD_SUSPEND) 
		ut.Start();
	}

}

void TWebSession::TASK_Stop(void)
{
	_URL_TASK::Stop();
	
	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		it != m_UrlTaskList.end();it++)
	{
		_URL_TASK  &ut = *(*it);
		if(ut.GetStatus()  != THREAD_SUSPEND)
		ut.Stop();
	}
}

/*
task exit
*/
void TWebSession::TASK_Destroy(void)
{
   if(GetStatus() == THREAD_EXIT)
   return ;
   
   SetTaskStatus(THREAD_EXIT);
   for( TASK_LIST::iterator it = m_UrlTaskList.begin();
	   it != m_UrlTaskList.end();it++)
   {
		_URL_TASK *pTask = *it;
		pTask->SetTaskStatus(_URL_TASK::THREAD_EXIT);
   }

   _URL_TASK::Destroy();
	//Unprepare();	
}
inline BOOL  AllThreadFree(TASK_LIST &taskList)
{
	for(TASK_LIST::iterator it = taskList.begin();it != taskList.end();it++)
	{
		TWebTask *ptsk = (TWebTask *)*it;
		if(ptsk->GetStatus() != _URL_TASK::THREAD_FREE
			/*&&ptsk->m_stdFile.m_pStream != NULL*/)
			return FALSE;
	}

	return TRUE;
}

inline BOOL AllThreadBusy(TASK_LIST &taskList)
{
	for(TASK_LIST::iterator it = taskList.begin();it != taskList.end();it++)
	{
		_URL_TASK *ptsk = *it;
		if(ptsk->GetStatus() != _URL_TASK::THREAD_RUNING)
			return FALSE;
	}

	return TRUE;
}

void TWebSession::SaveToHistory(void)
{
	if(m_bSaved) return ;

	SessionInf		sinf;
	sinf.m_rootUrl  =  *GetRootUrlObj().GetURL();

	if(sinf.m_rootUrl.IsEmpty())
		sinf.m_rootUrl = "Has No Root URl";
	sinf.m_TaskFile = GetTaskPathName();
	sinf.m_TaskName = GetTaskName();
	CHistoryList::Instance()->AddToList(&sinf);
	
	m_bSaved = TRUE;
}

#include "htmlparse.h"
#ifndef _DEBUG
#include "Register.h"
#endif
BOOL bIsFailureRetry;
void TWebSession::Run(void)
{
	
	
	Prepare();//不对称调用。。。

#if  0
	static BOOL  bNoReged = HasNoRegKey();
	if(bNoReged)
	{
		m_pConf->dwMaxFileNum  = 100;
		m_pConf->dwMaxFileSize = 1 ;
		m_pConf->dwMaxSize = 10;
		m_pConf->bEnableLimit = TRUE;
	}
#endif
   //if(bNoReged)

  
   if(!m_bInitThreads)
   {
	   ExitThread(EXIT_NO_INIT_THREADS);
	   return ;
   }
   
   TRACE("TWebSession:Run()\n");
  
   CComRun  comObj;

   bIsFailureRetry = FALSE;
LStart:
//   m_dwThreadStatus = _URL_TASK::THREAD_RUNING;
   while(m_dwThreadStatus != TWebSession::THREAD_COMPLETE &&
	   m_dwThreadStatus != TWebSession::THREAD_EXIT)
   {

	   if(m_dwThreadStatus == TWebSession::THREAD_SUSPEND) //已经设置停止工作的状态
	   {
		  SuspendThread(m_hThread);
		  goto LStart;
	   }
	  
	   /*
		  检查 线程列表看是否有完成下载的线程，
		  然后检查url列表，看是否有url未下。。。。。
	   */

	   THREAD_EXIT_CHECK(this,return);
	   for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		   it != m_UrlTaskList.end();it++)
	   {

		   THREAD_EXIT_CHECK(this,return);
		   _URL_TASK &Tsk = *(*it);
		   m_pCurrentTask = (_URL_TASK *)*it;

		   /*失败的话重新尝试,一直失败的话就放弃了 。。。*/
		  
		   if(Tsk.GetStatus() == _URL_TASK::THREAD_FAILURE)
		   {
			   Tsk.m_DownStatus.dwTryTimes++;
			   TRACE("FAILURE %d Times\n",Tsk.m_DownStatus.dwTryTimes);
			 //set retry times in  /*here maybe isnt safe !!!*/
			   if(Tsk.m_DownStatus.dwTryTimes > m_pSysConf->dwConnectTimes)
			   {
				   TWebTask *pTask = (TWebTask *) &Tsk;
				   
				   /***add to failure list here*****/
					ProcessTaskError(pTask);	

				   pTask->SetTaskStatus(_URL_TASK::THREAD_FREE);
				   pTask->UnPrepare();
				   pTask->SetURL(CString());
				   pTask->m_prgCtrl.UnPrepareProgress();
				  
			   }
			   else
				   Tsk.Start();

		   }
		   
            THREAD_EXIT_CHECK(this,return);
		   if(Tsk.GetStatus() == _URL_TASK::THREAD_COMPLETE) //挂起状态一般作为当前已无任务状态
		   {
			   /*
			   得到文件类型及路径。。如果是html则parse后放入prepareList;
			   */
			   TWebTask *pTask = (TWebTask *)&Tsk;

			  	_URL &obj = Tsk.GetURLObj();

			   pTask->UnPrepare();
			   if(m_pConf->bEnableLimit)
			   {
				   downFileSize = pTask->m_dwFileSize;
				   downAllFileSize += downFileSize;
			   }

			   switch(m_pConf->dwDownMode)
			   {

				  case  ALL_REF_IN_PAGE://&Download Html Page And All Relation Page
				   {
					    DWORD wantTag;
						DWORD nType;
						 
						if((obj.m_UrlFileType > _URL::FILE_JFIF) && IsHtmlTypeFile(obj.m_hstSavePath,nType))
						{ 
							downHtmlNum++;//纪录下载的文件 数量
							
							//if(dwDownLay < m_pConf->dwDownLay)
							if(obj.GetLay() < m_pConf->dwDownLay)
								wantTag = (TAG_SRC | TAG_URL);
							else
								wantTag = TAG_SRC;
							/*首先放入历史链表中*/
					
							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
															 _T("Parsing..."),0);
							/*parese新的链接*/
							if(nType == TYPE_ASP)
								ParseDynamicPage(obj,m_TmpRefList,m_TmpSrcList ,wantTag);
							else
								ParseStaticPage(obj,m_TmpRefList,m_TmpSrcList  ,wantTag); 

							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,_T("Complete"),100);

							THREAD_EXIT_CHECK(this,return);
							PutRefsToPrepareList( &Tsk,m_TmpRefList,m_TmpSrcList,m_pConf->dwDownMode,wantTag);

							///**********暂时将转换txt功能放在这里*************/
							//if(m_pSysConf->dwMode & 2)
							//{
							//	CString txtDir = TxtCompiler::GetTxtDirPath(m_TaskDirPath);
							//	if(txtDir.IsEmpty())continue;
							//	PrepareDirectory(txtDir,0);
							//	TxtCompiler::Instance()->DoCompiler( obj.m_hstSavePath,txtDir);
							//}
						}
						
					
						//pTask->UnPrepare();
						THREAD_EXIT_CHECK(this,return);
						
				   }
				   break;

				   case  ALL_REF_IN_WEB://&Download All The Relation Page In Web Sit
				   case  ALL_REF_IN_WEBHOST://&Download All The Relation Page In Web Host
				   case  ALL_REF_HAS_STRING:
				   {
						DWORD nType;
						
						if((obj.m_UrlFileType > _URL::FILE_JFIF) &&IsHtmlTypeFile(obj.m_hstSavePath,nType))
						{ 
							downHtmlNum++;//纪录下载的文件 数量

							/*首先放入历史链表中*/
							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
								_T("Parsing..."),0);
							/*parese新的链接*/
							if(nType == TYPE_ASP)
								ParseDynamicPage(obj,m_TmpRefList,m_TmpSrcList ,TAG_SRC | TAG_URL);
							else
								ParseStaticPage(obj,m_TmpRefList,m_TmpSrcList ,TAG_SRC | TAG_URL); 

							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
								_T("Complete"),100);
							THREAD_EXIT_CHECK(this,return);
							PutRefsToPrepareList( &Tsk,m_TmpRefList,m_TmpSrcList ,m_pConf->dwDownMode,TAG_SRC | TAG_URL);

							///**********暂时将转换txt功能放在这里*************/
							//if(m_pSysConf->dwMode & 2)
							//{
							//	CString txtDir = TxtCompiler::GetTxtDirPath(m_TaskDirPath);
							//	if(txtDir.IsEmpty())continue;
							//	PrepareDirectory(txtDir,0);
							//	TxtCompiler::Instance()->DoCompiler( obj.m_hstSavePath,txtDir);
							//}

						}
						//Tsk.SetTaskStatus(_URL_TASK::THREAD_FREE);//设置此线程空闲化。。。
						//pTask->UnPrepare();
						 THREAD_EXIT_CHECK(this,return);
						
				   }
				   break;
			   }

			   pTask->SetURL(CString());
			   pTask->m_prgCtrl.UnPrepareProgress();
		   }
	   }

#if 0
	   if(bNoReged)
	   {
		   if(downAllFileSize >= 10*1024*1024 && AllThreadFree(m_UrlTaskList))
			   goto _EXIT;
		   if(downFilesNum >= 100 && AllThreadFree(m_UrlTaskList) )
			   goto _EXIT;
	   }
#endif
	   if(m_pConf->bEnableLimit)
	   {
		   if(downAllFileSize >= enableMaxSize && AllThreadFree(m_UrlTaskList))
			   goto _EXIT;
		   if(downFilesNum >= m_pConf->dwMaxFileNum && AllThreadFree(m_UrlTaskList) )
			   goto _EXIT;
	   }

	   //AdjudgeStatus();
	   TRACE("***begin Test End !\n");

	   if(m_UrlRefFile.AtEnd() && m_UrlRefSrcFile.AtEnd() && AllThreadFree(m_UrlTaskList) && !bIsFailureRetry)
	   {

_EXIT:
			
		   if((CDlgFailureUrls::m_FailureUrlsList.size() || g_FailureUrlsList.size()) && !bIsFailureRetry) //for download has failured!.
		   {
			  CDlgFailureUrls  dlg;
			  dlg.DoModal();
			  if(dlg.m_dwStatus)
			  {
			    bIsFailureRetry = TRUE;
			    goto _JOBS;
			  }
		   };

		   TRACE("Session Exit!!"); 
		   this->SetTaskStatus(_URL_TASK::THREAD_EXIT);


		   if(downHtmlNum <= 0 && this->downFilesNum <= 0)
		   {
				AfxMessageBox(_T("Download Web Source Failure,Please Verify You Url,or Internet!"), MB_ICONERROR);
				RemoveFromSessionTree(this);  
				goto LEND;
		   }
		   else if( m_pSysConf->dwWorkMode == AUTO_MODE/*Auto Mode*/ && downHtmlNum <= 0)
		   {
			    AfxMessageBox(_T("Please Verify You Url,or Internet!"), MB_ICONERROR);
				RemoveFromSessionTree(this);  
				goto LEND;
		   }

		   OnFinish();
		   goto LEND;
	   }

       TRACE(" ***end Test End !\n");
	   /*
	   查找空闲的线程。。。重新分配新的任务
	   */
	   TRACE("***********The Task Nums:%d**********\n",m_UrlTaskList.size());
	
	   THREAD_EXIT_CHECK(this,return);

_JOBS:
	   for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		   it != m_UrlTaskList.end();it++)
	   {
		   _URL_TASK *pTsk = *it;
		   TRACE("task status:%d\n",pTsk->GetStatus());
			if(m_pConf->bEnableLimit)
			{
				if(downFilesNum >= m_pConf->dwMaxFileNum)
				break;
			}

			

#if  0 
			if(bNoReged)
			{
				if(downAllFileSize >= 10*1024*1024 
					|| downFilesNum >= 100)
					break;	
			}
#endif

		   if(pTsk->GetStatus() == _URL_TASK::THREAD_FREE || 
			   pTsk->GetStatus()== _URL_TASK::THREAD_FAILURE) //挂起状态一般作为当前已无任务状态
		   {


				if(bIsFailureRetry)
				{
					if(g_FailureUrlsList.size() <= 0)
					{
						bIsFailureRetry = 0;
					}
					else
					{
						_URL &obj = *g_FailureUrlsList.begin();
						pTsk->SetURLObj(obj);	

						g_FailureUrlsList.pop_front();
						bSucces = ((TWebTask *)pTsk)->Prepare();
						if(bSucces == S_OK)
							pTsk->Start();
						/*else
							CDlgFailureUrls::ReBkFailureUrl(pTsk->GetURLObj());*/
						
					}

					continue;
				}

			    URL_STRING  str ;
			LP:	
				
				THREAD_EXIT_CHECK(this,THREAD_EXIT);
				if(m_UrlRefSrcFile.AtEnd())
				{
					if(m_UrlRefFile.AtEnd())
					  break;
					else
						m_UrlRefFile.ReadFromBuffer(str);
				}else
					m_UrlRefSrcFile.ReadFromBuffer(str);

				THREAD_EXIT_CHECK(this,THREAD_EXIT);

				pTsk->SetURL(str);
				bSucces = ((TWebTask *)pTsk)->Prepare();
				
				downFilesNum++;
				if(bSucces == S_OK)
					pTsk->Start();
				
		   }
	   }

	    THREAD_EXIT_CHECK(this,THREAD_EXIT);
	    OnIdle();
	  ::Sleep(100);
   } 

LEND:;
 // UnPrepare();
  ExitThread(333);
   
}

URL_STRING &TWebSession:: GetTaskPathName(void)
{
  return m_TaskDirPath;
}
void TWebSession::SetTaskPathName(PATH_STRING &strPath)
{
	PATH_STRING strDir  = FileStripFileName(strPath.GetBuffer());
	strPath.ReleaseBuffer();
       
	m_DirCollection.SetRootPath(strDir);
	m_TaskDirPath = strPath;
	m_TaskName = strPath.Right( strPath.GetLength() - strPath.ReverseFind('\\') -1);
}
void TWebSession::SetRootUrl(URL_STRING & strUrl)
{
	if(m_UrlRefFile.m_pStream == NULL)
		m_UrlRefFile.Create();
	if(m_UrlRefSrcFile.m_pStream == NULL)
		m_UrlRefSrcFile.Create();

	if(strUrl.IsEmpty() == FALSE)
	{
		_URL obj(strUrl);
		if(obj.m_UrlTagType == TAG_SRC)
		    m_UrlRefSrcFile.WriteToBuffer(strUrl + _T("\n"));
		else
		{
			m_UrlRefFile.WriteToBuffer(strUrl + _T("\n"));
			m_RootUrl.SetURL(strUrl);
		}
	}
}

/*
解析新下载的html
*/

void TWebSession::ParseDynamicPage(_URL &urlObj,URL_LIST &urlRefs,URL_LIST &urlSrcs,int nTags)
{
   /*
	first extract the asp script moudle,and search the inline src
   */

	/*
	down the inline src ,and insert src to page,instead the url 
	*/

	ParseStaticPage(urlObj, urlRefs,urlSrcs,nTags);
}

void TWebSession::ParseStaticPage(_URL &urlObj,URL_LIST &urlRefs,URL_LIST &urlSrcs,int nTags)
{
	//URL_LIST  lst;
	
	if(nTags == 0)
		nTags = TAG_URL | TAG_SRC ;

	 
	GetHttpPageUrls((_URL_TASK *)this,&urlObj ,urlRefs,urlSrcs,m_pConf->dwDownMode,nTags);
	 
}

void TWebSession::OnFinish(UINT nCode)
{
	SaveToTaskFile();//save all status to tsk file...
	CreateRootPageQuickIcon();

	if(m_pSysConf->dwWorkMode == AUTO_MODE)
		return;

	DoConver(m_pSysConf->dwMode);

	if(m_pSysConf->dwMode == 0)/*no target*/
	{
		::PathRemoveFileSpec(m_TaskDirPath.GetBuffer());
		::ShellExecute(NULL, _T("open"),m_TaskDirPath, NULL, NULL, SW_SHOWNORMAL);
	}
	RemoveFromSessionTree(this);  
	SaveToHistory();
	AfxGetApp()->GetMainWnd()->PostMessage(IDM_CUT,0,0);//删除摘录的资源
	 
	if( nCode == 0) return ;
	if(this->m_pSysConf->bShutDown == FALSE ) return;

	CString WaitMsg;
	WaitMsg.LoadString(IDS_WAITFORSHUTDOWN);

	CWaitDlg Dlg(WaitMsg,30,AfxGetMainWnd());
	if(Dlg.DoModal()==IDOK)
	{
		::ShutDownWindows(GetSystemId());
	}

}

void TWebSession::Notify(int nStatCode)
{
	if(m_dwThreadStatus == _URL_TASK::THREAD_SUSPEND)
		_URL_TASK::Start();
	
}
 
PATH_STRING  TWebSession::GetUrlLocalUrl(URL_STRING *pRootUrl ,URL_STRING &strDesk)
{
	return m_DirCollection.UrlToLocalURL(pRootUrl,&strDesk);
}
PATH_STRING  TWebSession::GetUrlLocalPath(URL_STRING &strUrl,BOOL bAddRoot)
{
	return m_DirCollection.UrlToLocalPath(&strUrl,FALSE,bAddRoot);
}

void TWebSession:: EnableDisplay(BOOL bEnable)
{

}

BOOL TWebSession::IsRootURL(URL_STRING & strUrl)
{
	URL_STRING & rootUrl = m_RootUrl.m_Url; 
	if(rootUrl.GetLength() != strUrl.GetLength())
		return FALSE;

	rootUrl.MakeLower();
	strUrl.MakeLower();
	//ToLowerAlphas(rootUrl.GetBuffer(),rootUrl.GetLength());
	//rootUrl.ReleaseBuffer();

	//ToLowerAlphas(strUrl.GetBuffer(),strUrl.GetLength());
	//strUrl.ReleaseBuffer();

	return rootUrl.Compare(strUrl) == 0;
}

void  TWebSession::Save(void)/*保存当前的内容*/
{

}

void  TWebSession::Load(PATH_STRING &taskPath) /*载入以前存储的内容*/
{

}

BOOL TWebSession:: InitTaskList(int ntask)
{
	if(ntask <= 0)
		return FALSE;

	for(int i = 0;i < ntask;i++)
	{
		TWebTask * pweb_Task  = new TWebTask ;
		pweb_Task->Create(this);
		m_UrlTaskList.push_back(pweb_Task);
	}

	return TRUE;
}

BOOL TWebSession::Create(URL_STRING & strUrl, PATH_STRING & strPath, int ntrdNum)
{
	
	SetRootUrl(strUrl);
	SetTaskPathName(strPath);
	InitTaskList(ntrdNum);
	m_bInitThreads = TRUE;
	_URL_TASK::Create(this);

	this->SetTaskStatus(THREAD_SUSPEND);
	GP_TskPrgList->SetProgressSells(ntrdNum);
	m_pCurrentTask = NULL;
	return TRUE;
}

/*
根据当前用户设置和链接本身的类型决定 如何存放链接
*/
inline BOOL IsRightUrl(URL_STRING &url,DWORD nMode)
{
	if((nMode&TAG_SRC) && (nMode&TAG_URL) )
		return TRUE;
	
	
	return FALSE;
}

void TWebSession::PutToFileBuffer(URL_LIST  & list,int nMode,int nTag)
 {

	  URL_LIST &lst = list;
	/*
	根据用户的下载设置有选择的把一些url放入preparelist中，
	前提条件是：1）没有被下载 。。2）符合用户的下载设置
	*/
	DWORD Type;
	for(URL_LIST::iterator ita = lst.begin();ita != lst.end();ita++ )
	{
		URL_STRING  * pfindIt= NULL;
		URL_STRING  & urlStr = *ita;

		if(GetStatus() == THREAD_EXIT)
			break;
		
		//declare url obj
		_URL  obj(urlStr);

		if(IsInpageLink(*obj.GetURL()))
			continue;
		/*zxy  limit file type here ,maybe implement in other function*/
		if(!IsAllowFileType((_URL_TASK *)this,obj.m_UrlFileType))
			continue;

		CString validUrl;
		ExtractValidUrl(obj.GetURL(), &validUrl);
		URL_STRING  strPath = m_DirCollection.UrlToLocalPath(&validUrl,FALSE,TRUE);
		if(::PathFileExists(strPath) == TRUE)//已经存在不再下zai
			continue;

		/*没有在donelist中。。。进行域名级别验证。。然后载入prepare链表*/
		switch(nMode)
		{
		case ALL_REF_IN_PAGE ://all releation page 
			{ 
				if(obj.m_UrlTagType == TAG_SRC && ((nTag&TAG_SRC) == TAG_SRC))	
					m_UrlRefSrcFile.WriteToBuffer(urlStr + _T("\n"));
						

				if(obj.m_UrlTagType == TAG_URL && ((nTag&TAG_URL) == TAG_URL))
					m_UrlRefFile.WriteToBuffer(urlStr + _T("\n"));

			}
			break;
		case ALL_REF_IN_WEB://same webhost
			{
				_URL  ojbRoot(m_RootUrl);
			//	if(ojbRoot.SameWebHost(obj))
				{
					if(obj.m_UrlTagType == TAG_SRC)
						m_UrlRefSrcFile.WriteToBuffer(urlStr+ _T("\n"));
					else
					{
						if(ojbRoot.SameWebHost(obj))
							m_UrlRefFile.WriteToBuffer(urlStr+ _T("\n"));
					}
				}
			}
			break;
		case ALL_REF_IN_WEBHOST://same websit
			{
				_URL ojbRoot(m_RootUrl);
				//if(ojbRoot.SameWebStation(obj))
				{
					if(obj.m_UrlTagType == TAG_SRC)
						m_UrlRefSrcFile.WriteToBuffer(urlStr+ _T("\n"));
					else
					{
						if(ojbRoot.SameWebStation(obj))
							m_UrlRefFile.WriteToBuffer(urlStr+ _T("\n"));
					}
				}

			}
			break;
		case ALL_REF_HAS_STRING:
			{

				if(obj.m_UrlTagType == TAG_SRC)
						m_UrlRefSrcFile.WriteToBuffer(urlStr+ _T("\n"));
				else
				{
					//(ojbRoot.SameWebStation(obj))urlStr.Find(m_pConf->strIncPath,0) != -1
					if(_tcsstr(obj.m_Url,m_pConf->strIncPath) != NULL)
						m_UrlRefFile.WriteToBuffer(urlStr+ _T("\n"));
				}

			}break;
		}

	}
    lst.clear();

 }

void TWebSession::PutRefsToPrepareList(_URL_TASK  *purlTask,URL_LIST & urlRefList,URL_LIST & urlSrcList, DWORD nMode,DWORD nTag)
{

    URL_LIST &lst = urlRefList;
	if(lst.size() <= 0)
		return;

	_URL &obj = purlTask->GetURLObj();

	if(obj.GetLay() < m_pConf->dwDownLay
		&&m_pConf->bEnableEdit && ((nTag & TAG_SRC) == TAG_SRC))
	ui::SendMessageToMainFrame(	WM_URLS_EDIT,(WPARAM)&urlRefList,0);


	PutToFileBuffer(urlRefList,nMode,nTag);

	PutToFileBuffer(urlSrcList,nMode,nTag);
}

void TWebSession::OnIdle(void)
{
  
	if(GetStatus() == THREAD_EXIT)
	return;
//	URL_STRING validUrl;
//	ExtractValidUrl(m_RootUrl.GetURL(),&validUrl);//for 带参数的url.....
//	PATH_STRING  strPath = GetUrlLocalPath(validUrl);//get root page 

}

HRESULT TWebSession:: Prepare(void)
{
	

	m_pConf = ConfigManager::Instance()->GetSessionConfig((_URL_TASK *)this);
	m_pSysConf = ConfigManager::Instance()->GetSystemConfig();
	if(!m_pConf)
		m_pConf = ConfigManager::Instance()->GetDefaultSessionConf();

	if(m_pConf)
		SetPrivateConfInternet(/*m_hInterntClient*/NULL,m_pConf);
	if(m_pSysConf)
		SetSysConfInternet(/*m_hInterntClient*/NULL,m_pSysConf);

	CDefnameManager::ReleaseInstance();
	CDefnameManager::Instance();
	//CreateRootPageQuickIcon();
	ChmCompiler::_strLang.Empty();

	  /*
		纪录下载的文件数量和全部的文件大小。。。。 工作开始了。。。。。
   */

	enableMaxSize = 0;
	if(m_pConf->bEnableLimit)
		enableMaxSize  =  m_pConf->dwMaxSize * 1024 *1024;//至少是一兆。。

	this->downFilesNum = TAnchorManager::Instance()->GetPageCount();//初始化本地页面（已经采集下来的）
    return S_OK;
	
}

HRESULT  TWebSession::UnPrepare(void)
{
	
	UninitTaskList();
	
	GP_TskPrgList->Lock();
	GP_TskPrgList->ClearAllCellInf();
	GP_TskPrgList->UnLock();

	//if(m_UrlRefSrcFile.m_pStream)
	//	m_UrlRefSrcFile.Close();
	//	m_UrlRefFile.Close();
	return S_OK;
}

HRESULT TWebSession:: Exec()
{
	return S_OK;
}

void TWebSession::UninitTaskList(void)
{
	if(m_UrlTaskList.size()<= 0) return;
	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
				it != m_UrlTaskList.end();it++)
	{
		TWebTask * pTask =  (TWebTask *)*it;
		 
		pTask->Destroy();
		pTask->UnPrepare();
		pTask->m_prgCtrl.UnPrepareProgress();
	}

	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		it != m_UrlTaskList.end();it++)
	{
		TWebTask * pTask =  (TWebTask *)*it;
		pTask->m_prgCtrl.UnPrepareProgress();
		delete pTask;
	}
	m_UrlTaskList.clear();
}

 void TWebSession::ActiveTaskUI(void)
 {
	if(m_UrlTaskList.size() <=0)
		return;
	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		it != m_UrlTaskList.end();it++)
	{
		_URL_TASK  *pTask = *it;
		pTask->ActiveTaskUI();
	}
 }

 void TWebSession::UnActiveTaskUI(void)
 {
	 if(m_UrlTaskList.size() <=0)
		 return;
	 for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		 it != m_UrlTaskList.end();it++)
	 {
		 _URL_TASK  *pTask = *it;
		 pTask->UnActiveTaskUI();
	 }
 }

 BOOL TWebSession::UrlInDownSession(URL_STRING & purl)
 {
	
	 return FALSE;

 }

 URL_STRING& TWebSession::GetTaskName(void)
 {
	 return m_TaskName;
 }
 URL_STRING& TWebSession::GetWorkPath(void)
 {
	 //TODO: return statement
	 return m_DirCollection.GetRootPath();
 }

 
 void TWebSession::ProcessTaskError(TWebTask * pTask)
 {
/*
	 #define DWERR_OPEN_INTNET_CLIENT_FAILURE  0x89
#define DWERR_OPEN_INTNET_URL_FAILURE	  0x8a
#define DWERR_FILE_EXIST	  0x8b
#define DWERR_FILE_OPEN_FAILURE	  0x8c
DWERR_CREATE_DIR_FAILURE
*/
	 switch(pTask->m_dwError)
	 {
	
	 case	DWERR_FILE_EXIST:
		 {
			 DWORD nType;
			 _URL &obj = pTask->GetURLObj();
			 TRACE("url:%s\n",*obj.GetURL());
			 if(IsHtmlTypeFile(obj.m_hstSavePath,nType))
				downHtmlNum++;
			 obj.m_hstSavePath.Empty();

			 pTask->m_dwError = 0;
			 this->m_dwThreadStatus = _URL_TASK::THREAD_FREE;
		 }
		break; 
	 case	DWERR_OPEN_INTNET_CLIENT_FAILURE:
	 case	DWERR_OPEN_INTNET_URL_FAILURE:
	 case	DWERR_FILE_OPEN_FAILURE:
		 {
	        if(!bIsFailureRetry)
				g_FailureUrlsList.push_back(pTask->GetURLObj());
			else
				CDlgFailureUrls::ReBkFailureUrl(pTask->GetURLObj());

		 }		
		break;
	 case	DWERR_CREATE_DIR_FAILURE:
		break;
	 }
 }


 /*
format:
version(int)-session complete Tag (int)- downed num(int)-downed urls(String)- not downed url num(int) - 
not downed urls(String) - not download src url num(int) - not download src urls(String)
 */
 BOOL TWebSession::SaveToTaskFile(void)
 {
	 BOOL bRet;
	 int  num;
	 CStdioFile  stdFile;
	 bRet = stdFile.Open(m_TaskDirPath,CStdioFile::modeCreate|CStdioFile::modeReadWrite|CFile::shareDenyNone);
	 if(!bRet)
		 return FALSE;

	 num = FILE_VERSION1;
	 stdFile.Write(&num,4);

	 pSYSTEM_CONFIG  pconf = ConfigManager::Instance()->GetSystemConfig();

	 num = pconf->dwWorkMode ;//
	 num++;

	 stdFile.Write(&num,4);

	 if(m_UrlRefFile.AtEnd()== FALSE || m_UrlRefSrcFile.AtEnd() == FALSE)
	 {
		num = SESSION_NOCOMPLETE;
		stdFile.Write(&num,4);
	 }else
	 {
		num = SESSION_COMPLETE;
		stdFile.Write(&num,4);
	 }

	 stdFile.WriteString( *m_RootUrl.GetURL() + _T("\n"));

	 num = m_UrlRefFile.m_dwReadPos;//获得已经下过的数量

	 long  numpos1,numpos2,numt;
	 numpos1 = stdFile.GetPosition();	
	 stdFile.Write(&num,4);

//if anchor 先存放 anchor....
	 
//#ifdef _FILE_BUFFER//存放已经下过的
	 CString  stString,strValid;
	 m_UrlRefFile.SetToStart();  
	 m_UrlRefFile.ReadFromBuffer(stString);

	numt = num;//下载过的数量，但包括失败的。
	URL_LIST  failureList;
	 for(int i = 0; i < num ;i++,m_UrlRefFile.ReadFromBuffer(stString))
	 {
		 _URL ojb(stString);
		 ExtractValidUrl(ojb.GetURL(),&strValid);
		 if(::PathFileExists(GetUrlLocalPath(strValid)))
			stdFile.WriteString(stString + _T("\n"));
		 else
		 {
			 failureList.push_back(stString);
			 numt--; //过滤未成功下载的连接。。。
		 }
	 }
	 numpos2 = stdFile.GetPosition();
	 stdFile.Seek(numpos1,SEEK_SET);
	 stdFile.Write(&numt,4);//过滤掉下载失败链接的数目。
	 stdFile.Seek(numpos2,SEEK_SET);//write downloaded num;


//#ifdef _FILE_BUFFER//存放未下载过的。。。
	 num = m_UrlRefFile.m_iRecordNum - m_UrlRefFile.m_dwReadPos;//还没有未下载的html url数量
	 if(failureList.size())
		 num += failureList.size();

	 stdFile.Write(&num,4);
#if 0
	 while(!failureList.empty())//将下载失败的写入 。
	 {
		 URL_STRING strURL =  *failureList.end();
		 stdFile.WriteString(strURL + _T("\n"));
		 failureList.pop_back();
	 }
#endif
	 URL_LIST::iterator failit = failureList.begin();
	 for(;failit != failureList.end();failit++)
	 {
		  URL_STRING strURL = *failit;
		  stdFile.WriteString(strURL + _T("\n"));
	 }
	 while(!m_UrlRefFile.AtEnd())
	 {
		 m_UrlRefFile.ReadFromBuffer(stString);
		 stdFile.WriteString(stString + _T("\n"));
	 }

//#ifdef _FILE_BUFFER//纪录没有下载的图片资源，暂时不处理下载失败的链接。
	 num = m_UrlRefSrcFile.m_iRecordNum - m_UrlRefSrcFile.m_dwReadPos;//未下载的资源数量
	 stdFile.Write(&num,4);
	 while(!m_UrlRefSrcFile.AtEnd())
	 {
		 m_UrlRefSrcFile.ReadFromBuffer(stString);
		 stdFile.WriteString(stString + _T("\n"));
	 }
	 stdFile.Close();
	 return TRUE;;
 }

 BOOL TWebSession::LoadFromTaskFile(CString  fileName)
 {
#if 0
	 BOOL			bRet;
	 CStdioFile		stdFile;
	 CString		strUrl;
	 int			num;
	 int			isFinish;

	 bRet = stdFile.Open(fileName,CStdioFile::modeRead);
	 if(!bRet)
		 return FALSE;

	 /*head ,version and root url*/
	 stdFile.Read(&num,4);

	 isFinish = 0;
	 if(num == FILE_VERSION1)
		stdFile.Read(&isFinish,4);//if session complete....
	 else
		stdFile.SeekToBegin();

	 stdFile.ReadString( strUrl );//read root
	 m_RootUrl.SetURL(strUrl);
	 
	 if(isFinish == SESSION_COMPLETE) // if has down all,return.....
		 return isFinish;

	 int i;
	 stdFile.Read(&num,4);//read downed num
	 if(num)
//#ifdef  _FILE_BUFFER
	 for( i = 0;i < num;i++)
	 {
		 stdFile.ReadString(strUrl);
		 m_UrlRefFile.WriteToBuffer(strUrl + "\n");
	 }
	 m_UrlRefFile.m_dwReadPos = num; //set read pos
	 m_UrlRefFile.SetCurLocal();//设置文件读写位置。。。


	 stdFile.Read(&num,4);//read not downed num
	 if(num)
//#ifdef _FILE_BUFFER
	 for( i = 0;i < num;i++)
	 {
		 stdFile.ReadString(strUrl);
		 m_UrlRefFile.WriteToBuffer(strUrl + "\n");
	 }

	 stdFile.Read(&num,4);//read not down src num
	 if(num)
//#ifdef _FILE_BUFFER
	for(i = 0;i < num;i++)
	 {
		 stdFile.ReadString(strUrl);
		 m_UrlRefSrcFile.WriteToBuffer(strUrl + "\n");
	 }
	 stdFile.Close();
#endif
	 return TRUE;
 }

 void TWebSession::AdjudgeStatus(void)
 {
	/*m_UrlTaskList
	for()*/
 }


 void TWebSession::DoConver(int nType)
 {
	if(nType & 1)
	{
		AfxGetApp()->GetMainWnd()->SendMessage(WM_START_CONVERT,(LPARAM)m_TaskDirPath.GetBuffer(),(WPARAM)0);
		RunChm(this->m_TaskDirPath);
	}

	if(nType & 2)
	{
		AfxGetApp()->GetMainWnd()->SendMessage(WM_START_CONVERT,(LPARAM)m_TaskDirPath.GetBuffer(),(WPARAM)1);
	   ::ShellExecute(NULL ,_T("open"),TxtCompiler::GetTxtDirPath(m_TaskDirPath) , NULL, NULL, SW_SHOWNORMAL);
	}

	if(nType == 0)
	{
		::PathRemoveFileSpec(m_TaskDirPath.GetBuffer());
		::ShellExecute(NULL, _T("open"),m_TaskDirPath, NULL, NULL, SW_SHOWNORMAL);
	}

		
	MessageBeep(MB_ICONEXCLAMATION);

 }

 void TWebSession::RunChm(CString & strPath)
 {
	CFileFind fd;
	CString strFind = strPath;
	PathRemoveFileSpec(strFind.GetBuffer());
	strFind.ReleaseBuffer();
	 

	BOOL bret = fd.FindFile(strFind +  _T("\\*.chm"));
	if(bret) 
	{
		fd.FindNextFile();
		ShellExecute(NULL,_T("open"),fd.GetFileName(),NULL,strFind + _T("\\"),SW_NORMAL);
	}
 }

 void TWebSession::FillResourcUrlsBuffer(URL_LIST &list)
 {
	if(m_UrlRefSrcFile.m_pStream == NULL)
		 return;
	 if(list.size()<=0)
		 return;
	 for(URL_LIST::iterator it = list.begin();it != list.end();it++)
	 {
		    URL_STRING  urlStr = *it;
			m_UrlRefSrcFile.WriteToBuffer(urlStr + _T("\n"));
	 }
 }
 void TWebSession::FillUrlsBuffer(URL_LIST& list)
 {
	 if(m_UrlRefFile.m_pStream == NULL)
		 return;
	 if(list.size()<=0)
		 return;
	 for(URL_LIST::iterator it = list.begin();it != list.end();it++)
	 {
		    URL_STRING  urlStr = *it;
			m_UrlRefFile.WriteToBuffer(urlStr + _T("\n"));
	 }
 }

 void TWebSession::Task_ForceStop(void)
 {
	 _URL_TASK::Stop();
	 ::SuspendThread(this->m_hThread);
	
	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		it != m_UrlTaskList.end();it++)
	{
		_URL_TASK  &ut = *(*it);
		//if(ut.GetStatus()  != THREAD_SUSPEND)
		{
			ut.Stop();
			::SuspendThread(ut.m_hThread);
		}
	}
 }

 

 void TWebSession::UnprepareAllWorker(void)
 {
	for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		it != m_UrlTaskList.end();it++)
	{
		TWebTask *pTask  = (TWebTask *)(*it);
		//if(ut.GetStatus()  != THREAD_SUSPEND)
		{
			 pTask->UnPrepare();
			 pTask->m_prgCtrl.UnPrepareProgress();
		}
	}
 }

 void TWebSession::ForceDestroy(void)
 {
	 __super::ForceTerminate();
 }
 
 

 void TWebSession::CreateRootPageQuickIcon(void)
 {
	 
	 CString  validURL;
	 if(NULL == m_pSysConf) return ;
	 if(m_pSysConf->dwWorkMode == ANCHOR_MODE)
	 {
		 if(TAnchorManager::Instance()->GetPageCount() > 1)
			return;
		 if(TAnchorManager::Instance()->GetPageCount() <= 0)
		    return ;
		 URL_LIST pages;
		 TAnchorManager::Instance()->GetPageUrls(pages);
		 ExtractValidUrl(&(*(pages.begin())),&validURL);

	 }else
	 {
		 if(m_RootUrl.GetURL()->IsEmpty())
		 return; 
		 ExtractValidUrl(m_RootUrl.GetURL(),&validURL);//for 带参数的url.....
	 }
	
	PATH_STRING  strPath = GetUrlLocalPath(validURL);

	CShellLinkInfo sli;
	sli.m_sTarget = strPath;
	//sli.m
	sli.m_sWorkingDirectory =  m_TaskDirPath.Left(m_TaskDirPath.ReverseFind('\\')+ 1);
	sli.m_sDescription = _T("Start First Local Page Here.");
	sli.m_sIconLocation = _T("shell32.dll");
	sli.m_nIconIndex = 220; //Use an ie file icon
	sli.m_nShowCmd = SW_MAXIMIZE;

	CString skName = sli.m_sWorkingDirectory;
	skName += "StartPage.lnk";
  	if(::PathFileExists(skName))
		return;

	CUrlShellLink sl;
	if (sl.Create(sli))
		sl.Save(skName);
	
 }

 //CString TWebSession::GetDefName(URL_STRING url)
 //{
	// STR_MAP::iterator it = m_name_Map.find(url);
	// if(it != m_name_Map.end())
	//	 return (*it).second;
	// CString tmpName;
	// tmpName.Format("default%d.html",m_name_Map.size());

	// m_name_Map.insert(make_pair(url,tmpName));
	// return tmpName;
 //}
