#pragma once

// CColliderDlg 대화 상자
#include "CComponentDlg.h"

class CColliderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CColliderDlg)

public:
	CColliderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	CEdit m_arrPos[3];
	CEdit m_arrScale[3];
	CEdit m_Radius;

public:
	virtual void Init();
	virtual void Update(CGameObj* _pObj);
	afx_msg void OnChangeEdit(UINT _iId);
	DECLARE_MESSAGE_MAP()

	CComboBox m_cbType;
	afx_msg void OnCbnSelchange();
};