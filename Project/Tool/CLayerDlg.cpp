// CLayerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CLayerDlg.h"
#include "afxdialogex.h"
#include "CSceneMgr.h"
#include "CGameObj.h"
#include "MainFrm.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CSaveLoadMgr.h"

// CLayerDlg 대화 상자

IMPLEMENT_DYNAMIC(CLayerDlg, CDialogEx)

CLayerDlg::CLayerDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_LAYER, pParent)
	, m_iLayerID(0)
{
}

CLayerDlg::~CLayerDlg()
{
}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbLayer);
	DDX_Control(pDX, IDC_EDIT1, m_editLayer);
}

void CLayerDlg::Update(CGameObj * _pObj)
{
	if (CSceneMgr::GetInst()->GetCurScene() == nullptr)
		return;

	m_pTargetObj = _pObj;


	if (m_pTargetObj == nullptr)
		return;
	
	if (m_cbFocus)
		return;

	m_bClick = false;
	
	int	iSel = m_pTargetObj->GetID();
	m_cbLayer.SetCurSel(iSel);
	m_bClick = true;
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialogEx)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLayerDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLayerDlg::OnBnAddLayer)
	ON_BN_CLICKED(IDC_BUTTON5, &CLayerDlg::OnBnChangeLayer)
END_MESSAGE_MAP()


// CLayerDlg 메시지 처리기
BOOL CLayerDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLayerDlg::Init()
{
	m_cbLayer.ResetContent();
	UINT iCount = CSceneMgr::GetInst()->GetCurLayerCount();

	for (UINT i = 0; i < iCount; ++i)
	{
		wstring wcsName = CSceneMgr::GetInst()->GetLayerName(i);
		m_cbLayer.InsertString(i, wcsName.c_str());
	}
}

void CLayerDlg::OnCbnSelchangeCombo1()
{
	if (!m_bClick)
		return;

	if (m_pTargetObj == nullptr)
		return;

	m_iLayerID = m_cbLayer.GetCurSel();
	int iID  = m_pTargetObj->GetID();
	if (m_iLayerID == iID)
		return;


	if (m_pTargetObj->GetParent() == nullptr)
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CHANGE_LAYER,(INT_PTR)m_pTargetObj,(INT_PTR)m_iLayerID });
	else
		m_pTargetObj->SetID(m_iLayerID);
}

void CLayerDlg::OnBnAddLayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strText;
	m_editLayer.GetWindowTextW(strText);

	if (strText == L"")
		return;

	CSceneMgr::GetInst()->AddLayer(strText.GetBuffer());
	Init();
}

void CLayerDlg::OnBnChangeLayer()
{
	CString strText;
	m_editLayer.GetWindowTextW(strText);

	CString strSel;
	int iSel = m_cbLayer.GetCurSel();
	m_cbLayer.GetLBText(iSel, strSel);

	if (strText == L"")
		return;

	CSceneMgr::GetInst()->ChangeName(strSel.GetBuffer(), strText.GetBuffer());
	Init();
}