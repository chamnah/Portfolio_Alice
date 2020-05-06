#pragma once

#include "CComponentDlg.h"
class CGridDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CGridDlg)

public:
	CGridDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGridDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
public:
	virtual void Update(CGameObj* _pObj);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditChange(UINT _iIdx);
private:
	CEdit m_editSize[2];
};