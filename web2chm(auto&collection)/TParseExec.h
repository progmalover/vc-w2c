#pragma once
#include "texec.h"

class TParseExec :
	public TExec
{
public:
	TParseExec(void);
	~TParseExec(void);
	HRESULT Prepare(void);
	HRESULT UnPrepare(void);
	HRESULT Exec(void);
};
