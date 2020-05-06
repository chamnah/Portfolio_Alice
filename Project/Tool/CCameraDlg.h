#pragma once

#include "CComponentDlg.h"
// CCameraDlg 대화 상자

class CCameraDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Update(CGameObj* _pObj);
	void Init();
	void SetNothing();
	void SetEverthing();
	void SetMixing(UINT _iId);
	virtual void Reset(CGameObj* _pObj);

private:
	CComboBox m_cbCamType;
	CEdit m_EditFar;
	CEdit m_EditScale;
	CEdit m_EditFOV;
	CComboBoxEx m_cbnLayer;
	CImageList m_ImageList;
	bool  m_bLayerFocus;



public:
	afx_msg void OnChangeEdit(UINT _iId);
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnSelchangeLayer();
	afx_msg void OnCbnKillfocusLayer();
	afx_msg void OnCbnSetfocusLayer();
};