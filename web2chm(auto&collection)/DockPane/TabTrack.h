#pragma once


// CTabTrack
class  CTabPanelTitleWnd ;
class  CTabTrack 
{
public:
	CTabTrack();
	CTabTrack(CTabPanelTitleWnd *pParent);
	virtual ~CTabTrack();
public:
   void Track();
public:
	CTabPanelTitleWnd *m_pParent;
	void DrawTracker(CDC* pDC, CPoint csPoint);
public:
	//static CWnd *pLastTrackWnd;
	static CSize oldBuddySize;
};


