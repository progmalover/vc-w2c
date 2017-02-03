#pragma once


// CTabBar

class CTabBar : public CDialogBar
{
	DECLARE_DYNAMIC(CTabBar)

public:
	CTabBar();
	virtual ~CTabBar();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


