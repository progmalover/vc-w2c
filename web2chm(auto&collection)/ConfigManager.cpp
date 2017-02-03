#include "stdafx.h"
#include "ConfigManager.h"
/*
DWORD dwThrdNum; //每个task 使用的最大线程数量
DWORD dwWaitTime;
DWORD dwConnectTimes;
DWORD dwMode;
DWORD dwRecordNum;
*/

ConfigManager::ConfigManager(void)
{
	m_bSysInitVal = FALSE;
	m_bPrivateIniVal = FALSE;
}
ConfigManager::~ConfigManager(void)
{
	/*if(m_PrviteConfMap.size())
	for(CONFIG_MAP::iterator it = m_PrviteConfMap.begin();
		it != m_PrviteConfMap.end();it++)
	{
		TYPE_LIST &tp =  (*it).second.vcTypeList;
		if(tp.size())
			tp.clear();
	}*/

	m_PrviteConfMap.clear();
}

pSYSTEM_CONFIG    ConfigManager:: GetDefaultSystemConfig(void)
{
 return NULL;
}

BOOL  ConfigManager::SetSysConfig(pSYSTEM_CONFIG pSysconfig)
{
  if(!pSysconfig)
	  return FALSE;
  m_SystemConf.dwConnectTimes = pSysconfig->dwConnectTimes;
  m_SystemConf.dwThrdNum	= pSysconfig->dwThrdNum;
  m_SystemConf.dwRecordNum = pSysconfig->dwRecordNum;
  m_SystemConf.dwMode = pSysconfig->dwMode;
  m_SystemConf.dwWaitTime = pSysconfig->dwWaitTime;
  m_SystemConf.dwWorkMode = pSysconfig->dwWorkMode;
  m_SystemConf.bShutDown = pSysconfig->bShutDown;
  m_SystemConf.bWarzard = pSysconfig->bWarzard;
  m_SystemConf.bAskBeforclose = pSysconfig->bAskBeforclose;
  m_bSysInitVal = TRUE;
  return TRUE;
}

BOOL  ConfigManager::SetDefaultSessionConf(pPRIVATE_CONFIG pTaskConf)
{
	if(!pTaskConf)
		return FALSE;
	m_PrivateConf.bEnableLimit = pTaskConf->bEnableLimit;
	
	m_PrivateConf.dwDownMode = pTaskConf->dwDownMode;
	m_PrivateConf.dwDownLay = pTaskConf->dwDownLay;
	m_PrivateConf.bEnableEdit = pTaskConf->bEnableEdit;

	m_PrivateConf.dwMaxFileNum = pTaskConf->dwMaxFileNum;
	m_PrivateConf.dwMaxFileSize = pTaskConf->dwMaxFileSize;
	m_PrivateConf.dwMaxSize = pTaskConf->dwMaxSize;


	m_PrivateConf.bModifyDef = pTaskConf->bModifyDef;
	m_PrivateConf.bCScript = pTaskConf->bCScript;
	m_PrivateConf.bCNoScript = pTaskConf->bCNoScript;
	m_PrivateConf.bCLoad = pTaskConf->bCLoad;
	m_PrivateConf.bCUnload = pTaskConf->bCUnload;
	m_PrivateConf.bCMouseClick = pTaskConf->bCMouseClick;
	m_PrivateConf.bCMouseEnter = pTaskConf->bCMouseEnter;
	m_PrivateConf.bCMouseLeave = pTaskConf->bCMouseLeave;
	m_PrivateConf.bCMouseOut = pTaskConf->bCMouseOut;
	m_PrivateConf.bCMouseOver = pTaskConf->bCMouseOver;
	m_PrivateConf.bCMouseDown = pTaskConf->bCMouseDown;

	
	memcpy(m_PrivateConf.enableTypeArray,pTaskConf->enableTypeArray,sizeof(m_PrivateConf.enableTypeArray));

	m_PrivateConf.bEnableProxy = pTaskConf->bEnableProxy;
	m_PrivateConf.strProxyName = pTaskConf->strProxyName;
	m_PrivateConf.strPassWord = pTaskConf->strPassWord;
	m_PrivateConf.strUserName = pTaskConf->strUserName;

	
	m_bPrivateIniVal = TRUE;
	SaveSessionConfig();
	return TRUE;
}
pPRIVATE_CONFIG  ConfigManager:: GetDefaultSessionConf(void)
 {
	 if(!m_bPrivateIniVal)
		return NULL;
	 return  &m_PrivateConf;
 }
BOOL  ConfigManager::SetSessionConfig(_URL_TASK *pTask,pPRIVATE_CONFIG pTaskConf)
{
	 if(!pTask || !pTaskConf)
		 return FALSE;
	 CONFIG_MAP::const_iterator it = m_PrviteConfMap.find(pTask);
	 if(it != m_PrviteConfMap.end())
	 {
		pPRIVATE_CONFIG pprvConf = (pPRIVATE_CONFIG)&it->second;
		//*pprvConf = *pTaskConf;
		//memcpy(pprvConf,pTaskConf,sizeof(PRIVATE_CONFIG));
		pprvConf->bEnableLimit = pTaskConf->bEnableLimit;
		pprvConf->bEnableProxy = pTaskConf->bEnableProxy;
		pprvConf->dwDownMode  = pTaskConf->dwDownMode;
		pprvConf->dwMaxFileNum = pTaskConf->dwMaxFileNum;
		pprvConf->dwMaxFileSize = pTaskConf->dwMaxFileSize;
		pprvConf->dwPort = pTaskConf->dwPort;
		pprvConf->strPassWord = pTaskConf->strPassWord;
		pprvConf->strProxyName = pTaskConf->strProxyName;
		pprvConf->strUserName = pTaskConf->strUserName;
		pprvConf->dwMaxSize = pTaskConf->dwMaxSize;
		pprvConf->enableTypeArray[0] = pTaskConf->enableTypeArray[0];
		pprvConf->enableTypeArray[0] = pTaskConf->enableTypeArray[1];
		pprvConf->enableTypeArray[0] = pTaskConf->enableTypeArray[2];
		pprvConf->enableTypeArray[0] = pTaskConf->enableTypeArray[3];
		pprvConf->enableTypeArray[0] = pTaskConf->enableTypeArray[4];
		return TRUE;
	 }

	 m_PrviteConfMap.insert(std::make_pair(pTask,*pTaskConf));
	 return TRUE;
}

pSYSTEM_CONFIG  ConfigManager:: GetSystemConfig(void)
{
	if(!m_bSysInitVal)
		return GetDefaultSystemConfig();
	return  &m_SystemConf;
}

pPRIVATE_CONFIG ConfigManager::GetSessionConfig(_URL_TASK *pTask)
{
	if(!pTask)
		return  NULL;
	CONFIG_MAP::iterator it = m_PrviteConfMap.find( pTask);
	if(it != m_PrviteConfMap.end())
		return &(*it).second;
	return NULL;
}

/****从系统配置文件中读取***/
BOOL  ConfigManager::LoadSessionConfig()
{
	if(m_bPrivateIniVal) return  TRUE;
	//PRIVATE_CONFIG prvConfig;
	
	memset(m_PrivateConf.enableTypeArray,0,sizeof(m_PrivateConf.enableTypeArray));

	m_PrivateConf.dwDownMode = AfxGetApp()->GetProfileInt("PRIVATECONFIG","DOWN_MODE",DEFAULT_DOWNLOD_MODE);
	m_PrivateConf.dwDownLay = AfxGetApp()->GetProfileInt("PRIVATECONFIG","DOWN_LAY",DEFAULT_LAY);
	m_PrivateConf.bEnableEdit = AfxGetApp()->GetProfileInt("PRIVATECONFIG","ENABLE_EDIT",0);

	m_PrivateConf.bEnableLimit = AfxGetApp()->GetProfileInt("PRIVATECONFIG","ENABLE_LIMIT",0);
	
	m_PrivateConf.dwMaxSize = AfxGetApp()->GetProfileInt("PRIVATECONFIG","MAX_TOTLE_SIZE",DEFAULT_MAXSIZE);
	m_PrivateConf.dwMaxFileSize =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","MAX_FILE_SIZE",DEFAULT_MAXFILESIZE);//1 mb the minisize
	m_PrivateConf.dwMaxFileNum = AfxGetApp()->GetProfileInt("PRIVATECONFIG","MAX_FILE_NUM",DEFAULT_MAXFILENUMS);
	
	m_PrivateConf.bEnableProxy = AfxGetApp()->GetProfileInt("PRIVATECONFIG","PROXY_ENABLE",0);

	m_PrivateConf.strProxyName = AfxGetApp()->GetProfileString("PRIVATECONFIG","PROXY_NAME",NULL);
	m_PrivateConf.strUserName = AfxGetApp()->GetProfileString("PRIVATECONFIG","USER_NAME",NULL);
	m_PrivateConf.strPassWord = AfxGetApp()->GetProfileString("PRIVATECONFIG","USER_PASSWORD",NULL);


	m_PrivateConf.bModifyDef = AfxGetApp()->GetProfileInt("PRIVATECONFIG","MODIFY_DEF",TRUE);
	if(m_PrivateConf.bModifyDef)
	{
		m_PrivateConf.bCScript = TRUE;
		m_PrivateConf.bCNoScript = TRUE;

		m_PrivateConf.bCMouseClick = TRUE;
		m_PrivateConf.bCMouseEnter = TRUE;
		m_PrivateConf.bCMouseLeave = TRUE;
		m_PrivateConf.bCMouseOut = TRUE;
		m_PrivateConf.bCMouseOver = TRUE;
		m_PrivateConf.bCMouseDown = TRUE;
		m_PrivateConf.bCUnload = TRUE;
		m_PrivateConf.bCLoad = TRUE;

	}
	else
	{
        m_PrivateConf.bCScript   =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_SCRIPT",TRUE);;
		m_PrivateConf.bCNoScript =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_NOSCRIPT",TRUE);;

		m_PrivateConf.bCMouseClick =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_CLICK",TRUE);
		m_PrivateConf.bCMouseEnter =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_ENTER",TRUE);
		m_PrivateConf.bCMouseLeave =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_LEAVE",TRUE);
		m_PrivateConf.bCMouseOut   =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_OUT",TRUE);
		m_PrivateConf.bCMouseDown  =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_DOWN",TRUE);
		m_PrivateConf.bCMouseOver  =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_OVER",TRUE);
		m_PrivateConf.bCUnload     =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_UNLOAD",TRUE);;
		m_PrivateConf.bCLoad       =  AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_LOAD",TRUE);;
	}
	//m_PrivateConf.bCScript = AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_SCRIPT",TRUE);
	//m_PrivateConf.bCLoad = AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_LOAD",TRUE);
	//m_PrivateConf.bCMouseEvent = AfxGetApp()->GetProfileInt("PRIVATECONFIG","CLEAR_MOUSE_EVENT",TRUE);
	LPBYTE pData;
	UINT n;

	BOOL re = AfxGetApp()->GetProfileBinary("PRIVATECONFIG","DOWN_TYPES",(LPBYTE *)&pData,&n);
	if(re)
	{
		if(pData!=NULL)
		{
			memcpy(m_PrivateConf.enableTypeArray,pData,n);
			delete []pData;
		}
	}
	else
	{
		m_PrivateConf.enableTypeArray[0] = TRUE;
		m_PrivateConf.enableTypeArray[1] = TRUE;
	}

	m_bPrivateIniVal = TRUE;
	return TRUE;
}

BOOL ConfigManager::LoadSysConfig()
{
	SYSTEM_CONFIG  sysConfig;

	sysConfig.dwMode = AfxGetApp()->GetProfileInt("SYSCONFIG","DOWN_MODE",3);
	sysConfig.dwThrdNum = AfxGetApp()->GetProfileInt("SYSCONFIG","THREAD_NUM",3);//3:5trds
	sysConfig.dwRecordNum = AfxGetApp()->GetProfileInt("SYSCONFIG","RECORD_NUM",20);
	sysConfig.dwConnectTimes = AfxGetApp()->GetProfileInt("SYSCONFIG","CONNECT_TRY",2);
	sysConfig.dwWaitTime  = AfxGetApp()->GetProfileInt("SYSCONFIG","CONNECT_WAIT_TIME",4);//1,400 ms
	sysConfig.dwWorkMode = ANCHOR_MODE;// AfxGetApp()->GetProfileInt("SYSCONFIG","APP_WORK_MODE",ANCHOR_MODE);
	sysConfig.bShutDown = AfxGetApp()->GetProfileInt("SYSCONFIG","MACHINE_SHUTDOWN",0);
	sysConfig.bWarzard = AfxGetApp()->GetProfileInt("SYSCONFIG","USE_WARZARD",1);
	sysConfig.bAskBeforclose = 0;//AfxGetApp()->GetProfileInt("SYSCONFIG","ASK_BEFORE_CLOSE",1);
	SetSysConfig(&sysConfig);

	return TRUE;
}

BOOL  ConfigManager::SaveSessionConfig(void)//save to regesiter for next use
{

	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","ENABLE_LIMIT",m_PrivateConf.bEnableLimit);
	
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","DOWN_MODE",m_PrivateConf.dwDownMode);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","DOWN_LAY",m_PrivateConf.dwDownLay);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","ENABLE_EDIT",m_PrivateConf.bEnableEdit);

	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","MAX_TOTLE_SIZE",m_PrivateConf.dwMaxSize);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","MAX_FILE_SIZE",m_PrivateConf.dwMaxFileSize);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","MAX_FILE_NUM",m_PrivateConf.dwMaxFileNum);
	
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","PROXY_ENABLE",m_PrivateConf.bEnableProxy);

	//AfxGetApp()->WriteProfileInt("PRIVATECONFIG","PROXY_PORT",m_PrivateConf.dwPort);
	AfxGetApp()->WriteProfileString("PRIVATECONFIG","PROXY_NAME",m_PrivateConf.strProxyName);
	AfxGetApp()->WriteProfileString("PRIVATECONFIG","USER_NAME",m_PrivateConf.strUserName);
	AfxGetApp()->WriteProfileString("PRIVATECONFIG","USER_PASSWORD",m_PrivateConf.strPassWord);

	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","MODIFY_DEF",m_PrivateConf.bModifyDef);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_SCRIPT",m_PrivateConf.bCScript);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_NOSCRIPT",m_PrivateConf.bCNoScript);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_LOAD",m_PrivateConf.bCLoad);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_UNLOAD",m_PrivateConf.bCUnload);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_CLICK",m_PrivateConf.bCMouseClick);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_ENTER",m_PrivateConf.bCMouseEnter);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_LEAVE",m_PrivateConf.bCMouseLeave);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_OUT",m_PrivateConf.bCMouseOut);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_DOWN",m_PrivateConf.bCMouseDown);
	AfxGetApp()->WriteProfileInt("PRIVATECONFIG","CLEAR_MOUSE_OVER",m_PrivateConf.bCMouseOver);

	AfxGetApp()->WriteProfileBinary("PRIVATECONFIG","DOWN_TYPES",( LPBYTE)&m_PrivateConf.enableTypeArray,sizeof(m_PrivateConf.enableTypeArray));
	return TRUE;
}

BOOL  ConfigManager::SaveSysConfig(void)//save to regester
{
	AfxGetApp()->WriteProfileInt("SYSCONFIG","DOWN_MODE",m_SystemConf.dwMode);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","THREAD_NUM",m_SystemConf.dwThrdNum);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","RECORD_NUM",m_SystemConf.dwRecordNum);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","CONNECT_TRY",m_SystemConf.dwConnectTimes);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","CONNECT_WAIT_TIME",m_SystemConf.dwWaitTime);

	AfxGetApp()->WriteProfileInt("SYSCONFIG","APP_WORK_MODE",m_SystemConf.dwWorkMode);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","MACHINE_SHUTDOWN",m_SystemConf.bShutDown);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","USE_WARZARD",m_SystemConf.bWarzard);
	AfxGetApp()->WriteProfileInt("SYSCONFIG","ASK_BEFORE_CLOSE",m_SystemConf.bAskBeforclose);
	return TRUE;
}