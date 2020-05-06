#pragma once

class CGameObj;
class CComponentDlg :
	public CDialogEx
{

protected:
	CGameObj*   m_pTargetObj;
	bool		m_bFocus;
	bool		m_bClick; // 조사결과 update 중간에 change함수가 호출된다.
	bool	  m_cbFocus;
	bool	  m_ckFocus;

public:
	virtual void Update(CGameObj* _pObj) = 0;
	virtual void Init(){}

protected:
	virtual void OnCancel() {}
	virtual void OnOK() {}
	virtual void PostNcDestroy();
public:
	virtual void Reset(CGameObj* _pObj) {}

public:
	afx_msg void OnEditSetfocus(UINT _iId);
	afx_msg void OnEditKillfocus(UINT _iId);

	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbnSetfocus();

	afx_msg void OnCheckKillfocus();
	afx_msg void OnCheckSetfocus();

public:
	CComponentDlg(UINT _id, CWnd* _pParent);
	~CComponentDlg();
};