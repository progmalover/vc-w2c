// tabdoc.h : tabdoc Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CtabdocApp:
// �йش����ʵ�֣������ tabdoc.cpp
//

class CtabdocApp : public CWinAppEx
{
public:
	CtabdocApp();

public:
	CRecentFileList & GetRecentList(void);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
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
