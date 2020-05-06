// CTextDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CTextDlg.h"
#include "afxdialogex.h"
#include "CText.h"
#include "CGameObj.h"

// CTextDlg 대화 상자

IMPLEMENT_DYNAMIC(CTextDlg, CDialogEx)

CTextDlg::CTextDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_TEXTDLG, pParent)
{

}

CTextDlg::~CTextDlg()
{
}

void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editText);
	DDX_Control(pDX, IDC_EDIT2, m_editSize);
	for (size_t i = 0; i < 4; i++)
	{
		DDX_Control(pDX, IDC_EDIT3 + i, m_editColor[i]);
	}
}

void CTextDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_pTargetObj == nullptr)
		return;

	if (m_bFocus)
		return;
	m_bClick = true;

	m_editText.SetWindowTextW(m_pTargetObj->GetText()->GetTextName().c_str());
	float fSize = m_pTargetObj->GetText()->GetSize();
	CString strSize;
	strSize.Format(L"%f", fSize);
	m_editSize.SetWindowTextW(strSize);

	Vec4& vColor = m_pTargetObj->GetText()->GetColor();
	CString strColor;
	for (size_t i = 0; i < 4; i++)
	{
		strColor.Format(L"%f", vColor[i]);
		m_editColor[i].SetWindowTextW(strColor);
	}

	m_bClick = false;
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CTextDlg::OnEnChangeText)
	ON_EN_CHANGE(IDC_EDIT2, &CTextDlg::OnEnChangeSize)
	ON_CONTROL_RANGE(EN_CHANGE,IDC_EDIT3, IDC_EDIT6, &CTextDlg::OnEnChangeColor)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEditSetfocus)
END_MESSAGE_MAP()

void CTextDlg::OnEnChangeText()
{
	CString strText;
	m_editText.GetWindowTextW(strText);
	if (m_pTargetObj->GetText() == nullptr)
		return;
	m_pTargetObj->GetText()->SetText(strText.GetBuffer());
	m_pTargetObj->GetText()->ChangeFontAndSize();
}

void CTextDlg::OnEnChangeSize()
{
	CString strText;
	m_editSize.GetWindowTextW(strText);
	float fSize = _wtof(strText);
	m_pTargetObj->GetText()->SetSize(fSize);
	m_pTargetObj->GetText()->ChangeFontAndSize();
}

void CTextDlg::OnEnChangeColor(UINT _id)
{
	if (m_bClick)
		return;
	CString strColor;
	Vec4 vColor;
	for (size_t i = 0; i < 4; i++)
	{
		m_editColor[i].GetWindowTextW(strColor);
		vColor[i] = _wtof(strColor);
	}
	m_pTargetObj->GetText()->ChangeColor(vColor);
}