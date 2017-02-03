#pragma once
#include "Defs.h"
/*
Ϊ��������Ӧ���ļ�Ŀ¼�ṹ,����Ӧ����·��
*/
typedef URL_LIST	PATH_LIST;
typedef URL_STRING	PATH_STRING;
typedef PATH_LIST::iterator		PATH_DIR;

class TaskDirCollection
{
public:
	TaskDirCollection(PATH_STRING &strRoot);
	TaskDirCollection(void);

/*���ø�Ŀ¼·�������������ʼ��û�����õĻ���������������������á�������ȫ������ǰ����������*/
	void		SetRootPath(PATH_STRING &strRoot);
	PATH_STRING &GetRootPath(void){return m_RootPath;};
/*��һ��url���ݱ����û����õı���Ŀ¼��ת��Ϊʵ�ʵ������ļ�·��*/
	PATH_STRING	UrlToLocalPath(URL_STRING *strURL,BOOL bSave,BOOL bAddRoot);
	PATH_STRING	UrlToLocalURL(URL_STRING *pstrRoot,URL_STRING *pstrDesk);
#if 0
	void		CreatePathFromUrl(URL_STRING *urlStr,URL_STRING &strFile ,BOOL bAddRoot);
#endif
	void		CreateLocalUrlFromUrl(URL_STRING *purlRoot,URL_STRING *purlDesk,URL_STRING &strUrl );
	int			GetFathNum(void);
	//PATH_DIR &  GetDirRoot(void);
	//PATH_DIR &  GetNextPath(PATH_DIR &PathDir);
public:
	PATH_LIST	m_pathList;
	PATH_STRING m_RootPath;
};