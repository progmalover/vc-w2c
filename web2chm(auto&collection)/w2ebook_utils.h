#pragma once
#ifndef   _W2EBOOK_UTILE_H
#define _W2EBOOK_UTILE_H

BOOL     IsValidURL(CString strURL);
CString  GetSysTempFolder();
CString  TaskFileName(CString strPath,CString strNameNoExt);
CString  NameNoExt(CString strName);
void     AddComboxString(CComboBox *pBox,int ID);
void     ModifyCmbEdit(CComboBox *pBox,DWORD  dwStyle);
CString  TaskName(CString strPathEx);

#define  RATIO_CHECK \
	void    SetRatioCheck(int ID,BOOL bCheck)\
{\
	CButton *pRatio = (CButton*)GetDlgItem(ID);\
	if(pRatio!=NULL)\
        pRatio->SetCheck(bCheck);\
};\
BOOL    GetRatioCheck(int ID)\
{\
	CButton *pRatio = (CButton*)GetDlgItem(ID);\
	if(pRatio!=NULL)\
		return pRatio->GetCheck();\
	return FALSE;\
};\

#endif