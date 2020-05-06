// CTransformDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CTransformDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CTransform.h"


// CTransformDlg 대화 상자

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=nullptr*/)
	:CComponentDlg(IDD_TRANSFORM, pParent)
{
}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for(UINT i = IDC_EDIT1; i <= IDC_EDIT9; ++i)
		DDX_Control(pDX, i, m_arrTrans[i - IDC_EDIT1]);
}

BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEditSetfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT9, &CTransformDlg::OnEditChange)
END_MESSAGE_MAP()

void CTransformDlg::Update(CGameObj* _pObj)
{
	if (m_bFocus)
		return;
	m_bClick = false;
	m_pTargetObj = _pObj;


	Vec3 vPos = _pObj->GetTransForm()->GetPos();
	Vec3 vScale = _pObj->GetTransForm()->GetScale();
	Vec3 vRot = _pObj->GetTransForm()->GetRotation();
	CString strTemp;

	for (int i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vPos[i]);
		m_arrTrans[i].SetWindowTextW(strTemp);
	}

	for (int i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vScale[i]);
		m_arrTrans[i + 3].SetWindowTextW(strTemp);
	}

	for (int i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", XMConvertToDegrees(vRot[i]));
		m_arrTrans[i + 6].SetWindowTextW(strTemp);
	}
	m_bClick = true;
}

void CTransformDlg::OnEditChange(UINT _iId)
{
	if (!m_bClick)
		return;

	if (m_pTargetObj == nullptr)
		return;

	if (m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;

	CString strTemp;
	Vec3 vTrans[3] = {};

	for (int i = 0; i < 9; ++i)
	{
		m_arrTrans[i].GetWindowTextW(strTemp);
		vTrans[i / 3][i % 3] = (float)_wtof(strTemp);
	}

	m_pTargetObj->GetTransForm()->SetPos(vTrans[0]);
	m_pTargetObj->GetTransForm()->SetScale(vTrans[1]);

	Vec3 vRot;
	vRot.x = XMConvertToRadians(vTrans[2][0]);
	vRot.y = XMConvertToRadians(vTrans[2][1]);
	vRot.z = XMConvertToRadians(vTrans[2][2]);

	m_pTargetObj->GetTransForm()->SetRotation(vRot);
}