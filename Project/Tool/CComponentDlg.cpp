#include "stdafx.h"
#include "CComponentDlg.h"

CComponentDlg::CComponentDlg(UINT _id, CWnd * _pParent)
	:CDialogEx(_id, _pParent)
	, m_pTargetObj(nullptr)
	, m_bFocus(false)
	, m_bClick(false)
	, m_cbFocus(false)
{
}

CComponentDlg::~CComponentDlg()
{
}

void CComponentDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	CDialogEx::PostNcDestroy();
}

void CComponentDlg::OnEditSetfocus(UINT _iId)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bFocus = true;
}


void CComponentDlg::OnEditKillfocus(UINT _iId)
{
	m_bFocus = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CComponentDlg::OnCbnKillfocus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cbFocus = false;
}


void CComponentDlg::OnCbnSetfocus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cbFocus = true;
}

void CComponentDlg::OnCheckKillfocus()
{
	m_ckFocus = false;
}

void CComponentDlg::OnCheckSetfocus()
{
	m_ckFocus = true;
}
