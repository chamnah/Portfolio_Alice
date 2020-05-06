#pragma once

#include "CComponentDlg.h"

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScriptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void Init();
	void Update(CGameObj* _pObj);
	virtual void Reset(CGameObj* _pObj);
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lbScript;
	CComboBox m_cbScript;
	vector<CButton*> m_vecCheck;
	vector<CEdit*> m_vecInt;
	CScript* m_pCurScript;
public:
	afx_msg void OnBnAddScript();
	afx_msg void OnBnDeleteScript();
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnDestroy();
	afx_msg void OnBnClicked(UINT _id);
	afx_msg void OnEnChangeInt(UINT _id);
};