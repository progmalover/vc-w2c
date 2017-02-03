#pragma once
#include "httpapi.h"

class TFileSession :
	public _URL_TASK
{
public:
	TFileSession(void);
	TFileSession(URL_STRING strUrl);
	~TFileSession(void);
	void Run(void);
	void Notify(int nStatCode);
	void OnFinish(void);
   
};
