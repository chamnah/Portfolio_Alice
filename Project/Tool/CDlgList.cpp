// CDlgList.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CDlgList.h"
#include "afxdialogex.h"


// CDlgList 대화 상자

IMPLEMENT_DYNAMIC(CDlgList, CDialogEx)

CDlgList::CDlgList(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIST, pParent)
{

}

CDlgList::~CDlgList()
{
}

void CDlgList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DlgList);
}


BEGIN_MESSAGE_MAP(CDlgList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgList::OnListDBClick)
END_MESSAGE_MAP()


// CDlgList 메시지 처리기


void CDlgList::OnListDBClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iIdx = pNMItemActivate->iItem;
	m_strSelect = m_DlgList.GetItemText(m_iIdx,0);
	*pResult = 0;

	EndDialog(0);
}

BOOL CDlgList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_strCaption);

	m_DlgList.InsertColumn(0,L"");

	for (UINT i = 0; i < m_vecList.size(); ++i)
	{
		m_DlgList.InsertItem(i, m_vecList[i]);
	}

	return TRUE;  
}