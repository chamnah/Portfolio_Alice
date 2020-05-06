#pragma once


// CTransformDlg 대화 상자

#include "CComponentDlg.h"

class CTransformDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTransformDlg)


private:
	
	CEdit		m_arrTrans[9];

public:
	CTransformDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTransformDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void Update(CGameObj* _pObj);

	afx_msg void OnEditChange(UINT _iId);
};