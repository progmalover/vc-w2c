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

//�������� ���� ���� �ּ��� ���
BOOL _URL_DOMAIN:: ParseUrlDomain(void)
{
	INTERNET_PORT	port;
	URL_STRING		strServer,strObject;
	DWORD			dwServerType;

	if(m_Url.IsEmpty())
		return FALSE;

	if(m_DmArray.size())
		m_DmArray.clear();

	//�����������
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
//  ����������Ĭ��Ϊ FILE_URL���͡�������
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

	m_nLay = (BYTE)pBuf[0];//�жϵ�һ�� �ַ��ǲ��� lay number...

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

//������ͬһ���繫˾����վ��
BOOL _URL:: SameWebStation( _URL &_url)
{
	//���������Ƚ��ж��Ƿ�ͬһ��վ���� :ͬһ����վ���� ����2�� ������һ�� ��
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
/*������ͬһ�����ϣ�domainҪ��ȫ���*/
BOOL _URL:: SameWebHost( _URL &_url)
{
	//���������Ƚ��ж��Ƿ�ͬһ��վ���� :ͬһ����վ���� ����2�� ������һ�� ��
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

	/*���ﷵ�ص���Ӧ���Ѿ����������˵ģ�,�Ƚϡ��������ɾ��ġ�url*/
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

	//�����������
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
//	URL_STRING  m_Url;/*�����ԭʼurl,�����е�*/
//	URL_STRING  m_Domain;//������
//	URL_STRING  m_Object;//���������
//
//	
//	DWORD		m_dwSize;	  //�ļ��ĳߴ�
//	DWORD		m_dwReadSize; //�Ѿ��õ��ĳߴ�
//
//	DWORD       m_nServerType;
//	WORD		m_nPort;
//	
//	BYTE	    m_nLay;
//	DWORD		m_UrlTagType;
//	DWORD		m_UrlFileType;
//	URL_STRING  m_UrlExtension;
//	URL_STRING	m_hstDirctory;//������·��
//	URL_STRING  m_hstSavePath;//���صı�����ļ�·��
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

	::WaitForSingleObject(_StartUp.hEvent,INFINITE);//����ȷ�������߳��Ѿ�����run()
	::CloseHandle(_StartUp.hEvent);
	
	m_trdObj.Attach(m_hThread);
	return  (m_hThread != NULL);
}

_URL_TASK ::_URL_TASK(void)
{
	m_dwThreadStatus = _URL_TASK::THREAD_FREE ; // ��ʼ ״̬
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
/*ֻ�Ǳ����������̱߳�����*/
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
	
	
	Prepare();//���ԳƵ��á�����

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

	   if(m_dwThreadStatus == TWebSession::THREAD_SUSPEND) //�Ѿ�����ֹͣ������״̬
	   {
		  SuspendThread(m_hThread);
		  goto LStart;
	   }
	  
	   /*
		  ��� �߳��б��Ƿ���������ص��̣߳�
		  Ȼ����url�б����Ƿ���urlδ�¡���������
	   */

	   THREAD_EXIT_CHECK(this,return);
	   for(TASK_LIST::iterator it = m_UrlTaskList.begin();
		   it != m_UrlTaskList.end();it++)
	   {

		   THREAD_EXIT_CHECK(this,return);
		   _URL_TASK &Tsk = *(*it);
		   m_pCurrentTask = (_URL_TASK *)*it;

		   /*ʧ�ܵĻ����³���,һֱʧ�ܵĻ��ͷ����� ������*/
		  
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
		   if(Tsk.GetStatus() == _URL_TASK::THREAD_COMPLETE) //����״̬һ����Ϊ��ǰ��������״̬
		   {
			   /*
			   �õ��ļ����ͼ�·�����������html��parse�����prepareList;
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
							downHtmlNum++;//��¼���ص��ļ� ����
							
							//if(dwDownLay < m_pConf->dwDownLay)
							if(obj.GetLay() < m_pConf->dwDownLay)
								wantTag = (TAG_SRC | TAG_URL);
							else
								wantTag = TAG_SRC;
							/*���ȷ�����ʷ������*/
					
							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
															 _T("Parsing..."),0);
							/*parese�µ�����*/
							if(nType == TYPE_ASP)
								ParseDynamicPage(obj,m_TmpRefList,m_TmpSrcList ,wantTag);
							else
								ParseStaticPage(obj,m_TmpRefList,m_TmpSrcList  ,wantTag); 

							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,_T("Complete"),100);

							THREAD_EXIT_CHECK(this,return);
							PutRefsToPrepareList( &Tsk,m_TmpRefList,m_TmpSrcList,m_pConf->dwDownMode,wantTag);

							///**********��ʱ��ת��txt���ܷ�������*************/
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
							downHtmlNum++;//��¼���ص��ļ� ����

							/*���ȷ�����ʷ������*/
							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
								_T("Parsing..."),0);
							/*parese�µ�����*/
							if(nType == TYPE_ASP)
								ParseDynamicPage(obj,m_TmpRefList,m_TmpSrcList ,TAG_SRC | TAG_URL);
							else
								ParseStaticPage(obj,m_TmpRefList,m_TmpSrcList ,TAG_SRC | TAG_URL); 

							pTask->m_prgCtrl.SetProgressInf(pTask->m_prgCtrl.m_fName,
								_T("Complete"),100);
							THREAD_EXIT_CHECK(this,return);
							PutRefsToPrepareList( &Tsk,m_TmpRefList,m_TmpSrcList ,m_pConf->dwDownMode,TAG_SRC | TAG_URL);

							///**********��ʱ��ת��txt���ܷ�������*************/
							//if(m_pSysConf->dwMode & 2)
							//{
							//	CString txtDir = TxtCompiler::GetTxtDirPath(m_TaskDirPath);
							//	if(txtDir.IsEmpty())continue;
							//	PrepareDirectory(txtDir,0);
							//	TxtCompiler::Instance()->DoCompiler( obj.m_hstSavePath,txtDir);
							//}

						}
						//Tsk.SetTaskStatus(_URL_TASK::THREAD_FREE);//���ô��߳̿��л�������
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
	   ���ҿ��е��̡߳��������·����µ�����
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
			   pTsk->GetStatus()== _URL_TASK::THREAD_FAILURE) //����״̬һ����Ϊ��ǰ��������״̬
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
���������ص�html
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
	AfxGetApp()->GetMainWnd()->PostMessage(IDM_CUT,0,0);//ɾ��ժ¼����Դ
	 
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

void  TWebSession::Save(void)/*���浱ǰ������*/
{

}

void  TWebSession::Load(PATH_STRING &taskPath) /*������ǰ�洢������*/
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
���ݵ�ǰ�û����ú����ӱ�������;��� ��δ������
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
	�����û�������������ѡ��İ�һЩurl����preparelist�У�
	ǰ�������ǣ�1��û�б����� ����2�������û�����������
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
		if(::PathFileExists(strPath) == TRUE)//�Ѿ����ڲ�����zai
			continue;

		/*û����donelist�С�������������������֤����Ȼ������prepare����*/
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
//	ExtractValidUrl(m_RootUrl.GetURL(),&validUrl);//for ��������url.....
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
		��¼���ص��ļ�������ȫ�����ļ���С�������� ������ʼ�ˡ���������
   */

	enableMaxSize = 0;
	if(m_pConf->bEnableLimit)
		enableMaxSize  =  m_pConf->dwMaxSize * 1024 *1024;//������һ�ס���

	this->downFilesNum = TAnchorManager::Instance()->GetPageCount();//��ʼ������ҳ�棨�Ѿ��ɼ������ģ�
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

	 num = m_UrlRefFile.m_dwReadPos;//����Ѿ��¹�������

	 long  numpos1,numpos2,numt;
	 numpos1 = stdFile.GetPosition();	
	 stdFile.Write(&num,4);

//if anchor �ȴ�� anchor....
	 
//#ifdef _FILE_BUFFER//����Ѿ��¹���
	 CString  stString,strValid;
	 m_UrlRefFile.SetToStart();  
	 m_UrlRefFile.ReadFromBuffer(stString);

	numt = num;//���ع���������������ʧ�ܵġ�
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
			 numt--; //����δ�ɹ����ص����ӡ�����
		 }
	 }
	 numpos2 = stdFile.GetPosition();
	 stdFile.Seek(numpos1,SEEK_SET);
	 stdFile.Write(&numt,4);//���˵�����ʧ�����ӵ���Ŀ��
	 stdFile.Seek(numpos2,SEEK_SET);//write downloaded num;


//#ifdef _FILE_BUFFER//���δ���ع��ġ�����
	 num = m_UrlRefFile.m_iRecordNum - m_UrlRefFile.m_dwReadPos;//��û��δ���ص�html url����
	 if(failureList.size())
		 num += failureList.size();

	 stdFile.Write(&num,4);
#if 0
	 while(!failureList.empty())//������ʧ�ܵ�д�� ��
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

//#ifdef _FILE_BUFFER//��¼û�����ص�ͼƬ��Դ����ʱ����������ʧ�ܵ����ӡ�
	 num = m_UrlRefSrcFile.m_iRecordNum - m_UrlRefSrcFile.m_dwReadPos;//δ���ص���Դ����
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
	 m_UrlRefFile.SetCurLocal();//�����ļ���дλ�á�����


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
		 ExtractValidUrl(m_RootUrl.GetURL(),&validURL);//for ��������url.....
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
