#pragma once
class TWebSession;
typedef BOOL (WINAPI *COMPILERFUNC)(const void*, LPVOID,  LPVOID, LPVOID);
#define  WM_CHM_PRGRESS  WM_USER  + 301
#define	 CHM_CONVER  0
#define	 TXT_CONVER  1
class ChmCompiler
{
public:
	ChmCompiler(void);	
	~ChmCompiler(void);
public:
	CString  CreateHHP(CString strTaskfName,LPCTSTR strHHC,LPCTSTR strHHK);
	CString  CreateHHC(CString strTaskfName);
	CString  CreateHHC2(CString strTaskfName);
	CString  CreateHHK(CString strTaskfName);

	void     CreateDirStruct2(CString &strDir,CStdioFile *phhcFile);
	void     CreateDirStruct(CString &strDir,CStdioFile &hhcFile);
	void	 CreateDirStructStr(CString &strDir ,CString &strFiles);
	void	 CompilerExec(CString strTaskfName);
	class CLoadDll
	{
	public:
		CLoadDll(void)
		{
			if(m_hinstLib == NULL)
				m_hinstLib = ::LoadLibrary(_T("hha.dll")); 
			if(m_func == NULL &&m_hinstLib != NULL)
				m_func = (COMPILERFUNC)GetProcAddress(m_hinstLib,"HHA_CompileHHP");
			if(m_func == NULL)
				AfxMessageBox(_T("Not found hha.dll!"));
		 
		};
		~CLoadDll(void)
		{
			if(m_hinstLib)
				FreeLibrary(m_hinstLib);
			m_hinstLib= NULL;
			m_func = NULL;

		};
	};
#ifdef  _DEBUG
protected:
	 void InitLog(LPCTSTR lpPath);
	 static CStdioFile  m_logFile;
#endif
public:
	static COMPILERFUNC     m_func ;
	static HINSTANCE	    m_hinstLib ; 
	static BOOL		 CALLBACK  FuncProgress(char* pstr);
	static BOOL		 CALLBACK  FunLog(char* pstr);
	static CWnd		 *m_pWnd;
	static BOOL		 b_Exit;
	//static CString   m_strCharSet;
	static CString	 _strLang ;
	static void ConverCharSet(CString &charSetName, CString & pSource, int nlen);


};
class TxtCompiler
{
public:
	TxtCompiler();
	~TxtCompiler();
	static  TxtCompiler *Instance();
	static  void    	Uninstance();
	BOOL				DoCompiler(LPCTSTR strSrcName,LPCTSTR strTxtDir,CStdioFile *pFile = NULL ,BOOL bOneFile = 0);

private:
	static TxtCompiler *_pInstance;
	int	   _nPageNum;
public:
	static  CWnd		 *m_pWnd; 
	static  BOOL		 b_Exit;
	static  CString GetTxtDirPath(LPCTSTR  strRootPath);
	static  CString GetTxtDirPath(CString strRootPath);
	static  BOOL	CompilerExec(LPCTSTR strSrcName,LPCTSTR strTxtDir);
	static  BOOL    CompilerExec2(LPCTSTR strSrcName,LPCTSTR strTxtDir,int pageNum);
	static  BOOL    GetHtmlText(LPCTSTR strSrcName,CString &strOutText);

};