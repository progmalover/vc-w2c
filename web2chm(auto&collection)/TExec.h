#pragma once

virtual class TExec
{
public:
	virtual HRESULT  Prepare(void)=0;
	virtual HRESULT  UnPrepare(void)=0;
	virtual HRESULT  Exec(void)=0;
};
