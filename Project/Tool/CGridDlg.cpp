// CGridDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CGridDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CGrid.h"


// CGridDlg 대화 상자

IMPLEMENT_DYNAMIC(CGridDlg, CDialogEx)

CGridDlg::CGridDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_GRID, pParent)
{

}

CGridDlg::~CGridDlg()
{
}

void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editSize[0]);
	DDX_Control(pDX, IDC_EDIT2, m_editSize[1]);
}

void CGridDlg::Update(CGameObj * _pObj)
{
	if (m_bFocus)
		return;

	m_bClick = false;
	m_pTargetObj = _pObj;

	if (m_pTargetObj == nullptr)
		return;

	

	UINT iNum = m_pTargetObj->GetGrid()->GetCountX();
	CString strTemp;
	strTemp.Format(L"%d", iNum);
	m_editSize[0].SetWindowTextW(strTemp);

	iNum = m_pTargetObj->GetGrid()->GetCountY();
	strTemp.Format(L"%d", iNum);
	m_editSize[1].SetWindowTextW(strTemp);
	m_bClick = true;
}


BEGIN_MESSAGE_MAP(CGridDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT2, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT2, &CComponentDlg::OnEditSetfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT2, &CGridDlg::OnEditChange)
END_MESSAGE_MAP()


// CGridDlg 메시지 처리기


void CGridDlg::OnEditChange(UINT _iIdx)
{
	if (!m_bClick)
		return;

	if (m_pTargetObj == nullptr)
		return;


	

	CString strTemp;

	m_editSize[0].GetWindowTextW(strTemp);
	int iX = _wtoi(strTemp.GetBuffer());

	m_editSize[1].GetWindowTextW(strTemp);
	int iY = _wtoi(strTemp.GetBuffer());

	m_pTargetObj->GetGrid()->SetCount(iX,iY);
	m_pTargetObj->GetGrid()->ChangeTile();
}