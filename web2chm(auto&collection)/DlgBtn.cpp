#include "StdAfx.h"
#include ".\dlgbtn.h"
#include "WorkSpace.h"
#include "atlimage.h"
CDlgBtn::CDlgBtn(void)
{
	m_bShowTxt = FALSE;
	m_hBmp = NULL;
	m_bkclr = GetSysColor(COLOR_BTNFACE);
	m_bGetBmpHandle = FALSE;
}

CDlgBtn::~CDlgBtn(void)
{
	if(m_hBmp)
		::DeleteObject(m_hBmp);
}

 void CDlgBtn:: DrawBackground(CDC *pDC, const CRect &rc)
 {
	
	 CRect rcRect = rc;
	 CBrush brush;
	 brush.CreateSolidBrush(m_bkclr);

	 pDC->FillRect(&rcRect,&brush/*CBrush::FromHandle(GetSysColorBrush(COLOR_BTNFACE))*/); 
	
	 CBitmap bmp;
	 CDC memdc;
	 
	 rcRect.DeflateRect(1,1,1,1);
	 if(!GetCheck())
	 {
		 if(m_bGetBmpHandle)
		    bmp.Attach(this->m_hBmp);
		 else
		 {
			bmp.LoadBitmap(MAKEINTRESOURCE(m_dwBmpIDUP));
		  }
		 rcRect.DeflateRect(0,0,1,1);
	 }
	 else
	 {
		rcRect.DeflateRect(0,0,1,1);
		if(m_bGetBmpHandle)
			 bmp.Attach(this->m_hBmp);
		else
		{
			bmp.LoadBitmap(MAKEINTRESOURCE(m_dwBmpIDDOWN));
			//bmp.Attach(this->m_hBmp);
		}
	 }
     BITMAP bmpInf;
	 bmp.GetBitmap(&bmpInf);
	 memdc.CreateCompatibleDC(pDC);
	
	 CRect tpRc = rc;
	 tpRc.InflateRect(1,1,1,1);
	 memdc.FillRect( tpRc,&brush);
	// memdc.SetROP2(R2_WHITE);
	 memdc.SelectObject(bmp);

	 pDC->StretchBlt(rcRect.left,rcRect.top,rcRect.Width(),
			rcRect.Height(),&memdc,0,0,bmpInf.bmWidth,bmpInf.bmHeight,SRCCOPY);
	
	 if(m_bShowTxt)
	 {
		 pDC->DrawText(m_strTxt,rcRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	 }
	 bmp.Detach();
 }
BEGIN_MESSAGE_MAP(CDlgBtn, CFlatButton)
//	 ON_WM_PAINT()
//ON_WM_DRAWITEM()
ON_WM_SETCURSOR()
 
ON_WM_CREATE()
END_MESSAGE_MAP()



 BOOL CDlgBtn::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值
	if(GetCheck()==FALSE)
	{
		CWorkSpace *pOwner = (CWorkSpace *)GetBottomOwner();
		/*if(!pOwner->IsFloating())
			pOwner->SetFocus();*/
	}
	return CFlatButton::OnSetCursor(pWnd, nHitTest, message);
 }


 CWnd * CDlgBtn::GetBottomOwner(void)
 {
	 CWnd *pCur = NULL;
	 CWnd *pOwner = NULL;
	  
	 pCur = GetParent();

	 while(pCur)
	 {
		 pOwner = pCur;
		 if(pOwner->IsKindOf(RUNTIME_CLASS(CWorkSpace)))
			 break;
		 pCur = pOwner->GetParent();
	 }
	 return pOwner;
 }

 void CDlgBtn::SetBmpIDUP(DWORD bmpID)
 {
	 
	 CBitmap bmp;
	 m_dwBmpIDUP = bmpID;
	// bmp.LoadBitmap(MAKEINTRESOURCE(m_dwBmpIDUP));
	// m_hBmp = (HBITMAP)bmp.Detach();
 }
 void CDlgBtn::SetBmpIDDOWN(DWORD bmpID)
 {
	 CBitmap bmp;

	 m_dwBmpIDDOWN = bmpID;
	// bmp.LoadBitmap(MAKEINTRESOURCE(m_dwBmpIDDOWN));
	// m_hBmp = (HBITMAP)bmp.Detach();

 }
 void CDlgBtn::SetText(LPCSTR lpStr)
 {
	 this->m_strTxt = lpStr;
 }
 void CDlgBtn::ShowText(BOOL bEnable )
 {
	 this->m_bShowTxt = bEnable;
 }

 BOOL CDlgBtn:: LoadFromBuffer (LPBYTE lpBuffer, UINT uiSize)
{

	ASSERT(lpBuffer != NULL);

	HGLOBAL hRes = ::GlobalAlloc (GMEM_MOVEABLE, uiSize);
	if (hRes == NULL)
	{
		return FALSE;
	}

	IStream* pStream = NULL;
	LPVOID lpResBuffer = ::GlobalLock (hRes);
	ASSERT (lpResBuffer != NULL);

	memcpy (lpResBuffer, lpBuffer, uiSize);

	HRESULT hResult = ::CreateStreamOnHGlobal (hRes, FALSE, &pStream);

	if (hResult != S_OK)
	{
		return FALSE;
	}


	CImage Img;
	Img.Load(pStream);
	pStream->Release ();

	m_hBmp =Img.Detach ();

	return TRUE;
}

BOOL CDlgBtn:: LoadPNG(UINT  iPng)
 {
	HINSTANCE hinstRes = AfxFindResourceHandle(MAKEINTRESOURCE(iPng),_T("PNG"));
	HRSRC hRsrc = ::FindResource(hinstRes, MAKEINTRESOURCE(iPng), _T("PNG"));
	if (hRsrc == NULL)
	{
		return FALSE;
	}

	HGLOBAL hGlobal = LoadResource (hinstRes, hRsrc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	LPVOID lpBuffer = ::LockResource(hGlobal);
	if (lpBuffer == NULL)
	{
		FreeResource(hGlobal);
		return FALSE;
	}

	BOOL bRes = LoadFromBuffer ((LPBYTE) lpBuffer, (UINT) ::SizeofResource (hinstRes, hRsrc));

	UnlockResource(hGlobal);
	FreeResource(hGlobal);
    m_bGetBmpHandle = TRUE;
	return TRUE;
 }

 int CDlgBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
 {
	 if (CFlatButton::OnCreate(lpCreateStruct) == -1)
		 return -1;

	 // TODO:  Add your specialized creation code here
	
	
	 return 0;
 }

 BOOL CDlgBtn::LoadGIF(UINT iGif)
 {
	 HINSTANCE hinstRes = AfxGetResourceHandle();//AfxFindResourceHandle(MAKEINTRESOURCE(iGif),_T("GIF"));
	HRSRC hRsrc = ::FindResource(hinstRes, MAKEINTRESOURCE(iGif), _T("GIF"));
	if (hRsrc == NULL)
	{
		return FALSE;
	}

	HGLOBAL hGlobal = LoadResource (hinstRes, hRsrc);
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	LPVOID lpBuffer = ::LockResource(hGlobal);
	if (lpBuffer == NULL)
	{
		FreeResource(hGlobal);
		return FALSE;
	}

	BOOL bRes = LoadFromBuffer ((LPBYTE) lpBuffer, (UINT) ::SizeofResource (hinstRes, hRsrc));

	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	m_bGetBmpHandle = TRUE;
	return TRUE;
 }

 void CDlgBtn::SetBkColor(COLORREF clr)
 {
	 m_bkclr = clr;
 }
