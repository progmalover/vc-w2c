// DPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "DPlayer.h"
#include "filepath.h"
CString strFlash[]={"\\Collection.swf","\\Auto.swf","\\Tutorial.swf"};
CString strTitle[]={"Web2EBook Tutorial(How to use Collection Mode)","Web2EBook Tutorial(How to use Auto Mode)","Web2EBook Tutorial"};
IMPLEMENT_DYNAMIC(CDPlayer, CDialog)
CDPlayer::CDPlayer(int Mode,CWnd* pParent /*=NULL*/)
	: CDialog(CDPlayer::IDD, pParent)
	,m_Mode(Mode)
{
}

CDPlayer::~CDPlayer()
{
}

void CDPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYER, m_player);
}


BEGIN_MESSAGE_MAP(CDPlayer, CDialog)
END_MESSAGE_MAP()


// CDPlayer 消息处理程序
//Web2EBook Tutorial
BOOL CDPlayer::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString  strSWF = strFlash[m_Mode];

	char Path[MAX_PATH];
	::GetModuleFileName(NULL,Path,MAX_PATH);
	CString strPath = ::FileStripFileName(Path);
	strPath += strSWF;
	if(::PathFileExists(strPath) == FALSE)
	{
		CString  str;
		str.Format("Can't find file:\n %s",strPath);
		AfxMessageBox(str);
		this->EndDialog(-1);
		return 0;
	}
	CRect rc(0, 0, FLASH_WIDTH, FLASH_HEIGHT);
	m_player.MoveWindow(&rc);

	AdjustWindowRect(&rc, GetStyle(), FALSE);
	MoveWindow(&rc);
	CenterWindow(GetDesktopWindow());
	m_player.put_Movie(strPath);
    SetWindowText(strTitle[m_Mode]);
	return TRUE; 
}
