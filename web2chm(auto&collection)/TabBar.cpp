// TabBar.cpp : 实现文件
//

#include "stdafx.h"
#include "tabdoc.h"
#include "TabBar.h"


// CTabBar

IMPLEMENT_DYNAMIC(CTabBar, CDialogBar)
CTabBar::CTabBar()
{
	EnableAutomation();
}

CTabBar::~CTabBar()
{
}

void CTabBar::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清除代码。

	CDialogBar::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CTabBar, CDialogBar)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CTabBar, CDialogBar)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_ITabBar 支持
//以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//调度接口的 GUID 匹配。

// {A51585FD-2759-435D-9CC7-C977A690294C}
static const IID IID_ITabBar =
{ 0xA51585FD, 0x2759, 0x435D, { 0x9C, 0xC7, 0xC9, 0x77, 0xA6, 0x90, 0x29, 0x4C } };

BEGIN_INTERFACE_MAP(CTabBar, CDialogBar)
	INTERFACE_PART(CTabBar, IID_ITabBar, Dispatch)
END_INTERFACE_MAP()


// CTabBar 消息处理程序

