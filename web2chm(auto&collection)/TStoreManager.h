#pragma once
#include"httpapi.h"
class TStoreManager
{
public:
	TStoreManager(void);
	~TStoreManager(void);
public:
	void SaveToSession(TWebSession *pSession);
	void LoadToSession(LPCSTR lpFile,TWebSession *pSession);
};
