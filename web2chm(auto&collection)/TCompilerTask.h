#pragma once
#include "httpapi.h"
 
class TCompilerTask
	//public _URL_TASK
{
public:
	TCompilerTask(CWnd *pWnd,CString &strTaskFile);
	void Create(int nConver,void *p);
	~TCompilerTask(void);
	static void Run(void);
	static void Run2(void);
	static CString strTaskfName;

};
