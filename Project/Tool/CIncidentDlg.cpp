// CEventDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CIncidentDlg.h"
#include "afxdialogex.h"
#include "CTestMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CIncident.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CCore.h"
#include "CTile.h"
#include "CLayer.h"
#include "CGrid.h"
#include "CCollider.h"

// CEventDlg 대화 상자

IMPLEMENT_DYNAMIC(CIncidentDlg, CDialogEx)

CIncidentDlg::CIncidentDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_EVENT_DLG, pParent)
{
	m_eMode = EVENT_MODE::NONE;
}

CIncidentDlg::~CIncidentDlg()
{
}

void CIncidentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbKinds);
	DDX_Control(pDX, IDC_LIST2, m_listObjKinds);
	DDX_Control(pDX, IDC_LIST1, m_listEvent);
}

void CIncidentDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
	if (m_cbFocus)
		return;
	m_listEvent.ResetContent();

	//Vec3 vMousePos = CKeyMgr::GetInst()->GetDXMousePos();

	if (m_eMode == EVENT_MODE::WALL)
	{
		if (KEYTAB(KEY_TYPE::KEY_LBTN))
		{
			POINT ptIdx = {};

			/*CGameObj* pTile = CTileMgrScript::GetInst()->GetPickingTile(vMousePos, ptIdx);
			if (pTile == nullptr)
				return;
			pTile->GetTile()->SetCollType(COLL_TYPE::COLL);
			int i = 1;
			pTile->GetMeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);*/
			m_vecTilePos.push_back(ptIdx);
		}
		else if (KEYTAB(KEY_TYPE::KEY_RBTN))
		{
			if (m_vecTilePos.empty())
				return;

			POINT ptIdx = m_vecTilePos.back();
			vector<CGameObj*>& pGrid = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();
			CGameObj* pTile = pGrid[0]->GetGrid()->GetVecObj()[ptIdx.y][ptIdx.x];
			pTile->GetTile()->SetCollType(COLL_TYPE::NOCOLL);
			int i = 0;
			pTile->GetMeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);
			m_vecTilePos.pop_back();
		}
	}

	vector<EventObj>& vecObj = m_pTargetObj->GetIncident()->GetVecObj();

	UINT i = 0;
	for(; i < vecObj.size(); ++i)
		m_listEvent.InsertString(i , vecObj[i].wcsName.c_str());

	m_listEvent.InsertString(i, m_pTargetObj->GetIncident()->GetScene().c_str());
}

BEGIN_MESSAGE_MAP(CIncidentDlg, CDialogEx)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CIncidentDlg::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON6, &CIncidentDlg::OnBnClickedObjPlace)
	ON_BN_CLICKED(IDC_BUTTON1, &CIncidentDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON7, &CIncidentDlg::OnBnClickedPopBack)
	ON_BN_CLICKED(IDC_BUTTON3, &CIncidentDlg::OnBnClickedDelete)
END_MESSAGE_MAP()

// CEventDlg 메시지 처리기
BOOL CIncidentDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_cbKinds.InsertString(0,L"Monster");
	m_cbKinds.InsertString(1, L"Wall");
	m_cbKinds.InsertString(2, L"Scene");

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CIncidentDlg::OnCbnSelchangeCombo()
{    
	m_listObjKinds.ResetContent();

	int iSel = m_cbKinds.GetCurSel();

	if (iSel == 0)
	{
		m_listObjKinds.InsertString(0, L"Grunt_Mini");
		m_listObjKinds.InsertString(1, L"Grunt");
	}
	else if (iSel == 1)
	{
		m_listObjKinds.InsertString(0, L"Wall");
	}
	else if (iSel == 2)
	{
		m_listObjKinds.InsertString(0, L"FirstScene");
	}
}

void CIncidentDlg::OnBnClickedObjPlace()
{
	int icbSel = m_cbKinds.GetCurSel();
	int iSel = m_listObjKinds.GetCurSel();

	if (iSel == LB_ERR)
		return;

	if (m_pTargetObj == nullptr)
		return;

	CString strObj;
	m_listObjKinds.GetText(iSel, strObj);

	if (icbSel == 0)
	{
		m_eMode = EVENT_MODE::MONSTER;
		CGameObj* pObj = CResMgr::GetInst()->Load<CPrefab>(strObj.GetBuffer())->CreateObjClone(L"Default");
		m_vecObj.push_back(pObj);
	}
	else if (icbSel == 1)
		m_eMode = EVENT_MODE::WALL;
	else if (icbSel == 2)
	{
		m_eMode = EVENT_MODE::SCENE;
		m_pTargetObj->GetIncident()->SetScene(strObj.GetBuffer());
	}
}

void CIncidentDlg::OnBnClickedApply() // 다음으로 할일 이제 타일 좌표를 이벤트 컴포넌트에게 전달 해줘야 한다.0
{
	for (size_t i = 0; i < m_vecObj.size(); i++)
		m_pTargetObj->GetIncident()->AddObj(m_vecObj[i]);

	for (size_t i = 0; i < m_vecTilePos.size(); i++)
		m_pTargetObj->GetIncident()->AddTile(m_vecTilePos[i]);
	
	ClearObj();
	m_eMode = EVENT_MODE::NONE;
}

void CIncidentDlg::ClearObj()
{
	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecObj[i] ,0 };
		CEventMgr::GetInst()->AddEvent(event);
	}
	m_vecObj.clear();

	for (size_t i = 0; i < m_vecTilePos.size(); i++)
	{
		vector<CGameObj*>& pGrid = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();
		CGameObj* pTile = pGrid[0]->GetGrid()->GetVecObj()[m_vecTilePos[i].y][m_vecTilePos[i].x];
		pTile->GetTile()->SetCollType(COLL_TYPE::NOCOLL);
		int iType = 0;
		pTile->GetMeshRender()->GetMtrl()->SetParam(&iType, SHADER_PARAM::INT_0);
	}

	m_vecTilePos.clear();
}

void CIncidentDlg::OnBnClickedPopBack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_vecObj.empty())
		return;
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecObj.back() ,0 });
	m_vecObj.pop_back();
}

void CIncidentDlg::OnBnClickedDelete()
{
	m_listEvent.ResetContent();

	m_pTargetObj->GetIncident()->GetVecObj().clear();
	m_pTargetObj->GetIncident()->SetScene(L"");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}