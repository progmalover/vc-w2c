#pragma once

#define SIZE_MB 1048576




#define DEFINE_REGISTERED_MESSAGE(message)	UINT message = ::RegisterWindowMessage("5F7AA60C-ED3A-4d60-A04A-7E2148BA8461:"#message);
#define DEFINE_CLIPBOARD_FORMAT(format)		UINT format = ::RegisterClipboardFormat("5F7AA60C-ED3A-4d60-A04A-7E2148BA8461:"#format);


CString LoadResStr(UINT nID);
void SecondToTime(int nSecondCouont, int *pHour, int *pMinute, int *pSecond);
int GetSecondCounts(int nHour, int nMinute, int nSecond);
int GetDirectxVersion(void);
int GetSystemId(void);
BOOL ShutDownWindows(int SystemID);