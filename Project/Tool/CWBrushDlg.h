#pragma once


// CWBrushDlg 대화 상자
#include "CComponentDlg.h"

class CTexture;
class CWBrushDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CWBrushDlg)

public:
	CWBrushDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CWBrushDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CWBRUSHDLG };
#endif

public:
	void Update(CGameObj* _pObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CButton m_radioTex[4];
	CEdit m_editName[4];
	CTexture* m_pTexBrush[4];
public:
	afx_msg void OnBnClickedTexure(UINT _iId);
	afx_msg void OnBnClickedTextureLoad(UINT _iId);
	virtual BOOL OnInitDialog();
};
