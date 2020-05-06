#pragma once

#include "CComponentDlg.h"
// CHBrushDlg 대화 상자
class CHBrushDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CHBrushDlg)

public:
	CHBrushDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHBrushDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HBRUSHDLG };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_editTexName;

public:
	void Update(CGameObj* _pObj);

public:
	afx_msg void OnBnClickedTextureLoad();
};
