#include "StdAfx.h"
#include ".\w2ebook_utils.h"
#include <wininet.h>
#include < AFXINET.H>
#include "HttpApi.h"

BOOL IsValidURL(CString strURL)
{
	FixUrlStr(strURL);
	CString strServer, strObject;
	DWORD dwServiceType = AFX_INET_SERVICE_HTTP;
	INTERNET_PORT nPort;
	return AfxParseURL(strURL,dwServiceType ,strServer,strObject,nPort);
}

CString  GetSysTempFolder()
{
	CString strTemp;
	char temp[MAX_PATH];
	GetTempPath(sizeof(temp),temp);
	return temp;
}

//strPath
CString  TaskFileName(CString strPath,CString strNameNoExt)
{
	CString strTaskName,strTaskPath;
	if(strPath.ReverseFind('\\')!=(strPath.GetLength()-1))
		strPath += "\\";
	strTaskPath.Format("%s%s\\%s_WEB_SRC\\%s.job",strPath,strNameNoExt,strNameNoExt,strNameNoExt);
	return strTaskPath;
}

CString TaskName(CString strPathEx)
{ 
	CString strPath = strPathEx;
	CString tskName = strPath.Right(strPath.GetLength() - strPath.ReverseFind('\\')-1);
	CString tskDirName;
	tskName = tskName.Left(tskName.Find('.'));
	tskDirName.Format("\\%s_WEB_SRC\\",tskName);
	tskName +=  tskDirName;
	strPath.Insert(strPath.ReverseFind('\\')+1,tskName);
	return strPath;
}

CString  NameNoExt(CString strName)
{
	CString strNoExt = strName;
	int i = strName.Find('.');
	if(i>=0)
		strName = strName.Left(i-1);
	return strNoExt;
}

void AddComboxString(CComboBox *pBox,int ID)
{
	pBox->ResetContent();
	CString strExtensions;
	strExtensions.LoadString(ID);
	for(int n = 0;;n++)
	{
		CString strTempExtension;
		AfxExtractSubString( strTempExtension, strExtensions, n, ';');
		if(strTempExtension.IsEmpty()) break;
		pBox->AddString(strTempExtension);
	};
}

void ModifyCmbEdit(CComboBox *pBox,DWORD  dwStyle)
{
	ASSERT(pBox);
	CEdit *pEdt = (CEdit *)pBox->GetDlgItem(0x3e9);
	if(NULL != pEdt)
		pEdt->ModifyStyle(0,dwStyle);

}

