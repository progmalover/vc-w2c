#pragma once


// CReBarEx

class CReBarEx : public CMFCReBar
{
	DECLARE_DYNAMIC(CReBarEx)

public:
	CReBarEx();
	virtual ~CReBarEx();
    virtual void DrawGripper(CDC* pDC, const CRect& rect);
	virtual void DrawBorders(CDC *pDC,CRect &rect);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


