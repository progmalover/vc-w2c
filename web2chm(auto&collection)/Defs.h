#pragma once
#include <list>
#include <vector>
#include <map>

using namespace std;

class _URL_TASK;
class TWebSession;
class TWebTask;
class CTskPrgList;
class _URL;
typedef	 CString			URL_STRING;
typedef  list<URL_STRING>	URL_LIST; //存放url的链表 
typedef  list<_URL>			URL_OBJ_LIST;
typedef  vector<URL_STRING> DOMAIN_ARRAY;//存放 domian 名字节点

typedef	list< _URL_TASK *>  TASK_LIST;
typedef list<TWebTask>	  WEBTASK_LIST;	

typedef vector<DWORD>  TYPE_LIST;


#define	 WM_TASKINFO	WM_USER + 34		
#define  WM_DOWNPROGRESS  WM_USER + 10

#define  FIRST_LAY 1
#define  END_LAY   6

#define  WM_URLS_EDIT		WM_USER + 30

typedef	 struct System_Config
{
	DWORD dwThrdNum; //每个task 使用的最大线程数量
	DWORD dwWaitTime;
	DWORD dwConnectTimes;//可以重复连接的次数。。。
	DWORD dwRecordNum;
	volatile	DWORD dwWorkMode; /*1:auto mode,0:Anchor mode*/
	volatile	DWORD dwMode;
	volatile	BOOL  bShutDown;
	BOOL        bWarzard;//默认为使用
	BOOL        bAskBeforclose;  //关闭前是否提示--默认TRUE
	
	
}SYSTEM_CONFIG,*pSYSTEM_CONFIG;

typedef  struct Private_Config
{
volatile	BOOL		bEnableLimit;
volatile	BOOL		bEnableProxy;
	
volatile	DWORD		dwDownMode;
volatile	DWORD		dwDownLay;
volatile	BOOL		bEnableEdit;
	
volatile	DWORD		dwMaxSize;
volatile	DWORD		dwMaxFileSize;
volatile	DWORD		dwMaxFileNum;

volatile	WORD		dwPort;		//端口

/*page modify*/
volatile    BOOL		bModifyDef;
volatile    BOOL		bCScript;//clear script/noscript
volatile    BOOL		bCNoScript;
volatile	BOOL		bCLoad; //clear load/unload
volatile	BOOL		bCUnload; //clear load/unload
volatile    BOOL		bCMouseClick; //clear mouse event
volatile    BOOL		bCMouseLeave; //clear mouse event
volatile    BOOL		bCMouseEnter; //clear mouse event
volatile    BOOL		bCMouseOver; //clear mouse event
volatile    BOOL		bCMouseOut; //clear mouse event
volatile	BOOL		bCMouseDown;

BOOL		enableTypeArray[10];
CString		strProxyName;//代理服务器名
CString     strUserName;
CString		strPassWord;

CString		strIncPath;
	//TYPE_LIST	vcTypeList;//enable file Types

}PRIVATE_CONFIG,*pPRIVATE_CONFIG;

enum{
	URL_OPEN_FAILURE = 1,
	INET_INIT_FAILURE,
	BUFF_FILE_FAILURE
};

typedef struct TaskStatus
{
	_URL_TASK *pTask;
	DWORD dwBitRat;
	DWORD dwFileSize;
	DWORD dwDownSize; /*down load totle size*/
	DWORD dwEscapeTime;
	DWORD dwResponTime;
	DWORD dwTryTimes;
	DWORD dwItem;//item used in ui list
	BOOL  bDownSuccess;
}TASK_DOWN_STATUS,*pTASK_DOWN_STATUS;

typedef struct ConnectInfmation
{
	DWORD bitRat;
	DWORD respondTime;/*响应时间*/
	DWORD serverType;
	DWORD downSize; /*down load totle size*/
	DWORD useTime;
	WORD  port;

	CString nameServer;
	CString nameObject;
}CONNECT_INF,*pCONNECT_INF;

typedef map<_URL_TASK *,PRIVATE_CONFIG>CONFIG_MAP;

extern TWebSession   *GP_CurrentSession;
extern CTskPrgList   *GP_TskPrgList ;

#define  CONNECT_TRD_NUMBER   5
#define  CONNECT_WAIT_TIME    7
#define  CONNECT_NUMS_MAX     5
extern  DWORD GA_dwConnectTrdsArray[];
extern  DWORD GA_dwWaitTimeArray[];
extern  DWORD GA_dwConnectRepondsArray[] ;
extern  TASK_LIST	 GP_SessionList;

#define ANCHOR_MODE   0
#define AUTO_MODE     1

//#define  COLLECTION_AND_MODE  1