#pragma once
#include "cmycontrolbar.h"

class CW2CBar :
	public CMyControlBar
{
	DECLARE_DYNAMIC(CW2CBar)
public:
	CW2CBar(void);
	~CW2CBar(void);
	 virtual	CRect GetGripperRect();
	 virtual void DrawGripper(CDC* pDC);
	 void  GetClientRectExcludeTitle(CRect *rcRect);
	 DECLARE_MESSAGE_MAP()
protected:
	void LoadBarStatus();
	void SaveBarStatus();
public:
	 afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	 afx_msg void OnClose();
	 afx_msg void OnDestroy();
};
