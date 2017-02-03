#include "StdAfx.h"
#include ".\bufferfile.h"
#include "filepath.h"
#include "httpapi.h"
extern TWebSession   *GP_CurrentSession ;
CBufferFile::CBufferFile(void)
{

}

BOOL CBufferFile::Create(void)
{
#ifdef _DEBUG_TEST_BUF_FILE
	m_filePathName = GetTempFile("E:\\","tmp","data");
#else
	m_filePathName = GetTempFile(NULL,"~tmp","data");
#endif
	if(PathFileExists(m_filePathName))
		DeleteFile(m_filePathName);
	TRY{
		BOOL  bRet = Open(m_filePathName,CStdioFile::modeCreate|CStdioFile::modeReadWrite);
		if(bRet == FALSE)
		{
			AfxMessageBox("Create buffer file failured,please check you \"c:\" disk.");
		//	delete this;
			exit(-3);
		}
		m_endLocal = m_curLocal = ftell(this->m_pStream);
		m_iRecordNum = 0;
		m_dwReadPos = 0;
		this->m_bCloseOnDelete = TRUE;
	}
	CATCH(CFileException ,p)
	{
		p->ThrowOsError(GetLastError(),m_filePathName);
	}
	END_TRY
	
	return  TRUE;
}
CBufferFile::~CBufferFile(void)
{
	
}


/*
写的时候写在尾部，读的时候根据当前位置。。。。
*/
BOOL CBufferFile::WriteToBuffer(LPCTSTR lpStr)
{
	if(!this->m_pStream)
		return FALSE;

	
	SeekToEnd();
	WriteString(lpStr);
	//Flush();
	m_iRecordNum++;
	m_endLocal = GetPosition();
	return TRUE;
}

BOOL CBufferFile::ReadFromBuffer(CString & str)
{
	if(!this->m_pStream)
		return FALSE;

	ASSERT(m_curLocal <=  m_endLocal);
	if(m_curLocal == m_endLocal)
		return FALSE;
	
	Seek(m_curLocal,SEEK_SET);
	BOOL bRet =  ReadString(str);
	m_curLocal = GetPosition();
	m_dwReadPos++;
	return bRet;
}

BOOL CBufferFile::AtEnd(void)
{
	return m_dwReadPos >= m_iRecordNum;
}

void CBufferFile::SetToStart(void)
{
	this->SeekToBegin();
	m_curLocal = GetPosition();
	m_dwReadPos = 0;

}
 
void CBufferFile::SetCurLocal(void)
{
	if(this->m_pStream)
		m_curLocal = GetPosition();
}
