#include "StdAfx.h"
#include "JobFileReader.h"
#include "httpapi.h"
CJobFileReader::CJobFileReader(void)
{
}

CJobFileReader::~CJobFileReader(void)
{
}


BOOL CJobFileReader::LoadFile(LPCTSTR lpStrFile)
{
    ASSERT(lpStrFile != NULL);

	//file exist ?
	if(!::PathFileExists(lpStrFile))
	return FALSE;

	TRY
	{
		CStdioFile  cf(lpStrFile,CStdioFile::modeRead);
		cf.Seek(4,CStdioFile::SeekPosition::begin);
		cf.Read(&this->m_downloadMode,4); //read download mode.

		 
		cf.Read( &m_downStatus,4);
		if(m_downStatus == SESSION_COMPLETE)
			return TRUE;

		cf.ReadString(m_rootURL);

		int dNum;
		cf.Read(&dNum,4);//downloaded number.

		URL_STRING strURL;
		for( int i = 0;i < dNum;i++)
		{
			cf.ReadString(strURL);
			this->m_listDowned.push_back(strURL);
		}

		cf.Read(&dNum,4); //Î´ÏÂÔØµÄÁ´½Ó
		for(int j = 0;j < dNum;j++)
		{

			cf.ReadString(strURL);
			this->m_listPageURL.push_back(strURL);
		}

		cf.Read(&dNum,4);
		for(int k = 0;k < dNum;k++)
		{
			cf.ReadString(strURL);
			this->m_listSourceURL.push_back(strURL);
		}

		cf.Close();

	}CATCH(CFileException ,e)
	{
		e->ReportError();
		return FALSE;
	}
	END_CATCH;
	return TRUE;   
}
