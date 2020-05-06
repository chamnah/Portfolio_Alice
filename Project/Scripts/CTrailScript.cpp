#include "CTrailScript.h"
#include "CScriptMgr.h"
#include "CTexture.h"
#include "CTransForm.h"
#include "CResMgr.h"
#include "CMeshRender.h"
#include "KeyMgr.h"

CTrailScript::CTrailScript()
: m_iCurTrail(0)
, m_pCSMtrl(nullptr)

{
	SetScriptType((UINT)SCRIPT_TYPE::CTRAILSCRIPT);
	m_pPosTex = CTexture::CreateTexture(20 , 2, D3D10_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pDrawTex = CTexture::CreateTexture(20 , 2, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_pCSMtrl = CResMgr::GetInst()->Load<CMaterial>(L"AddTrail");
}

CTrailScript::~CTrailScript()
{
	SAFE_DELETE(m_pPosTex);
	SAFE_DELETE(m_pDrawTex);
}

int CTrailScript::FinalUpdate()
{
	m_vUpPos = XMVector3TransformCoord(Vec3(0.f, 30.f, 0.f), TransForm()->GetWorldMtx());
	m_vDownPos = XMVector3TransformCoord(Vec3(0.f, -30.f, 0.f), TransForm()->GetWorldMtx());

	m_vUpPos.w = 2.f;
	m_vDownPos.w = 2.f;

	Vec4* pData = (Vec4*)m_pDrawTex->GetPixel();
	m_vPrePos = *(pData + 1);

	float vDis = Vec4::Distance(m_vPrePos, m_vUpPos);
	int iPush = 0;

	if (vDis > 10.f || m_vPrePos.w <= 1.f)
		iPush = 1;

	m_pTempTex = m_pPosTex;
	m_pPosTex = m_pDrawTex;
	m_pDrawTex = m_pTempTex;

	m_pCSMtrl->SetParam(m_pPosTex, SHADER_PARAM::TEX_0);
	m_pCSMtrl->SetParam(m_pDrawTex, SHADER_PARAM::RWTEX_0);
	m_pCSMtrl->SetParam(&iPush, SHADER_PARAM::INT_1);
	m_pCSMtrl->SetParam(&m_vUpPos, SHADER_PARAM::VEC4_0);
	m_pCSMtrl->SetParam(&m_vDownPos, SHADER_PARAM::VEC4_1);
	m_pCSMtrl->Excute(1, 1, 1);

	if (MeshRender() != nullptr && MeshRender()->GetMtrl() != nullptr)
		MeshRender()->GetMtrl()->SetParam(m_pDrawTex, SHADER_PARAM::TEX_0);

	return 0;
}
/*
if (m_iCurTrail == 0)
	{

		m_arrTrail[m_iCurTrail].vPos = XMVector3TransformCoord(Vec3::Zero, TransForm()->GetWorldMtx());
		m_arrTrail[m_iCurTrail].fAge = 1.f;

		m_arrTrail[m_iCurTrail + 1].vPos = XMVector3TransformCoord(Vec3(0.f, 1.f, 0.f), TransForm()->GetWorldMtx());
		m_arrTrail[m_iCurTrail + 1].fAge = 1.f;
		m_iCurTrail += 2;
	}
	else
	{
		float fDis = Vec3::Distance(m_arrTrail[m_iCurTrail - 1].vPos, m_arrTrail[m_iCurTrail].vPos);
		if (fDis > 20.f)
		{
			if (m_iCurTrail < 18)
				m_iCurTrail += 2;
		}

		m_arrTrail[m_iCurTrail].vPos = XMVector3TransformCoord(Vec3::Zero, TransForm()->GetWorldMtx());
		m_arrTrail[m_iCurTrail].fAge = 1.f;

		m_arrTrail[m_iCurTrail + 1].vPos = XMVector3TransformCoord(Vec3(0.f, 1.f, 0.f), TransForm()->GetWorldMtx());
		m_arrTrail[m_iCurTrail + 1].fAge = 1.f;
	}

	for (size_t i = 0; i <= m_iCurTrail;)
	{
		m_arrTrail[i].fAge -= DT;
		m_arrTrail[i + 1].fAge -= DT;
		if (m_arrTrail[i].fAge <= 0.f)
		{
			m_iCurTrail -= 2;
			for (size_t j = i; j <= m_iCurTrail; j += 2)
			{
				m_arrTrail[j] = m_arrTrail[j + 2];
				m_arrTrail[j + 1] = m_arrTrail[j + 3];
			}
		}
		else
			i += 2;
	}
*/