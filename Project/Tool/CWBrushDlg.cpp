// CWBrushDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CWBrushDlg.h"
#include "afxdialogex.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CGameObj.h"
#include "CTerrain.h"

// CWBrushDlg 대화 상자

IMPLEMENT_DYNAMIC(CWBrushDlg, CDialogEx)

CWBrushDlg::CWBrushDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_CWBRUSHDLG, pParent)
	, m_pTexBrush{}
{

}

CWBrushDlg::~CWBrushDlg()
{
}

void CWBrushDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
}

void CWBrushDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	for (size_t i = 0; i < 4; i++)
	{
		DDX_Control(pDX, IDC_RADIO1 + i, m_radioTex[i]);
		DDX_Control(pDX, IDC_EDIT1 + i, m_editName[i]);
	}
}


BEGIN_MESSAGE_MAP(CWBrushDlg, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO4, &CWBrushDlg::OnBnClickedTexure)
	ON_CONTROL_RANGE(BN_CLICKED,ID_BUTTON1,ID_BUTTON4, &CWBrushDlg::OnBnClickedTextureLoad)
END_MESSAGE_MAP()


// CWBrushDlg 메시지 처리기


void CWBrushDlg::OnBnClickedTexure(UINT _iId)
{
	UINT iIdx = _iId - IDC_RADIO1;

	for (size_t i = 0; i < 4; i++)
	{
		if(i != iIdx)
			m_radioTex[i].SetCheck(0);
	}

	m_pTargetObj->GetTerrain()->SetSelectWeight(iIdx);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CWBrushDlg::OnBnClickedTextureLoad(UINT _iId)
{
	if (m_pTargetObj == nullptr)
		return;
	wstring strPath = CPathMgr::GetInst()->GetPath();
	strPath += L"Texture\\Tile\\";
	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"tga", L"*.*",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	wstring wcsPath = dlg.GetPathName();
	wstring wcsKey = dlg.GetFileName();

	UINT iIdx = _iId - ID_BUTTON1;

	m_editName[iIdx].SetWindowTextW(wcsKey.c_str());
	
	SAFE_DELETE(m_pTexBrush[iIdx]);
	m_pTexBrush[iIdx] = nullptr;
	m_pTexBrush[iIdx] = CTexture::CreateTexture(wcsPath,D3D11_BIND_SHADER_RESOURCE,DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pTargetObj->GetTerrain()->SetWeightTexture(iIdx, m_pTexBrush[iIdx]);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL CWBrushDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_radioTex[0].SetCheck(1);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}