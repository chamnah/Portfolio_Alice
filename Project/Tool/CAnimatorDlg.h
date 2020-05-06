#pragma once
#include "CComponentDlg.h"

// CAnimaotrDlg 대화 상자

class CAnimatorDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimatorDlg)

public:
	CAnimatorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimatorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATOR };
#endif

private:
	CEdit m_EditName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
public:
	void Update(CGameObj* _pObj);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoad();
};