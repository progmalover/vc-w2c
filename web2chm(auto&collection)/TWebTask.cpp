#include "StdAfx.h"
#include ".\twebtask.h"
#include "resource.h"
#include "filepath.h"
#include "mmsystem.h"
#include "ui.h"
#include "hmcompiler.h"
using namespace ui;
TWebTask::TWebTask(void)
{
	m_hConnect = NULL;
	m_hInterntClient = NULL;
	m_dwFileSize = 0;
	
}

TWebTask::~TWebTask(void)
{
}

#if 0
HRESULT  DownWebFile(_URL_TASK *pTask,CString Url,CString svPath,TASK_DOWN_STATUS &CInf)
{	
	//if(!pWnd)return FALSE;

	CString		strAgent;
	CString		strHeader;
	
	DWORD		dwFlags;
	DWORD		dwFSize;
	DWORD		dwServerType;
	HINTERNET	inetHandle;
	HINTERNET   hConnect;

	//check connect option and open url
	InternetGetConnectedState(&dwFlags, 0);//得到连接状态..是否使用代理

	strAgent.Format("Agent :%ld",timeGetTime());
	strHeader = "Accept: */*\r\n\r\n";

	if(dwFlags & INTERNET_CONNECTION_PROXY)
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	else
		inetHandle = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);


	if(!inetHandle)
		return INET_INIT_FAILURE;

/*打开一个url直到得到回应。。。获得相应时间*/
	CInf.dwResponTime = timeGetTime();
	if ( !(hConnect = InternetOpenUrlA ( inetHandle, Url, strHeader,
		strHeader.GetLength(), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
		return URL_OPEN_FAILURE;
	CInf.dwResponTime =(timeGetTime() - CInf.dwResponTime);

/*从网络读写数据。并统计下载数据率。。。。*/
	CStdioFile	cFile;

	DWORD		dwReadSize;
	
	DWORD		timeStart = 0;
	DWORD		timeUse = 0;
	BOOL		enable = 0;

	DWORD		dwByteToRead = 0;
	DWORD		dwSizeOfRq = 4;
	DWORD		dwBytes = 0;

	TCHAR		tdBuf[1028];

	if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}

/*根据目录建立文件*/
	DWORD dwErr;
	BOOL bRet = PrepareDirectory(FileStripFileName(svPath),0);
	if(!bRet)
		dwErr = GetLastError();

	if(!cFile.Open(svPath,CStdioFile::typeBinary | 
		CStdioFile::modeCreate|CStdioFile::modeWrite))
		return BUFF_FILE_FAILURE;


	timeStart = timeGetTime();

	TWebTask *pWebTask = (TWebTask *)pTask;
	while(1)
	{
		InternetReadFile(hConnect,tdBuf,1024,&dwReadSize);
		cFile.Write(tdBuf,dwReadSize);

		CInf.dwEscapeTime = timeGetTime() - timeStart;
		CInf.dwDownSize = cFile.GetLength();
		if(CInf.dwEscapeTime == 0)
			CInf.dwEscapeTime = 1;
		CInf.dwBitRat = 8*CInf.dwDownSize / CInf.dwEscapeTime;
		
/*如果传入了句柄，不是后台运行的话。。。*/
		if(pTask->GetStatus() == _URL_TASK::THREAD_EXIT)
			break;
		if(pWebTask->Display()) 
			gp_DownTaskList->PostMessage(WM_DOWNPROGRESS,(DWORD)pTask,(DWORD)&CInf);
		
		m_FileSize += dwReadSize;
		if(dwReadSize != 1024)
			break;
	}
	CInf.dwEscapeTime = timeGetTime() - timeStart;

	if(timeUse == 0)
		timeUse = 10;

	//CInf.bitRat = (float)8*cFile.GetLength() * 1000.0f / (float)timeUse;
	//CInf.useTime = timeUse;
	CInf.dwDownSize = cFile.GetLength();

	cFile.Close();

	InternetCloseHandle(inetHandle);
	return S_OK;
}

#endif

void TWebTask ::Run(void)
{
	DWORD dwByteToRead = 0 ;
	DWORD dwReadSize = 0;
	DWORD dwSizeOfRq = 0;
	DWORD dwTime = 0;
	TCHAR tdBuf[1025]={0};

	TRACE("Ok Entrer here :%d\n",m_dwThreadStatus);
	pPRIVATE_CONFIG   pconf = ConfigManager::Instance()->GetSessionConfig(this);
	if(NULL == pconf)
		pconf = ConfigManager::Instance()->GetDefaultSessionConf();

LStart:
	while(m_dwThreadStatus != _URL_TASK::THREAD_EXIT)
	{
		TRACE("Ok  I'm Runing %d\n",m_dwThreadID);
		if(m_dwThreadStatus == _URL_TASK::THREAD_FREE)
		{
			SuspendThread(m_hThread);
			goto LStart;
		}

		if(m_dwThreadStatus == _URL_TASK::THREAD_SUSPEND)
		{
			SuspendThread(m_hThread);
			goto LStart;
		}

/*download the file,and then notify parent session*/
		
		if(m_hConnect == NULL)
		{
		
			{
				DWORD dwErr = GetLastError();
				TRACE("======>TWebTask::Run() err:%d\n",dwErr);
				m_dwThreadStatus = _URL_TASK::THREAD_FAILURE;
				SuspendThread(m_hThread);
				goto LStart;
			}
		}

		THREAD_EXIT_CHECK(this,break);
		/*
		TASK_DOWN_STATUS  inf;
		DownWebFile(this,*(CString *)m_UrlObj.GetURL(),"e:\\dddd.html",inf);*/
		HttpQueryInfo(m_hConnect, HTTP_QUERY_CONTENT_LENGTH /*| HTTP_QUERY_FLAG_NUMBER*/, 
			(LPVOID)NULL, &dwSizeOfRq, NULL);
		if (!HttpQueryInfo(m_hConnect, HTTP_QUERY_CONTENT_LENGTH /*| HTTP_QUERY_FLAG_NUMBER*/, 
			(LPVOID)tdBuf, &dwSizeOfRq, NULL))
		{
			dwByteToRead = 0;
			TRACE("HttpQueryInfo ERROR:%x\n",GetLastError());
		}else
		{
			tdBuf[dwSizeOfRq] = '\0';
			dwByteToRead = StrToInt(tdBuf);
		}

		if(pconf->bEnableLimit )
		{
			if(dwByteToRead > pconf->dwMaxFileSize * 1024 * 1024)
			{
				m_stdFile.Close();
				DeleteFile(m_stdFile.GetFilePath());
			}
		}

		 THREAD_EXIT_CHECK(this,break);

		if(m_hConnect && m_stdFile.m_pStream)
		{
			while(1)
			{

				if(m_dwThreadStatus == _URL_TASK::THREAD_SUSPEND)
				{
					SuspendThread(m_hThread);
				}
				THREAD_EXIT_CHECK(this,break);
				if(InternetReadFile(m_hConnect,tdBuf,1024,&dwReadSize)== FALSE)
				{
					m_dwThreadStatus = _URL_TASK::THREAD_FAILURE;
					break;
				}
				THREAD_EXIT_CHECK(this,break);
				m_stdFile.Write(tdBuf,dwReadSize);
				
				THREAD_EXIT_CHECK(this,break);
			
	/*如果传入了句柄，不是后台运行的话。。。*/
				if(dwByteToRead > 0)
				{
					int  percent;
					 
					if(m_stdFile.GetLength() >dwByteToRead)
						percent = 0;
					else
						percent = m_stdFile.GetLength() * 100 / dwByteToRead;

					m_prgCtrl.SetProgressVal(  percent );
				}

				if(dwReadSize <= 0)
					break;
				else
					m_dwFileSize += dwReadSize;
			}
			m_dwThreadStatus = _URL_TASK::THREAD_COMPLETE;

		}
		else
		  m_dwThreadStatus = _URL_TASK::THREAD_FAILURE;

		THREAD_EXIT_CHECK(this,break);

		//if(m_stdFile.m_pStream)
		//{
		//	//m_stdFile.Flush();
		//	m_stdFile.Close();
		//	//ASSERT(m_stdFile.m_pStream == NULL);
		//}
 
		
		if(m_dwThreadStatus != THREAD_EXIT)
		::SuspendThread(m_hThread);
	
	}

	ExitThread(0x99);
	//::CloseHandle(m_hThread);
	//m_hThread = NULL;
	
}


void TWebTask :: Notify(int nStatCode)
{

}

void TWebTask :: OnFinish(void)
{

}


BOOL TWebTask::Display(void)
{
	
	return TRUE;
}


HRESULT TWebTask::Prepare()
{
	
/*
#define DWERR_OPEN_INTNET_CLIENT_FAILURE  0x89
#define DWERR_OPEN_INTNET_URL_FAILURE	  0x8a
#define DWERR_FILE_EXIST	  0x8b
#define DWERR_FILE_OPEN_FAILURE	  0x8c
DWERR_CREATE_DIR_FAILURE
*/
	DWORD dwErr;
	m_dwError = 0;

	if(m_UrlObj.GetURL()->IsEmpty())
		return S_FALSE;

/*建立文件夹和文件*/
	
	URL_STRING  validUrl;
	URL_STRING *pUrl = m_UrlObj.GetURL();	
	TWebSession *pSession =(TWebSession *) m_pParent;

	ExtractValidUrl(pUrl,&validUrl);//for 带参数的url.....

	PATH_STRING  strPath = pSession->GetUrlLocalPath(validUrl);
	URL_STRING  filePath = FileStripFileName(strPath.GetBuffer());
	strPath.ReleaseBuffer();

	BOOL bRet = FALSE;
	//ToDosPath(strPath);
	m_UrlObj.SetHstFileName(strPath);
	bRet = ::PathFileExists(strPath);
	if(TRUE == bRet)
	{
		m_dwError = DWERR_FILE_EXIST;
		m_dwThreadStatus =  _URL_TASK::THREAD_FREE;
		 
		return S_FALSE;
	}

	bRet = PrepareDirectory(filePath.GetBuffer(),0);
		filePath.ReleaseBuffer();
	
	if(FALSE == bRet)
	{
		dwErr = GetLastError();
		TRACE("========>TWebTask::Prepear() err:%d\n",dwErr);
		m_dwThreadStatus =  _URL_TASK::THREAD_FAILURE;
		return S_FALSE;
	}

	if(m_stdFile.m_pStream)
	{
	//	m_stdFile.Flush();
		m_stdFile.Close();
	}
	if(!m_stdFile.Open(strPath,CStdioFile::typeBinary | 
		CStdioFile::modeCreate|CStdioFile::modeWrite|CStdioFile::modeRead))
	{
		dwErr = GetLastError();
		TRACE("========>TWebTask::Prepear() err:%d\n",dwErr);

		m_dwError = DWERR_FILE_OPEN_FAILURE;
		m_dwThreadStatus =  _URL_TASK::THREAD_FAILURE;
	
		return S_FALSE;
	}
	
    strPath.ReleaseBuffer();
//	m_UrlObj.SetHstFileName(strPath);

	
	/**begin web set...*/
	if(m_hInterntClient)
	{
		::InternetCloseHandle(m_hInterntClient);
		m_hInterntClient = NULL;
	}

	if(m_hInterntClient == NULL)
		m_hInterntClient = CreateInternetHandle(m_dwError);

	if(m_hInterntClient == NULL)
	{
		//m_dwThreadStatus =  _URL_TASK::THREAD_FAILURE;
		m_dwError = DWERR_OPEN_INTNET_CLIENT_FAILURE;
		dwErr = GetLastError();
		TRACE("========>TWebTask::Prepear() err:%d\n",dwErr);
		m_dwError = DWERR_OPEN_INTNET_CLIENT_FAILURE;
		m_stdFile.Close();
		DeleteFile(strPath);
		return S_FALSE;
	}

	if(m_hConnect)
		::InternetCloseHandle(m_hConnect);
	
	m_hConnect = OpenInternetUrl(m_hInterntClient,m_UrlObj.GetURL(),m_dwError);

	if(!m_hConnect)
	{
		//m_dwThreadStatus =  _URL_TASK::THREAD_FAILURE;
		m_dwError = DWERR_OPEN_INTNET_URL_FAILURE;
		dwErr = GetLastError();
		TRACE("========>TWebTask::Prepear() err:%d\n",dwErr);
		m_dwError = DWERR_OPEN_INTNET_URL_FAILURE;
		::InternetCloseHandle(m_hInterntClient);
		//m_stdFile.Close();
		DeleteFile(strPath);

		return S_FALSE;
	}

	DWORD dwLen =  200;
	LPSTR _pBuf =  (LPSTR)_alloca(200);// ChmCompiler::_strLang.GetBufferSetLength();

    if(HttpQueryInfo(m_hConnect,HTTP_QUERY_CONTENT_LANGUAGE,_pBuf,&dwLen,NULL))//get lang id here.
	{
		_pBuf[dwLen]  = '\0';
		 ChmCompiler::_strLang = _pBuf;
		 TRACE("***get lang : %s \n",_pBuf);
	}
	 
	m_prgCtrl.SetOwner(this);
	m_prgCtrl.PrepareProgress();
	
	m_prgCtrl.SetProgressInf(m_stdFile.GetFileName(),pUrl->GetBuffer(),0);
	pUrl->ReleaseBuffer();
	m_dwFileSize = 0 ;
   	return S_OK;
}

HRESULT  TWebTask::UnPrepare()
{

	if(m_stdFile.m_pStream)
	{
		m_stdFile.Close();
	}
	if(m_hInterntClient)
	{	
		::InternetCloseHandle(m_hInterntClient);
		m_hInterntClient = NULL;
	}

	if(m_hConnect)
	{
		::InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}

//	m_prgCtrl.UnPrepareProgress();

	m_dwThreadStatus =  _URL_TASK::THREAD_FREE;
//	m_UrlObj.SetURL(URL_STRING());
	//m_dwFileSize = 0 ;
	return S_OK;
}

HRESULT TWebTask:: Exec()
{
	return S_OK;
}
BOOL TWebTask::SetURL(URL_STRING uStr)
{
	BOOL bret = _URL_TASK::SetURL(uStr);
	if(bret)
	{
	 HRESULT hres = Prepare();
	 if(hres != S_OK)
	 TRACE("======>TWebTask::SetURL():Prepare() Failure!!");
	}
	return bret;
}

void TWebTask::ActiveTaskUI(void)
{
	 m_prgCtrl.SetActive();
}

void TWebTask::UnActiveTaskUI(void)
{
	 m_prgCtrl.SetUnActive();
}

void TWebTask::FreeFile(void)
{
	/*if(m_stdFile.m_pStream)
		m_stdFile.Close();*/
	 
}
