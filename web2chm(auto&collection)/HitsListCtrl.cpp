// HitsListCtrl.cpp : 实现文件
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



// CHitsListCtrl 消息处理程序
//
//char *gColumnLabel[] = 
//{
//	"相关内容",
//	"附加信息"
//};
//
//const int NUM_COLUMNS = 2;
int CHitsListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
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
