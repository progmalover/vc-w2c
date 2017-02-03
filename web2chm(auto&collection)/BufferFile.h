#pragma once
#include "afx.h"

class CBufferFile :
	public CStdioFile
{
public:
	CBufferFile(void);
	//CBufferFile(LPCTSTR pstrfName = NULL);
	~CBufferFile(void);
public:
	CString  m_filePathName;
	DWORD	 m_iRecordNum;
	DWORD	 m_dwReadPos;
	BOOL	 Create(void);
protected:
	ULONGLONG  m_curLocal;
	ULONGLONG  m_endLocal; 
public:
	BOOL WriteToBuffer(LPCTSTR lpStr);
	BOOL ReadFromBuffer(CString & str);
	BOOL AtEnd(void);
	void SetToStart(void);
	void SetCurLocal(void);
};
