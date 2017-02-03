#include "StdAfx.h"
#include ".\tiesinkmanager.h"
#include "IEComCtrlSink.h"
#include "ConfigManager.h"
CIEComCtrlSink *TIESinkManager:: m_pCtrlSink = NULL;

CIEComCtrlSink * TIESinkManager::CreateSinkInstance()
{
	pSYSTEM_CONFIG pConf = ConfigManager::Instance()->GetSystemConfig();
	if(m_pCtrlSink && (pConf->dwWorkMode == ANCHOR_MODE) )
		return m_pCtrlSink;

	if(m_pCtrlSink)
	{
		m_pCtrlSink->OnQuit();
		delete m_pCtrlSink;
		m_pCtrlSink = NULL;
	}

	
	if(pConf->dwWorkMode ==ANCHOR_MODE)
		m_pCtrlSink = new CIEComCtrlSink();

	return m_pCtrlSink;
}

BOOL TIESinkManager::ReleaseSinkInstance()
{
	if(m_pCtrlSink)
	{
	//	m_pCtrlSink->OnQuit();
		///m_pUnkSink->Release();
//		delete m_pCtrlSink;
		m_pCtrlSink = NULL;
	}
	return TRUE;
}
CIEComCtrlSink * TIESinkManager::SinkInstance()
{
	return m_pCtrlSink;
}

 