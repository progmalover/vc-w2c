// TabBar.cpp : ʵ���ļ�
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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDialogBar::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CTabBar, CDialogBar)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CTabBar, CDialogBar)
END_DISPATCH_MAP()

// ע��: ������� IID_ITabBar ֧��
//��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//���Ƚӿڵ� GUID ƥ�䡣

// {A51585FD-2759-435D-9CC7-C977A690294C}
static const IID IID_ITabBar =
{ 0xA51585FD, 0x2759, 0x435D, { 0x9C, 0xC7, 0xC9, 0x77, 0xA6, 0x90, 0x29, 0x4C } };

BEGIN_INTERFACE_MAP(CTabBar, CDialogBar)
	INTERFACE_PART(CTabBar, IID_ITabBar, Dispatch)
END_INTERFACE_MAP()


// CTabBar ��Ϣ�������

