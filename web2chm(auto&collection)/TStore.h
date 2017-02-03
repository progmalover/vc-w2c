#pragma once
#include "httpapi.h"
class TStore
{
public:
	TStore(void);
	~TStore(void);
public:
	IStorage *  OpenFile(LPCSTR lpStgFile);
	IStorage *  OpenDir(LPCSTR strDir,IStorage *iFile);	

	IStream  *  OpenStream(LPCSTR lpDir,IStorage *iFile);
	void		WriteString(IStream *iStream,LPCSTR lpStr);
	void		ReadString(IStream *iStream ,CString cStr);
protected:
	int		    GetItemNum(IStream *iStream);
	int		    WriteStreamNum(IStream *iStream,int num);
	
};
