#pragma once
#include "CComponentDlg.h"
enum class LIGHT_DLG
{
	DIFFUSE_X,
	DIFFUSE_Y,
	DIFFUSE_Z,
	SPECULAR_X,
	SPECULAR_Y,
	SPECULAR_Z,
	AMBIENT_X,
	AMBIENT_Y,
	AMBIENT_Z,
	DIRECTION_X,
	DIRECTION_Y,
	DIRECTION_Z,
	DISTANCE,
	ANGLE,
	END
};

class CLightDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CLightDlg)

public:
	CLightDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLightDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHTDLG};
#endif

public:
	void Update(CGameObj* _pObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_LightType;
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_Edit[(UINT)LIGHT_DLG::END];
public:
	afx_msg void OnCbnSelchange();
	afx_msg void OnEnChange(UINT _iIdx);
};
