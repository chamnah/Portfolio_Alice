// CLightDlg.cpp: 구현 파일
//
#include "stdafx.h"
#include "Tool.h"
#include "CLightDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CLight.h"

// CLightDlg 대화 상자

IMPLEMENT_DYNAMIC(CLightDlg, CDialogEx)

CLightDlg::CLightDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_LIGHTDLG, pParent)
{
}

CLightDlg::~CLightDlg()
{
}

void CLightDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_pTargetObj == nullptr)
		return;

	if (m_bFocus || m_cbFocus)
		return;

	m_bClick = false;

	CString str;
	float fDis = m_pTargetObj->GetLight()->GetDistance();
	float fAngle = m_pTargetObj->GetLight()->GetAngle();
	tLightColor& color = m_pTargetObj->GetLight()->GetLightColor();
	Vec4&   vDir = m_pTargetObj->GetLight()->GetDirection();


	for (UINT i = 0; i < 3; i++)
	{
		str.Format(L"%f", color.vDiff[i]);
		m_Edit[(UINT)LIGHT_DLG::DIFFUSE_X + i].SetWindowTextW(str);
	}
	str = L"";
	for (UINT i = 0; i < 3; i++)
	{
		str.Format(L"%f", color.vSpec[i]);
		m_Edit[(UINT)LIGHT_DLG::SPECULAR_X + i].SetWindowTextW(str);
	}
	str = L"";
	for (UINT i = 0; i < 3; i++)
	{
		str.Format(L"%f", color.vAmb[i]);
		m_Edit[(UINT)LIGHT_DLG::AMBIENT_X + i].SetWindowTextW(str);
	}
	str = L"";
	for (UINT i = 0; i < 3; i++)
	{
		str.Format(L"%f", vDir[i]);
		m_Edit[(UINT)LIGHT_DLG::DIRECTION_X + i].SetWindowTextW(str);
	}

	str = L"";
	str.Format(L"%f", fDis);
	m_Edit[(UINT)LIGHT_DLG::DISTANCE].SetWindowTextW(str);

	str = L"";
	str.Format(L"%f", fAngle);
	m_Edit[(UINT)LIGHT_DLG::ANGLE].SetWindowTextW(str);
	

	m_bClick = true;
}

void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_LightType);
	for (size_t i = 0; i <= IDC_EDIT14 - IDC_EDIT1; i++)
	{
		DDX_Control(pDX, IDC_EDIT1 + i, m_Edit[i]);
	}
	
}

BEGIN_MESSAGE_MAP(CLightDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLightDlg::OnCbnSelchange)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT14, &CLightDlg::OnEnChange)

	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT14, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT14, &CComponentDlg::OnEditSetfocus)

	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
END_MESSAGE_MAP()

// CLightDlg 메시지 처리기

BOOL CLightDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_LightType.InsertString(0, L"Direction");
	m_LightType.InsertString(1, L"Point");
	m_LightType.InsertString(2, L"Spot");

	m_LightType.SetCurSel(0);

	for (UINT i = (UINT)LIGHT_DLG::DIRECTION_X; i <= (UINT)LIGHT_DLG::DIRECTION_Z; i++)
		m_Edit[i].EnableWindow(true);

	m_Edit[(UINT)LIGHT_DLG::DISTANCE].EnableWindow(false);
	m_Edit[(UINT)LIGHT_DLG::ANGLE].EnableWindow(false);

	for (size_t i = 0; i < 14; i++)
	{
		m_Edit[i].SetWindowTextW(L"0");
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLightDlg::OnCbnSelchange()
{
	if (m_pTargetObj == nullptr)
		return;
	if (!m_bClick)
		return;

	int iSel = m_LightType.GetCurSel();
	
	if (iSel == 0)
	{
		for (UINT i = (UINT)LIGHT_DLG::DIRECTION_X; i <= (UINT)LIGHT_DLG::DIRECTION_Z; i++)
			m_Edit[i].EnableWindow(true);
		
		m_Edit[(UINT)LIGHT_DLG::DISTANCE].EnableWindow(false);
		m_Edit[(UINT)LIGHT_DLG::ANGLE].EnableWindow(false);
	}
	else if(iSel == 1)
	{
		for (UINT i = (UINT)LIGHT_DLG::DIRECTION_X; i <= (UINT)LIGHT_DLG::DIRECTION_Z; i++)
			m_Edit[i].EnableWindow(false);

		m_Edit[(UINT)LIGHT_DLG::DISTANCE].EnableWindow(true);
		m_Edit[(UINT)LIGHT_DLG::ANGLE].EnableWindow(false);
	}
	else if (iSel == 2)
	{
		for (UINT i = (UINT)LIGHT_DLG::DIRECTION_X; i <= (UINT)LIGHT_DLG::DIRECTION_Z; i++)
			m_Edit[i].EnableWindow(true);

		m_Edit[(UINT)LIGHT_DLG::DISTANCE].EnableWindow(true);
		m_Edit[(UINT)LIGHT_DLG::ANGLE].EnableWindow(true);
	}

	m_pTargetObj->GetLight()->SetLightType(iSel);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLightDlg::OnEnChange(UINT _iIdx)
{
	if (m_pTargetObj == nullptr)
		return;

	if (!m_bClick)
		return;

	CString str;
	float fValue = 0.f;
	tLightColor color;
	Vec3   vDir;

	
	for (UINT i = 0; i < 3; i++)
	{
		m_Edit[(UINT)LIGHT_DLG::DIFFUSE_X + i].GetWindowTextW(str);
		fValue = (float)_wtof(str);
		color.vDiff[i] = fValue;
	}

	for (UINT i = 0; i < 3; i++)
	{
		m_Edit[(UINT)LIGHT_DLG::SPECULAR_X + i].GetWindowTextW(str);
		fValue = (float)_wtof(str);
		color.vSpec[i] = fValue;
	}

	for (UINT i = 0; i < 3; i++)
	{
		m_Edit[(UINT)LIGHT_DLG::AMBIENT_X + i].GetWindowTextW(str);
		fValue = (float)_wtof(str);
		color.vAmb[i] = fValue;
	}

	for (UINT i = 0; i < 3; i++)
	{
		m_Edit[(UINT)LIGHT_DLG::DIRECTION_X + i].GetWindowTextW(str);
		fValue = (float)_wtof(str);
		vDir[i] = fValue;
	}
	
	m_pTargetObj->GetLight()->SetDirection(vDir);
	m_pTargetObj->GetLight()->SetLightColor(color);

	m_Edit[(UINT)LIGHT_DLG::DISTANCE].GetWindowTextW(str);
	fValue = (float)_wtof(str);
	m_pTargetObj->GetLight()->SetDistance(fValue);

	m_Edit[(UINT)LIGHT_DLG::ANGLE].GetWindowTextW(str);
	fValue = (float)_wtof(str);
	m_pTargetObj->GetLight()->SetAngle(fValue);
}