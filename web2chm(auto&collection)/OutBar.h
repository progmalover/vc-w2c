#pragma once

#include "CProgressList.h"
#include "defs.h"
#include ".\AppDockablePane.h"

class COutBar :
	public CAppDockablePane
{
public:
	COutBar(void);
	~COutBar(void);
	
	 DECLARE_MESSAGE_MAP()
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:

	 CTskPrgList		m_prgList;
	 pSYSTEM_CONFIG		m_pSysConf;

	 afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	 void ResizeContrlBar(void);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
};
