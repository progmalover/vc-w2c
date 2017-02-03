#pragma once
typedef struct charSet{
int nSetID;
char *pSet;
}CHAR_SET;
extern CHAR_SET CharSetArray[];
extern int CharSetStrToInt(CString &str);
extern int StrToLangID(CString & str);