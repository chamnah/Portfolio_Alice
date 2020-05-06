// CTileSize.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CTileSize.h"
#include "afxdialogex.h"


// CTileSize 대화 상자

IMPLEMENT_DYNAMIC(CTileSize, CDialogEx)

CTileSize::CTileSize(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TILE_SIZE, pParent)
{

}

CTileSize::~CTileSize()
{
}

void CTileSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editSizeX);
	DDX_Control(pDX, IDC_EDIT2, m_editSizeY);
}

POINT CTileSize::GetSize()
{
	int  iX = _wtoi(m_strX);
	int  iY = _wtoi(m_strY);

	return POINT{ iX, iY };
}


BEGIN_MESSAGE_MAP(CTileSize, CDialogEx)
END_MESSAGE_MAP()


// CTileSize 메시지 처리기

void CTileSize::OnOK()
{
	m_editSizeX.GetWindowTextW(m_strX);
	m_editSizeY.GetWindowTextW(m_strY);

	CDialogEx::OnOK();
}