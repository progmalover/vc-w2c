#pragma once
#include "afxwin.h"



// CAddPanel ������ͼ
//CLookPanel
class CAddPanel : public CFormView
{
	DECLARE_DYNCREATE(CAddPanel)

public:
	CAddPanel();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAddPanel();

public:
//	enum { IDD = IDD_ADDPANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//CEdit m_AddEdit;
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CListBox m_PrepareList;
	virtual void OnInitialUpdate();
};


