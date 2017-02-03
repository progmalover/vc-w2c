#pragma once


// CAnchorTree

class CAnchorTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CAnchorTree)

public:
	CAnchorTree();
	virtual ~CAnchorTree();
public:
	HTREEITEM m_hroot;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HRESULT  OnDelPiece(WPARAM,LPARAM);
	afx_msg void OnTvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	HICON	   m_hListIcon;
private:
	BOOL m_bDraging;
	HTREEITEM  m_hDragItem;
	HTREEITEM  m_hDropItem;
	 
};
#define ID_ANCHOR_TREE  WM_USER + 22
extern CAnchorTree  GO_AnchorTree;

