// CTerrainDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CTerrainDlg.h"
#include "afxdialogex.h"
#include "CDirect.h"
#include "CTerrain.h"
#include "CResMgr.h"
#include "CWBrushDlg.h"
#include "CHBrushDlg.h"

// CTerrainDlg 대화 상자

IMPLEMENT_DYNAMIC(CTerrainDlg, CDialogEx)

CTerrainDlg::CTerrainDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_TerrainDlg, pParent)
	, m_iOldIndex(INT_MAX)
	, m_iChangeIndex(INT_MAX)
	, m_bFirst(false)
	, m_bIng(false)
	, m_TexLoad(false)
	, m_pWBrush(nullptr)
	, m_pHBrush(nullptr)
{
}

CTerrainDlg::~CTerrainDlg()
{
}

void CTerrainDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
	m_bIng = true;

	if (m_pTargetObj == nullptr || m_pTargetObj->GetTerrain() == nullptr)
		return;

	if (m_bFocus)
		return;

	m_pWBrush->Update(m_pTargetObj);
	m_pHBrush->Update(m_pTargetObj);

	 Vec2 vRange = m_pTargetObj->GetTerrain()->GetTeselRange();

	 CString strEdit;
	 strEdit.Format(L"%f", vRange.x);
	 m_editMin.SetWindowTextW(strEdit.GetBuffer());

	 strEdit.Format(L"%f", vRange.y);
	 m_editMax.SetWindowTextW(strEdit.GetBuffer());

	 Vec2 vBrushSize = m_pTargetObj->GetTerrain()->GetBrushSize();
	 strEdit.Format(L"%f", vBrushSize.x);
	 m_editBrushSize.SetWindowTextW(strEdit.GetBuffer());

	if (m_iChangeIndex != m_iOldIndex)
	{
		if (m_bFirst)
			m_listBrush.SetItemText(m_iOldIndex,0, m_strOldName);

		m_bFirst = true;

		if (!m_TexLoad)
		{
			m_TexLoad = true;
			CResMgr::GetInst()->AddResource<CTexture>(L"Brush_01", CTexture::CreateTexture(L"..\\content\\Texture\\Brush_01.png"));
			CResMgr::GetInst()->AddResource<CTexture>(L"Brush_02", CTexture::CreateTexture(L"..\\content\\Texture\\Brush_02.png"));
		}

		m_pTargetObj->GetTerrain()->SetBrushTexture(CResMgr::GetInst()->Load<CTexture>(m_strName.GetBuffer()));

		m_iOldIndex = m_iChangeIndex;
		m_strOldName = m_strName;
		m_listBrush.SetItemText(m_iChangeIndex, 0, m_strName + L"(선택)");
	}

	m_bIng = false;
}

void CTerrainDlg::StringSelCut(CString& _strSel)
{
	for (int i = _strSel.GetLength() - 1; i >= 0; --i)
	{
		if (_strSel[i] == L'(')
		{
			_strSel.GetBuffer()[i] = L'\0';
			break;
		}
	}
}

void CTerrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBrush);
	DDX_Control(pDX, IDC_COMBO1, m_cbBrush);
	DDX_Control(pDX, IDC_EDIT1, m_editBrushSize);
	DDX_Control(pDX, IDC_TAB1, m_tabTerrain);
	DDX_Control(pDX, IDC_EDIT2, m_editMin);
	DDX_Control(pDX, IDC_EDIT3, m_editMax);
}

BEGIN_MESSAGE_MAP(CTerrainDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTerrainDlg::OnChangedList)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTerrainDlg::OnCbnSelchange)
	ON_EN_CHANGE(IDC_EDIT1, &CTerrainDlg::OnEnChangeBrushSize)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT2, IDC_EDIT3, &CTerrainDlg::OnEnChangeRange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTerrainDlg::OnTcnSelchangeTab)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditSetfocus)
END_MESSAGE_MAP()


// CTerrainDlg 메시지 처리기


BOOL CTerrainDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	CBitmap bit;
	CBitmap bit2;
	
	CPngImage png;
	bool bLoad = png.LoadFromFile(L"..\\content\\Texture\\Brush_01.png");
	//bit.Attach(png.Detach());
	m_imglist.Create(64, 64, ILC_COLOR32 | ILC_MASK, 2, 2);
	bit.LoadBitmapW(IDB_BITMAP2);
	m_imglist.Add(&bit,RGB(0,0,0));
	bit2.LoadBitmapW(IDB_BITMAP3);
	m_imglist.Add(&bit2, RGB(0, 0, 0));

	LVITEMW item = {};

	m_listBrush.InsertItem(0,L"Brush_01",0);
	m_listBrush.InsertItem(1, L"Brush_02", 1);
	m_listBrush.InsertItem(2, L"Brush_03", 1);
	m_listBrush.SetImageList(&m_imglist, LVSIL_NORMAL);

	POINT upPt = {};
	m_listBrush.GetItemPosition(0,&upPt);
	POINT pt = { upPt.x,upPt.y + 100};
	m_listBrush.SetItemPosition(2, pt);

	m_listBrush.GetSelectedCount();
	m_listBrush.SetBkColor(RGB(255, 255, 255));
	
	m_listBrush.SetParent(m_tabTerrain.GetWindow(NULL));
	//m_listBrush.SetIconSpacing(64, 92);

	RECT rt = {};
	m_tabTerrain.GetWindowRect(&rt);
	
	RECT rtlist = { rt.left - 18,rt.top - 41, rt.right - 22, rt.bottom - 60 };
	m_listBrush.MoveWindow(&rtlist);
	/*m_listBrush.SetWindowPos(nullptr, rt.left, rt.top, rt.right, rt.bottom,0);

	SCROLLINFO info;

	m_listBrush.GetScrollBarCtrl(0)->SetWindowPos(nullptr, rt.left, rt.top,20, rt.bottom,0);*/

	m_cbBrush.InsertString(0, L"Height");
	m_cbBrush.InsertString(1, L"Weight");

	m_cbBrush.SetCurSel(0);

	m_tabTerrain.InsertItem(0,L"BrushTex");
	m_tabTerrain.InsertItem(1, L"WeightTex");
	m_tabTerrain.InsertItem(2, L"HeightTex");
	
	m_pWBrush = new CWBrushDlg;
	m_pWBrush->Create(IDD_CWBRUSHDLG,&m_tabTerrain);
	m_pWBrush->MoveWindow(3, 25, rt.right - 55, rt.bottom - 180);
	m_pWBrush->ShowWindow(SW_HIDE);
	m_pWBrush->SetBackgroundColor(RGB(255, 255, 255));

	m_pHBrush = new CHBrushDlg;
	m_pHBrush->Create(IDD_HBRUSHDLG, &m_tabTerrain);
	m_pHBrush->MoveWindow(3, 25, rt.right - 55, rt.bottom - 180);
	m_pHBrush->ShowWindow(SW_HIDE);
	m_pHBrush->SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
}

//copyImg.Create(64,64,16);
	/*img.Load(L"..\\content\\Texture\\aa.png");

	img.StretchBlt(copyImg.GetDC(), 0, 0, 64, 64, SRCCOPY);
	bit.Attach(copyImg);*/

void CTerrainDlg::OnChangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bIng)
		return;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString strName = m_listBrush.GetItemText(pNMLV->iItem,0);

	StringSelCut(strName);
	m_strName = strName;
	m_iChangeIndex = pNMLV->iItem;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CTerrainDlg::OnCbnSelchange()
{
	if (m_bIng)
		return;

	m_pTargetObj->GetTerrain()->SetBrushType((BRUSH_TYPE)m_cbBrush.GetCurSel());

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTerrainDlg::OnEnChangeBrushSize()
{
	/*if (m_bIng)
		return;*/
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CComponentDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSize;
	m_editBrushSize.GetWindowTextW(strSize);
	float fSize = _wtof(strSize);

	m_pTargetObj->GetTerrain()->SetBrushSize(fSize);
}

void CTerrainDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bIng)
		return;

	int iTab = m_tabTerrain.GetCurSel();

	if (iTab == 0)
	{
		m_listBrush.ShowWindow(SW_SHOW);
		m_pWBrush->ShowWindow(SW_HIDE);
		m_pHBrush->ShowWindow(SW_HIDE);
	}
	else if(iTab == 1)
	{
		m_listBrush.ShowWindow(SW_HIDE);
		m_pWBrush->ShowWindow(SW_SHOW);
		m_pHBrush->ShowWindow(SW_HIDE);
	}
	else
	{
		m_listBrush.ShowWindow(SW_HIDE);
		m_pWBrush->ShowWindow(SW_HIDE);
		m_pHBrush->ShowWindow(SW_SHOW);
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CTerrainDlg::OnEnChangeRange(UINT _iId)
{
	CString strMin, strMax;
	Vec2 vRange;
	m_editMin.GetWindowTextW(strMin);
	m_editMax.GetWindowTextW(strMax);
	vRange.x = _wtof(strMin.GetBuffer());
	vRange.y = _wtof(strMax.GetBuffer());
	m_pTargetObj->GetTerrain()->SetTeselRange(vRange);
}