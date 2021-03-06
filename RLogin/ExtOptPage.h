#pragma once

#include "afxwin.h"
#include "ListCtrlExt.h"

/////////////////////////////////////////////////////////////////////////////
// CExtOptPage ダイアログ

class CExtOptPage : public CTreePage
{
	DECLARE_DYNAMIC(CExtOptPage)

public:
	CExtOptPage();
	~CExtOptPage();

// ダイアログ データ
	enum { IDD = IDD_EXTOPTPAGE };

public:
	CListCtrlExt m_ExtList;
	CString m_InlineExt;

public:
	void DoInit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

public:
	virtual BOOL OnApply();
	virtual void OnReset();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClickExtlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetInfoTipExtlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdate();
};
