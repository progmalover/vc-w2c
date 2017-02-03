#pragma once
#include <list>
using namespace std;
namespace VIRSUAL_CONTROLS{
typedef list<CMFCRibbonBaseElement *> VISUAL_LIST;

class CVisualManager;
class CMFCRibbonFocusCaption;
class CMFCRibbonBaseElementEx;
class CMFCRibbonPlayerSlider;
class CMFCRibbonApplicationButtonEx;
class CMFCRibbonButtonEx;
class CMFCRibbonCheckBoxEx;


class CVisualManager {
public:
	CVisualManager(){};
	~CVisualManager(){Clear();};
	LRESULT WndProc(HWND hWnd,UINT message,WPARAM wparam,LPARAM lparam);
	void AddToManager(CMFCRibbonBaseElement *pElement);
	void Clear();
private:
	VISUAL_LIST  visualList;
};

class CMFCRibbonBaseElementEx:public CMFCRibbonBaseElement
{
	friend class CVisualManager;
	 
};
class CMFCRibbonSliderEx:public CMFCRibbonSlider
{
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
public:
	CMFCRibbonSliderEx(CWnd *pParent = NULL)
	{
		ASSERT(pParent!=NULL);
		m_pOwner = pParent;
		this->m_bIntermediateMode = TRUE;
		this->m_bIsRadio = TRUE;
	};
	CMFCRibbonSliderEx(CWnd *pParent,UINT nID, int nWidth = 100);
	//void SetDisable(BOOL bDisable){this->m_bIsDisabled = bDisable;};
	virtual CWnd* GetParentWnd() const;
	virtual void OnDraw(CDC* pDC);
	virtual void Redraw();
	virtual void OnMouseMove(CPoint point);
	virtual void OnLButtonUp(CPoint point);
	virtual void OnLButtonDown(CPoint point);
	virtual void Redraw(BOOL bAll);
	//virtual void SetRect();
	virtual void MoveControl(CRect rect);
protected:
	CRect m_oldThumbRect;
private:
	CWnd *m_pOwner;
public:
	void SetDisable(bool bDisabled);
};

 
class CMFCRibbonPlayerSlider :public CMFCRibbonSliderEx
{
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
	friend class CMFCRibbonFocusCaption;
public:
	CMFCRibbonPlayerSlider(CWnd *pParent = NULL);
	
	CMFCRibbonPlayerSlider(CWnd *pParent,UINT nID, int nWidth = 100);
	
	virtual void OnDraw(CDC* pDC);
	virtual void Redraw();
	void Redraw(BOOL bAll);
	 
	void SetCaption(CMFCRibbonFocusCaption *pCaption);
	void SetDisable(BOOL bDisable);
	void EnableTimeMode(BOOL bEnable = TRUE);
	BOOL GetTimeMode(){return m_bTimeMode;};

public:
	CMFCRibbonFocusCaption  *m_pCaption;
private:
	BOOL  m_bTimeMode;
};

class CMFCRibbonApplicationButtonEx:public CMFCRibbonApplicationButton
{
	
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
public:
	CMFCRibbonApplicationButtonEx(CWnd *pParent,CRect rect) 
	{ 
		ASSERT_VALID(this);
		m_rect = rect; 
		m_pOwner = pParent;
		m_dwStatus = 1;

	};
	CMFCRibbonApplicationButtonEx(CWnd *pParent,UINT uiBmpResID,CRect &rect) 
	{ 

		ASSERT_VALID(this);
		SetImage(uiBmpResID); 
		m_rect = rect;
		m_pOwner = pParent;
		m_dwStatus = 1;
	};
	CMFCRibbonApplicationButtonEx(CWnd *pParent,HBITMAP hBmp,CRect &rect) 
	{

		ASSERT_VALID(this);
		SetImage(hBmp); 
		m_rect = rect;
		m_pOwner = pParent;
		m_dwStatus = 1;
	};

	void LoadDisableImage(UINT uiBmpResID)
	{

		ASSERT_VALID(this);
		m_ImagesDisable.Load(uiBmpResID);
		m_ImagesDisable.SetSingleImage();
	};

	UINT  GetStatus()
	{
		return m_dwStatus;
	}

	void SetStatus(UINT uiStatus)
	{
		ASSERT_VALID(this);
		UINT ui = m_dwStatus;
		m_dwStatus = uiStatus;
		if(ui != m_dwStatus)
			  Redraw();

	};
	void LoadNextStatusImage(UINT uiBmpResID)
	{
		ASSERT_VALID(this);
		m_ImagesNext.Load(uiBmpResID);
		m_ImagesNext.SetSingleImage();
	};
	void   SetDisable(BOOL bDisable){
		ASSERT_VALID(this);
		BOOL oldStatus = m_bIsDisabled ;
		m_bIsDisabled = bDisable;
		if(m_bIsDisabled != oldStatus)
			this->Redraw();
	};
	virtual void OnDraw(CDC *pDC);
	virtual void Redraw();
	virtual void OnMouseMove(CPoint point);
	void OnLButtonDown(CPoint point);
	virtual CWnd* GetParentWnd() const;
 
private:
	CWnd *m_pOwner;
	DWORD				m_dwStatus;
	CMFCToolBarImages  m_ImagesDisable;
    CMFCToolBarImages  m_ImagesNext;

};

class CMFCRibbonButtonEx:public CMFCRibbonButton
{
		
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
public:
    CMFCRibbonButtonEx(CWnd *pParent,CRect rect);
	CMFCRibbonButtonEx(CWnd *pParent,UINT nID, LPCTSTR lpszText, int nSmallImageIndex = -1, int nLargeImageIndex = -1, BOOL bAlwaysShowDescription = FALSE);
	CMFCRibbonButtonEx(CWnd *pParent,UINT nID, LPCTSTR lpszText, HICON hIcon, BOOL bAlwaysShowDescription = FALSE, HICON hIconSmall = NULL, BOOL bAutoDestroyIcon = FALSE, BOOL bAlphaBlendIcon = FALSE);
	virtual ~CMFCRibbonButtonEx(){};
   // virtual BOOL IsApplicationButton() const { return TRUE; }//
	 
	virtual CWnd* GetParentWnd() const;
	void OnLButtonDown(CPoint point);
	void SetCheck(BOOL bCheck){
		 m_bIsChecked = bCheck;
		 this->Redraw();
	 };
protected:
	CWnd *m_pOwner;
};

#define  BOTTOM_LEFT	1
#define  BOTTOM_MID		2
#define  BOTTOM_RIGHT	3
#define  CHANNEL_LEFT	4
#define  CHANNEL_RIGHT	5
class CMFCRibbonFocusCaption:public CMFCRibbonButtonEx
{
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
	friend class CMFCRibbonPlayerSlider;
public:
    CMFCRibbonFocusCaption(CWnd *pParent,CRect rect);
	CMFCRibbonFocusCaption(CWnd *pParent,UINT nID, LPCTSTR lpszText, int nSmallImageIndex = -1, int nLargeImageIndex = -1, BOOL bAlwaysShowDescription = FALSE);
	CMFCRibbonFocusCaption(CWnd *pParent,UINT nID, LPCTSTR lpszText, HICON hIcon, BOOL bAlwaysShowDescription = FALSE, HICON hIconSmall = NULL, BOOL bAutoDestroyIcon = FALSE, BOOL bAlphaBlendIcon = FALSE);
	virtual ~CMFCRibbonFocusCaption(){};
	virtual int DrawRibbonText(CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags, COLORREF clrText = (COLORREF)-1);
	CMFCRibbonBaseElementEx *GetHolder(void)
	{
		ASSERT(m_pHolder != NULL);
		return m_pHolder;
	};
	
	void SetHolder(CMFCRibbonBaseElementEx *pHolder){m_pHolder = pHolder;};
	void SetAlignMode(int nmode){m_alignMode = nmode;};
	void Redraw(){ };
	void OnDraw(CDC *pDC){};
	void OnDraw(CDC *pDC,int){
		m_bIsHighlighted = m_bEnableMouseTouch;
		//CMFCRibbonPlayerSlider *p = (CMFCRibbonPlayerSlider*)m_pHolder;
		//CMemDC memDC(*pDC,p->GetParentWnd());
		__super::OnDraw( pDC);
	
	};
	void SetMouseTouchEnable(bool bEnable = true){this->m_bEnableMouseTouch = bEnable;};
	void OnMouseMove(CPoint point)
	{
			ASSERT(m_pHolder);

			if(m_pHolder == NULL) return;
			if(!m_bEnableMouseTouch)
				return;

			CMFCRibbonPlayerSlider *p = (CMFCRibbonPlayerSlider*)m_pHolder;
			 
			BOOL bChange = this->m_bIsHighlighted;
			this->m_bIsHighlighted = !this->m_bIsDisabled;

			if(bChange != m_bIsHighlighted && !p->m_bIsHighlighted 
				&& !p->m_bIsPressed &&!p->m_bIsDisabled)
			{
				p->m_bIsHighlighted = TRUE; //not fouces mode
				p->m_nHighlighted = 0;
				p->Redraw();
			}
		
	};
	void SetText(LPCTSTR strText)
	{
		CString oldStrText = this->GetText();
		__super::SetText(strText);
		
		ASSERT(m_pOwner);

		CDC *pDC = m_pOwner->GetDC();
		m_textSize = pDC->GetTextExtent(strText,_tcslen(strText));
		m_pOwner->ReleaseDC(pDC);

		CRect oldRect = this->GetRect();
		CRect newRect = oldRect;
		if(m_pHolder) 
		{
			switch(m_alignMode)
			{
				case BOTTOM_LEFT:
					{
						newRect.left = m_pHolder->GetRect().right - m_textSize.cx;
						newRect.top =  m_pHolder->GetRect().bottom  - m_pHolder->GetRect().Height() / 2 ;
						newRect.right = m_pHolder->GetRect().right;
						newRect.bottom = newRect.top + m_textSize.cy ;
					}
					break;
				case BOTTOM_MID:
					{
						int mid = (m_pHolder->GetRect().left + m_pHolder->GetRect().right) /2;
						int midLeft = mid - m_textSize.cx /2;
						int midRight = midLeft + m_textSize.cx ;
						newRect.left = midLeft;
						newRect.right = midRight;
						newRect.top = m_pHolder->GetRect().bottom - m_pHolder->GetRect().Height() / 2 ;
						newRect.bottom =  newRect.top + m_textSize.cy;
					}
					break;
				case BOTTOM_RIGHT:
					{
						newRect.left = m_pHolder->GetRect().left;
						newRect.top =  m_pHolder->GetRect().bottom - m_pHolder->GetRect().Height() / 2 ;
						newRect.right =  newRect.left + m_textSize.cx;
						newRect.bottom = newRect.top + m_textSize.cy;
					}
					break;
				case CHANNEL_LEFT:
					{
						newRect.left = m_pHolder->GetRect().right;
						newRect.top =  m_pHolder->GetRect().top +  (m_pHolder->GetRect().Height()-  m_textSize.cy)/2 ;
						newRect.right = newRect.left +  m_textSize.cx;
						newRect.bottom = newRect.top + m_textSize.cy ;
					}
					break;
				case CHANNEL_RIGHT:
					{

					}
					break;

			}
		
		 
			if(oldRect != newRect)
			{
				//newRect += oldRect;
				CPoint pt1,pt2;
				pt1 = newRect.CenterPoint();
				newRect.UnionRect(oldRect,newRect);
				pt2 = newRect.CenterPoint();
				newRect.OffsetRect( pt1.x - pt2.x,0);

				if(oldStrText.GetLength() > ::_tcslen(strText)) //Ëõ½ô »æ»­ÇøÓò
					newRect.InflateRect( -(oldRect.Width() - newRect.Width()) /2,0);
				else
					newRect.InflateRect( -(newRect.Width() - m_textSize.cx) /2,0);
				
				this->SetRect(newRect);
				this->Redraw();
			}
		}
		
	};
	void OnLButtonDown(CPoint point){};
protected:
    CMFCRibbonBaseElementEx *m_pHolder;
	CSize m_textSize;
	UINT  m_alignMode;
	bool  m_bEnableMouseTouch;
};

class CMFCRibbonCheckBoxEx:public CMFCRibbonCheckBox
{
	friend class CVisualManager ;
	friend class CMFCRibbonBaseElementEx;
public:
	CMFCRibbonCheckBoxEx(CWnd *);
	CMFCRibbonCheckBoxEx(CWnd *,UINT nID, LPCTSTR lpszText);
	 ~CMFCRibbonCheckBoxEx();
	 CWnd* GetParentWnd() const;
	  void OnLButtonDown(CPoint point);
private:
	CWnd *m_pOwner ;
};
#define VISUAL_MSG_PROC()\
	m_VisualManager.WndProc(this->m_hWnd,message,wParam,lParam);
#define  DEC_VISUAL_MANAGER()\
	static CVisualManager  m_VisualManager;\
	void   AddToManager(CMFCRibbonBaseElement *);\
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

#define IMP_VISUAL_MANAGER(T) \
	CVisualManager T::m_VisualManager;\
	void  T::AddToManager(CMFCRibbonBaseElement *p)\
	{ m_VisualManager.AddToManager(p); };\
	LRESULT T::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)\
	{\
		HRESULT hRes ;\
		hRes = VISUAL_MSG_PROC();\
		if(hRes == S_FALSE)\
		hRes = __super::WindowProc(message,wParam,lParam);\
		return hRes;\
	}
 

#define IMP_VISUAL_MANAGER2(T) \
	CVisualManager T::m_VisualManager;\
	void  T::AddToManager(CMFCRibbonBaseElement *p)\
	{ m_VisualManager.AddToManager(p); };\

};