#include "CLight.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CFilterCom.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CSceneMgr.h"
#include "CTexture.h"
#include "CMRT.h"
#include "CRT.h"
#include "CMeshRender.h"
#include "CPlayerMgr.h"

CLight::CLight()
	:m_Info{}
	, m_pFilter(nullptr)
	, m_pShadowTex{}
{
	m_eType = COMPONENT_TYPE::LIGHT;

	m_pCam = new CGameObj;
	m_pCam->SetName(L"LightCam");
	m_pCam->AddComponent(new CCamera);
	m_pCam->GetCamera()->SetRegister(false);
	
	
	m_vPointDir[(UINT)POINT_DIR::LEFT] = Vec3(-1.f,0.f,0.f);
	m_vPointDir[(UINT)POINT_DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_vPointDir[(UINT)POINT_DIR::UP] = Vec3(0.f, 1.f, 0.f);
	m_vPointDir[(UINT)POINT_DIR::DOWN] = Vec3(0.f, -1.f, 0.f);
	m_vPointDir[(UINT)POINT_DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
	m_vPointDir[(UINT)POINT_DIR::BACK] = Vec3(0.f, 0.f, -1.f);

	m_vRotDir[(UINT)POINT_DIR::LEFT] = Vec3(0.f, -XM_PIDIV2, 0.f);
	m_vRotDir[(UINT)POINT_DIR::RIGHT] = Vec3(0.f, XM_PIDIV2, 0.f);
	m_vRotDir[(UINT)POINT_DIR::UP] = Vec3(XM_PIDIV2, 0.f, 0.f);
	m_vRotDir[(UINT)POINT_DIR::DOWN] = Vec3(-XM_PIDIV2, 0.f, 0.f);
	m_vRotDir[(UINT)POINT_DIR::FRONT] = Vec3(0.f, 0.f, 0.f);
	m_vRotDir[(UINT)POINT_DIR::BACK] = Vec3(0.f, XM_PI, 0.f);

	for (size_t i = 0; i < 1; i++)
	{
		CTexture* pTex = CTexture::CreateTexture(4096, 4096, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);
		m_pShadowRT[i] = new CRT(pTex, Vec4(1.f, 0.f, 0.f, 1.f));
	}
	;
	CTexture* pDepthTex = CTexture::CreateTexture(4096, 4096, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pMRT = new CMRT;
	m_pMRT->Create(m_pShadowRT, pDepthTex, 1);
}

CLight::CLight(const CLight & _other)
{
	*this = _other;
	m_pCam = new CGameObj;
	m_pCam->SetName(L"LightCam");
	m_pCam->AddComponent(new CCamera);
	m_pCam->GetCamera()->SetRegister(false);

	for (size_t i = 0; i < 1; i++)
	{
		CTexture* pTex = CTexture::CreateTexture(_other.m_pShadowTex[0]->GetWidth(), _other.m_pShadowTex[0]->GetHeight(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT);
		m_pShadowRT[i] = new CRT(pTex, Vec4(1.f, 0.f, 0.f, 1.f));
	}
	;
	CTexture* pDepthTex = CTexture::CreateTexture(_other.m_pShadowTex[0]->GetWidth(), _other.m_pShadowTex[0]->GetHeight(), D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);
	m_pMRT = new CMRT;
	m_pMRT->Create(m_pShadowRT, pDepthTex, 1);
}

CLight::~CLight()
{
	SAFE_DELETE(m_pCam);
	SAFE_ARRAY_DELETE(m_pShadowRT, 1);
	SAFE_DELETE(m_pMRT);
	SAFE_DELETE(m_pFilter);
}

int CLight::Update()
{
	return 0;
}

int CLight::FinalUpdate()
{
	if (m_pFilter == nullptr)
		return 0;

	m_Info.vPos = Vec4(TransForm()->GetPos(), 1.f);
	m_pFilter->FinalUpdate();

	if(!GetOwner()->GetDead())
		CRenderMgr::GetInst()->RegisterLight(this);

	*m_pCam->GetTransForm() = *TransForm();
	m_pCam->GetCamera()->FinalUpdate();

	return 0;
}

void CLight::Render()
{
	if (m_pFilter == nullptr)
		return;

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"LightInfo");
	pBuffer->UpdateData(&m_Info,sizeof(tLightInfo));
	pBuffer->UpdateRegisterAll();
	
	if (m_Info.iType == (UINT)LIGHT_TYPE::DIR)
	{
		Mtx mtxLightVP = m_pCam->GetCamera()->GetMtxView() * m_pCam->GetCamera()->GetMtxProj();
		m_pFilter->GetFilter()->GetMaterial()->SetParam(&mtxLightVP, SHADER_PARAM::MTX_1);
		static int bLim = 0;
		/*if (KEYTAB(KEY_TYPE::KEY_TAB))
			bLim = ++bLim % 2;*/
		m_pFilter->GetFilter()->GetMaterial()->SetParam(&bLim, SHADER_PARAM::INT_0);
	}
	else if (m_Info.iType == (UINT)LIGHT_TYPE::POINT)
	{
		for (UINT i = (UINT)SHADER_PARAM::MTX_1; i <= (UINT)SHADER_PARAM::MTX_1; i++)
		{
			Mtx mtxLightVP = m_mtxCamView[i - (UINT)SHADER_PARAM::MTX_1] * m_pCam->GetCamera()->GetMtxProj();
			m_pFilter->GetFilter()->GetMaterial()->SetParam(&mtxLightVP, (SHADER_PARAM)i);
		}
	}
	m_pFilter->GetFilter()->SetTransForm(TransForm());
	m_pFilter->GetFilter()->Render();
}

void CLight::ShadowRender()
{
	m_pMRT->Clear();
	m_pMRT->OMSet();
	if (m_Info.iType == (UINT)LIGHT_TYPE::DIR)
	{
		m_pCam->GetCamera()->SortShadowObject();
		m_pCam->GetCamera()->ShadowRender();
	}
	else if (m_Info.iType == (UINT)LIGHT_TYPE::POINT)
	{
		CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"Target");
		tTarget Target = {};

		m_pCam->GetCamera()->SortShadowObject();
		
		CGameObj* pPlayer = CPlayerMgr::GetInst()->GetPlayer();
		if (pPlayer == nullptr)
			return;
		Vec3 vDir = pPlayer->GetTransForm()->GetWorldPos() - TransForm()->GetPos();
		vDir.Normalize();
		m_pCam->GetTransForm()->SetLook(vDir);
		m_pCam->GetTransForm()->FinalUpdate();
		m_pCam->GetCamera()->CalcView();
		m_mtxCamView[0] = m_pCam->GetCamera()->GetMtxView();

		Target.iTarget = 0;
		pBuffer->UpdateData(&Target, sizeof(tTarget));
		pBuffer->UpdateRegisterAll();

		m_pCam->GetCamera()->PointShadowRender();

		/*for (size_t i = 0; i < 6; i++)
		{
			m_pCam->GetTransForm()->SetLook(TransForm()->GetWorldDir((DIR_TYPE)(i / 2)));
			m_pCam->GetTransForm()->FinalUpdate();
			m_pCam->GetCamera()->CalcView();
			m_mtxCamView[i] = m_pCam->GetCamera()->GetMtxView();

			Target.iTarget = i;
			pBuffer->UpdateData(&Target, sizeof(tTarget));
			pBuffer->UpdateRegisterAll();

			m_pCam->GetCamera()->PointShadowRender();
		}*/
	}
}

void CLight::SetDirection(Vec3 & _vDir)
{
	m_Info.vDir = Vec4(_vDir,0.f);
	TransForm()->SetLook(_vDir);
}

void CLight::SetDistance(float _fDist)
{
	m_Info.fDist = _fDist;
	TransForm()->SetScale(Vec3(_fDist, _fDist, _fDist));
}

void CLight::SetLightType(int _iType)
{
	m_Info.iType = _iType;

	if (_iType == (int)LIGHT_TYPE::DIR)
	{
		SAFE_DELETE(m_pFilter);
		m_pFilter = CRenderMgr::GetInst()->FindFilter(L"DirLight")->Clone();
		m_pCam->GetCamera()->SetPerspective(false);
		m_pCam->GetCamera()->SetScale(5.f);
		m_pFilter->GetFilter()->SetMaterial(m_pFilter->GetFilter()->GetMaterial()->Clone());
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_0]->GetTexture(), SHADER_PARAM::TEX_2);
	}
	else if (_iType == (int)LIGHT_TYPE::POINT)
	{
		SAFE_DELETE(m_pFilter);
		m_pFilter = CRenderMgr::GetInst()->FindFilter(L"PointLight")->Clone();
		m_pCam->GetCamera()->SetPerspective(false);
		m_pCam->GetCamera()->SetScale(1.f);

		m_pFilter->GetFilter()->SetMaterial(m_pFilter->GetFilter()->GetMaterial()->Clone());
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_0]->GetTexture(), SHADER_PARAM::TEX_2);
		/*m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_1]->GetTexture(), SHADER_PARAM::TEX_3);
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_2]->GetTexture(), SHADER_PARAM::TEX_4);
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_3]->GetTexture(), SHADER_PARAM::TEX_5);
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_4]->GetTexture(), SHADER_PARAM::TEX_6);
		m_pFilter->GetFilter()->GetMaterial()->SetParam(m_pShadowRT[(UINT)SHADOW_RT::SHADOW_5]->GetTexture(), SHADER_PARAM::TEX_7);*/
		m_pCam->GetCamera()->LayerMaskClear();
		UINT iPlayer = CSceneMgr::GetInst()->GetLayerIndex(L"Player");
		m_pCam->GetCamera()->CheckLayerMask(iPlayer);
	}
	else if (_iType == (int)LIGHT_TYPE::SPOT)
	{
		m_pFilter = CRenderMgr::GetInst()->FindFilter(L"SpotLight");
	}
}

CTexture * CLight::GetShadowTexture(SHADOW_RT _eRT)
{
	return m_pShadowRT[(UINT)_eRT]->GetTexture();
}

void CLight::Save(FILE * _pFile)
{
	fwrite(&m_Info.iType, sizeof(int), 1, _pFile);
	fwrite(&m_Info.vDir, sizeof(Vec4), 1, _pFile);
	fwrite(&m_Info.vPos, sizeof(Vec4), 1, _pFile);
	fwrite(&m_Info.fDist, sizeof(float), 1, _pFile);
	fwrite(&m_Info.fAngle, sizeof(float), 1, _pFile);
	fwrite(&m_Info.color.vDiff, sizeof(Vec4), 1, _pFile);
	fwrite(&m_Info.color.vAmb, sizeof(Vec4), 1, _pFile);
	fwrite(&m_Info.color.vSpec, sizeof(Vec4), 1, _pFile);
}

void CLight::Load(FILE * _pFile)
{
	fread(&m_Info.iType, sizeof(int), 1, _pFile);
	fread(&m_Info.vDir, sizeof(Vec4), 1, _pFile);
	fread(&m_Info.vPos, sizeof(Vec4), 1, _pFile);
	fread(&m_Info.fDist, sizeof(float), 1, _pFile);
	fread(&m_Info.fAngle, sizeof(float), 1, _pFile);
	fread(&m_Info.color.vDiff, sizeof(Vec4), 1, _pFile);
	fread(&m_Info.color.vAmb, sizeof(Vec4), 1, _pFile);
	fread(&m_Info.color.vSpec, sizeof(Vec4), 1, _pFile);

	Vec3 vDir = Vec3{ m_Info.vDir.x, m_Info.vDir.y, m_Info.vDir.z };
	SetDirection(vDir);
	SetDistance(m_Info.fDist);
	SetLightType((int)m_Info.iType);
}