#pragma once
#include "Defs.h"
#include "StaticInstance.h"
#define DEFAULT_MAXSIZE        1000
#define DEFAULT_MAXSIZE_INDEX  3
#define DEFAULT_MAXFILESIZE    5
#define DEFAULT_MAXFILESIZE_INDEX 2
#define DEFAULT_MAXFILENUMS    500
#define DEFAULT_DOWNLOD_MODE   1
#define DEFAULT_LAY            0
class ConfigManager :public StaticInst<ConfigManager>
{
public:
	 ConfigManager(void);
	 ~ConfigManager(void);

	 BOOL  SetSysConfig(pSYSTEM_CONFIG  pSysconfig);
	 BOOL  SetSessionConfig(_URL_TASK *pTask,pPRIVATE_CONFIG pTaskConf);
	 BOOL  SetDefaultSessionConf(pPRIVATE_CONFIG pTaskConf);
	 BOOL  LoadSessionConfig(void);//载入session配置项
	 BOOL  LoadSysConfig(void);//载入系统配置项
	 BOOL  SaveSessionConfig(void);//save to regesiter for next use
	 BOOL  SaveSysConfig(void);//save to regester
	 pPRIVATE_CONFIG   GetDefaultSessionConf(void);
	 pSYSTEM_CONFIG    GetSystemConfig(void);
	 pSYSTEM_CONFIG    GetDefaultSystemConfig(void);
	 pPRIVATE_CONFIG   GetSessionConfig(_URL_TASK *pTask);
	
private:
	 CONFIG_MAP  m_PrviteConfMap;

	 SYSTEM_CONFIG	m_SystemConf;
	 PRIVATE_CONFIG m_PrivateConf;

	 BOOL			m_bSysInitVal;
	 BOOL			m_bPrivateIniVal;
};