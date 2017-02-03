#pragma once
#include "flatbutton.h"

class CDlgBtn :
	public CFlatButton
{
public:
	CDlgBtn(void);
	~CDlgBtn(void);
public:
	virtual void DrawBackground(CDC *pDC, const CRect &rc);  
	void SetText(LPCSTR  lpStr);
	void ShowText(BOOL bEnable = TRUE);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	CWnd * GetBottomOwner(void);
	 BOOL  LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize);
	 BOOL  LoadPNG(UINT  iPng);
  
private:
	DWORD		m_dwBmpIDUP;
	DWORD		m_dwBmpIDDOWN;
	BOOL		m_bShowTxt;
	HBITMAP		m_hBmp;
	CString		m_strTxt;
	COLORREF	m_bkclr;
	BOOL		m_bGetBmpHandle;
public:
	void SetBmpIDUP(DWORD bmpID);
	void SetBmpIDDOWN(DWORD bmpID);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL LoadGIF(UINT iGif);
	void SetBkColor(COLORREF clr);
};
