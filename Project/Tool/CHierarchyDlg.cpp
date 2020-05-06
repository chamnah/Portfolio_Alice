// CHierarchyDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CHierarchyDlg.h"
#include "CHierachyView.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "MainFrm.h"
#include "CComView.h"
#include "CEventMgr.h"
#include "CAnimView.h"
#include "CPathMgr.h"
#include "CSaveLoadMgr.h"
#include "CResMgr.h"
#include "CResDlg.h"
#include "CTestMgr.h"
#include "CMeshRender.h"
#include "CMaterial.h"
// CHierarchyDlg 대화 상자

IMPLEMENT_DYNAMIC(CHierarchyDlg, CDialogEx)
struct tStack
{
	CGameObj* pObj;
	HTREEITEM hParent;
};

CHierarchyDlg::CHierarchyDlg(CWnd* pParent /*=nullptr*/)
	: CTreeDlg(IDD_HIERARCHY, pParent)
{
}

CHierarchyDlg::~CHierarchyDlg()
{
}

void CHierarchyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);
}


BEGIN_MESSAGE_MAP(CHierarchyDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OBJECT_ADD, &CHierarchyDlg::ClickedAddObject)
	ON_COMMAND(ID_OBJECT_32773, &CHierarchyDlg::OnObjectDelete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CHierarchyDlg::OnTvnSelchangedTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CHierarchyDlg::OnTvnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CHierarchyDlg::OnTvnEndlabeleditTree1)
	ON_COMMAND(ID_OBJECT_PREFAB, &CHierarchyDlg::OnObjectPrefab)
	ON_COMMAND(ID_HIERACHY_32791, &CHierarchyDlg::OnObjCopy)
END_MESSAGE_MAP()

void CHierarchyDlg::PostNcDestroy()
{
	delete this;

	CDialogEx::PostNcDestroy();
}		
	
int CHierarchyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CHierarchyDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenuW(IDR_OBJ_CTRL);
	CMenu* pMenu = menu.GetSubMenu(0); // 최상위 메뉴에서 0부터 시작해서 4가 표시할 메뉴
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CHierarchyDlg::ClickedAddObject()
{
	CGameObj* pObj = new CGameObj;
	pObj->SetName(L"GameObject");
	CSceneMgr::GetInst()->AddObject(L"Default",pObj);
	AddItem(L"GameObject",DWORD_PTR(pObj),nullptr);
}

void CHierarchyDlg::OnObjectDelete()
{
	HTREEITEM hSel = m_ctrlTree.GetSelectedItem();
	CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(hSel);
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ, (INT_PTR)pObj, 0 });
	m_ctrlTree.DeleteItem(hSel);
}

void CHierarchyDlg::Init()
{
	CLayer* pLayer = nullptr;

	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
	{
		pLayer = CSceneMgr::GetInst()->GetLayer(i);
		
		if (pLayer == nullptr)
			continue;

		vector<CGameObj*>& vecObj = pLayer->GetGameObj();
		for (UINT i = 0; i < vecObj.size(); ++i)
		{
			AddGameObject(vecObj[i]->GetName().c_str(),((DWORD_PTR)vecObj[i]));
		}
	}

}

void CHierarchyDlg::Reset()
{
	m_ctrlTree.DeleteAllItems();

	m_Drag = nullptr;
	Init();
}

void CHierarchyDlg::AddGameObject(CString _strText, DWORD_PTR _pItem, HTREEITEM _Parent)
{
	HTREEITEM item;

	tStack stack = { ((CGameObj*)_pItem),_Parent};
	vector<tStack> vecStack;

	vecStack.push_back(stack);

	while (true)
	{
		stack = vecStack.back();
		vector<CGameObj*>& vecChild = stack.pObj->GetChild();
		item = AddItem(stack.pObj->GetName().c_str(), ((DWORD_PTR)stack.pObj), stack.hParent);
		
		vecStack.pop_back();

		for (int i = int(vecChild.size() - 1); i >= 0; --i)
		{
			tStack stack = { vecChild[i],item };
			vecStack.push_back(stack);
		}

		if (vecStack.size() <= 0)
			break;
	}
}

void CHierarchyDlg::MoveItem(HTREEITEM _Drag, HTREEITEM _Target)
{
	wchar_t wcsText[100] = {};

	TVITEM item{};
	item.hItem = _Drag;
	item.mask = TVIF_TEXT;
	item.pszText = wcsText;
	item.cchTextMax = 100;

	m_ctrlTree.GetItem(&item);
	DWORD_PTR data = m_ctrlTree.GetItemData(item.hItem);

	HTREEITEM TargetItem = AddItem(wcsText, data, _Target);

	vector<HTREEITEM> vecChild;
	GetChildItem(vecChild, _Drag);
	
	for (UINT i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], TargetItem);
	}

	if (item.state & TVIS_EXPANDED)
		m_ctrlTree.Expand(TargetItem, TVE_EXPAND);

	// 목적이 아이템도 펼쳐놓는다.
	m_ctrlTree.Expand(_Target, TVE_EXPAND);

	m_ctrlTree.DeleteItem(_Drag);

	m_ctrlTree.SelectItem(TargetItem);
}

void CHierarchyDlg::GetChildItem(vector<HTREEITEM>& _vecChild, HTREEITEM _item)
{
	if (!m_ctrlTree.ItemHasChildren(_item))
		return;

	HTREEITEM child = m_ctrlTree.GetChildItem(_item);

	while (child)
	{
		_vecChild.push_back(child);
		child = m_ctrlTree.GetChildItem(child);
	}
}

void CHierarchyDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(pNMTreeView->itemNew.hItem);

	((CComView*)(((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::COMPONENT)))->SetTargetObj(pObj);
	((CComView*)(((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::COMPONENT)))->Reset();
	CTestMgr::GetInst()->SetTargetObj(pObj);
	((CAnimView*)(((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::ANIMATION)))->SetTargetObj(pObj);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CHierarchyDlg::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	m_Drag = pNMTreeView->itemNew.hItem;

	SetCapture();

	*pResult = 0;
}

void CHierarchyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CGameObj* TargetObj = nullptr;
	CGameObj* DragObj = nullptr;

	m_ctrlTree.SelectDropTarget(nullptr);
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
	
	if (m_Drag != nullptr && m_Target != m_Drag)
	{
		DragObj = (CGameObj*)m_ctrlTree.GetItemData(m_Drag);
		if(m_Target != nullptr)
			TargetObj = (CGameObj*)m_ctrlTree.GetItemData(m_Target);

		if (m_Target == nullptr)
		{
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DISCONNECT_PARENT,(INT_PTR)DragObj,0 });
			MoveItem(m_Drag, m_Target);
		}
		else if (!TargetObj->IsFamily(DragObj))
		{
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD,(INT_PTR)TargetObj,(INT_PTR)DragObj });
			MoveItem(m_Drag, m_Target);
		}
		else
			int i = 0;
	}
	m_Drag = nullptr;
	m_Target = nullptr;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CHierarchyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_Drag == nullptr)
		return;

	::ClientToScreen(m_hWnd, &point);
	m_ctrlTree.ScreenToClient(&point);
	m_Target = m_ctrlTree.HitTest(point);

	if (m_Target == m_Drag)
		m_Target = nullptr;

	if (m_Target != nullptr)
		m_ctrlTree.SelectDropTarget(m_Target);


	CDialogEx::OnMouseMove(nFlags, point);
}

void CHierarchyDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CHierarchyDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	HTREEITEM select = m_ctrlTree.GetSelectedItem();
	m_ctrlTree.EditLabel(select);
}

void CHierarchyDlg::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CEdit *pEdit = m_ctrlTree.GetEditControl();
	if (pEdit)
	{
		CString str;
		pEdit->GetWindowText(str);
		if (str.GetLength() > 0)
		{
			m_ctrlTree.SetItemText(pTVDispInfo->item.hItem, str);
			
			CGameObj* pObj = ((CGameObj*)m_ctrlTree.GetItemData(pTVDispInfo->item.hItem));
			pObj->SetName(str.GetBuffer());
		}
	}
	*pResult = 0;
}

void CHierarchyDlg::OnObjectPrefab()
{
	HTREEITEM hSel = m_ctrlTree.GetSelectedItem();
	CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(hSel);

	CResMgr::GetInst()->ChangeResource<CPrefab>(pObj->GetName().c_str(), new CPrefab(pObj->Clone()));
	CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)pObj,0});
}

void CHierarchyDlg::OnObjCopy()
{
	HTREEITEM hSel = m_ctrlTree.GetSelectedItem();
	CGameObj* pObj = (CGameObj*)m_ctrlTree.GetItemData(hSel);
	pObj = pObj->Clone();
	if(pObj->GetMeshRender() != nullptr)
	pObj->GetMeshRender()->SetMtrl(pObj->GetMeshRender()->GetMtrl()->Clone());
	CSceneMgr::GetInst()->AddObject(pObj->GetLayerName(), pObj);
	AddItem(pObj->GetName().c_str(), DWORD_PTR(pObj), nullptr);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}