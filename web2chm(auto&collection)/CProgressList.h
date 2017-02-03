#pragma  once
#include <vector>
#include "ugctrl.h"
#include "UGCTprog.h"
//#include "TWebTask.h"
#define  WM_SET_SELL_INF    WM_USER + 234
//class CUGAdvProgressType;
using namespace std;


class TWebTask;

typedef struct prg_sell{
	
prg_sell()
{
	pTrd = NULL;
	pProgress = NULL;
	PrgID = -1;
	icol = -1;
	lrow = -1;
	bused = 0;

};
CUGProgressType   *pProgress;
int					PrgID;
int					icol;
long				lrow;
BOOL				bused;
TWebTask		  *pTrd;

}*pPROGRESS_SELL,PROGRESS_SELL;
typedef vector<PROGRESS_SELL> PROGRESS_LIST;

typedef struct ProgInf{
	ProgInf()
	{
		Str1 = "";
		Str2 = "";
		val   = 0;
		pCell = NULL;
	};

	CString  Str1;
	CString  Str2;
	int			val;
	pPROGRESS_SELL pCell;
}PROG_INF,*pPROG_INF;

class CTskPrgList:public CUGCtrl
{

public:
	CTskPrgList();
	~CTskPrgList();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(MyCug1)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//***** Over-ridable Notify Functions *****
	virtual void OnSetup();
	
	//movement and sizing
	virtual int  OnCanSizeCol(int col);
	virtual int  OnCanSizeTopHdg();
    
//	void	
public:
	//PROGRESS_LIST  m_prgList;
	PROGRESS_LIST   m_prgList;
	void	SetProgressSells(int num);
	void	FreeAllSel();
	void GetFreePrgBar(int &prgID,CUGProgressType *pCtrl);
	void SetupSection(long row, LPCSTR title);
	void OnLClicked(int col,long row,int updn,RECT *rect,POINT *point,BOOL processed);
	virtual void GetProgressBar(pPROGRESS_SELL &lpPrgCell,TWebTask *pTrd);
	virtual void SetProgressBarInf(pPROGRESS_SELL lpPrgCell , LPCTSTR lpfName, 
				LPCTSTR lpUrl, LPCTSTR lpPercent);
	TWebTask *GetRowTask(int iRaw);
	virtual int StartMenu(int col,long row,POINT *point,int section);
protected:
	volatile long  m_bLock;
public:
	void Lock(void);
	void UnLock(void);
	void ClearAllCellInf(void);
	afx_msg HRESULT  OnSetSellInf(WPARAM wparam,LPARAM lparam);
protected:
	 CRITICAL_SECTION   m_Critical;
//	virtual void PostNcDestroy();
public:
	BOOL ValidCellPtr(pPROGRESS_SELL lpCell);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	void ClearInfMessage(void);
};
//extern  CTskPrgList *GP_TskPrgList; 
