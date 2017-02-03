#include "StdAfx.h"
#include ".\hmcompiler.h"
#include ".\tcompilertask.h"
#include ".\refpathcalculator.h"
#include "comrun.h"
#include "shlwapi.h"
#include "filepath.h"

CString TCompilerTask::strTaskfName;
TCompilerTask::TCompilerTask(CWnd *pWnd ,CString &strTaskFile)
{
	strTaskfName = strTaskFile;
	ChmCompiler::m_pWnd = pWnd;
	TxtCompiler::m_pWnd = pWnd;
	 
}

TCompilerTask::~TCompilerTask(void)
{
	 
}

void  TCompilerTask::Create(int nConver,void *p)
{
	DWORD trdID;

	if(nConver == CHM_CONVER)
		::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE )Run,NULL,0,&trdID);
	else
		::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE )Run2,NULL,0,&trdID);
}
 void TCompilerTask::Run(void)
 {
	 CComRun  comObject;
	 
	 Sleep(30);
	 if(!strTaskfName.IsEmpty() && PathFileExists(strTaskfName))
	 {
		 ChmCompiler  cp;
		 cp.CompilerExec(strTaskfName);
	 }

	 Sleep(30);
	 ChmCompiler::b_Exit = TRUE;
	 if(ChmCompiler::m_pWnd )
		 ChmCompiler::m_pWnd->SendMessage(WM_CLOSE,0,0);
	 ChmCompiler::b_Exit = FALSE;
	 ChmCompiler::m_pWnd = NULL;
 	 ExitThread(89);
 }

 void TCompilerTask::Run2(void)
 {
	  CComRun  comObject;

	  
	  if(TxtCompiler::m_pWnd )
		  TxtCompiler::m_pWnd->SetWindowText(_T("Converting  Sources to Text...")); 
	  Sleep(30);
	 if(!strTaskfName.IsEmpty() && PathFileExists(strTaskfName))
	 {
		 	pSYSTEM_CONFIG  pconf = ConfigManager::Instance()->GetSystemConfig();
			CString txtDir = TxtCompiler::GetTxtDirPath(strTaskfName);
			if(txtDir.IsEmpty())
			{
				AfxMessageBox("Error: task job file is wrong!");
				return ;
			}
			PrepareDirectory(txtDir,0);

			CStdioFile tskFile;
			BOOL ret = tskFile.Open(strTaskfName,CStdioFile::modeRead|CFile::shareDenyNone);
			ASSERT(ret == TRUE);

			int  nUrlNum;
			CString		 urlstr,validUrl;
			CString		 dirStr = strTaskfName.Left(strTaskfName.ReverseFind('\\'));
			if(pconf->dwWorkMode  == AUTO_MODE) //auto mode ..
			{
				tskFile.Seek(12,SEEK_SET);
				tskFile.ReadString(urlstr);//read root url
			}else
			{
				tskFile.Seek(14,SEEK_SET);
			}


			tskFile.Read(&nUrlNum,sizeof(int));

			CString textName = strTaskfName;

			::PathStripPath(textName.GetBuffer());
			textName.ReleaseBuffer();
			::PathRemoveExtension(textName.GetBuffer());
			textName.ReleaseBuffer();

			txtDir += textName + _T(".txt");

			CStdioFile  txtFile(txtDir,CStdioFile::modeCreate|CStdioFile::modeWrite);//one text file mode
			for( int i = 0;i < nUrlNum;i++)
			{
				if(FALSE == tskFile.ReadString(urlstr))
					break;

				_URL obj(urlstr);
				ExtractValidUrl(obj.GetURL(),&validUrl);//for 带参数的url.....
				CreatePathFromUrl(&dirStr,&validUrl ,urlstr,TRUE);
				CRefPathCalculator::ToSafeWay(urlstr);	
				TxtCompiler::Instance()->DoCompiler( urlstr,txtDir,&txtFile,1);
				if(TxtCompiler::m_pWnd )
					TxtCompiler::m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)urlstr.GetBuffer(),(LPARAM)0);
				urlstr.ReleaseBuffer();
				urlstr.Empty();
			}
			txtFile.Close();
			 
	 }


	 Sleep(30);
	 TxtCompiler::b_Exit = TRUE;
	 if(TxtCompiler::m_pWnd )
		 TxtCompiler::m_pWnd->SendMessage(WM_CLOSE,0,0);
	 TxtCompiler::b_Exit = FALSE;
	 TxtCompiler::m_pWnd = NULL;
 	 ExitThread(89);
 }