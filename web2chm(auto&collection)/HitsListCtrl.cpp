// HitsListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tabdoc.h"
#include "HitsListCtrl.h"
#include ".\hitslistctrl.h"


// CHitsListCtrl

IMPLEMENT_DYNAMIC(CHitsListCtrl, CListCtrl)
CHitsListCtrl::CHitsListCtrl()
{
}

CHitsListCtrl::~CHitsListCtrl()
{
}


BEGIN_MESSAGE_MAP(CHitsListCtrl, CListCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CHitsListCtrl ��Ϣ�������
//
//char *gColumnLabel[] = 
//{
//	"�������",
//	"������Ϣ"
//};
//
//const int NUM_COLUMNS = 2;
int CHitsListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
  /* 
	LV_COLUMN  lvc;
	
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	for(int i = 0; i<NUM_COLUMNS; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = gColumnLabel[i];
		lvc.cx = 80;
		lvc.fmt = LVCFMT_LEFT;
		InsertColumn(i,&lvc);
	}*/
	return 0;
}
