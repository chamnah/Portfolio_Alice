#pragma once
#include "CComponentDlg.h"
// CTextDlg 대화 상자
class CTextDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTextDlg)

public:
	CTextDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTextDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Update(CGameObj* _pObj);

public:
	CEdit m_editText;
	CEdit m_editSize;
	CEdit m_editColor[4];
	afx_msg void OnEnChangeText();
	afx_msg void OnEnChangeSize();
	afx_msg void OnEnChangeColor(UINT _id);
};