#pragma once

#include "CComponentDlg.h"
// CEventDlg 대화 상자

enum class EVENT_MODE
{
	NONE,
	WALL,
	MONSTER,
	SCENE
};

class CIncidentDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CIncidentDlg)

private:
	EVENT_MODE m_eMode;
	CString    m_strObj;
	vector<CGameObj*> m_vecObj;
	vector<POINT>     m_vecTilePos;
	wstring m_strScene;

public:
	CIncidentDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CIncidentDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Update(CGameObj* _pObj);
	virtual BOOL OnInitDialog();
private:
	CComboBox m_cbKinds;
public:
	afx_msg void OnCbnSelchangeCombo();
private:
	CListBox m_listObjKinds;
	CListBox m_listEvent;
public:
	afx_msg void OnBnClickedObjPlace();
	afx_msg void OnBnClickedApply();
	void ClearObj();
	afx_msg void OnBnClickedPopBack();
	afx_msg void OnBnClickedDelete();
};
