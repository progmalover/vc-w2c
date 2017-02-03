#include "StdAfx.h"
#include ".\TStore.h"

TStore::TStore(void)
{
}

TStore::~TStore(void)
{

}

IStorage *TStore:: OpenFile(LPCSTR lpStgFile)
{
return NULL;
}

IStorage * TStore::OpenDir(LPCSTR strDir,IStorage *iFile)
{
	return NULL;
}

IStream  * TStore::OpenStream(LPCSTR lpDir,IStorage *iFile)
{
	return NULL;
}

int	  TStore::  GetItemNum(IStream *iStream)
{
	return 0;
}
int	  TStore::WriteStreamNum(IStream *iStream,int num)
{
    return 0;
}
void  TStore::WriteString(IStream *iStream,LPCSTR lpStr)
{
	 
}
void  TStore::ReadString(IStream *iStream ,CString cStr)
{
	 
}