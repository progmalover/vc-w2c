#include "StdAfx.h"
#include ".\tanchormanager.h"
#include ".\tanchorpage.h"
#include "TAnchorPiece.h"
#include "AnchorTree.h"
#include "filepath.h"

#include "httpapi.h"
#include  <algorithm>
#include "htmlparse.h"
#include ".\refpathcalculator.h"
#include "hmCompiler.h"
TAnchorPage::TAnchorPage(void)
{
}
 
TAnchorPage::~TAnchorPage(void)
{
	if(m_piecsVect.size() <= 0) return ;

	for(PIECES_VECT::iterator it = m_piecsVect.begin();it != m_piecsVect.end();
		it++)
		delete *it;
		
}

void TAnchorPage::AddPiece(TAnchorPiece *pPiece)
{
	m_piecsVect.push_back(pPiece);
}

/**/
void TAnchorPage::ModifyPageContent(CHtmlParse &parse,CString &strDir,URL_STRING strRoot)
{
	CString urlstr,strPath;
	//parse.ClearPropertyByName();
	if(DoPageCHTFilter(parse) == FALSE) return;

	ENTRY_LIST &list = parse.GetByPropertyName("href");
	ENTRY_LIST::iterator _end = list.end();
	for(ENTRY_LIST::iterator it = list.begin();it != _end;it++)
	{
		
		TagEntry &tagEn = *it;
		
		urlstr = tagEn.m_strTagText;
		
		if(IsInpageLink(urlstr))
			continue;
		if(urlstr.GetLength() <= 3)
			continue;

		urlstr.MakeLower();
		//ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
		//urlstr.ReleaseBuffer();

		TRACE("*%s\n",urlstr);
		if(IsRefLink(urlstr)) // 相对url的话，先生成完整的url
			urlstr = ComposeRefURL(&strRoot,&urlstr);

		if(FALSE == IsRightLink(urlstr))
		{
			tagEn.ModifyText(" ");
			continue;
		}
	    _URL Obj(urlstr);
		if(Obj.m_UrlTagType == TAG_UNKNOW) //
			continue;
		
		URL_STRING  str;
		ExtractValidUrl(&urlstr,&str);
		urlstr = str;	
		
		if(AssertUrlHttpStr(urlstr) == FALSE)
			urlstr.Insert(0,"http://");
	 //当前前是下载 url状态。。或者。。。网页上得url已经进入下载流程。。。。
		{
			CRefPathCalculator   pathCalculator(strRoot,urlstr);
			pathCalculator.ExecEx(strPath);

			tagEn.ModifyText(strPath);
			strPath.Empty();
		} 
	}

 
	ENTRY_LIST &list2 = parse.GetByPropertyName("src");
	ENTRY_LIST::iterator _end2 = list2.end();
	for(ENTRY_LIST::iterator it = list2.begin();it != _end2;it++)
	{
		
		TagEntry &tagEn = *it;
		
		urlstr = tagEn.m_strTagText;
		

		if(IsInpageLink(urlstr))
			continue;
		if(urlstr.GetLength() <= 3)
			continue;

		urlstr.MakeLower();
		//ToLowerAlphas(urlstr.GetBuffer(),urlstr.GetLength());
		//urlstr.ReleaseBuffer();
		TRACE("*%s\n",urlstr);
		if(IsRefLink(urlstr)) // 相对url的话，先生成完整的url
			urlstr = ComposeRefURL(&strRoot,&urlstr);

		if(FALSE == IsRightLink(urlstr))
		{
			tagEn.ModifyText(" ");
			continue;
		}
	    _URL Obj(urlstr);
		if(Obj.m_UrlTagType == TAG_UNKNOW) //
			continue;
		
		URL_STRING  str;
		ExtractValidUrl(&urlstr,&str);
		urlstr = str;	

		//ExtractValidUrl(pUrl,&validUrl);//for 带参数的url.....

		//PATH_STRING  strPath = pSession->GetUrlLocalPath(validUrl);

		if(AssertUrlHttpStr(urlstr) == FALSE)
			urlstr.Insert(0,"http://");
	 //当前前是下载 url状态。。或者。。。网页上得url已经进入下载流程。。。。
		{
			CRefPathCalculator   pathCalculator(strRoot,urlstr);
			pathCalculator.ExecEx(strPath);

			tagEn.ModifyText(strPath);
			strPath.Empty();
		} 
	}

}

/*按照原始 url 建立路径，保存文件 */
CString  TAnchorPage::SavePage(CString  localName)
{
	 CString  pieceTxt;
	
	 if(NULL != GP_CurrentSession) return CString();

	//pieceTxt = "<html> \n<body>";

	HTREEITEM iItem = TAnchorManager::Instance()->GetPageItemHandle(this);
	iItem = GO_AnchorTree.GetNextItem(iItem,TVGN_CHILD);
	while(iItem)
	{
		TAnchorPiece *pPage =(TAnchorPiece *) GO_AnchorTree.GetItemData(iItem) ;
		if(pPage == NULL)
			break;
		pieceTxt += "<br>";
		pieceTxt += pPage->m_rawHtml;
		pieceTxt += "</br>";
		iItem = GO_AnchorTree.GetNextItem(iItem,TVGN_NEXT);
	}

	//pieceTxt += "</body> \n</html>";
	int nRet = HasHtmlBodyAndHead(pieceTxt);
	switch(nRet)
	{
		case 3://has body and html
			{
				LPSTR  pTxt = (LPSTR)pieceTxt.GetBuffer();
				LPSTR pStr = StrStrI(pTxt,"<body");
				pieceTxt.ReleaseBuffer();
				pieceTxt.Insert(pStr - pTxt,"onerror=function(){return true}"\
												"<\/script>");
			}
			break; 
		case 2://has body
			{
				pieceTxt.Insert(0,_T("<html> \n<script> onerror=function(){return true}"\
												"<\/script>"));
				pieceTxt += "\n</html>";
			}
			break;
		case 1://has html
			{
				LPSTR  pTxt = (LPSTR)pieceTxt.GetBuffer();
				LPSTR pStr = StrStrI(pTxt,"<html");
				pieceTxt.ReleaseBuffer();
				pStr += 5;
				pieceTxt.Insert(pStr - pTxt,"onerror=function(){return true}"\
												"<\/script>");
			}
			break;
		default:
			{
				pieceTxt.Insert(0,_T("<html> \n<script> onerror=function(){return true}"\
												"<\/script><body>"));
				pieceTxt += "</body> \n</html>";
			}break;

	}


/*原位置 建立文件*/
	//CString  localName;
	CString  filePath = localName;

    //need safevalida ??
	URL_STRING  strValid;
	ExtractValidUrl( &this->m_BindPageUrl,&strValid);
	CreatePathFromUrl(&localName,&strValid,filePath,1);
	CRefPathCalculator::ToSafeWay(filePath);
	
	URL_STRING  dirPath = FileStripFileName(filePath.GetBuffer());
	filePath.ReleaseBuffer();
	BOOL bRet = PrepareDirectory(dirPath,0);
	if(bRet)
	{
		/*修改 内存 中url链接 到本地 ，然后保存文件 */
		CHtmlParse  parse ;
		parse.AttachBuffer(pieceTxt);

		ModifyPageContent(parse,localName ,this->m_BindPageUrl);
		
		parse.SaveToFile(filePath);

		//if( ConfigManager::Instance()->GetSystemConfig()->dwMode & 2)//for convert Text
		//{
		//	CString txtDir = TxtCompiler::GetTxtDirPath(localName);
		//	if(txtDir.IsEmpty())
		//		return filePath;
		//	PrepareDirectory(txtDir,0);
		//	TxtCompiler::Instance()->DoCompiler( filePath,txtDir);
		//}

		return filePath;
	}

	return CString();
}


int TAnchorPage::GetPieceCount(void)
{
	return m_piecsVect.size();
}

void TAnchorPage::RemovePiece(TAnchorPiece* pPiece)
{
	m_piecsVect.erase(std::find( m_piecsVect.begin(), m_piecsVect.end(),pPiece));
	delete pPiece;
}

BOOL TAnchorPage::HasThisPiece(TAnchorPiece* pPiece)
{
	for(PIECES_VECT::iterator it = m_piecsVect.begin();it != m_piecsVect.end();
		it++)
	{
		
		TAnchorPiece *piece =  *it;
		if(strstr(piece->m_rawHtml,pPiece->m_rawHtml))
			return TRUE;
		if(strstr(pPiece->m_rawHtml,piece->m_rawHtml))
			return TRUE;
	}
	return FALSE;
}



BOOL  TAnchorPage::DoPageCHTFilter(CHtmlParse & parse)
{
		CString  tmpSvPath;
		pPRIVATE_CONFIG  pConf = ConfigManager::Instance()->GetDefaultSessionConf();

		tmpSvPath = GetTempFile(NULL,"~tmp","data");
		if(pConf->bCScript || pConf->bCNoScript)
		{
			if(pConf->bCNoScript)
			  parse.ClearRangeByName("noscript");
			if(pConf->bCScript)
			  parse.ClearRangeByName("script");

			REFRESH_FILE(tmpSvPath)
		}
		

		if(pConf->bCLoad)
			parse.ClearPropertyByName("onload");
	
		if(pConf->bCUnload)
			parse.ClearPropertyByName("onunload");

		if(pConf->bCMouseClick)
			parse.ClearPropertyByName("onclick");

		if(pConf->bCMouseLeave)
			parse.ClearPropertyByName("onmouseleave");

		if(pConf->bCMouseEnter)
			parse.ClearPropertyByName("onmouseenter");

		if(pConf->bCMouseOver)
			parse.ClearPropertyByName("onmouseover");

		if(pConf->bCMouseOut)
			parse.ClearPropertyByName("onmouseout");
		
		if(pConf->bCMouseDown)
			parse.ClearPropertyByName("onmousedown");

		REFRESH_FILE(tmpSvPath)
		DeleteFile(tmpSvPath);
		return TRUE;
}

inline int TAnchorPage::HasHtmlBodyAndHead(CString & htmText)
{
	int iRet = 0;

	LPCSTR pStr = htmText.GetBuffer();
	LPSTR  pfind = StrStrI(pStr,_T("<html"));
	if(pfind) iRet += 1;
	
	pfind = StrStrI(pStr,_T("<body"));
	if(pfind) iRet += 2;
	
	htmText.ReleaseBuffer();
	return iRet;
}
