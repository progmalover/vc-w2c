#pragma once

class CComRun
{
public:
	 CComRun(void);
	~CComRun(void);
public:
static	DWORD  m_comRef;
static	void ReleaseAll();
};
