#include "CParticleScript.h"
#include "CScriptMgr.h"
#include "TimeMgr.h"
#include "CMaterial.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CTexture.h"
#include "CMeshRender.h"
#include "CRenderMgr.h"
#include "CCamera.h"

CParticleScript::CParticleScript()
	:m_fPushTime(0.f)
	, m_pParticle(nullptr)
	, m_pMesh(nullptr)
	, m_iVtxCount(0)
	, pDrawTex(nullptr)
	, pPosTex(nullptr)
	, m_fDeathTime(0.f)
	, m_iCount(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::CPARTICLESCRIPT);
	m_pParticle = CResMgr::GetInst()->Load<CMaterial>(L"AddTrailParticle")->Clone();
}

CParticleScript::~CParticleScript()
{
	SAFE_DELETE(pDrawTex);
	SAFE_DELETE(pPosTex);
	SAFE_DELETE(m_pMesh);
}

void CParticleScript::Start()
{
	//SetCount(1);
}

int CParticleScript::Update()
{
	if (MeshRender() == nullptr)
		return 0;
	m_fPushTime += DT;
	m_fDeathTime += DT;

	int iPush = 0;
	if (m_fPushTime > 0.2f)
	{
		m_fPushTime = 0.f;
		Vec4 vPos = Vec4(TransForm()->GetWorldPos(),2.f);
		m_pParticle->SetParam(&vPos, SHADER_PARAM::VEC4_0);
		iPush = 1;
		m_iVtxCount++;
	}

	m_pParticle->SetParam(&iPush,SHADER_PARAM::INT_0);

	CTexture* pTemp = pPosTex;
	pPosTex = pDrawTex;
	pDrawTex = pTemp;
	m_pParticle->SetParam(pPosTex, SHADER_PARAM::TEX_0);
	m_pParticle->SetParam(pDrawTex, SHADER_PARAM::RWTEX_0);

	m_pParticle->Excute(m_iCount,1,1);

	if (m_fDeathTime >= 1.f)
	{
		m_fDeathTime = 0.f;
		m_iVtxCount--;
	}
	if(m_iVtxCount != 0)
		MeshRender()->SetParticleCount(m_iVtxCount);

	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	Vec4 vPos = Vec4(pMainCam->TransForm()->GetWorldPos(), 1.f);
	Vec4 vDir = Vec4(pMainCam->TransForm()->GetWorldDir(DIR_TYPE::UP), 0.f);
	MeshRender()->SetParam(&vDir, SHADER_PARAM::VEC4_0);
	MeshRender()->SetParam(&vPos, SHADER_PARAM::VEC4_1);

	MeshRender()->SetParam(pDrawTex, SHADER_PARAM::TEX_1);
	Vec3 vScale = TransForm()->GetScale();
	MeshRender()->SetParam(&vScale, SHADER_PARAM::VEC2_0);
	return 0;
}

void CParticleScript::SetCount(int _iCount)
{
	SAFE_DELETE(pDrawTex);
	SAFE_DELETE(pPosTex);
	SAFE_DELETE(m_pMesh);

	VTX arrPoint = {};
	UINT iPoint = 0;

	m_pMesh = CMesh::CreateMesh(&arrPoint, sizeof(VTX), 20, &iPoint, sizeof(UINT), 1);
	MeshRender()->SetMesh(m_pMesh);

	m_iCount = _iCount;
	pDrawTex = CTexture::CreateTexture(_iCount * 20,1, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	pPosTex = CTexture::CreateTexture(_iCount * 20, 1, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	MeshRender()->SetIsParticle(true);
}