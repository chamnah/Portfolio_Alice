// CAddComponentDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CAddComponentDlg.h"
#include "afxdialogex.h"
#include "CDlgList.h"
#include "CGameObj.h"
#include "CTransForm.h"
#include "CMeshRender.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CScript.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CGrid.h"
#include "CIncident.h"
#include "CLight.h"
#include "CTerrain.h"
#include "MainFrm.h"
#include "CComView.h"
#include "CText.h"

// CAddComponentDlg 대화 상자

IMPLEMENT_DYNAMIC(CAddComponentDlg, CDialogEx)

CAddComponentDlg::CAddComponentDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_ADDCOMPONENT, pParent)
{
}

CAddComponentDlg::~CAddComponentDlg()
{
}

void CAddComponentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddComponentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddComponentDlg::OnBnAddComponent)
	ON_BN_DOUBLECLICKED(IDC_BUTTON1, &CAddComponentDlg::OnBnDoubleclickedButton1)
END_MESSAGE_MAP()

// CAddComponentDlg 메시지 처리기

void CAddComponentDlg::OnBnAddComponent()
{
	if (m_pTargetObj == nullptr)
		return;

	CDlgList List;

	List.SetCaption(L"AddComponent");

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		switch (COMPONENT_TYPE(i))
		{
		case COMPONENT_TYPE::TRANSFORM:
			List.AddData(L"Transform");
			break;
		case COMPONENT_TYPE::MESH_RENDER:
			List.AddData(L"MeshRender");
			break;
		case COMPONENT_TYPE::CAMERA:
			List.AddData(L"Camera");
			break;
		case COMPONENT_TYPE::COLLIDER:
			List.AddData(L"Collider");
			break;
		case COMPONENT_TYPE::ANIMATION_2D:
			List.AddData(L"Animation2D");
			break;
		case COMPONENT_TYPE::ANIMATION_3D:
			List.AddData(L"Animation3D");
			break;
		case COMPONENT_TYPE::GRID:
			List.AddData(L"Grid");
			break;
		case COMPONENT_TYPE::INCIDENT:
			List.AddData(L"Incident");
			break;
		case COMPONENT_TYPE::LIGHT:
			List.AddData(L"Light");
			break;
		case COMPONENT_TYPE::FILTER:
			List.AddData(L"Filter");
			break;
		case COMPONENT_TYPE::TERRAIN:
			List.AddData(L"Terrain");
			break;
		case COMPONENT_TYPE::TILE:
			List.AddData(L"Tile");
			break;
		case COMPONENT_TYPE::TEXT:
			List.AddData(L"Text");
			break;
		}

	}

	List.AddData(L"Script");
	INT_PTR id =List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();
		UINT iType = List.GetIndex();
		CComponent* pCom = nullptr;

		switch (COMPONENT_TYPE(iType))
		{
		case COMPONENT_TYPE::TRANSFORM:
			pCom = new CTransForm;
			break;
		case COMPONENT_TYPE::MESH_RENDER:
			pCom = new CMeshRender;
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER:
			pCom = new CCollider;
			break;
		case COMPONENT_TYPE::ANIMATION_2D:
			pCom = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATION_3D:
			pCom = new CAnimator3D;
			break;
		case COMPONENT_TYPE::GRID:
			pCom = new CGrid;
			break;
		case COMPONENT_TYPE::INCIDENT:
			pCom = new CIncident;
			break;
		case COMPONENT_TYPE::LIGHT:
			pCom = new CLight;
			break;
		case COMPONENT_TYPE::TERRAIN:
			pCom = new CTerrain;
			break;
		case COMPONENT_TYPE::TILE:
			return;
		case COMPONENT_TYPE::TEXT:
			pCom = new CText;
			break;

		}

		if (iType == (UINT)COMPONENT_TYPE::END)
		{
			iType = (UINT)COMPONENT_TYPE::SCRIPT;
			//pCom = new CScript;
			return;
		}
		bool bCom = m_pTargetObj->AddComponent(pCom,(COMPONENT_TYPE)iType);
		if (bCom)
		{
			pCom->Awake();
			((CComView*)(((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::COMPONENT)))->Reset();
		}
	}
}


void CAddComponentDlg::OnBnDoubleclickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = 0;
}

void CAddComponentDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
}