#pragma  once

#include "wininet.h"
#include "Defs.h"
#include "TaskDir.h"
#include "ConfigManager.h"
#include "TExec.h"
#include "bufferFile.h"
#include <map>
using namespace::std;

#define  THREAD_EXIT_CHECK(T1,T2)\
{\
	if(T1->m_dwThreadStatus == _URL_TASK::THREAD_EXIT)\
		#T2;\
}

enum HTML_TYPE{
TYPE_HTM = 0,
TYPE_HTML,
TYPE_SHTML,
TYPE_ASP
};

class  _URL;
class  _URL_DOMAIN;
class  _URL_TASK;
 

//过滤url串中的\0
void	ToLowerAlphas(LPTSTR pstr,int len);
BOOL	IsInpageLink(LPCTSTR  url);
void	SafeStrMoudleScan(LPTSTR lpstr,int len);
/*避免添加重复的*/
//void	 FreeAllTask(void);
BOOL	 WayIsFileOf(CString &strPath,LPCTSTR lpExtStr);
void	 ToDosFileNameStr( CString  &_tcsstr);
BOOL     AlreadInList( URL_LIST &listUrl,URL_STRING &strUrl);
// CNewTaskDlg 消息处理程序
void     FixUrlStr(URL_STRING  &strUrl);
BOOL	 ExtractValidUrl(URL_STRING *urlStr,URL_STRING *pValidUrl);
BOOL	 IsRefLink(URL_STRING & urlStr);
BOOL	 IsRightLink(URL_STRING & urlStr);
BOOL	 IsUntLink(LPCTSTR  purlStr);
void	 RunChmCompiler(CString strTaskfName);//run chm converter....
BOOL	 IsHtmlTypeFile(PATH_STRING &strPath,DWORD &nfType);
URL_STRING ComposeRefURL(URL_STRING *pRoot,URL_STRING *pRef  );
void	 CreatePathFromUrl(URL_STRING *dirRoot,URL_STRING *urlStr,URL_STRING &strFile ,BOOL bAddRoot);

HINTERNET CreateInternetHandle(DWORD &dwError);
HINTERNET OpenInternetUrl(HINTERNET hInternet,URL_STRING *pStr,DWORD &dwError);

HRESULT  SetSysConfInternet(HINTERNET hnet,SYSTEM_CONFIG *pSysConfig);
HRESULT  SetPrivateConfInternet(HINTERNET hNet,PRIVATE_CONFIG *pPrConfig);  

//HRESULT  DownWebFile(_URL_TASK *pTask,CString Url,CString svPath,TASK_DOWN_STATUS &CInf);
HRESULT  ConnectTest(CWnd *pWnd,CString Url,CONNECT_INF &CInf);

BOOL     AssertUrlHttpStr(LPCTSTR  url); 
BOOL	 IsAllowFileType(_URL_TASK *pParent,DWORD nType);
BOOL	 VerifyFile(LPCTSTR strFile);
#define  TAG_URL  0x1000
#define  TAG_SRC  0x2000
#define  TAG_UNKNOW 0x4000
#define  TAG_XX   ..

#define  FILE_URL TAG_URL
BOOL	 GetHttpPageUrls(_URL_TASK *pTask,_URL *urlObj,URL_LIST &urlList,URL_LIST &urlSrcsList,DWORD nDownMode,DWORD nTag);
class  _URL_DOMAIN
{
public:
	_URL_DOMAIN(URL_STRING *strUrl);
	~_URL_DOMAIN(void);
	int  GetDomainNums();
	BOOL GetDomainName( URL_STRING &strUrl,int stage);
protected:
	BOOL  ParseUrlDomain(void);
private:
	URL_STRING    m_Url;
	DOMAIN_ARRAY  m_DmArray;
};

class  _URL
{	
	friend class  _URL_TASK;
	friend class  TWebSession;
public:
	_URL(URL_STRING &strUrl);
	_URL(void){};
	~_URL(void);
	void SetURL(URL_STRING strUrl);
	BOOL SameWebHost( _URL &_url);
	BOOL SameWebStation( _URL &_url);
	BYTE GetLay(){return m_nLay;};
	URL_STRING  * GetURL();
	
	BOOL IsEmpty(void);
	void Empty(void);
	void SetHstFileName(PATH_STRING & strPath);
	void CopyObj(_URL & obj);
	enum{
		FILE_JPG = 0,FILE_GIF,FILE_PNG,FILE_BMP,FILE_JPEG,FILE_JFIF,FILE_TEXT,FILE_ZIP,FILE_MP3,FILE_WMA,
		FILE_WMV,FILE_RM,FILE_SWF,FILE_PDF,FILE_CHM
	};
protected:
	URL_STRING  m_Url;/*保存的原始url,此类中的*/
	URL_STRING  m_Domain;//主域名
	URL_STRING  m_Object;//请求对象名

	
	DWORD		m_dwSize;	  //文件的尺寸
	DWORD		m_dwReadSize; //已经得到的尺寸

	DWORD       m_nServerType;
	WORD		m_nPort;
	
	BYTE	    m_nLay;

protected:	
	void		ParseUrl(void);
	
public: //parse result
	DWORD		m_UrlTagType;
	DWORD		m_UrlFileType;
	URL_STRING  m_UrlExtension;
	URL_STRING	m_hstDirctory;//主机的路径
	URL_STRING  m_hstSavePath;//下载的保存的文件路径
};

class  _URL_TASK
{

public :
	_URL_TASK(void);
	~_URL_TASK(void);
public:
	enum TASK_STATUS{THREAD_EXIT= 0,THREAD_RUNING=1,THREAD_SUSPEND,THREAD_FREE,THREAD_COMPLETE,THREAD_FAILURE};
	BOOL Create(_URL_TASK *pParent);

    DWORD GetTaskStatus(void);
	void  SetTaskStatus(int nStatus){m_dwThreadStatus = nStatus;};
	BOOL  SetURL(URL_STRING uStr);
	BOOL  SetURLObj(_URL  & urlObj);
	_URL &GetURLObj(void){return m_UrlObj;};
	BOOL Start(void);
	BOOL Stop(void);
	BOOL Destroy(void);
	 void SafExit(void);
	int  virtual GetStatus(void){ return m_dwThreadStatus;};
	//user logcal here
	virtual void Run(void){};
	virtual void Notify(int nStatCode){};
	virtual void OnFinish(UINT nCode ){};
	virtual void ActiveTaskUI(void){};
	virtual void UnActiveTaskUI(void){};
	virtual void ForceTerminate();

	typedef struct  Thread_Startup{
		_URL_TASK *pTask;
		HANDLE hEvent;
	}THREAD_STARTUP;

private:
	static THREAD_STARTUP  _StartUp;
	static DWORD WINAPI  TaskEntry(LPVOID pParam)
	{
		THREAD_STARTUP *pStartup = (THREAD_STARTUP *)pParam;
		_URL_TASK *p =  pStartup->pTask;
		::SetEvent(pStartup->hEvent);
		//::CloseHandle(pStartup->hEvent)
		p->Run();
		
		/*::CloseHandle(m_hThread);*/
		return TRUE;
	};
	
protected:
	class CHandle
	{
		public:
			CHandle(void)
			{
				m_handle = NULL;
			};
			~CHandle(void)
			{
				if(m_handle)
				{
					try{
					DWORD dwExitCode;
					if(GetExitCodeThread(m_handle,&dwExitCode)== FALSE)
						return ;
					if(dwExitCode == STILL_ACTIVE)
					{
						//while(ResumeThread(m_hThread));
						//if(::WaitForSingleObject(m_hThread,500)== WAIT_TIMEOUT)
						TerminateThread(m_handle,0x99);
					}
					::CloseHandle(m_handle); 
					
					}catch(...){}

				}
			};
		public:
			void Attach(HANDLE thdHandle)
			{
				m_handle = thdHandle;
			};
			
		private:
			HANDLE		m_handle;
	};
protected:
	_URL		m_UrlObj;
	DWORD		m_dwThreadID;
	

	HANDLE      m_hEvent;
	_URL_TASK	*m_pParent;
	CHandle	    m_trdObj;
	
public:
	TASK_DOWN_STATUS  m_DownStatus;
	volatile DWORD	m_dwThreadStatus;	
	HANDLE		m_hThread;
};

/*
此类负责网站下载。。。。但是局限于：1）1个网页及网页上全部的url相关网页
2）一个网页及其所在网站的内容 3）
*/
#define  ALL_REF_IN_PAGE		1
#define  ALL_REF_IN_WEB			2
#define  ALL_REF_IN_WEBHOST		3
#define  ALL_REF_HAS_STRING		4
/*
tag for session save file  :SessionName.tsk 
*/
#define  SESSION_COMPLETE       23
#define  SESSION_NOCOMPLETE		24
#define  FILE_VERSION1			2

#define  WM_START_CONVERT		WM_USER + 300
class TUrlsLayManager;
class TWebSession:public _URL_TASK ,TExec//任务类,实现下载任务状态的控制...以及与ui层的交互
{
friend class TUrlsLayManager;
public:
	TWebSession(void);
	TWebSession(URL_STRING & strUrl);
	~TWebSession(void);
	BOOL InitTaskList(int ntask);
	void SetRootUrl(URL_STRING & strUrl);//设置开始时的第一个url
	void SetTaskPathName(PATH_STRING &strPath);
	//void SetTaskStatus(int nStatus);

	URL_STRING & GetTaskPathName(void);
	
	void ParseStaticPage(_URL &urlObj,URL_LIST &urlRefs,URL_LIST &urlSrcs,int nTags);
	void ParseDynamicPage(_URL &urlObj,URL_LIST &urlRefs,URL_LIST &urlSrcs,int nTags);
	void PutRefsToPrepareList(_URL_TASK  *purlTask,URL_LIST & urlRefList,URL_LIST & urlSrcList, DWORD nMode,DWORD nTag);
public:
	enum TASK_MODE{NONE_MODE = -1,WEB2_CHM = 0,WEB_DOWN,FILE_DOWN};
	//enum TASK_STATUS{TASK_RUNING=1,TASK_STOP,TASK_COMPLETE,TASK_EXIT};
	int  TASK_GetMode(void){return m_Model;};
	void TASK_Start(void);
	void TASK_Stop(void);
	void TASK_Destroy(void);
    void Run(void);
	void Notify(int nStatCode);
	void EnableDisplay(BOOL bEnable);
	virtual void ActiveTaskUI(void);
	virtual void UnActiveTaskUI(void);
	virtual void OnFinish(UINT nCode = 1);
	BOOL IsRootURL(URL_STRING & strUrl);
	_URL& GetRootUrlObj(void){return m_RootUrl;};  

	void OnIdle(void);
	void UninitTaskList(void);
	BOOL UrlInDownSession(URL_STRING& purl);
	URL_STRING& GetWorkPath(void);
	URL_STRING& GetTaskName(void);

public:
	void Save(void);/*保存当前的内容*/
	void Load(PATH_STRING &taskPath); /*载入以前存储的内容*/
	PATH_STRING GetUrlLocalPath(URL_STRING &strUrl,BOOL bAddRoot=TRUE);
	PATH_STRING GetUrlLocalUrl(URL_STRING *pRootUrl ,URL_STRING &strDesk);
	BOOL Create(URL_STRING & strUrl, PATH_STRING & strPath, int ntrdNum);
	BOOL SaveUrlLocalPath(PATH_STRING & strPath);
	void SaveToHistory(void);

    HRESULT  Prepare(void);
	HRESULT  UnPrepare(void);
	HRESULT	 Exec();

public:
	
	CBufferFile			m_UrlRefSrcFile;
	CBufferFile			m_UrlRefFile;
protected:
	URL_LIST			m_TmpUseList; // linshi

	URL_LIST			m_TmpSrcList;
	URL_LIST			m_TmpRefList;

	TASK_LIST			m_UrlTaskList;//不用管理最好...
	_URL				m_RootUrl;
	
	TaskDirCollection	m_DirCollection;
	PATH_STRING			m_TaskDirPath;
	PATH_STRING			m_TaskName;

	PRIVATE_CONFIG		*m_pConf;
	SYSTEM_CONFIG		*m_pSysConf;
	BOOL				m_bSaved;
public:

	int  m_Model;
	BOOL m_bInitThreads;
	_URL_TASK  *m_pCurrentTask; //for html parsing...
	void ProcessTaskError(TWebTask * pTask);
	BOOL SaveToTaskFile(void);
	BOOL LoadFromTaskFile(CString  fileName);
	void AdjudgeStatus(void);
	void PutToFileBuffer(URL_LIST  & list,int nMode,int nTag);
	void DoConver(int nType);
	void RunChm(CString & strPath);
	void FillUrlsBuffer(URL_LIST& list);
	void FillResourcUrlsBuffer(URL_LIST &list);
	void Task_ForceStop(void);
	 
	void UnprepareAllWorker(void); 
	void CreateRootPageQuickIcon(void);
	
	void ForceDestroy(void);
	//CString GetDefName(URL_STRING url);
public:
   DWORD		downFileSize; //单个文件最大也就是4个g...
   DWORD		downFilesNum;

   LONGLONG		downAllFileSize;//all file size ...
   LONGLONG		enableMaxSize;

   DWORD		downHtmlNum;
   BOOL			bSucces;
 
  
};
 
