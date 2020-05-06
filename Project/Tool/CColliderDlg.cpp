// CColliderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CColliderDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CCollider.h"
#include "MainFrm.h"
// CColliderDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderDlg, CDialogEx)

CColliderDlg::CColliderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_COLLIDER, pParent)
{

}

CColliderDlg::~CColliderDlg()
{
}

void CColliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	for (UINT i = IDC_EDIT1; i <= IDC_EDIT3; ++i)
		DDX_Control(pDX, i, m_arrPos[i - IDC_EDIT1]);

	for (UINT i = IDC_EDIT4; i <= IDC_EDIT6; ++i)
		DDX_Control(pDX, i, m_arrScale[i - IDC_EDIT4]);

	DDX_Control(pDX, IDC_EDIT7, m_Radius);

	DDX_Control(pDX, IDC_COMBO1, m_cbType);
}

void CColliderDlg::Init()
{
	m_cbType.InsertString(0, L"Rect");
	m_cbType.InsertString(1, L"Box");
	m_cbType.InsertString(2, L"Circle2D");
	m_cbType.InsertString(3, L"Circle3D");
	m_cbType.InsertString(4, L"IsoMetrix2D");
}

void CColliderDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_bFocus || m_cbFocus)
		return;


	m_bClick = false;

	m_cbType.SetCurSel((UINT)m_pTargetObj->GetCollider()->GetColliderType());
	if (m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_RECT ||
		m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_BOX ||
		m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_ISO)
	{
		for (UINT i = 0; i < 3; ++i)
			m_arrScale[i].EnableWindow(true);
		
		m_Radius.EnableWindow(false);
	}
	else if(m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_2D ||
		m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		for (UINT i = 0; i < 3; ++i)
			m_arrScale[i].EnableWindow(false);
		
		m_Radius.EnableWindow(true);
	}

	Vec3 vPos = m_pTargetObj->GetCollider()->GetPos();
	Vec3 vScale = m_pTargetObj->GetCollider()->GetScale();
	float fRad = m_pTargetObj->GetCollider()->GetRadius();
	CString strTemp;
	
	
	for (UINT i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vPos[i]);
		m_arrPos[i].SetWindowTextW(strTemp);
	}

	for (UINT i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vScale[i]);
		m_arrScale[i].SetWindowTextW(strTemp);
	}

	strTemp.Format(L"%f", fRad);
	m_Radius.SetWindowTextW(strTemp);

	m_bClick = true;
}

void CColliderDlg::OnChangeEdit(UINT _iId)
{
	if (!m_bClick)
		return;

	CString strTemp;
	Vec3 vEdit;
	float fRad;

	for (UINT i = 0; i < 3; ++i)
	{
		m_arrPos[i].GetWindowTextW(strTemp);
		vEdit[i] = (float)_wtof(strTemp);
		m_pTargetObj->GetCollider()->SetPos(vEdit);
	}

	if (m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_2D ||
		m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE_3D)
	{
		m_Radius.GetWindowTextW(strTemp);
		fRad = (float)_wtof(strTemp);
		m_pTargetObj->GetCollider()->SetRadius(fRad);
		vEdit = Vec3(fRad, fRad, fRad);
		m_pTargetObj->GetCollider()->SetScale(vEdit);
	}
	else
	{
		for (UINT i = 0; i < 3; ++i)
		{
			m_arrScale[i].GetWindowTextW(strTemp);
			vEdit[i] = (float)_wtof(strTemp);
			m_pTargetObj->GetCollider()->SetScale(vEdit);
		}
	}
}

BEGIN_MESSAGE_MAP(CColliderDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT7, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT7, &CComponentDlg::OnEditSetfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT7, &CColliderDlg::OnChangeEdit)

	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CColliderDlg::OnCbnSelchange)
END_MESSAGE_MAP()

void CColliderDlg::OnCbnSelchange()
{
	m_pTargetObj->GetCollider()->SetColliderType((COLLIDER_TYPE)m_cbType.GetCurSel());

	if (m_cbType.GetCurSel() == 2)
	{
		CString strTemp;
		float fRad = 0.f;
		m_Radius.GetWindowTextW(strTemp);
		fRad = (float)_wtof(strTemp);

		m_pTargetObj->GetCollider()->SetScale(Vec3(fRad, fRad, 0.f));
	}
	else if (m_cbType.GetCurSel() == 3)
	{
		CString strTemp;
		float fRad = 0.f;
		m_Radius.GetWindowTextW(strTemp);
		fRad = (float)_wtof(strTemp);

		m_pTargetObj->GetCollider()->SetScale(Vec3(fRad, fRad, fRad));
	}

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
}