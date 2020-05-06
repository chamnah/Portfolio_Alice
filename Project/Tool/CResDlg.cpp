// CResDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CResDlg.h"
#include "afxdialogex.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "MainFrm.h"
#include "CGameObj.h"
#include "CSceneMgr.h"
#include "CMeshRender.h"
#include "CCollider.h"
#include "CTransform.h"
#include "KeyMgr.h"
#include "CTexture.h"
#include "CLayer.h"
#include "CTestMgr.h"
#include "CPrefab.h"
#include "CAnimator2D.h"
#include "CHierarchyDlg.h"
#include "CHierachyView.h"
#include "CAnimView.h"
#include "CEventMgr.h"

// CResDlg 대화 상자

IMPLEMENT_DYNAMIC(CResDlg, CDialogEx)

CResDlg::CResDlg(CWnd* pParent /*=nullptr*/)
	: CTreeDlg(IDD_RESDLG, pParent)
	, m_bDrag(false)
{
}

CResDlg::~CResDlg()
{
}

void CResDlg::Init()
{
	DataLoad(RESOURCE_TYPE::TEXTURE, wstring(L"Baysign"));
	DataLoad(RESOURCE_TYPE::TEXTURE, wstring(L"Terrain"));
	DataLoad(RESOURCE_TYPE::PREFAB, wstring(L"Prefab"));
}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);
}

void CResDlg::DataLoad(RESOURCE_TYPE _eType, wstring & _Path)
{
	wstring strResPath = CPathMgr::GetInst()->GetPath();
	wstring strExtention = L"";
	HTREEITEM pParentItem = 0;
	WIN32_FIND_DATA tData = {};

	if (_eType == RESOURCE_TYPE::TEXTURE)
	{
		strResPath += L"Texture\\Object\\";
		strExtention = L"\\*.png";
		pParentItem = AddItem(_Path.c_str(), NULL, nullptr);
	}
	else
	{
		strExtention = L"\\*.prefab";
		pParentItem = AddItem(L"Prefab", NULL, nullptr);
	}
	HANDLE handle = FindFirstFile(wstring(strResPath + _Path + strExtention).c_str(), &tData);

	strResPath += _Path + L"\\";

	while (true)
	{
		if (!FindNextFile(handle, &tData))
			return;

		switch (_eType)
		{

		case RESOURCE_TYPE::MESH:
			break;

		case RESOURCE_TYPE::TEXTURE:
		{
			wstring strTexture = strResPath + tData.cFileName;
			strTexture =  CPathMgr::GetInst()->AbsoluteToRelativeResPath(strTexture);

			CResMgr::GetInst()->AddResource<CTexture>(tData.cFileName, CTexture::CreateTexture(strTexture));
			
			CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(tData.cFileName);
			
			HTREEITEM pTextureItem = AddItem(tData.cFileName, (DWORD_PTR)pTex, pParentItem);
		}
		break;
		case RESOURCE_TYPE::MATERIAL:
			break;
		case RESOURCE_TYPE::PREFAB:
		{
			map<wstring,CRes*>& mapPrefab = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::PREFAB);
			for (auto&prefab : mapPrefab)
			{
				AddItem(prefab.first.c_str(), (DWORD_PTR)prefab.second, pParentItem);
			}
		}
			break;
		case RESOURCE_TYPE::END:
			break;
		default:
			break;
		}
	}
}

void CResDlg::CreateNewObject()
{
	CGameObj* pGrid = CTestMgr::GetInst()->GetTargetObj();
	Vec3 vMousePos;

	if (pGrid == nullptr)
		vMousePos = CKeyMgr::GetInst()->GetDXMousePos();
	else
		vMousePos = pGrid->GetTransForm()->GetPos();

	vMousePos.x -= g_World.mtxView._41;
	vMousePos.y -= g_World.mtxView._42;

	vMousePos.z = 1.f;

	CString strParent = m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(m_Drag));
	CGameObj* pNewObject = nullptr;
	CTexture* pData = nullptr;
	if (strParent == L"Terrain" || strParent == L"Baysign")
	{
		pData = (CTexture*)m_ctrlTree.GetItemData(m_Drag);
		pNewObject = new CGameObj;
		pNewObject->SetName(pData->GetName());
		pNewObject->AddComponent(new CMeshRender);
		pNewObject->AddComponent(new CCollider);
		
		pNewObject->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
		pNewObject->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"),0);
		pNewObject->GetMeshRender()->SetTexture(pData);

		pNewObject->GetTransForm()->SetScale(Vec3((float)pData->GetWidth(), (float)pData->GetHeight(), 1));
		pNewObject->GetCollider()->SetScale(Vec3((float)pData->GetWidth(), (float)pData->GetHeight(), 0.f));
		pNewObject->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_BOX);
	}

	if (strParent == L"Terrain")
	{
		vMousePos.z = 9999.f;
		pNewObject->GetTransForm()->SetPos(vMousePos);
		CSceneMgr::GetInst()->AddObject(L"Back", pNewObject);
		((CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::HIERACHY))->GetHirerarchy()->Reset();
		return;
	}
	else if (strParent == L"Baysign")
	{
		vMousePos.z = 0.f;
		pNewObject->GetTransForm()->SetPos(vMousePos);
	}
	else if (strParent == L"Prefab")
	{
		CPrefab* pData = (CPrefab*)m_ctrlTree.GetItemData(m_Drag);
		pNewObject = pData->Instantiate();
		pNewObject->SetName(m_ctrlTree.GetItemText(m_Drag).GetBuffer());
		vMousePos.z = pNewObject->GetTransForm()->GetPos().z;
		pNewObject->GetTransForm()->SetPos(vMousePos);
		pNewObject->GetMeshRender()->SetMtrl(pNewObject->GetMeshRender()->GetMtrl()->Clone());
		pNewObject->Awake();
	}
	CSceneMgr::GetInst()->AddObject(L"Default", pNewObject);
	((CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::HIERACHY))->GetHirerarchy()->Reset();
}

void CResDlg::Reset()
{
	HTREEITEM hRoot = m_ctrlTree.GetNextSiblingItem(m_ctrlTree.GetRootItem());
	hRoot = m_ctrlTree.GetNextSiblingItem(hRoot);
	HTREEITEM hSibling;
	HTREEITEM hDelete;
	wstring strName;

	hDelete = m_ctrlTree.GetChildItem(hRoot);
	hSibling = m_ctrlTree.GetNextSiblingItem(hDelete);
	while (true)
	{
		if (hDelete == NULL)
			break;
		m_ctrlTree.DeleteItem(hDelete);
		hDelete = hSibling;
		hSibling = m_ctrlTree.GetNextSiblingItem(hDelete);
	}

	map<wstring,CRes*>& mapPrefab = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::PREFAB);

	for (auto& prefab : mapPrefab)
	{
		AddItem(prefab.first.c_str(),(DWORD_PTR)prefab.second, hRoot);
	}
}

BEGIN_MESSAGE_MAP(CResDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CResDlg::OnDBClickTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CResDlg::OnBeginDragTree)
	ON_WM_LBUTTONUP()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CResDlg::OnTvnSelchangedTree1)
	ON_COMMAND(ID_AFAF_SSS, &CResDlg::OnResourceDelete)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_RESOURCE_32790, &CResDlg::OnCreateObject)
END_MESSAGE_MAP()

// CResDlg 메시지 처리기

void CResDlg::OnDBClickTree(NMHDR *pNMHDR, LRESULT *pResult) // 다른걸 더블 클릭햇을 경우 이미지 띄우기
{
	Reset();

	HTREEITEM item = m_ctrlTree.GetSelectedItem();

	CString strParent = m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(item));

	if (strParent == L"Prefab" || strParent == L"")
		return;

	if (strParent == L"Baysign" || strParent == L"Terrain")
	{
		CTexture* pObject = (CTexture*)m_ctrlTree.GetItemData(item);
		CString strName = pObject->GetName().c_str();

		CTexture* pTex = CResMgr::GetInst()->Load<CTexture>((wstring)strName);

		CString ResPath = CPathMgr::GetInst()->GetPath();
		ResPath += L"Texture\\Object\\" + strParent + L"\\";
		ResPath += strName;

		CRect rt;
		GetClientRect(rt);

		CClientDC dc(this);
		CImage Image;
		Image.Load(ResPath);
		Image.StretchBlt(dc.m_hDC, int(((float)rt.Width()) * 0.5f - 50), int(((float)rt.Height())*0.7), 100, 100, 0, 0, pTex->GetWidth(), pTex->GetHeight());
	}
	*pResult = 0;
}


void CResDlg::OnBeginDragTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_bDrag = true;
	CTestMgr::GetInst()->SetStop(true); // 이미 배치되어 있는 객체가 이동되지 않도록 멈춤
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_Drag = pNMTreeView->itemNew.hItem;
	SetCapture();
	*pResult = 0;
}


void CResDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	if (m_bDrag)
		m_bDrag = false;
	else
		return;
	CTestMgr::GetInst()->SetStop(false);
	ReleaseCapture();

	CRect rt;
	((CMainFrame*)AfxGetMainWnd())->GetGameView()->GetClientRect(rt);
	POINT pt;
	GetCursorPos(&pt);
	((CMainFrame*)AfxGetMainWnd())->GetGameView()->ScreenToClient(&pt);

	if (nullptr == m_Drag)
		return;
	if (((CTexture*)m_ctrlTree.GetItemData(m_Drag)) == nullptr)
		return;

	if (rt.Width() < pt.x || rt.Height() < pt.y || 0 > pt.x || 0 > pt.y)
	{
		m_Drag = nullptr;
		return;
	}

	wchar_t szText[255] = {};
	TV_ITEM item = {};
	item.hItem = m_Drag;
	item.pszText = szText;
	item.cchTextMax = 255;
	item.mask = TVIF_TEXT | TVIF_IMAGE;

	m_ctrlTree.GetItem(&item);

	CreateNewObject();

	m_Drag = nullptr;

	CTreeDlg::OnLButtonUp(nFlags, point);
}

void CResDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult) // 다른걸 선택햇을 경우 이미지 띄우기
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM item = pNMTreeView->itemNew.hItem;

	//HTREEITEM item = m_ctrlTree.GetSelectedItem();

	CString strParent = m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(item));

	if (strParent == L"Prefab" || strParent == L"")
		return;

	if (strParent == L"Baysign" || strParent == L"Terrain")
	{
		CTexture* pObject = (CTexture*)m_ctrlTree.GetItemData(item);
		CString strName = pObject->GetName().c_str();

		CTexture* pTex = CResMgr::GetInst()->Load<CTexture>((wstring)strName);

		CString ResPath = CPathMgr::GetInst()->GetPath();
		ResPath += L"Texture\\Object\\" + strParent + L"\\";
		ResPath += strName;

		CRect rt;
		GetClientRect(rt);

		CClientDC dc(this);
		CImage Image;
		Image.Load(ResPath);
		Image.StretchBlt(dc.m_hDC, int(((float)rt.Width()) * 0.5f - 50), int(((float)rt.Height())*0.7), 100, 100, 0, 0, pTex->GetWidth(), pTex->GetHeight());
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CResDlg::OnResourceDelete()
{
	
	HTREEITEM hSel = m_ctrlTree.GetSelectedItem();
	CString strText = m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(hSel));
	if (strText == L"Prefab")
	{
		strText = m_ctrlTree.GetItemText(hSel);
		CResMgr::GetInst()->DeleteResource<CPrefab>(strText.GetBuffer());
		m_ctrlTree.DeleteItem(hSel);
	}
	//CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(hSel);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CResDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_OBJ_CTRL);
	CMenu* pMenu = menu.GetSubMenu(1); // 최상위 메뉴에서 0부터 시작해서 4가 표시할 메뉴
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CResDlg::OnCreateObject()
{
	HTREEITEM hSel = m_ctrlTree.GetSelectedItem();
	CString strText = m_ctrlTree.GetItemText(m_ctrlTree.GetParentItem(hSel));
	if (strText == L"Prefab")
	{
		strText = m_ctrlTree.GetItemText(hSel);
		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(strText.GetBuffer());
		CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::CREATE_OBJ,0,(INT_PTR)pPrefab->Instantiate()});
	}
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}