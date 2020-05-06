#pragma once

#include "CComponentDlg.h"

class CAddComponentDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAddComponentDlg)

public:
	CAddComponentDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAddComponentDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDCOMPONENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnAddComponent();
	afx_msg void OnBnDoubleclickedButton1();

	virtual void Update(CGameObj* _pObj);
};