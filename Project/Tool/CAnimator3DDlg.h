#pragma once
#include "CComponentDlg.h"

// CAnimator3DDlg 대화 상자

class CAnimator3DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimator3DDlg)

public:
	CAnimator3DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimator3DDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATOR3DDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual void Update(CGameObj* _pObj);
	virtual void Reset(CGameObj* _pObj);
private:
	CListBox m_listAnim;
	afx_msg void OnBnAddAnim();
	afx_msg void OnBnDeleteAnim();
public:
	CEdit m_editKey;
	CEdit m_editFrame;
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnPlayAnim();
	afx_msg void OnBnChange();
	afx_msg void OnBnAnimDeleteAll();
	afx_msg void OnBnSaveAnimFile();
	afx_msg void OnBnLoadAnimFile();
};
