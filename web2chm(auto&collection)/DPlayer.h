#pragma once
#include "ShockwaveFlash.h"
#include "resource.h"

// CDPlayer �Ի���
#define  FLASH_WIDTH   800
#define  FLASH_HEIGHT  600
class CDPlayer : public CDialog
{
	DECLARE_DYNAMIC(CDPlayer)

public:
	CDPlayer(int Mode=2,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDPlayer();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CShockwaveFlash m_player;
	int   m_Mode;
public:
	virtual BOOL OnInitDialog();
};
