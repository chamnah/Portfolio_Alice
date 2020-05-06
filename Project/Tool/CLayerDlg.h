#pragma once

#include "CComponentDlg.h"
// CLayerDlg 대화 상자

class CLayerDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLayerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void Init();
	virtual void Update(CGameObj* _pObj);
private:
	CComboBox m_cbLayer;
	UINT      m_iLayerID;
public:
	afx_msg void OnCbnSelchangeCombo1();
	CEdit m_editLayer;
	afx_msg void OnBnAddLayer();
	afx_msg void OnBnChangeLayer();
};