#pragma once
#include "resource.h"
#include ".\tanchormanager.h"
#include "tcatalogbuilder.h"
#include "httpapi.h"
#include "shlwapi.h"
#include "CProgressList.h"
#include "Defs.h"
#include "Simiebar.h"
#include "ui.h"

using namespace ::ui;
// CFinishPage �Ի���

class CFinishPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CFinishPage)

public:
	CFinishPage();
	virtual ~CFinishPage();

	// �Ի�������
	enum { IDD = IDD_PROPPAGE_FINISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	void    CreateAutoTask(TWebSession *tpSession,int dwThrdNum);
	void    CreateAnchorTask(TWebSession *tpSession,int dwThrdNum);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();

};
