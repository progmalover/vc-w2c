#include "StdAfx.h"
#include ".\particular.h"

CString LoadResStr(UINT nID)
{
	CString str;
	str.LoadString(nID);
	return str;
}

void SecondToTime(int nSecondCouont, int *pHour, int *pMinute, int *pSecond)
{
	(*pHour) = nSecondCouont / 3600;
	(*pMinute) = nSecondCouont % 3600 / 60;
	(*pSecond) = nSecondCouont % 60;
}

int GetSecondCounts(int nHour, int nMinute, int nSecond)
{
	return nHour * 3600 + nMinute * 60 + nSecond;
}




int GetSystemId(void)
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	int SystemID=0;
	switch(osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2)	SystemID=13;//Win 2003
		if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==1)	SystemID=12;//Win XP
		if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0)	SystemID=11;//Win 2K
		if(osvi.dwMajorVersion<=4)  							SystemID=10;//Win NT	
		break;

	case VER_PLATFORM_WIN32_WINDOWS:
		if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0)	SystemID=1;//Win 95
		if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==10)	SystemID=2;//Win 98
		if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==90)	SystemID=3;//Win Me
		break;
	}	

	return SystemID;
}

BOOL ShutDownWindows(int SystemID)
{
	

	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}	

	UINT Flags;
	switch(SystemID)
	{
	case 1:
	case 2:
	case 3:
	case 10:
		Flags=EWX_SHUTDOWN|EWX_FORCE;
		break;
	case 11:
		Flags=EWX_POWEROFF|0x00000010;//EWX_FORCEIFHUNG
		break;
	default:
		Flags=EWX_SHUTDOWN|0x00000010;//EWX_FORCEIFHUNG
	}

	return ExitWindowsEx(Flags,0)!=0;
}
