// CFilterComDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CFilterComDlg.h"
#include "afxdialogex.h"


// CFilterComDlg 대화 상자

IMPLEMENT_DYNAMIC(CFilterComDlg, CDialogEx)

CFilterComDlg::CFilterComDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTERCOMDLG, pParent)
{

}

CFilterComDlg::~CFilterComDlg()
{
}

void CFilterComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFilterComDlg, CDialogEx)
END_MESSAGE_MAP()


// CFilterComDlg 메시지 처리기
