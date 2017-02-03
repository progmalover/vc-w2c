#if !defined(AFX_OWNERDRAWLISTBOX_H__1AFDF05D_179F_4269_887D_B47D0D7A11FD__INCLUDED_)
#define AFX_OWNERDRAWLISTBOX_H__1AFDF05D_179F_4269_887D_B47D0D7A11FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
/////////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;
// COwnerDrawListBox window
#define WM_LBDELETEITEM  WM_USER + 1213 
class COwnerDrawListBox : public CListBox
{

// Construction
public:
	DECLARE_DYNCREATE(COwnerDrawListBox)

	enum Item_Pos{ITEM_RIGHT,ITEM_LEFT};
	COwnerDrawListBox();
	int  AddString(CString strItemName, HICON hDataIcon =NULL);
	void SetIconPosition(Item_Pos Ico_Pos);
	void Destroy();
    int  DeleteString(UINT nIndex);
	virtual void OnDeleteString(LPCTSTR lpStr){};
	BOOL  IsMouseOnIcon2() const ;

	CRect GetItemTextRect(int iItem) ;
	CSize GetItemIconSize(HICON hicon);
	CRect GetItemIconRect( int iItem );
	BOOL  IsEnabled(int nIndex);
    int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
	int SetItemSubDataPtr(int nIndex ,void *pData)  ;
	void * GetItemSubDataPtr(int nIndex) const;
	virtual void OnBeforeDelItem(){};

protected:
	// This is the Struct which holds the List Box Data
	struct DataItems
	{
		DataItems(){
			hIcon = NULL;
			icoSize.cx = 0;
			icoSize.cy = 0;
		};
		CString strItemData;
		CSize  icoSize;
		HICON   hIcon;
	};

	DataItems *m_pListBoxDataItems;
	Item_Pos m_IconPos;
	CRect m_rectListBoxItem;//, m_rectListBoxItemText, m_rectListBoxItemIcon;

	bool  m_bPropertyInit;
	BOOL  m_bEnableFullPath;
	
// Attributes
public:

// Operations
public:
   int hitIconItem;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COwnerDrawListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int  CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
 
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COwnerDrawListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COwnerDrawListBox)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	HICON m_icon;
    vector<void *> m_itemSubDataPtrs;
public:
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OWNERDRAWLISTBOX_H__1AFDF05D_179F_4269_887D_B47D0D7A11FD__INCLUDED_)
