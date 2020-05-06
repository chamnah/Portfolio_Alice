#pragma once
#include "CComponentDlg.h"
// CAnimationDlg 대화 상자

class CAnimation2D;
class CAddAnim;
class CAnimationDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimationDlg)

public:
	CAnimationDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimationDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION };
#endif

private:
	CListBox m_ListClip;
	CListBox m_ListFrm;
	CEdit	 m_EditAnim[3];
	CAnimation2D* m_pCurSelAnim;
	bool     m_bSelectFrm;
	CButton m_Check;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual void Init();
	virtual void Update(CGameObj* _pObj);
	bool GetCheck() { return m_Check.GetCheck(); }

public:
	afx_msg void OnLbnChangeClip();
	afx_msg void OnLbnChangeFrame();
	afx_msg void OnEnChangeEdit(UINT _iId);
	afx_msg void OnBnPlay();
	afx_msg void OnBnPause();
	afx_msg void OnBnAddAnimation();
	afx_msg void OnBnRemoveAnimation();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnAddAnim();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedAddTex();
private:
	CEdit m_editLeft;
	CEdit m_editTop;
	CEdit m_editRight;
	CEdit m_editBottom;
};