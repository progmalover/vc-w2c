#pragma once
#include "HttpApi.h"
#include "Defs.h"
#include "TExec.h"
#include "CProgressList.h"
#include "ui.h"
//
//ui progress for task 
//
#define  CHECK_ACTIVE()\
	if(m_bActive == FALSE) return;
class TWebSession;
class TWebTask;
class ProgressCtrl{
public:
	 
	ProgressCtrl( )
	{
		m_pCell=NULL;
		m_fName = " ";
		m_fUrl = " ";
		m_bActive = FALSE;
		m_pTrd = NULL;
	};
	~ProgressCtrl(){};
public:
	void  SetOwner(TWebTask *pTrd){m_pTrd = pTrd;};
	void  SetActive(void){m_bActive = TRUE;};
	void  SetUnActive(void){m_bActive = FALSE;}; 
	void  SetProgressInf(LPCSTR str1,LPCSTR str2,int val)
	{
		CHECK_ACTIVE();

		m_DataInf.pCell = m_pCell;
		m_DataInf.Str1 = str1;
		m_DataInf.Str2 = str2;
		m_DataInf.val = val;

		 if(GP_TskPrgList)
		 {
			 GP_TskPrgList->Lock();

			 GP_TskPrgList->SendMessage(WM_SET_SELL_INF,(WPARAM)&m_DataInf,0 );

			 GP_TskPrgList->UnLock(); 
			
		 }
		
		 m_fName = str1;
		 m_fUrl = str2;
	};

	void SetProgressVal(int val)
	{
		CHECK_ACTIVE();
		
		if(GP_TskPrgList)
		{	
			GP_TskPrgList->Lock();
			
			m_DataInf.pCell = m_pCell;
			m_DataInf.Str1 = m_fName;
			m_DataInf.Str2 = m_fUrl;
			m_DataInf.val = val;

			GP_TskPrgList->SendMessage(WM_SET_SELL_INF,(WPARAM)&m_DataInf,0 );

			GP_TskPrgList->UnLock();
			
		}
	}

	void PrepareProgress()
	{
		CHECK_ACTIVE();
       
		if(!GP_TskPrgList) return;

		GP_TskPrgList->Lock();
		GP_TskPrgList->GetProgressBar(m_pCell,m_pTrd);

		m_DataInf.pCell = m_pCell;
		m_DataInf.Str1 = m_fName;
		m_DataInf.Str2 = m_fUrl;
		m_DataInf.val = 0;
		if(m_pCell)
			  GP_TskPrgList->SendMessage(WM_SET_SELL_INF,(WPARAM)&m_DataInf,0 );

		 GP_TskPrgList->UnLock();
	};

	void UnPrepareProgress()
	{
		CHECK_ACTIVE();
		if(!GP_TskPrgList)
			return;

		m_fUrl = "";
		m_fName= "";

		GP_TskPrgList->Lock();

		m_DataInf.pCell = m_pCell;
		m_DataInf.Str1 = m_fName;
		m_DataInf.Str2 = m_fUrl;
		m_DataInf.val = 0;

		if(m_pCell)
			 GP_TskPrgList->SendMessage(WM_SET_SELL_INF,(WPARAM)&m_DataInf,0 );
		
		GP_TskPrgList->UnLock();

		if(m_pCell)
		{
			m_pCell->bused = FALSE;
			m_pCell->pTrd = NULL;
		}
	};
public:
	pPROGRESS_SELL  m_pCell;
	URL_STRING m_fName;
	URL_STRING m_fUrl;
	PROG_INF   m_DataInf;

	TWebTask  * m_pTrd;
private:
	volatile BOOL m_bActive;
} ;

/************************************************************************/
class TWebTask :
	public _URL_TASK,TExec
{

	friend class TWebSession;
public:
	TWebTask(void);
	~TWebTask(void);
	void Run(void);
	void Notify(int nStatCode);
	void OnFinish(void);
/*TExec*/
	HRESULT Prepare();
	HRESULT UnPrepare();
	HRESULT Exec();
	BOOL Display(void);
public:
	//TASK_DOWN_STATUS	m_TaskDownStatus; 
	ProgressCtrl		m_prgCtrl;
	DWORD				m_dwError;//error code for judge .....
//private:
	CStdioFile m_stdFile;
	HINTERNET  m_hConnect;
	HINTERNET  m_hInterntClient;
	HRESULT    hres;
public:
	BOOL SetURL(URL_STRING uStr);
	virtual void ActiveTaskUI(void);
	virtual void UnActiveTaskUI(void);
	void FreeFile(void);
protected:
	DWORD  m_dwFileSize;
};
#define DWERR_OPEN_INTNET_CLIENT_FAILURE  0x89
#define DWERR_OPEN_INTNET_URL_FAILURE	  0x8a
#define DWERR_FILE_EXIST	  0x8b
#define DWERR_FILE_OPEN_FAILURE	  0x8c
#define DWERR_CREATE_DIR_FAILURE  0x8d