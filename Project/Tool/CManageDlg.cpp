// CManageDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CManageDlg.h"
#include "afxdialogex.h"


// CManageDlg 대화 상자

IMPLEMENT_DYNAMIC(CManageDlg, CDialogEx)

CManageDlg::CManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MANAGE, pParent)
	, m_iOldScroll(0)
{
}

CManageDlg::~CManageDlg()
{
}

void CManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll);
}


BEGIN_MESSAGE_MAP(CManageDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

int CManageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CManageDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nSBCode)
	{
	case SB_LINEUP:
		m_iOldScroll = m_Scroll.GetScrollPos();
		m_Scroll.SetScrollPos(m_iOldScroll - 10);
		break;
	case SB_LINEDOWN:
		m_iOldScroll = m_Scroll.GetScrollPos();
		m_Scroll.SetScrollPos(m_iOldScroll + 10);
		break;
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Scroll.SetScrollRange(0, 500);
	m_Scroll.SetScrollPos(0);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CManageDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	CDialogEx::PostNcDestroy();
}