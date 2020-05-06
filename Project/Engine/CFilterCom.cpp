#include "CFilterCom.h"
#include "CTransForm.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CResMgr.h"
#include "CRenderMgr.h"
#include "CDepthStencilState.h"
#include "CMRT.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CDirect.h"

void CFilterCom::SetTransForm(CTransForm * _pTransForm)
{
	if (m_pVolumeMesh != nullptr)
		*TransForm() = *_pTransForm;
}

CFilterCom::CFilterCom()
	:m_pVolumeMesh(nullptr)
	, m_eFilter(FILTER_TYPE::NONE)
	, m_pRWTex(nullptr)
	, m_pIn(nullptr)
	, m_pMesh(nullptr)
{
	m_eType = COMPONENT_TYPE::FILTER;
}

CFilterCom::~CFilterCom()
{
	SAFE_DELETE(m_pBlurOut);
	SAFE_DELETE(m_pRWTex);
	//SAFE_DELETE(m_pIn);
}

void CFilterCom::Start()
{
	if (m_pIn == nullptr)
		m_pIn = CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::OUTPUT);
	m_pRWTex = CTexture::CreateTexture(m_pIn->GetWidth(), m_pIn->GetHeight(), D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, m_pIn->GetFormat());
	m_pBlurOut = CTexture::CreateTexture(m_pIn->GetWidth(), m_pIn->GetHeight(), D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, m_pIn->GetFormat());
	m_pHorBlur = CResMgr::GetInst()->Load<CMaterial>(L"HorBlur");
	m_pVerBlur = CResMgr::GetInst()->Load<CMaterial>(L"VerBlur");


	Vec2 vSize = Vec2(m_pIn->GetWidth(), m_pIn->GetHeight());
	m_pHorBlur->SetParam(&vSize, SHADER_PARAM::VEC2_0);
	m_pVerBlur->SetParam(&vSize, SHADER_PARAM::VEC2_0);
	m_pHorBlur->SetParam(m_pIn, SHADER_PARAM::TEX_0);
	m_pHorBlur->SetParam(m_pRWTex, SHADER_PARAM::RWTEX_0);
	

	CTexture* pPosTex = CRenderMgr::GetInst()->GetRenderTargetTexture(RT_TYPE::POSITION);
	/*m_pVerBlur->SetParam(pPosTex, SHADER_PARAM::TEX_1);
	m_pHorBlur->SetParam(pPosTex, SHADER_PARAM::TEX_1);*/
}

void CFilterCom::Render()
{
	Mtx mtxCamInvView = CRenderMgr::GetInst()->GetMainCam()->GetMtxInvView();
	m_pMtrl->SetParam(&mtxCamInvView, SHADER_PARAM::MTX_0);
	if (m_pVolumeMesh == nullptr)
	{
		if (m_eFilter == FILTER_TYPE::BLUR)
			BlurRender();

		TransForm()->UpdateData();

		if (m_pMesh != nullptr && m_pMtrl != nullptr)
		{
			m_pMtrl->UpdateData();
			m_pMesh->SetShader(m_pMtrl->GetShader());
			m_pMesh->Render(0);
		}
	}
	else
	{
		if (m_eFilter == FILTER_TYPE::BLUR)
			BlurRender();

		TransForm()->UpdateData();
		m_pVolumeMesh->SetShader(m_pMtrl->GetShader());
		m_pMtrl->SetRasterizeState(RASTERIZE_TYPE::CULL_FRONT);
		m_pMtrl->UpdateData();
		//CRenderMgr::GetInst()->RSSetState(RASTERIZE_TYPE::CULL_FRONT);
		m_pVolumeMesh->Render(0);
		//m_pMtrl->SetRasterizeState(RASTERIZE_TYPE::CULL_BACK);
	}
}

void CFilterCom::BlurRender()
{
	if (m_pHorBlur != nullptr)
	{
		m_pHorBlur->SetParam(m_pRWTex, SHADER_PARAM::RWTEX_0);
		m_pHorBlur->UpdateData();
		m_pHorBlur->Excute(UINT(m_pRWTex->GetWidth() / 256.f) + 1, m_pRWTex->GetHeight(), 1);

		m_pVerBlur->SetParam(m_pRWTex, SHADER_PARAM::TEX_0);
		m_pVerBlur->SetParam(m_pBlurOut, SHADER_PARAM::RWTEX_0);
		m_pVerBlur->UpdateData();
		m_pVerBlur->Excute(m_pRWTex->GetWidth(), UINT(m_pRWTex->GetHeight() / 256.f) + 1, 1);
		
		m_pMtrl->SetParam(m_pBlurOut, SHADER_PARAM::TEX_1);
	}
}