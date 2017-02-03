#pragma once
#include "flatbutton.h"

class CDockBtn :
	public CFlatButton
{
public:
	CDockBtn(void);
	~CDockBtn(void);
	virtual void DrawStuff(CDC *pDC, const CRect &rc);
	virtual CSize GetStuffSize(); 
	virtual void DrawBackground(CDC *pDC, const CRect &rc);
public:
	DECLARE_MESSAGE_MAP()

};
