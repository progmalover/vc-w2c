#pragma once

#include "ExListBox.h"
// CAnchorUrlsList
#define  WM_SETCLMCAPTION WM_USER + 23
class CAnchorUrlsList : public CExListBox
{
	//DECLARE_DYNAMIC(CAnchorUrlsList)

public:
	CAnchorUrlsList();
	virtual ~CAnchorUrlsList();
public:
	void  Create(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT  CAnchorUrlsList::SetClmCaption(WPARAM,LPARAM);
	 
	void DeleteAllItems(void);
	void SetSelLong(int ){};
 
	 
	CHeaderCtrl m_Head;
	void MoveWindow(CRect & rect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnLbnKillfocus();
	afx_msg void OnLbnDblclk();
};
#define ID_ANCHOR_URLS_LIST   WM_USER + 21
extern CAnchorUrlsList GO_AnchorUrlsList;
