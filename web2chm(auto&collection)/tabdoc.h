// tabdoc.h : tabdoc 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号


// CtabdocApp:
// 有关此类的实现，请参阅 tabdoc.cpp
//

class CtabdocApp : public CWinAppEx
{
public:
	CtabdocApp();

public:
	CRecentFileList & GetRecentList(void);
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void  OnHelp();
	afx_msg void  OnTutorial();
	afx_msg void  OnUpdate();
	afx_msg void  OnAppAbout();

	DECLARE_MESSAGE_MAP()
	CFrameWnd * AppGetMainForame(void);

	virtual int ExitInstance();
	
	afx_msg void OnHelpTopics();
};

extern CtabdocApp theApp;
