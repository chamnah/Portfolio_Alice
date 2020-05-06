
// MainFrm.cpp: CMainFrame 클래스의 구현
//
#include "stdafx.h"
#include "Tool.h"
#include "CPathMgr.h"
#include "MainFrm.h"
#include "CComponentView.h"
#include "CHierachyView.h"
#include "CGameView.h"
#include "CBtnView.h"
#include "CDbgView.h"
#include "CDockView.h"
#include "CAnimView.h"
#include "CTileSize.h"
#include<iostream>
#include<fstream>
#include<string>
#include "CSaveLoadMgr.h"
#include "CHierarchyDlg.h"
#include "CResDlg.h"
#include "CResMgr.h"
#include "CLayerDlg.h"
#include "TimeMgr.h"
#include "CFBXLoader.h"
#include "CSceneMgr.h"
#include "CGameObj.h"
#include "CCameraDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_ANIMATION, &CMainFrame::OnViewAnimation)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SAVE, &CMainFrame::OnSave)
	ON_COMMAND(ID_LOAD, &CMainFrame::OnLoad)
	ON_COMMAND(ID_TILE_CHAGESIZE, &CMainFrame::OnTileChagesize)
	ON_COMMAND(ID_SAVE_PREFAB, &CMainFrame::OnSavePrefab)
	ON_COMMAND(ID_LOAD_PREFAB, &CMainFrame::OnLoadPrefab)
	ON_COMMAND(ID_OBJECT_ADDFBXOBJ, &CMainFrame::OnObjectAddfbxobj)
	ON_COMMAND(ID_OBJECT_ADDMESHDATA, &CMainFrame::OnObjectAddmeshdata)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
	:m_View{}
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rt = { 0, 0, 1772,822 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_Splitter.CreateStatic(this, 2, 1);
	m_Splitter.SetBorder(1, 1);
	m_Splitter.SetColumnInfo(0, GAMEVIEW_X, GAMEVIEW_X); // 지정 된 열 정보를 설정 하려면 호출 합니다.(MSDN)
	
	//m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CHierarchyView), CSize(HIERACHYVIEW_X, HIERACHYVIEW_Y), nullptr);
	//m_Splitter.CreateView(0, 2, RUNTIME_CLASS(CComponentView), CSize(COMVIEW_X, COMVIEW_Y), nullptr);

	//m_SubSplitter.CreateStatic(&m_Splitter,3,1,WS_CHILD | WS_VISIBLE,m_Splitter.IdFromRowCol(0,0));

	//m_SubSplitter.SetBorder(1, 1);

	m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CBtnView), CSize(BTNVIEW_X, BTNVIEW_Y), nullptr);
	m_Splitter.CreateView(1, 0, RUNTIME_CLASS(CGameView), CSize(GAMEVIEW_X, GAMEVIEW_Y + DBGVIEW_Y), nullptr);
	//m_Splitter.CreateView(2, 0, RUNTIME_CLASS(CDbgView), CSize(DBGVIEW_X, DBGVIEW_Y), nullptr);

	EnableDocking(CBRS_ALIGN_ANY);

	m_View[(UINT)VIEW_TYPE::COMPONENT] = new CComView;
	m_View[(UINT)VIEW_TYPE::ANIMATION] = new CAnimView;
	m_View[(UINT)VIEW_TYPE::HIERACHY] = new CHierachyView;

	m_View[(UINT)VIEW_TYPE::HIERACHY]->Create(L"Hierachy", this, CRect(0, 0, COMVIEW_X, COMVIEW_Y), true, 1001, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);

	m_View[(UINT)VIEW_TYPE::COMPONENT]->Create(L"Component",this,CRect(0,0, COMVIEW_X, COMVIEW_Y),true, 1000, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI);

	m_View[(UINT)VIEW_TYPE::ANIMATION]->Create(L"Animation", this, CRect(0, 0, GAMEVIEW_X, int(((float)COMVIEW_Y) * 0.5f)), true, 1003, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI);
	
	m_View[(UINT)VIEW_TYPE::HIERACHY]->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(m_View[(UINT)VIEW_TYPE::HIERACHY]);

	m_View[(UINT)VIEW_TYPE::COMPONENT]->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(m_View[(UINT)VIEW_TYPE::COMPONENT]);

	CTabbedPane* pTab = nullptr;
	m_View[(UINT)VIEW_TYPE::COMPONENT]->AttachToTabWnd(m_View[(UINT)VIEW_TYPE::HIERACHY], DM_SHOW, TRUE);

	pTab = m_View[(UINT)VIEW_TYPE::COMPONENT]->CreateTabbedPane();

	pTab->m_bTabsAlwaysTop = true;

	m_View[(UINT)VIEW_TYPE::ANIMATION]->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(m_View[(UINT)VIEW_TYPE::ANIMATION]);

	OnApplicationLook(theApp.m_nAppLook);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	/*if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;*/

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::Init()
{
	//((CComponentView*)GetComponentView())->Init();
	
	m_View[(UINT)VIEW_TYPE::COMPONENT]->ShowPane(true, false, false);
	m_View[(UINT)VIEW_TYPE::HIERACHY]->ShowPane(true, false, false); // 무조건 보이게 하는 코드
	m_View[(UINT)VIEW_TYPE::ANIMATION]->ShowPane(true, false, false); // 무조건 보이게 하는 코드
	
	m_View[(UINT)VIEW_TYPE::COMPONENT]->Init();
	m_View[(UINT)VIEW_TYPE::HIERACHY]->Init();
	
	m_View[(UINT)VIEW_TYPE::ANIMATION]->Init();

	m_Splitter.GetPane(1, 0)->SetFocus();
}

void CMainFrame::Update()
{
	if (CSaveLoadMgr::GetLoading())
	{
		((CHierachyView*)m_View[(UINT)VIEW_TYPE::HIERACHY])->GetHirerarchy()->Reset();
		((CComView*)m_View[(UINT)VIEW_TYPE::COMPONENT])->GetLayerDlg()->Init();
		((CComView*)m_View[(UINT)VIEW_TYPE::COMPONENT])->GetCameraDlg()->Reset(nullptr);

		CSaveLoadMgr::SetLoading(false);
	}

	m_View[(UINT)VIEW_TYPE::COMPONENT]->Update();
	m_View[(UINT)VIEW_TYPE::HIERACHY]->Update();
	m_View[(UINT)VIEW_TYPE::ANIMATION]->Update();

	/*static float fAccTime = 0.f;
	fAccTime += CTimeMgr::GetInst()->DeltaTime();
	if (fAccTime > 0.2f)
	{
		std::cout << CTimeMgr::GetInst()->GetFPS() << endl;
		fAccTime = 0.f;
	}*/
}

void CMainFrame::OnViewAnimation()
{
	m_View[(UINT)VIEW_TYPE::ANIMATION]->ShowWindow(true);
}

void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();

	for (UINT i = 0; i < (UINT)VIEW_TYPE::END; ++i)
	{
		if (m_View[i] != nullptr)
			delete m_View[i];
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMainFrame::OnSave()
{
	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Scene\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"Scene",L"*.scene", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter,NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();

	CSaveLoadMgr::SaveScene(Path);
}

void CMainFrame::OnLoad()
{
	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Scene\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"Scene", L"*.scene", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();

	CSaveLoadMgr::LoadScene(Path);

	((CHierachyView*)(GetView(VIEW_TYPE::HIERACHY)))->GetHirerarchy()->Reset();
	((CHierachyView*)(GetView(VIEW_TYPE::HIERACHY)))->GetRes()->Reset();

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
}

void CMainFrame::OnTileChagesize()
{
	/*CTileSize dlg;
	dlg.DoModal();

	POINT ptSize = dlg.GetSize();

	for (int i = 0; i < ptSize.x; ++i)
	{
		for (int j = 0; j < ptSize.y; ++j)
		{

		}
	}*/
}

void CMainFrame::OnSavePrefab()
{
	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Prefab\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"Prefab", L"*.prefab", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, Path.c_str(),L"wb");

	CSaveLoadMgr::SaveResource(pFile);

	fclose(pFile);
}

void CMainFrame::OnLoadPrefab()
{
	wstring Path = CPathMgr::GetInst()->GetPath();
	Path += L"Prefab\\";

	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"Prefab", L"*.prefab", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = Path.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	Path = dlg.GetPathName();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile,Path.c_str(),L"rb");

	CSaveLoadMgr::LoadResource(pFile);
	fclose(pFile);

	((CHierachyView*)(GetView(VIEW_TYPE::HIERACHY)))->GetRes()->Reset();
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
}

void CMainFrame::OnObjectAddfbxobj()
{
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"fbx", L"*.fbx",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	wstring strContent = CPathMgr::GetInst()->GetPath();
	strContent += L"Mesh\\";

	dlg.m_ofn.lpstrInitialDir = strContent.c_str();
	if (dlg.DoModal() != IDOK)
		return;

	CString strPath = dlg.GetPathName();
	if (CResMgr::GetInst()->Load<CMeshData>(CPathMgr::GetInst()->GetFileName(strPath.GetBuffer())) == nullptr)
	{
		CMeshData* pData = CResMgr::GetInst()->LoadFBX(strPath.GetBuffer());
		pData->SaveMeshDataFile();
		CGameObj* pObj = pData->instantiate();
		pObj->SetName(L"GameObj");
		CSceneMgr::GetInst()->AddObject(L"Default", pObj);
	}
	
	((CHierachyView*)m_View[(UINT)VIEW_TYPE::HIERACHY])->GetHirerarchy()->Reset();
}

void CMainFrame::OnObjectAddmeshdata()
{
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"mdat", L"*.mdat",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	wstring strContent = CPathMgr::GetInst()->GetPath();
	strContent += L"Mesh\\MeshData\\";

	dlg.m_ofn.lpstrInitialDir = strContent.c_str();
	if (dlg.DoModal() != IDOK)
		return;

	CString strPath = dlg.GetPathName();
	CString strName = CPathMgr::GetInst()->GetFileName(strPath.GetBuffer());
	CMeshData* pData = CResMgr::GetInst()->Load<CMeshData>(strName.GetBuffer());
	if (pData == nullptr)
		pData = CResMgr::GetInst()->LoadMeshData(strPath.GetBuffer());


	CGameObj* pObj = pData->instantiate();
	pObj->SetName(strName.GetBuffer());
	CSceneMgr::GetInst()->AddObject(L"Default", pObj);

	((CHierachyView*)m_View[(UINT)VIEW_TYPE::HIERACHY])->GetHirerarchy()->Reset();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}