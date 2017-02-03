#pragma once
#include "Defs.h"
/*
为任务建立相应的文件目录结构,的响应保存路径
*/
typedef URL_LIST	PATH_LIST;
typedef URL_STRING	PATH_STRING;
typedef PATH_LIST::iterator		PATH_DIR;

class TaskDirCollection
{
public:
	TaskDirCollection(PATH_STRING &strRoot);
	TaskDirCollection(void);

/*设置根目录路径。。。如果初始化没有设置的话。。但如果属于重新设置。。会完全丢弃以前的所有数据*/
	void		SetRootPath(PATH_STRING &strRoot);
	PATH_STRING &GetRootPath(void){return m_RootPath;};
/*把一个url根据本地用户设置的保存目录，转化为实际的下载文件路径*/
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