// CMeshRenderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CMeshRenderDlg.h"
#include "afxdialogex.h"
#include "CDlgList.h"
#include "CResMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CTexture.h"
#include "CRenderMgr.h"
#include "CBlendState.h"
#include "CDepthStencilState.h"

// CMeshRenderDlg 대화 상자

IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_MESHRENDER, pParent)
{
}

CMeshRenderDlg::~CMeshRenderDlg()
{
}

void CMeshRenderDlg::Update(CGameObj * _pObj)
{
	m_bClick = false;
	if (_pObj == nullptr)
		return;

	if (m_pTargetObj != _pObj)
		m_pTargetObj = _pObj;

	int iCheck = m_pTargetObj->GetMeshRender()->GetShadow();
	m_checkShadow.SetCheck(iCheck);

	//m_pTargetObj->GetMeshRender()->GetMtrl()->GetDepthStencil();
	//m_editDepth.SetWindowTextW();

	if (!m_cbFocus)
	{
		iCheck = (UINT)(m_pTargetObj->GetMeshRender()->GetRenderType());
		m_cbRenderType.SetCurSel(iCheck);
	}

	if (m_pTargetObj->GetMeshRender()->GetMesh() != nullptr)
	{
		CString strMesh = m_pTargetObj->GetMeshRender()->GetMesh()->GetName().c_str();
		m_EditMesh.SetWindowTextW(strMesh);
	}
	else
		m_EditMesh.SetWindowTextW(L"");

	int iSel = m_cbSubset.GetCurSel();
	if (iSel == -1)
	{
		m_EditMaterial.SetWindowTextW(L"");
		m_Tex1.SetWindowTextW(L"");
		m_Tex2.SetWindowTextW(L"");
		return;
	}
	CMaterial* pMtrl = m_pTargetObj->GetMeshRender()->GetMtrl(iSel);

	if (pMtrl != nullptr)
	{
		CString strMtrl = pMtrl->GetName().c_str();
		m_EditMaterial.SetWindowTextW(strMtrl);

		if (pMtrl->GetArrTexture()[0] != nullptr)
			m_Tex1.SetWindowTextW(pMtrl->GetArrTexture()[0]->GetName().c_str());
		else
			m_Tex1.SetWindowTextW(L"");
		if (pMtrl->GetArrTexture()[1] != nullptr)
			m_Tex2.SetWindowTextW(pMtrl->GetArrTexture()[1]->GetName().c_str());
		else
			m_Tex2.SetWindowTextW(L"");

		m_editRasterize.SetWindowTextW(CRenderMgr::GetInst()->GetRSName(pMtrl->GetRasterizeState()).c_str());
		m_editSampler.SetWindowTextW(CRenderMgr::GetInst()->GetSPName(pMtrl->GetSamplerState()).c_str());

		m_editDepth.SetWindowTextW(CRenderMgr::GetInst()->GetDepthStencilName(pMtrl->GetDepthStencil()).c_str());

		if (m_bFocus == false)
		{
			Vec4 vDiff = pMtrl->GetDiff();
			CString strDiff;
			for (size_t i = 0; i < 4; i++)
			{
				strDiff.Format(L"%f", vDiff[i]);
				m_editDiff[i].SetWindowTextW(strDiff);
			}

			Vec2 vUV = m_pTargetObj->GetMeshRender()->GetUV();

			CString strUV;
			strUV.Format(L"%f", vUV.x);
			m_editUV[0].SetWindowTextW(strUV);

			strUV.Format(L"%f", vUV.y);
			m_editUV[1].SetWindowTextW(strUV);
		}
	}
	else
		m_EditMaterial.SetWindowTextW(L"");


	if (m_pTargetObj->GetMeshRender()->GetMtrl(iSel) != nullptr && m_pTargetObj->GetMeshRender()->GetMtrl(iSel)->GetBlend() != nullptr)
	{
		CString strBlend = m_pTargetObj->GetMeshRender()->GetMtrl(m_cbSubset.GetCurSel())->GetBlend()->GetName().c_str();
		m_Blend.SetWindowTextW(strBlend);
	}
	else
		m_Blend.SetWindowTextW(L"NONE");

	m_bClick = true;
}

void CMeshRenderDlg::Init()
{
}

void CMeshRenderDlg::Reset(CGameObj * _pObj)
{
	m_cbSubset.ResetContent();
	vector<CMaterial*>& vecMtrl = _pObj->GetMeshRender()->GetVecMtrl();
	for (size_t i = 0; i < vecMtrl.size(); i++)
	{
		CString strSubset = L"Subset";
		CString strIndex;
		strIndex.Format(L"%d", i);
		strSubset += strIndex;
		m_cbSubset.InsertString(i, strSubset);
	}

	m_cbSubset.SetCurSel(0);


}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditMesh);
	DDX_Control(pDX, IDC_EDIT2, m_EditMaterial);
	DDX_Control(pDX, IDC_EDIT4, m_Blend);
	DDX_Control(pDX, IDC_EDIT3, m_Tex1);
	DDX_Control(pDX, IDC_EDIT6, m_Tex2);
	DDX_Control(pDX, IDC_COMBO2, m_cbSubset);
	DDX_Control(pDX, IDC_EDIT7, m_editRasterize);

	for (size_t i = 0; i < 4; i++)
	{
		DDX_Control(pDX, IDC_EDIT8 + i, m_editDiff[i]);
	}
	DDX_Control(pDX, IDC_CHECK2, m_checkShadow);
	DDX_Control(pDX, IDC_COMBO3, m_cbRenderType);
	DDX_Control(pDX, IDC_EDIT12, m_editUV[0]);
	DDX_Control(pDX, IDC_EDIT13, m_editUV[1]);
	DDX_Control(pDX, IDC_EDIT5, m_editDepth);
	DDX_Control(pDX, IDC_EDIT14, m_editSampler);
}

BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnBnClickedMesh)
	ON_BN_CLICKED(IDC_BUTTON5, &CMeshRenderDlg::OnBnClickedMaterial)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshRenderDlg::OnBnAddTexture)
	ON_BN_CLICKED(IDC_BUTTON9, &CMeshRenderDlg::OnBnClickedBlend)
	ON_BN_CLICKED(IDC_BUTTON10, &CMeshRenderDlg::OnBnClickedDepthStencil)
	ON_BN_CLICKED(IDC_BUTTON6, &CMeshRenderDlg::OnAddTexture2)
	ON_BN_CLICKED(IDC_BUTTON12, &CMeshRenderDlg::OnBnClickedButtonRasterize)

	ON_CBN_KILLFOCUS(IDC_COMBO3, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO3, &CComponentDlg::OnCbnSetfocus)

	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT8, IDC_EDIT13, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT8, IDC_EDIT13, &CComponentDlg::OnEditSetfocus)

	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT8, IDC_EDIT11, &CMeshRenderDlg::OnEnChangeDiffuse)
	ON_BN_CLICKED(IDC_CHECK2, &CMeshRenderDlg::OnBnCheckShadow)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CMeshRenderDlg::OnCbnSelchangeRenderType)
	ON_CONTROL_RANGE(EN_CHANGE,IDC_EDIT12,IDC_EDIT13, &CMeshRenderDlg::OnEnChangeUV)
	ON_BN_CLICKED(IDC_BUTTON14, &CMeshRenderDlg::OnBnClickedSampler)
END_MESSAGE_MAP()

/*wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"", L"",
		OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = L"D:\\TransistorExport(split)";

	if (dlg.DoModal() != IDOK)
		return;

	CString strPath = dlg.GetFolderPath();
	dlg.SetEditBoxText();
	strPath;*/

void CMeshRenderDlg::OnBnClickedMesh()
{
	CDlgList List;
	List.SetCaption(L"Mesh");

	map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::MESH);

	for (auto pair : mapRes)
	{
		List.AddData(pair.first.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();
		if(wcsSelect != L"")
		m_pTargetObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(wcsSelect));
	}
}

void CMeshRenderDlg::OnBnClickedMaterial()
{
	CDlgList List;

	List.SetCaption(L"Material");

	map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::MATERIAL);

	for (auto pair : mapRes)
	{
		List.AddData(pair.first.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();
		if (wcsSelect != L"")
		{
			int iSel = 0;
			if (m_cbSubset.GetCurSel() != -1)
				iSel = m_cbSubset.GetCurSel();
			m_pTargetObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(wcsSelect)->Clone(), iSel);
		}
	}

	m_cbSubset.ResetContent();
	vector<CMaterial*>& vecMtrl = m_pTargetObj->GetMeshRender()->GetVecMtrl();
	for (size_t i = 0; i < vecMtrl.size(); i++)
	{
		CString strSubset = L"Subset";
		CString strIndex;
		strIndex.Format(L"%d", i);
		strSubset += strIndex;
		m_cbSubset.InsertString(i, strSubset);
	}

	m_cbSubset.SetCurSel(0);
}

void CMeshRenderDlg::OnBnAddTexture()
{
	wstring strPath = CPathMgr::GetInst()->GetPath();

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"png", L"*.*",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	wstring wcsPath = dlg.GetPathName();
	wstring wcsKey = dlg.GetFileName();      

	CResMgr::GetInst()->AddResource<CTexture>(wcsKey, CTexture::CreateTexture(wcsPath.c_str()));
	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(wcsKey);
	m_pTargetObj->GetTransForm()->SetScale(Vec3((float)pTex->GetWidth(), (float)pTex->GetHeight(),1.f));
	m_pTargetObj->GetMeshRender()->SetParam(pTex,SHADER_PARAM::TEX_0,m_cbSubset.GetCurSel());
}

void CMeshRenderDlg::OnBnClickedBlend()
{
	CDlgList List;

	List.SetCaption(L"Blend");

	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		wstring strName = ((CBlendState*)CRenderMgr::GetInst()->GetBlend((BLEND_TYPE)i))->GetName();
		List.AddData(strName.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();

		if (wcsSelect != L"")
			m_pTargetObj->GetMeshRender()->GetMtrl(m_cbSubset.GetCurSel())->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsSelect));
	}
}

void CMeshRenderDlg::OnBnClickedDepthStencil()
{
	CDlgList List;

	List.SetCaption(L"DepthStencil");

	for (UINT i = 0; i < (UINT)DEPTH_STENCIL_TYPE::END; ++i)
	{
		wstring strName = ((CDepthStencilState*)CRenderMgr::GetInst()->GetDepthStencil((DEPTH_STENCIL_TYPE)i))->GetName();
		List.AddData(strName.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		if (m_pTargetObj->GetMeshRender()->GetMtrl() != nullptr)
		{
			wstring wcsSelect = List.GetSelect();
			if (wcsSelect != L"")
				m_pTargetObj->GetMeshRender()->GetMtrl(m_cbSubset.GetCurSel())->SetDepthStencil(CRenderMgr::GetInst()->GetDepthStencil(wcsSelect));
		}
	}
}

void CMeshRenderDlg::OnAddTexture2()
{
	wstring strPath = CPathMgr::GetInst()->GetPath();

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"png", L"*.*",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	wstring wcsPath = dlg.GetPathName();
	wstring wcsKey = dlg.GetFileName();

	CResMgr::GetInst()->AddResource<CTexture>(wcsKey, CTexture::CreateTexture(wcsPath.c_str()));
	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(wcsKey);
	m_pTargetObj->GetTransForm()->SetScale(Vec3((float)pTex->GetWidth(), (float)pTex->GetHeight(), 1.f));
	m_pTargetObj->GetMeshRender()->SetTexture(pTex,SHADER_PARAM::TEX_1);
}

void CMeshRenderDlg::Sort()
{
	RECT rt = {};
	m_EditMesh.GetWindowRect(&rt);
}

void CMeshRenderDlg::OnBnClickedButtonRasterize()
{
	CDlgList List;

	List.SetCaption(L"Rasterize");

	for (UINT i = 0; i < (UINT)RASTERIZE_TYPE::END; ++i)
	{
		wstring strName = CRenderMgr::GetInst()->GetRSName((RASTERIZE_TYPE)i);
		List.AddData(strName.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();

		if (wcsSelect != L"")
			m_pTargetObj->GetMeshRender()->GetMtrl(m_cbSubset.GetCurSel())->SetRasterizeState(CRenderMgr::GetInst()->GetRSState(wcsSelect));
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMeshRenderDlg::OnEnChangeDiffuse(UINT _id)
{
	if (m_bFocus == false)
		return;
	Vec4 vDiff;
	CString strDiff;
	for (size_t i = 0; i < 4; i++)
	{
		m_editDiff[i].GetWindowTextW(strDiff);
		float fDiff = _wtof(strDiff);
		vDiff[i] = fDiff;
	}

	m_pTargetObj->GetMeshRender()->SetParam(&vDiff,SHADER_PARAM::MTRL, m_cbSubset.GetCurSel());
}

void CMeshRenderDlg::OnBnCheckShadow()
{
	int iCheck = m_checkShadow.GetCheck();
	m_pTargetObj->GetMeshRender()->SetShadow((bool)iCheck);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL CMeshRenderDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_cbRenderType.InsertString(0, L"Deffered");
	m_cbRenderType.InsertString(1, L"Forward");
	m_cbRenderType.InsertString(2, L"PostEffect");
	m_cbRenderType.InsertString(3, L"NoInstance");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMeshRenderDlg::OnCbnSelchangeRenderType()
{
	int iSel = m_cbRenderType.GetCurSel();
	RENDER_TYPE eType = (RENDER_TYPE)iSel;
	m_pTargetObj->GetMeshRender()->SetRenderType(eType);

	/*if (eType == RENDER_TYPE::POSTEFFECT)
	{
		CTexture* pTex = CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT);
		m_pTargetObj->GetMeshRender()->GetMtrl()->SetParam(pTex, SHADER_PARAM::TEX_0);
	}*/
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMeshRenderDlg::OnEnChangeUV(UINT _iId)
{
	if (!m_bClick)
		return;
	CString strTexU;
	m_editUV[0].GetWindowTextW(strTexU);

	CString strTexV;
	m_editUV[1].GetWindowTextW(strTexV);
	
	Vec2 vUV = Vec2(_wtof(strTexU), _wtof(strTexV));
	m_pTargetObj->GetMeshRender()->SetUV(vUV);
}

void CMeshRenderDlg::OnBnClickedSampler()
{
	CDlgList List;

	List.SetCaption(L"Sampler");

	for (UINT i = 0; i < (UINT)SAMPLER_TYPE::END; ++i)
	{
		wstring strName = CRenderMgr::GetInst()->GetSPName((SAMPLER_TYPE)i);
		List.AddData(strName.c_str());
	}

	INT_PTR id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();

		if (wcsSelect != L"")
			m_pTargetObj->GetMeshRender()->GetMtrl(m_cbSubset.GetCurSel())->SetSamplerState(CRenderMgr::GetInst()->GetSPState(wcsSelect));
	}
}