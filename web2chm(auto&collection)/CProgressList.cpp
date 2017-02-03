/***************************************************
****************************************************
Skeleton Class for a Derived MyCug1
****************************************************
****************************************************/

#include "stdafx.h"
#include "resource.h"
#include "CProgressList.h"
#include <stdlib.h>
#include <time.h>
#include ".\cprogresslist.h"
#include <algorithm>

#include "Defs.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  CHECK_LOCKED()\
  if(m_bLock> 1)return;


BEGIN_MESSAGE_MAP(CTskPrgList,CUGCtrl)
	//{{AFX_MSG_MAP(MyCug1)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_MESSAGE(WM_SET_SELL_INF,OnSetSellInf)
    ON_WM_CREATE()
	/*ON_COMMAND(ID__PAUSEJOB,OnPauseJob)
	ON_COMMAND(ID__RESUMEJOB,OnResumeJob)
	ON_COMMAND(ID__ABANDONJOB,OnAbandonJob)
	ON_UPDATE_COMMAND_UI(ID__ABANDONJOB,OnAbandonJobUI)*/
	//}}AFX_MSG_MAP
	
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/***************************************************
****************************************************/
CTskPrgList::CTskPrgList():m_bLock(0)
{
	InitializeCriticalSection(&m_Critical);
}
/***************************************************
****************************************************/
CTskPrgList::~CTskPrgList()
{
	DeleteCriticalSection(&m_Critical);
	if(m_prgList.size())
	{
		for(PROGRESS_LIST::iterator it = m_prgList.begin();
			it != m_prgList.end();it++)
		 delete (*it).pProgress;
	}
	m_prgList.empty();
}

/***************************************************
OnSetup
	This function is called just after the grid window 
	is created or attached to a dialog item.
	It can be used to initially setup the grid
****************************************************/
void CTskPrgList::OnSetup()
{
		
	/*设置scrollbar*/
	SetSH_Width(0);
	SetHS_Height(0);

	m_bLock = FALSE;

	SetNumberCols(3);
	this->SetUserSizingMode(2);
	this->SetHScrollMode(UG_SCROLLJOYSTICK  );
	SetUniformRowHeight(TRUE);

	CUGEdit* editClass = (CUGEdit*)GetEditClass();
	editClass->SetAutoSize(TRUE);//
	
    CUGCell cell;// set header text align 
	GetCell(0,-1,&cell);
//	cell.UseThemes(false);
	cell.SetAlignment(UG_ALIGNCENTER);
	cell.SetFont(&afxGlobalData.fontRegular);
	cell.SetTextColor(afxGlobalData.clrInactiveCaptionText);
	//cell.SetTextColor(RGB(255,255,255));
	//cell.SetBackColor(afxGlobalData.clrActiveCaption);
	SetCell(0,-1,&cell);
	
	GetCell(1,-1,&cell);
	cell.SetFont(&afxGlobalData.fontRegular);
//	cell.UseThemes(false);
	cell.SetTextColor(afxGlobalData.clrInactiveCaptionText);
	cell.SetAlignment(UG_ALIGNCENTER);
	//cell.SetBackColor(afxGlobalData.clrActiveCaption);
	SetCell(1,-1,&cell);

	GetCell(2,-1,&cell);
	cell.SetFont(&afxGlobalData.fontRegular);
	cell.SetTextColor(afxGlobalData.clrInactiveCaptionText);
//	cell.UseThemes(false);
	cell.SetAlignment(UG_ALIGNCENTER);
//	cell.SetBackColor(afxGlobalData.clrActiveCaption);
	SetCell(2,-1,&cell);
	
	QuickSetText(0,-1,"FILENAME ");
	QuickSetText(1,-1,"PROGRESS");
	QuickSetText(2,-1,"URL ");
}
/***************************************************
OnCanSizeCol
	Sent when the user is over a separation line on
	the top heading
	A return value of TRUE allows the possibiliy of
	a resize
****************************************************/
int CTskPrgList::OnCanSizeCol(int col){
	return TRUE;
}

int  CTskPrgList::OnCanSizeTopHdg(){
	return FALSE;
}

/*清除所有的sell*/
void	CTskPrgList::FreeAllSel()
{
	
	SetNumberRows(0);
	
}

//重新设置progress sell得个数。。。
 void CTskPrgList::	SetProgressSells(int num)
 {
		
	 //clear all instance of progress sell type
		FreeAllSel();
		SetNumberRows(num);

	//-------------------------------new create here
		for(int i = m_prgList.size();i <  num;i++)
		{
			SetupSection(i, "Progress Bars");

			PROGRESS_SELL  prgtype;
			prgtype.pProgress = new CUGProgressType();
			prgtype.icol = 1;
			prgtype.lrow = i;
			prgtype.bused = FALSE;
		
			prgtype.PrgID = AddCellType(prgtype.pProgress);
			m_prgList.push_back(prgtype);
			
			SetRowHeight(i,20);

			//set progress 
			CUGCell cell;
			GetCell(1,i,&cell);
			cell.SetFont(&afxGlobalData.fontRegular);
			cell.SetCellType(prgtype.PrgID);
			cell.SetAlignment(UG_ALIGNCENTER|UG_ALIGNVCENTER);
			cell.SetParam(0);
			cell.UseThemes(true);

			cell.SetHBackColor(afxGlobalData.clrActiveBorder);
			cell.SetBackColor(afxGlobalData.clrBarFace);
			cell.SetTextColor(RGB(0,0,100));
			cell.SetHTextColor(RGB(250,220,250));
		
			cell.SetText("0");
			SetCell(1,i,&cell); 
            
		}
	
		Invalidate();
 }

 void CTskPrgList::GetFreePrgBar(int &prgID,CUGProgressType *pCtrl)
 {

 }


 /*
 set row color property
 */
 void CTskPrgList::SetupSection(long row, LPCSTR title)
 {
	 CUGCell cell;

	 cell.SetBackColor(RGB(240,240,240));
	 cell.SetTextColor(RGB(0,0,128));
	 QuickSetRange(0,row,GetNumberCols() - 1,row,&cell);
 }

void CTskPrgList::OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed)
{
	
  processed = TRUE;
}

/*need a release progressbar */
void CTskPrgList::GetProgressBar(pPROGRESS_SELL &lpPrgCell,TWebTask *pTrd)
{

	CHECK_LOCKED();
    //lock thread here ....
    //EnterCriticalSection(&m_Critical);

	lpPrgCell = NULL;
	if(m_prgList.size() <= 0)
	return;

	for(PROGRESS_LIST::iterator it = m_prgList.begin();
		it != m_prgList.end();it++)
	{
		PROGRESS_SELL &cell = *it;
		if(cell.bused == FALSE)
		{
			 
			cell.bused = TRUE;
			cell.pTrd = pTrd;
			lpPrgCell = &(*it);
			break;
		}
	}

    // LeaveCriticalSection(&m_Critical);
}



void CTskPrgList::SetProgressBarInf(pPROGRESS_SELL lpPrgCell , LPCTSTR lpfName, LPCTSTR lpUrl, LPCTSTR lpPercent)
{
	CHECK_LOCKED();

	if(!ValidCellPtr(lpPrgCell))
		return ;
	CUGCell cell;
	GetCell(lpPrgCell->icol -1,lpPrgCell->lrow,&cell);
	cell.SetText(lpfName);
	SetCell(lpPrgCell->icol -1,lpPrgCell->lrow ,&cell);
  

	GetCell(lpPrgCell->icol,lpPrgCell->lrow ,&cell);
	cell.SetText(lpPercent);
	SetCell(lpPrgCell->icol,lpPrgCell->lrow ,&cell);

	
	GetCell(lpPrgCell->icol +1,lpPrgCell->lrow,&cell);
	cell.SetText(lpUrl);
	SetCell(lpPrgCell->icol +1,lpPrgCell->lrow ,&cell);

	RedrawRange(lpPrgCell->icol -1,lpPrgCell->lrow,lpPrgCell->icol +1,lpPrgCell->lrow);

}

void CTskPrgList::Lock(void)
{
	//InterlockedIncrement(&m_bLock);
	m_bLock = 1;
}

void CTskPrgList::UnLock(void)
{
	//InterlockedDecrement(&m_bLock);
	m_bLock = 0;
}

/*清理字符串信息，和复位progress得进度。。。*/
void CTskPrgList::ClearAllCellInf(void)
{
//	CHECK_LOCKED();
	if(m_prgList.size() <= 0)
		return;
	
	CUGCell cell;
	for(PROGRESS_LIST::iterator it = m_prgList.begin();
		it != m_prgList.end();it++)
	{
		PROGRESS_SELL &prgCell = *it;
		prgCell.bused = FALSE;
		GetCell(prgCell.icol,prgCell.lrow,&cell);
		cell.SetText("0");
		SetCell(prgCell.icol,prgCell.lrow,&cell);

		GetCell(prgCell.icol-1,prgCell.lrow,&cell);
		cell.SetText("");
		SetCell(prgCell.icol-1,prgCell.lrow,&cell);

		GetCell(prgCell.icol+1,prgCell.lrow,&cell);
		cell.SetText("");
		SetCell(prgCell.icol+1,prgCell.lrow,&cell);
		
	}
}

void inline ClearSameMessage(int msg ,HWND hwnd)
{
		MSG Msg;
	/*	while(PeekMessage(&Msg,hwnd,msg,msg,PM_NOREMOVE));
		{
			if(Msg.message  == (UINT) msg)
			{
				PeekMessage(&Msg,hwnd,msg,msg,PM_REMOVE);
			}else
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}
		}*/
		/*if(PeekMessage(&Msg,hwnd,msg,msg,PM_REMOVE))
		{
			if(Msg.message != (UINT)msg)
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}
		}*/
		
}

 HRESULT CTskPrgList:: OnSetSellInf(WPARAM wparam,LPARAM lparam)
 {
	pPROG_INF pinf = (pPROG_INF)wparam;
	CString  strVal;
	strVal.Format("%d",pinf->val);
	if(GP_CurrentSession == NULL) 
		return S_FALSE;

	SetProgressBarInf(pinf->pCell,pinf->Str1,pinf->Str2,strVal);
//	ClearSameMessage(WM_SET_SELL_INF,m_hWnd);
	return S_OK;
 }


 BOOL CTskPrgList::ValidCellPtr(pPROGRESS_SELL lpCell)
 {	 
	 int i, size = m_prgList.size();
	 for( i = 0;i < size;i++)
		 if(lpCell == &m_prgList[i])
			 return TRUE;
	 return FALSE;
 }


 int CTskPrgList::OnCreate(LPCREATESTRUCT lpCreateStruct)
 {
	 if (CUGCtrl::OnCreate(lpCreateStruct) == -1)
		 return -1;

	 // TODO:  Add your specialized creation code here
	
	 this->EnableMenu(TRUE);
	 return 0;
 }
 int  CTskPrgList::StartMenu(int col,long row,POINT *point,int section)
 {
	 if(row == -1)
		 return TRUE;
	 CMenu Menu;
	 Menu.LoadMenu(MAKEINTRESOURCE(IDR_TASK_POPMENU));
	 CMenu *pPop = Menu.GetSubMenu(0);
	 pPop->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point->x, 
      point->y, AfxGetMainWnd());
	return TRUE;
 }

 TWebTask *  CTskPrgList::GetRowTask( int iRaw)
 {
	PROGRESS_SELL &Cell = m_prgList[iRaw];
	return Cell.pTrd;
 }


 void CTskPrgList::ClearInfMessage(void)
 {
	 DWORD msg = WM_SET_SELL_INF;
	 MSG  Msg;
	while(PeekMessage(&Msg,m_hWnd,msg,msg,PM_REMOVE));
	
 }
