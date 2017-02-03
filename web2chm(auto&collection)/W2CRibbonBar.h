#pragma once


// CW2CRibbonBar

class CW2CRibbonBar : public CMFCRibbonBar
{
	DECLARE_DYNAMIC(CW2CRibbonBar)

public:
	CW2CRibbonBar();
	virtual ~CW2CRibbonBar();
	void OnPaint();
	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
};


