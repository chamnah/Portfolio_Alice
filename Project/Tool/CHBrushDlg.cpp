// CHBrushDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CHBrushDlg.h"
#include "afxdialogex.h"
#include "CPathMgr.h"
#include "CGameObj.h"
#include "CTerrain.h"
#include "CTexture.h"
// CHBrushDlg 대화 상자

IMPLEMENT_DYNAMIC(CHBrushDlg, CDialogEx)

CHBrushDlg::CHBrushDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_HBRUSHDLG, pParent)
{

}

CHBrushDlg::~CHBrushDlg()
{
}

void CHBrushDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editTexName);
}


BEGIN_MESSAGE_MAP(CHBrushDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CHBrushDlg::OnBnClickedTextureLoad)
END_MESSAGE_MAP()


// CHBrushDlg 메시지 처리기


void CHBrushDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
}

void CHBrushDlg::OnBnClickedTextureLoad()
{
	if (m_pTargetObj == nullptr)
		return;
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Texture\\Height\\";
	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"tga", L"*.*",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	wstring wcsPath = dlg.GetPathName();
	wstring wcsKey = dlg.GetFileName();
	m_editTexName.SetWindowTextW(wcsKey.c_str());

	wcsPath = CPathMgr::GetInst()->AbsoluteToRelativeResPath(wcsPath);
	//CTexture::CreateTexture(L"..\\content\\Texture\\Height\\Film_Set_SciFi_Terrain_01_H.tga", D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	CTexture* pTex = CTexture::CreateTexture(wcsPath, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pTargetObj->GetTerrain()->SetHeightMap(pTex);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}