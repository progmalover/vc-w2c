#include "StdAfx.h"
#include ".\comrun.h"

DWORD CComRun:: m_comRef = 0;
CComRun::CComRun(void)
{
	CoInitialize(NULL);
	CComRun::m_comRef++;
}

CComRun::~CComRun(void)
{
	CoUninitialize();
	CComRun::m_comRef--;
}

void CComRun:: ReleaseAll()
{
	while(--m_comRef >= 0)
		CoUninitialize();
}