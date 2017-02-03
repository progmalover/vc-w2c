#include "StdAfx.h"
#include <iostream>
#include ".\hmcompiler.h"
#include "HttpApi.h"
#include "filepath.h"
#include "htmlparse.h"
#include <comdef.h>
#include <mshtml.h>
#include ".\tcatalogbuilder.h"
#include "configmanager.h"
#include "charset.h"
#include ".\refpathcalculator.h"
#include "utils.h"
COMPILERFUNC  ChmCompiler::m_func = NULL ;
HINSTANCE	  ChmCompiler::m_hinstLib = NULL ; 
CWnd		* ChmCompiler::m_pWnd;
BOOL		  ChmCompiler::b_Exit ;
#ifdef _DEBUG
CStdioFile    ChmCompiler::m_logFile;
#endif
//如果减去不成功就还是原来的

//CString  ChmCompiler::m_strCharSet;
CString	ChmCompiler:: _strLang ;
CString strMessage;


ChmCompiler::ChmCompiler(void)
{
	
}  

ChmCompiler::~ChmCompiler(void)
{
#ifdef _DEBUG
	if(ChmCompiler::m_logFile.m_pStream)
		ChmCompiler::m_logFile.Close();
#endif
}

BOOL ChmCompiler::FuncProgress(char* pstr) //进度回调
{
	 strMessage = pstr;
	if(m_pWnd)
		m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)strMessage.GetBuffer(),(LPARAM)0);
	strMessage.ReleaseBuffer();
	//if(b_Exit)
		//return -1;
	return TRUE;
}

BOOL ChmCompiler::FunLog(char* pstr)
{
	 
	//if(m_pWnd)
	//	m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)pstr,(LPARAM)0);
#ifdef _DEBUG
	ChmCompiler::m_logFile.WriteString(pstr);
	ChmCompiler::m_logFile.WriteString(_T("\n"));
#endif
	return TRUE;
}

//建立hhp工程设置文件。。。。。
CString ChmCompiler:: CreateHHP(CString strTaskfName, LPCTSTR strHHC,LPCTSTR strHHK)
{
	CStdioFile   hhpFile;
	CStdioFile   tskFile;

	CString		 strHHP;
	CString		 dirStr = strTaskfName.Left(strTaskfName.ReverseFind('\\'));
	CString		 taskName =  strTaskfName.Right(strTaskfName.GetLength() - strTaskfName.ReverseFind('\\') - 1);
	pSYSTEM_CONFIG  pconf = ConfigManager::Instance()->GetSystemConfig();

	if(dirStr.IsEmpty())
		return CString();

	strHHP = GetTempFile(dirStr,_T("~temp"),_T("hhp"));

	BOOL ret = hhpFile.Open(strHHP,CStdioFile::modeCreate|CStdioFile::modeWrite);
    ASSERT(ret == TRUE);
	ret = tskFile.Open(strTaskfName,CStdioFile::modeRead|CFile::shareDenyNone);
	ASSERT(ret == TRUE);
	if(FALSE == ret)
		return CString();
	CString defaultPage,pcstr,urlstr;
	
	if(pconf->dwWorkMode  == AUTO_MODE) //auto mode ..
	{
		tskFile.Seek(12,SEEK_SET);
		tskFile.ReadString(pcstr);//read root url
		ExtractValidUrl(&pcstr,&urlstr);
		
		CreatePathFromUrl(&dirStr,&urlstr,defaultPage,TRUE);
		CRefPathCalculator::ToSafeWay(defaultPage);
	}

	else
	{
		tskFile.Seek(14,SEEK_SET);
		if(S_FALSE == TCatalogBuilder::GetIndexPageName(defaultPage))
			TCatalogBuilder::GetStartPagesName(defaultPage);
	}

	CString hhpString ,sectionStr;
	
	hhpString += _T("[OPTIONS]\nCompatibility=1.1 or later\n");
    hhpString += _T("Binary Index=No\n");//for org index order...
	sectionStr.Format(_T("Compiled file=%s.chm\n"),taskName.Left(taskName.Find('.')));
	hhpString += sectionStr;
	
	sectionStr.Format(_T("Contents file=%s\n"),strHHC);
	hhpString += sectionStr;
   
	hhpString +=  _T("Default Window=winmy\n");
	
	sectionStr.Format(_T("Default topic=%s\n"),defaultPage);
	hhpString += sectionStr;
	
	hhpString += _T("Display compile progress=Yes\n");
	sectionStr.Format(_T("Index file=%s\n"),strHHK);
	hhpString += sectionStr;


	if(ChmCompiler::_strLang.IsEmpty())
		sectionStr.Format(_T("Language=0x%x\n"), GetUserDefaultLangID());
	else
	{
		int langID = StrToLangID(ChmCompiler::_strLang);
		langID = (langID == -1)?GetUserDefaultLangID():langID;//默认
		sectionStr.Format(_T("Language=0x%x\n"),langID);	 
	}

	//hhpFile.WriteString(sectionStr);//409英语 804 汉语
	hhpString += sectionStr;
	hhpString += _T("Title=Powered By W2EBook-www.ten-dollarsoft.com\n[WINDOWS]\n");

	CString FormatStr = _T("winmy=,\"%s\",\"%s\",\"%s\",\"%s\",,,,,0x2120,,0x106e,,0x10c30000,0x22000,,,,,0\n\n\n");

	sectionStr.Format(FormatStr,strHHC,strHHK,defaultPage,defaultPage);
	
	hhpString += sectionStr;
	hhpString += _T("[FILES]\n");

	 if(pconf->dwWorkMode == ANCHOR_MODE)//collection
	{
		if(S_OK  == TCatalogBuilder::GetIndexPageName(defaultPage))
		{
			hhpString += defaultPage + _T("\n");
			URL_LIST  &list = TCatalogBuilder::GetStartPagesNameList();
			for(URL_LIST::iterator it = list.begin();it != list.end();it++)
			{
				defaultPage = *it;
				hhpString += defaultPage + _T("\n");
			}
		}else
		{
			TCatalogBuilder::GetStartPagesName(defaultPage);
			hhpString += defaultPage + _T("\n");
		}
	}

    int urlNum;
	URL_STRING   urlString,validUrl ;
	tskFile.Read(&urlNum,4);//get downloaded file number...

	int i  ;
	for(i= 0 ;i < urlNum;i++)
	{
			if(FALSE == tskFile.ReadString(urlString))
				break;

			_URL obj(urlString);
			ExtractValidUrl(obj.GetURL(),&validUrl);//for 带参数的url.....
			CreatePathFromUrl(&dirStr,&validUrl ,urlstr,TRUE);
			CRefPathCalculator::ToSafeWay(urlstr);	

			urlstr.ReleaseBuffer();
			if(VerifyFile(urlstr))
				hhpString += urlstr + _T("\n");
			urlstr.Empty();
	}

	hhpFile.WriteString(hhpString);

	tskFile.Close();
	hhpFile.Close();
	if(i <= 0)
	{
		::DeleteFile(strHHP);
		strHHP.Empty();
	}
	return strHHP;
}

void   ChmCompiler::CreateDirStructStr(CString &strDir ,CString &strFiles)
{
	CFileFind find;
	CString subDirStr,formatStr;
	BOOL bfind = find.FindFile(strDir);
	 
	while(bfind)
	{
		bfind = find.FindNextFile();
		/*if(!bfind)
			break;*/

		if(find.IsDots())
			continue;
		if(find.IsDirectory())//is dir
		{
			strFiles += _T("<LI> <OBJECT type=\"text/sitemap\">\n");
			formatStr = _T("<param name=\"Name\" value=\"%s\">\n");
			subDirStr.Format(formatStr,find.GetFileName());
		
			strFiles += subDirStr;
			strFiles += _T("</OBJECT>\n");
			subDirStr = find.GetFilePath();

			subDirStr += _T("\\*.*");
			strFiles += _T("<UL>");

			CreateDirStructStr(subDirStr,strFiles);
			strFiles += _T("</UL>\n");
		}
		else
		{
			if(find.IsHidden())
				continue;
			if(find.IsSystem())
				continue;

			CString  strFileName = find.GetFileName();
			CString  strfileExt = ::PathFindExtension(strFileName);

			if(strfileExt.Compare(_T(".hhc"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhk"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhp"))==0)
				continue;
			if(strfileExt.Compare(_T(".tsk"))==0)
				continue;
			if(strfileExt.Compare(_T(".job"))==0)
				continue;
			if(strfileExt.Compare(_T(".lnk"))==0)
				continue;

		
			DWORD	dwType;
			int		nlen = 0;
			CString  filePath = find.GetFilePath();

			formatStr = _T(" <param name=\"Name\" value=\"%s\">\n");//something wrong here ...

			if(VerifyFile(filePath) == FALSE)
				 continue;
			if(find.IsCompressed())
				goto _NoHtmlFile;

			if(IsHtmlTypeFile(filePath,dwType))
			{
				CHtmlParse		parse;
				CString			title;
			   if(parse.LoadFromFile(filePath) == S_FALSE)
					continue;
			  // if(parse.SimpleTest()!= TRUE)
				//    continue;
			   if(parse.GetPageTitle(title ,nlen) &&  nlen > 1) 
			   {
				   CString chSetName;
				   if(parse.GetCharSetName(chSetName))
						ConverCharSet(chSetName,title,nlen);
			   }
				else
					title = filePath.Right(filePath.GetLength() - filePath.ReverseFind('\\') - 1);

			   subDirStr.Format(formatStr,title);
				 
			}
			else
			{
_NoHtmlFile:
				subDirStr.Format(formatStr,strFileName);//zero 文件 会不会 导致 问题  ？？
			}

			strFiles += _T("<LI> <OBJECT type=\"text/sitemap\">\n");
			strFiles += subDirStr;

			formatStr = _T(" <param name=\"Local\" value=\"%s\">\n</OBJECT>\n");

			subDirStr.Format(formatStr,filePath);
			strFiles += subDirStr;
		}
	}
}

void   ChmCompiler::CreateDirStruct2(CString &strDir,CStdioFile *phhcFile)
{
	CFileFind find;
	BOOL bfind = find.FindFile(strDir);
	
	CString subDirStr;
	CString formatStr;
	CString hhcTxt;
	
	hhcTxt.Empty();
	while(bfind)
	{
		bfind = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory())//is dir
		{
			hhcTxt += _T("<LI> <OBJECT type=\"text/sitemap\">\n");
			formatStr = _T("<param name=\"Name\" value=\"%s\">\n");
			subDirStr.Format(formatStr,find.GetFileName());
			
			hhcTxt += subDirStr + _T("</OBJECT>\n");

			subDirStr = find.GetFilePath();
			
			subDirStr += _T("\\*.*");
			hhcTxt += _T("<UL>");

			phhcFile->Write(hhcTxt,hhcTxt.GetLength());
			hhcTxt.Empty();
			CreateDirStruct2(subDirStr,phhcFile);
			hhcTxt += _T("</UL>\n");
		}
		else// if(!find.IsDirectory())//is file file
		{
			CString  strFileName = find.GetFileName();
			CString  strfileExt = ::PathFindExtension(strFileName);
			if(strfileExt.Compare(_T(".hhc"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhk"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhp"))==0)
				continue;
			if(strfileExt.Compare(_T(".tsk"))==0)
				continue;
			if(strfileExt.Compare(_T(".job"))==0)
				continue;
			if(strfileExt.Compare(_T(".lnk"))==0)
				continue;

		
			DWORD	dwType;
			int		nlen = 0;
			CString  filePath = find.GetFilePath();

			if(VerifyFile(filePath) == FALSE)
				 continue;

			formatStr = _T(" <param name=\"Name\" value=\"%s\">\n");//something wrong here ...
			if(IsHtmlTypeFile(filePath,dwType))
			{
				CHtmlParse		parse;
				CString			title;
			   if(parse.LoadFromFile(filePath) == S_FALSE)
					continue;
			  // if(parse.SimpleTest()!= TRUE)
				//    continue;
			   if(parse.GetPageTitle(title ,nlen) &&  nlen > 1) 
			   {
				   CString chSetName;
				   if(parse.GetCharSetName(chSetName))
						ConverCharSet(chSetName,title,nlen);
			   }
				else
					title = filePath.Right(filePath.GetLength() - filePath.ReverseFind('\\') - 1);

			   subDirStr.Format(formatStr,title);
				 
			}
			else
			{
				subDirStr.Format(formatStr,strFileName);//zero 文件 会不会 导致 问题  ？？
			}

			hhcTxt += _T("<LI> <OBJECT type=\"text/sitemap\">\n");
			hhcTxt +=  subDirStr;

			formatStr = _T(" <param name=\"Local\" value=\"%s\">\n</OBJECT>\n");

			subDirStr.Format(formatStr,filePath);
			hhcTxt += subDirStr;
		}
	}

	if(hhcTxt.IsEmpty()== false)
		phhcFile->Write(hhcTxt,hhcTxt.GetLength());

}

 

void   ChmCompiler::CreateDirStruct(CString &strDir,CStdioFile &hhcFile)
{
	CFileFind find;
	CString subDirStr,formatStr;
	BOOL bfind = find.FindFile(strDir);
	 
	while(bfind)
	{
		bfind = find.FindNextFile();
		if(!bfind)
			break;
		if(find.IsDots())
			continue;
		if(find.IsDirectory())//is dir
		{
			hhcFile.WriteString(_T("<LI> <OBJECT type=\"text/sitemap\">\n"));
			formatStr = _T("<param name=\"Name\" value=\"%s\">\n");
			subDirStr.Format(formatStr,find.GetFileName());
			hhcFile.WriteString(subDirStr);
			hhcFile.WriteString(_T("</OBJECT>\n"));
			
			
			subDirStr = find.GetFilePath();
			
			subDirStr += _T("\\*.*");

			hhcFile.WriteString(_T("<UL>"));
			CreateDirStruct(subDirStr,hhcFile);
			hhcFile.WriteString(_T("</UL>\n"));
		}
		else if(!find.IsDirectory())//is file file
		{
			CString  strFileName = find.GetFileName();
			CString  strfileExt = ::PathFindExtension(strFileName);
			if(strfileExt.Compare(_T(".hhc"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhk"))==0)
				continue;
			if(strfileExt.Compare(_T(".hhp"))==0)
				continue;
			if(strfileExt.Compare(_T(".tsk"))==0)
				continue;
			if(strfileExt.Compare(_T(".job"))==0)
				continue;
			if(strfileExt.Compare(_T(".lnk"))==0)
				continue;

		
			DWORD	dwType;
			int		nlen = 0;
			CString  filePath = find.GetFilePath();

			formatStr = _T(" <param name=\"Name\" value=\"%s\">\n");//something wrong here ...

			if(VerifyFile(filePath) == FALSE)
				 continue;
			if(IsHtmlTypeFile(filePath,dwType))
			{
				CHtmlParse		parse;
				CString			title;
			   if(parse.LoadFromFile(filePath) == S_FALSE)
					continue;
			   if(parse.SimpleTest()!= TRUE)
				    continue;
			   if(parse.GetPageTitle(title ,nlen) &&  nlen > 1) 
			   {
				   CString chSetName;
				   if(parse.GetCharSetName(chSetName))
						ConverCharSet(chSetName,title,nlen);
			   }
				else
					title = filePath.Right(filePath.GetLength() - filePath.ReverseFind('\\') - 1);

			   subDirStr.Format(formatStr,title);
				 
			}
			else
			{
				subDirStr.Format(formatStr,strFileName);//zero 文件 会不会 导致 问题  ？？
			}

			hhcFile.WriteString(_T("<LI> <OBJECT type=\"text/sitemap\">\n"));
			hhcFile.WriteString(subDirStr);

			formatStr = _T(" <param name=\"Local\" value=\"%s\">\n</OBJECT>\n");

			subDirStr.Format(formatStr,filePath);
			hhcFile.WriteString(subDirStr);
		}
	}
}

CString ChmCompiler:: CreateHHC2(CString strTaskfName)
{
		CString		strHHC;

		return strHHC;

}

#define  _FUNC_1
CString ChmCompiler:: CreateHHC(CString strTaskfName)//建立内容目录文件。。。。
{
	CStdioFile  hhcFile;  
	CString		strHHC;
	CString	dirStr = strTaskfName.Left(strTaskfName.ReverseFind('\\'));
	CString taskName =  strTaskfName.Right(strTaskfName.GetLength() - strTaskfName.ReverseFind('\\') - 1);

	if(dirStr.IsEmpty())
		return CString();
	 
	strHHC = GetTempFile(dirStr,_T("~temp"),_T("hhc"));
	 
	BOOL ret = hhcFile.Open(strHHC,CStdioFile::modeCreate|CStdioFile::modeWrite);
	ASSERT(ret == TRUE);

	//header
	hhcFile.WriteString(
		_T("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">\n<HTML>\n<HEAD>\n")
		_T("<meta name=\"GENERATOR\" content=\"Web2Chm 1.0 \">\n")
		_T("<!-- Sitemap 1.0 -->\n")
		_T("</HEAD><BODY>\n")
		_T("<OBJECT type=\"text/site properties\">\n")
		_T("<param name=\"ImageType\" value=\"book\">\n")
		_T("</OBJECT>\n <UL>")
		);

	CString findName = dirStr;
	if(findName.Right(1)!= '\\')
		findName += '\\';

	findName += _T("*.*");

#ifdef _FUNC_0
	CString  strFiles;
	CreateDirStructStr(findName,strFiles);
	hhcFile.WriteString(strFiles + "</UL>""</BODY></HTML>");
#endif

#ifdef _FUNC_1
	CreateDirStruct2(findName,&hhcFile);
#endif
#ifdef _FUNC_2
	CreateDirStruct(findName,hhcFile);
#endif
	hhcFile.WriteString(_T("</UL></BODY></HTML>"));//write root ul end
	hhcFile.Close();
	return strHHC;
}

//modify 
CString  ChmCompiler:: CreateHHK(CString strTaskfName)//建立索引文件。。。。。
{
	CStdioFile  hhkFile;
	CStdioFile	taskFile;

	URL_STRING	strHHK;
	CString	dirStr = strTaskfName.Left(strTaskfName.ReverseFind('\\'));
	CString taskName =  strTaskfName.Right(strTaskfName.GetLength() - strTaskfName.ReverseFind('\\') - 1);
	pSYSTEM_CONFIG  pconf = ConfigManager::Instance()->GetSystemConfig();

	if(dirStr.IsEmpty())
		return CString();

	 
	strHHK = GetTempFile(dirStr,_T("~temp"),_T("hhk"));//得到 临时文件名 
	 
	BOOL ret = hhkFile.Open(strHHK,CStdioFile::modeCreate|CStdioFile::modeWrite);
	ASSERT(ret == TRUE);
	ret = taskFile.Open(strTaskfName,CStdioFile::modeRead|CFile::shareDenyNone);
	ASSERT(ret == TRUE);
	if(FALSE == ret)
		return CString();
	
	CString  hhkString;

	//hhkFile.WriteString("<HTML>\n<HEAD>\n</HEAD>\n<BODY>\n<UL>\n");
	hhkString = _T("<HTML>\n<HEAD>\n</HEAD>\n<BODY>\n<UL>\n");


	int urlNum ;
	URL_STRING	urlString;
	
	if(pconf->dwWorkMode == ANCHOR_MODE)
	{
		taskFile.Seek(12,SEEK_SET);//new add : version(int),downTag(int,whether download finished )
		taskFile.ReadString(urlString); //jump root url...
		//taskFile.Read(&urlNum,4);//get downed file num...ltPage);
	}
	else
	{
		taskFile.Seek(14,SEEK_SET);
		//if(S_FALSE == TCatalogBuilder::GetIndexPageName(defaultPage))
		//	TCatalogBuilder::GetStartPagesName(defaultPage);
	}

	taskFile.Read(&urlNum,4);//get downed file num...ltPage);
	URL_STRING  pathstr1,pathstr2,pathstr3;

	int nlen;
	for(int i = 0;i < urlNum;i++)
	{
		taskFile.ReadString(urlString);
		if(urlString.GetLength()< 3)
			continue;

		_URL  obj(urlString);
		ExtractValidUrl(obj.GetURL(),&pathstr2);
		CreatePathFromUrl(&dirStr,&pathstr2,pathstr1,TRUE);
		CRefPathCalculator::ToSafeWay(pathstr1);	

		CHtmlParse		parse;
		if(parse.LoadFromFile(pathstr1) == S_FALSE)
			continue;
	//	if(parse.SimpleTest() == FALSE)
		//	continue;
		if(  parse.GetPageTitle(pathstr2 ,nlen) &&  nlen > 1) 
		{
			  CString chSetName;
			  if(parse.GetCharSetName(chSetName))
				 ConverCharSet(chSetName,pathstr2,nlen);
		}
		else
			pathstr2 = pathstr1.Right(pathstr1.GetLength() - pathstr1.ReverseFind('\\') - 1);
				 
		hhkString += _T("<LI> <OBJECT type=\"text/sitemap\">\n");
		pathstr3.Format(_T("<param name=\"Name\" value=\"%s\">\n"),pathstr2);

		hhkString += pathstr3;

		pathstr3.Format(_T("<param name=\"Local\" value=\"%s\"></OBJECT>"),pathstr1);
		hhkString += pathstr3;

		urlString.Empty();
		pathstr2.Empty();
		pathstr1.Empty();
		pathstr3.Empty();
	}


	//hhkFile.WriteString("</UL>\n</BODY>\n</HTML>");
	hhkString += _T("</UL>\n</BODY>\n</HTML>");
	hhkFile.Write(hhkString,hhkString.GetLength());

	hhkFile.Close();
	taskFile.Close();
	return strHHK;
}


 struct HHAData { unsigned long sizeIn; // Size of structure. unsigned long unk1_1; // ?? 
 unsigned long unk1_2; // ?? 
 unsigned long unk1_3; // ??
 unsigned long nTopics; 
 unsigned long nLocalLinks; 
 unsigned long nGraphics; 
 unsigned long nInternetLinks; 
 unsigned long unk3_1; // ?? 
 unsigned long unk3_2; // ?? unsigned long unk3_3; // ?? 
 unsigned long unk3_4; // ?? 
 char tgtname[MAX_PATH]; };


#ifdef _DEBUG
 void ChmCompiler::InitLog(LPCTSTR lpPath)
 {
	CString strPath(lpPath);

	//::PathRemoveFileSpec(strPath.GetBuffer());
	strPath.ReleaseBuffer();
	strPath += _T("_log.txt");

	TRY
	{
		CFileException  *pException = new CFileException();
		ChmCompiler::m_logFile.Open(strPath,CStdioFile::modeCreate|CStdioFile::modeReadWrite,pException);
		pException->Delete();
	}
	CATCH(CFileException,e)
	{
		e->ReportError();
	}
	END_CATCH
 }
#endif

void ChmCompiler::CompilerExec(CString strTaskfName)
{

  //  Console  dosMode;
	CString  hhcfileStr,hhpfileStr,hhkfileStr;

#ifdef _DEBUG
    InitLog(strTaskfName);
#endif

	if(m_pWnd)
		m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)_T("Create HHC File..."),(LPARAM)0);
	hhcfileStr = CreateHHC(strTaskfName);
	
	if(m_pWnd)
		m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)_T("Create HHK File..."),(LPARAM)0);
	hhkfileStr = CreateHHK(strTaskfName);

	CString hhcName = hhcfileStr.Right(hhcfileStr.GetLength() - hhcfileStr.ReverseFind('\\')-1);
    CString hhkName = hhkfileStr.Right(hhkfileStr.GetLength() - hhkfileStr.ReverseFind('\\')-1);

	//cout << ("Create HHP FILE.....\n");
	if(m_pWnd)
		m_pWnd->SendMessage(WM_CHM_PRGRESS , (WPARAM)_T("Create HHP File..."),(LPARAM)0);
	hhpfileStr = CreateHHP(strTaskfName,hhcName,hhkName);

	if(!PathFileExists(hhpfileStr))
	{
#ifndef _DEBUG
		::DeleteFile(hhcfileStr);
		::DeleteFile(hhkfileStr);
#endif
		return ;
	}
	//cout << ("HHP FILE:%s\n",hhpfileStr);

	LPCTSTR  lpHppName = hhpfileStr.GetBuffer();

	CLoadDll  loadDll;
    if(m_func && m_hinstLib)
	{
		HHAData  inf ;
		/*CA2W  wName(lpHppName);*/
		memset(&inf,0,sizeof(inf));
		inf.sizeIn = sizeof(HHAData);
		(*m_func)(_tstr_w2a(lpHppName) /*wName.m_psz*/,FuncProgress,FunLog,&inf);
	}
    hhkfileStr.ReleaseBuffer();

	if(!m_func && this->m_pWnd)
	{
		this->m_pWnd->SendMessage(WM_CLOSE,0,0);
	}
	//printf("\nALL Done .....:)");

#ifndef _DEBUG
	DeleteFile(hhcfileStr);
	DeleteFile(hhkfileStr);
	DeleteFile(hhpfileStr);
#endif

    Sleep(1000);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TxtCompiler *TxtCompiler::_pInstance;
CWnd *TxtCompiler:: m_pWnd;
BOOL  TxtCompiler:: b_Exit;
TxtCompiler:: TxtCompiler()
{
	_nPageNum = 0;
}

TxtCompiler::~TxtCompiler()
{

}

TxtCompiler * TxtCompiler::Instance()
{
    if(!_pInstance)
		_pInstance = new TxtCompiler;

	return _pInstance;
}

void TxtCompiler::Uninstance()
{

    if(_pInstance)
		delete _pInstance;

	_pInstance = NULL;
}

BOOL TxtCompiler::DoCompiler(LPCTSTR strSrcName,LPCTSTR strTxtDir,CStdioFile *pFile,BOOL bOneFile)
{
	if(bOneFile)
	{
		if(!pFile)
		{
			::AfxMessageBox("Cant found text file!\n");
			return FALSE;
		}

		CString  csText;
		this->GetHtmlText(strSrcName,csText);
		csText += _T("\n");
		pFile->Write(csText.GetBuffer(),csText.GetLength());


	}else
	if(!TxtCompiler::CompilerExec2(strSrcName,strTxtDir,++_nPageNum))
	{
		--_nPageNum;
		return FALSE;
	}

	return TRUE;
}
CString TxtCompiler:: GetTxtDirPath(CString strRootPath)
{
	if(strRootPath.IsEmpty())
		return CString();

	CString txtDirName = strRootPath.Left(strRootPath.ReverseFind('\\'));
	txtDirName = txtDirName.Left(txtDirName.ReverseFind('\\'));
	if(txtDirName.IsEmpty())
		return CString();

	CString  dirName2;
	CString  dirName = txtDirName.Right(txtDirName.GetLength() - txtDirName.ReverseFind('\\') - 1);
	
	dirName2.Format(_T("\\%s_TXT_DIR\\"),dirName);
	txtDirName += dirName2;
	PrepareDirectory(txtDirName.GetBuffer(),0);
	txtDirName.ReleaseBuffer();
	return txtDirName;
}

CString TxtCompiler:: GetTxtDirPath(LPCTSTR strRootPath)
{
	CString strRoot(strRootPath);
	return  GetTxtDirPath(strRoot);
}

BOOL	TxtCompiler:: CompilerExec(LPCTSTR strSrcName,LPCTSTR strTxtDir)
{

	CHtmlParse		parse;
	int				nlen;
	CString strTitle;
	CString srcPathName = strSrcName;
	CString strTxtDirName = strTxtDir;

	if(parse.LoadFromFile(srcPathName) == S_FALSE)
		return  FALSE;
	if(  parse.GetPageTitle(strTitle ,nlen) &&  nlen > 1) 
	{
		/*char *pChar = (char *)alloca(2*nlen);
		::MultiByteToWideChar(CP_ACP,0,pChar,2*nlen,(LPWSTR)strTitle.GetBuffer(),nlen);
		strTitle.ReleaseBuffer();
		strTitle = pChar;*/
	//	subDirStr.Format(formatStr,strTitle);
	}
	else
		strTitle = srcPathName.Right(srcPathName.GetLength() - srcPathName.ReverseFind('\\') - 1);

	ToDosFileNameStr(strTitle);
	if(strTitle.IsEmpty() || strTitle.GetLength()< 3)//长度小于3人就认为是无效的。。。。
	{
		strTitle = srcPathName.Right(srcPathName.GetLength() - srcPathName.ReverseFind('\\') -1);
		strTitle = strTitle.Left(strTitle.ReverseFind('.'));
		strTxtDirName += _T("\\");
		strTxtDirName += strTitle;
		strTxtDirName += _T(".txt");
		
	}else
	{
		CString  tmpPathName =   strTxtDirName;
		tmpPathName += _T("\\");
		tmpPathName += strTitle;
		tmpPathName += _T(".txt");
		if(PathFileExists(tmpPathName))
			tmpPathName = GetTempFile(strTxtDirName,strTitle,_T("txt"));
		strTxtDirName = tmpPathName;
	}
	parse.SaveToTextFile(strTxtDirName);
	return TRUE;

}
  
BOOL  TxtCompiler:: GetHtmlText(LPCTSTR strSrcName,CString &strOutText)
{
	
	CHtmlParse		parse;
	if(parse.LoadFromFile(strSrcName) != S_OK)
		return FALSE;

	CString chSetName;
	CString csWholeFile = parse.GetRawData();
	if(parse.GetCharSetName(chSetName))
	{
		if(chSetName.CompareNoCase("utf-8") == 0)
			ChmCompiler::ConverCharSet(chSetName,csWholeFile,csWholeFile.GetLength());
	}

	CComPtr<IHTMLDocument2> pDoc;
	HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IHTMLDocument2, (void**)&pDoc);

	ASSERT(pDoc != NULL);
	if(pDoc == NULL)return FALSE ;

	//put the code into SAFEARRAY and write it into document
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;

	_bstr_t bsData = (LPCTSTR)csWholeFile;
	hr = SafeArrayAccessData(psa, (LPVOID*)&param);

	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;

	hr = pDoc->write(psa);
	hr = pDoc->close();

	SafeArrayDestroy(psa);

	CComPtr<IHTMLElement>pBody;
    pDoc->get_body(&pBody);
	
	if(pBody != NULL)
	{
		CComBSTR bstr;
		pBody->get_innerText(&bstr);
		strOutText += bstr.m_str;
	}
}

BOOL	TxtCompiler:: CompilerExec2(LPCTSTR strSrcName,LPCTSTR strTxtDir,int pageNum)
{
	CString strTitle;
	CString srcPathName = strSrcName;
	CString strTxtDirName ;

	//let's open file and read it into CString (u can use any buffer to read though
	
	CHtmlParse		parse;
	if(parse.LoadFromFile(srcPathName) != S_OK)
		return FALSE;

	CString chSetName;
	CString csWholeFile = parse.GetRawData();
	if(parse.GetCharSetName(chSetName))
	{
		if(chSetName.CompareNoCase("utf-8") == 0)
			ChmCompiler::ConverCharSet(chSetName,csWholeFile,csWholeFile.GetLength());
	}

	CComPtr<IHTMLDocument2> pDoc;
	HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IHTMLDocument2, (void**)&pDoc);

	ASSERT(pDoc != NULL);
	if(pDoc == NULL)return FALSE ;

	//put the code into SAFEARRAY and write it into document
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;

	_bstr_t bsData = (LPCTSTR)csWholeFile;
	hr = SafeArrayAccessData(psa, (LPVOID*)&param);

	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;

	hr = pDoc->write(psa);
	hr = pDoc->close();

	SafeArrayDestroy(psa);

	CComPtr<IHTMLElement>pBody;
    pDoc->get_body(&pBody);
	
	if(pBody != NULL)
	{
		CComBSTR bstr;
	
		pDoc->get_title(&bstr);
		strTitle = bstr.m_str;

		ToDosFileNameStr(strTitle);
		if(strTitle.IsEmpty() || strTitle.GetLength()< 3)//长度小于3人就认为是无效的。。。。
		{
			strTitle = srcPathName.Right(srcPathName.GetLength() - srcPathName.ReverseFind('\\') -1);
			strTitle = strTitle.Left(strTitle.ReverseFind('.'));
			strTxtDirName.Format(_T("%s%d.%s"),strTxtDir,pageNum,strTitle);
			strTxtDirName += _T(".txt");
			
		}else
		{
			strTxtDirName.Format(_T("%s%d.%s"),strTxtDir,pageNum,strTitle);
			strTxtDirName += _T(".txt");
		}

		if(::PathFileExists(strTxtDirName))
			return FALSE;
	
		pBody->get_innerText(&bstr);
		
		CStdioFile ff;
		BOOL bRet = ff.Open( strTxtDirName,CStdioFile::modeCreate|CStdioFile::modeReadWrite);
		if(!bRet)
		{
			return FALSE;
		}

		ff.WriteString(CString(bstr.m_str));
		ff.Close();
	}
	return TRUE;
}

void ChmCompiler::ConverCharSet(CString &charSetName, CString & pSource, int nlen)
{

	USES_ATL_SAFE_ALLOCA;
	TRACE("***charset:%s",charSetName);
	//是否unicode的判断，如果是的话就转化。
	if(IsTextUnicode(pSource.GetBuffer(),nlen,NULL))
	{
		char *pBuf = (char *)new char[nlen *2+1];
		int nRet = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pSource.GetBuffer(), nlen, pBuf, nlen*2, NULL, NULL);
		pBuf[nRet] = '\0';
		pSource = pBuf;
		delete []pBuf;
		return;
	}

	int iCode =  CharSetStrToInt(charSetName);

	if(iCode == -1) return ;
		
	char *pChar = new char[nlen * 4];
	char *pChar2 = new char[nlen * 4];

	int iLen = MultiByteToWideChar(iCode, 0,(LPCSTR) pSource.GetBuffer(), nlen, (LPWSTR)pChar, nlen * 4);
    pSource.ReleaseBuffer();

	 
	int nRet = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pChar, iLen, pChar2, nlen *4 , NULL, NULL );//
	 
	if(nRet != 0)
	{
		 pChar2[nRet] = '\0';
		 pSource = pChar2;
	}

	delete []pChar;
	delete []pChar2;
}

